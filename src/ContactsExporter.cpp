// (c) 2013 Florian Behrens.

#include "ContactsExporter.h"

#include <QByteArray>
#include <QDebug>
#include <QDir>
#include <QList>
#include <QMap>
#include <QVariantList>
#include <QtDeclarative>

#include <bb/cascades/Application>
#include <bb/cascades/pickers/ContactPicker>
#include <bb/pim/contacts/Contact>
#include <bb/pim/contacts/ContactPostalAddress>
#include <bb/pim/contacts/ContactSearchFilters>
#include <bb/system/SystemProgressDialog>
#include <bb/system/SystemToast>

#include "unique_ptr.h"
#include "CsvGenerator.h"
#include "Utilities.h"

using bb::cascades::Application;
using namespace bb::cascades::pickers;
using namespace bb::pim::contacts;
using namespace bb::system;

namespace {

const QString fileExtensionsMap[] = {
		".vcf",
		".csv"
};

struct operation_cancelled : public std::exception
{};

} // Anynomous namespace

ContactsExporter::ContactsExporter()
  : mFiletype(vCard),
    mAll(true)
{}

void ContactsExporter::qmlRegisterType()
{
	::qmlRegisterType<ContactsExporter>("CustomComponents", 1, 0, "ContactsExporter");
}

void ContactsExporter::setFiletype(ContactsExporter::Filetype filetype)
{
	if (mFiletype != filetype) {
		mFiletype = filetype;
		emit filetypeChanged(mFiletype);
	}
}

void ContactsExporter::setFilename(QString filename)
{
	if (mFilename != filename) {
		mFilename = filename;
		emit filenameChanged(mFilename);
	}
}

void ContactsExporter::setAll(bool all)
{
	if (mAll != all) {
		mAll = all;
		emit allChanged(mAll);
	}
}

void ContactsExporter::setContacts(const QVariantList &contacts)
{
	if (mContacts != contacts) {
		mContacts = contacts;
		emit contactsChanged(mContacts);
	}
}

void ContactsExporter::exportContacts(const QString &filename)
{
	// Obtain list of contact ids to be exported
	QList<ContactId> contactIds;
	if (mContacts.length() == 0) {
		// Get all contacts from contact service
		ContactListFilters contactListFilters;
		contactListFilters.setLimit(INT_MAX);
		QList<Contact> contactList = mContactService.contacts(contactListFilters);
		qDebug() << contactList.size() << "contacts found";

		// Convert list of contacts to list of contact ids
		for (QList<Contact>::const_iterator it = contactList.begin(); it != contactList.end(); ++it)
			contactIds << (*it).id();
	} else {
		// Convert mContacts to list of contact ids
		for (QVariantList::const_iterator it = mContacts.begin(); it != mContacts.end(); ++it)
			contactIds << (*it).toInt();

		qDebug() << contactIds.size() << "contacts given";
	}

	// Check filename for proper extension
	QString actualFilename = filename;
	if (!actualFilename.endsWith(fileExtensionsMap[int(mFiletype)]))
		actualFilename += fileExtensionsMap[int(mFiletype)];

	// Create vCard stream
	QFile file(actualFilename);
	if (file.open(QIODevice::WriteOnly)) {
		// Show progress bar
		std::unique_ptr<SystemProgressDialog, std::delete_later<SystemProgressDialog> > systemProgressDialog(new SystemProgressDialog());
		systemProgressDialog->setTitle(tr("Exporting contacts..."));
		systemProgressDialog->setProgress(0);
		systemProgressDialog->setStatusDetails(QString(tr("%1 contacts exported")).arg(0));
		systemProgressDialog->confirmButton()->setLabel(QString());
		systemProgressDialog->cancelButton()->setLabel(tr("Cancel"));
		systemProgressDialog->setDismissAutomatically(false);
		systemProgressDialog->show();

		try {
			if (mFiletype == vCard) {
				// TODO: Since the ContactService API seems to only support a maximum of 20
				// contact ids within the list, we need to iterate over the list.
				QList<ContactId> contactIdsSubset;
				for (int pos = 0; pos < contactIds.size(); ) {
					QList<ContactId> contactIdSubset = contactIds.mid(pos, 20);
					file.write(mContactService.exportContactVCards(contactIdSubset, VCardPhotoEncoding::BASE64));
					pos += contactIdSubset.size();

					// Check whether operation was cancelled by user
					Application::instance()->processEvents();
					if (systemProgressDialog->result() == SystemUiResult::CancelButtonSelection)
						throw operation_cancelled();

					// Update progress bar
					systemProgressDialog->setProgress(pos * 100 / contactIds.size());
					systemProgressDialog->setStatusDetails(QString(tr("%1 contacts exported")).arg(pos));
					systemProgressDialog->show();
				}
			} else if (mFiletype == CSV) {
				// For each contact, all attribute kind/subkind combinations are stored in a
				// global set container that finally contains the kind/subkind combinations
				// of all contacts.
				// Each contact is loaded in a map container with the attribute kind/subkind
				// combination used as the key. All contact maps are stored in a vector.
				QSet<QString> attributeNames;
				QList<QMap<QString, QString> > contacts;
				CsvGenerator csvGenerator(file);

				QList<ContactId>::const_iterator it = contactIds.constBegin();
				for (int index = 0; it != contactIds.constEnd(); ++it, ++index) {
					const Contact contact = mContactService.contactDetails(*it);
					QMap<QString, QString> contactMap;

					qDebug() << "Exporting contact:";

					// Iterate over contacts attributes
					typedef QList<ContactAttribute> ContactAttributes;
					ContactAttributes contactAttributes = contact.attributes();
					for (ContactAttributes::const_iterator it = contactAttributes.constBegin(); it != contactAttributes.constEnd(); ++it) {
						QString attributeName = QString(mAttributeKinds[it->kind()] + "/" + mAttributeSubKinds[it->subKind()]);

						qDebug().nospace() << attributeName << ": " << it->value();

						// Put attribute in global attribute name set and contact map
						attributeNames.insert(attributeName);
						contactMap[attributeName] = it->value();
					}

					// Iterate over contacts postal addresses
					typedef QList<ContactPostalAddress> ContactPostalAddresses;
					ContactPostalAddresses contactPostalAddresses = contact.postalAddresses();
					for (ContactPostalAddresses::const_iterator it = contactPostalAddresses.constBegin(); it != contactPostalAddresses.constEnd(); ++it) {
						qDebug().nospace()
							<< "City: " << it->city()
							<< "Country: " << it->country()
							<< "Label: " << it->label()
							<< "Line1: " << it->line1()
							<< "Line2: " << it->line2()
							<< "Latitude: " << it->latitude()
							<< "Longitude: " << it->longitude()
							<< "Postal code: " << it->postalCode()
							<< "Subkind: " << it->subKind()
							<< "Region: " << it->region();

						QString attributeName;
						QString& prefix = postalAddressAttributePrefix;
						if (!it->line1().isEmpty()) {
							attributeName = prefix + "Line1/" + mAttributeSubKinds[it->subKind()];
							attributeNames.insert(attributeName);
							contactMap[attributeName] = it->line1();
						}
						if (!it->line2().isEmpty()) {
							attributeName = prefix + "Line2/" + mAttributeSubKinds[it->subKind()];
							attributeNames.insert(attributeName);
							contactMap[attributeName] = it->line2();
						}
						if (!it->postalCode().isEmpty()) {
							attributeName = prefix + "Code/" + mAttributeSubKinds[it->subKind()];
							attributeNames.insert(attributeName);
							contactMap[attributeName] = it->postalCode();
						}
						if (!it->city().isEmpty()) {
							attributeName = prefix + "City/" + mAttributeSubKinds[it->subKind()];
							attributeNames.insert(attributeName);
							contactMap[attributeName] = it->city();
						}
						if (!it->region().isEmpty()) {
							attributeName = prefix + "Region/" + mAttributeSubKinds[it->subKind()];
							attributeNames.insert(attributeName);
							contactMap[attributeName] = it->region();
						}
						if (!it->country().isEmpty()) {
							attributeName = prefix + "Country/" + mAttributeSubKinds[it->subKind()];
							attributeNames.insert(attributeName);
							contactMap[attributeName] = it->country();
						}
						if (!it->label().isEmpty()) {
							attributeName = prefix + "Label/" + mAttributeSubKinds[it->subKind()];
							attributeNames.insert(attributeName);
							contactMap[attributeName] = it->label();
						}
						if (it->isValidLatitudeLongitude()) {
							attributeName = prefix + "Latitude/" + mAttributeSubKinds[it->subKind()];
							attributeNames.insert(attributeName);
							contactMap[attributeName] = QString("%1").arg(it->latitude());
							attributeName = prefix + "Longitude/" + mAttributeSubKinds[it->subKind()];
							attributeNames.insert(attributeName);
							contactMap[attributeName] = QString("%1").arg(it->longitude());
						}
					}
					contacts.push_back(contactMap);

					// Check whether operation was cancelled by user
					Application::instance()->processEvents();
					if (systemProgressDialog->result() == SystemUiResult::CancelButtonSelection)
						throw operation_cancelled();

					// Update progress bar
					systemProgressDialog->setProgress(index * 100 / contactIds.size());
					systemProgressDialog->setStatusDetails(QString(tr("%1 contacts exported")).arg(index));
					systemProgressDialog->show();
				}

				// Save contact attribute data to file, starting with a header
				file.write(QString("\"").toUtf8() + QStringList(attributeNames.toList()).join("\",\"").toUtf8() + "\"\n");

				// Followed by the contact attribute data
				typedef QList<QMap<QString, QString> >::const_iterator ContactsListConstIterator;
				for (ContactsListConstIterator contacts_it = contacts.constBegin(); contacts_it != contacts.constEnd(); ++contacts_it) {
					typedef QSet<QString>::const_iterator StringSetConstIterator;
					for (StringSetConstIterator attributeNames_it = attributeNames.constBegin(); attributeNames_it != attributeNames.constEnd(); ++attributeNames_it) {
						// Generate CSV output
						csvGenerator << contacts_it->value(*attributeNames_it);
					}
					csvGenerator.terminateLine();
				}
			}

			Utilities::showSystemToast(this, QString(tr("%1 contacts successfully exported.")).arg(contactIds.size()));
		} catch (operation_cancelled&) {}

		systemProgressDialog->cancel();
	} else
		Utilities::showSystemToast(this, QString(tr("Could not write contacts to file %1.")).arg(actualFilename));
}

void ContactsExporter::onContactsSelected(const QList<int> &contactList)
{
	qDebug() << contactList.size() << "contacts selected";
}
