/*
 * ContactsImporter.cpp
 *
 *  Created on: 29.08.2013
 *      Author: willy
 */

#include "ContactsImporter.h"

#include <QtDeclarative>

#include <bb/cascades/Application>
#include <bb/pim/contacts/Contact>
#include <bb/pim/contacts/ContactAttributeBuilder>
#include <bb/pim/contacts/ContactBuilder>
#include <bb/pim/contacts/ContactPostalAddressBuilder>
#include <bb/pim/contacts/ContactSearchFilters>
#include <bb/system/SystemProgressDialog>

#include <boost/tokenizer.hpp>

#include "Utilities.h"

using bb::cascades::Application;
using namespace bb::pim::contacts;
using namespace bb::system;

void ContactsImporter::qmlRegisterType()
{
	::qmlRegisterType<ContactsImporter>("CustomComponents", 1, 0, "ContactsImporter");
}

void ContactsImporter::process(const QString &filename)
{
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly)) {
		if (filename.endsWith(".vcf"))
			importVcard(file);
		else if (filename.endsWith(".csv"))
			importCsv(file);
	} else
		Utilities::showSystemToast(this, QString("Error opening file %1.").arg(filename));
}

void ContactsImporter::importVcard(QFile &file)
{
	int invalid = 0, ignored = 0, overwritten = 0, merged = 0, added = 0;

	// Show progress bar
	std::auto_ptr<SystemProgressDialog> systemProgressDialog(new SystemProgressDialog);
	systemProgressDialog->setTitle(tr("Importing contacts..."));
	systemProgressDialog->setProgress(0);
	systemProgressDialog->setStatusDetails(QString(tr("%1 contacts imported")).arg(0));
	systemProgressDialog->confirmButton()->setLabel(QString());
	systemProgressDialog->cancelButton()->setLabel(tr("Cancel"));
	systemProgressDialog->setDismissAutomatically(false);
	systemProgressDialog->show();

	// Determine file size for using with progress bar
	QFileInfo fileInfo(file);
	qint64 fileSize = fileInfo.size();

	// Read vCards from file one by one
	for (int i = 0;; ++i) {
		QByteArray vcard;

		// Read vCard line by line
		for (;;) {
			QByteArray line = file.readLine(0);
			vcard += line;
			if (line.isEmpty() || line.startsWith(QByteArray("END:VCARD")))
				break;
		}

		// Test vCard
		if (vcard.isEmpty())
			break;

		// Convert vCard to contact using the contact service
		Contact contact = mContactService.contactFromVCard(QString(vcard));
		ImportResult importResult = importContact(contact);
		switch (importResult) {
		case Invalid:
			invalid += 1; break;
		case Ignored:
			ignored += 1; break;
		case Overwritten:
			overwritten += 1; break;
		case Merged:
			merged += 1; break;
		case Added:
			added += 1; break;
		}

		// Check whether operation was cancelled by user
		Application::instance()->processEvents();
		if (systemProgressDialog->result() == SystemUiResult::CancelButtonSelection)
			break;

		// Update progress bar
		systemProgressDialog->setProgress(file.pos() * 100 / fileSize);
		systemProgressDialog->setStatusDetails(QString(tr("%1 contacts processed")).arg(i));
		systemProgressDialog->show();
	}

	// Remove progress bar
	systemProgressDialog->cancel();

	// Show result toast
	QString toast = QString(tr(
			"%1 contact(s) ignored.\n"
			"%2 contact(s) overwritten.\n"
			"%3 contact(s) merged.\n"
			"%4 contact(s) added.\n"
			"%5 contact(s) invalid.")).arg(ignored).arg(overwritten).arg(merged).arg(added).arg(invalid);
	Utilities::showSystemToast(this, toast);
}

void ContactsImporter::importCsv(QFile &file)
{
	int invalid = 0, ignored = 0, overwritten = 0, merged = 0, added = 0;

	// Determine file size for using with progress bar
	QFileInfo fileInfo(file);
	qint64 fileSize = fileInfo.size();

	// Read header CSV record
	QByteArray byteArray = file.readLine().trimmed();
	if (!byteArray.isEmpty()) {
		// Show progress bar
		std::auto_ptr<SystemProgressDialog> systemProgressDialog(new SystemProgressDialog);
		systemProgressDialog->setTitle(tr("Importing contacts..."));
		systemProgressDialog->setProgress(0);
		systemProgressDialog->setStatusDetails(QString(tr("%1 contacts imported")).arg(0));
		systemProgressDialog->confirmButton()->setLabel(QString());
		systemProgressDialog->cancelButton()->setLabel(tr("Cancel"));
		systemProgressDialog->setDismissAutomatically(false);
		systemProgressDialog->show();

		// Parse header line and put into vector
		std::string str = byteArray.constData();
		boost::tokenizer<boost::escaped_list_separator<char> > tokenizer(str);
		QVector<QString> headerRow;
		for(boost::tokenizer<boost::escaped_list_separator<char> >::iterator it = tokenizer.begin(); it != tokenizer.end(); ++it)
			headerRow << QString::fromUtf8(it->c_str());

		qDebug().nospace() << "Header row: " << str.c_str();

		// Read address records, one at a time
		size_t addressRecordIndex = 0;
		while (!file.atEnd()) {
			byteArray = file.readLine().trimmed();

			// Ignore empty lines
			if (byteArray.isEmpty())
				continue;

			// This container temporarily holds a set of postal addresses
			typedef QMap<AttributeSubKind::Type, QMap<QString, QString> > PostalAddressMap;
			PostalAddressMap postalAddresses;
			ContactBuilder contactBuilder;

			str = byteArray.constData();
			tokenizer.assign(str);

			qDebug().nospace() << "Current row: " << str.c_str();

			boost::tokenizer<boost::escaped_list_separator<char> >::iterator it = tokenizer.begin();
			for(int i = 0; it != tokenizer.end() && i < headerRow.size(); ++it, ++i) {
				QString attributeName = headerRow.at(i);
				QString value = QString::fromUtf8(it->c_str());

				if (!value.isEmpty()) {
					QStringList stringList = attributeName.split('/');
					QString attributeKindName = stringList.value(0);
					QString attributeSubKindName = stringList.value(1);

					AttributeKind::Type attributeKind = AttributeKind::Note;

					// Lookup attribute subkind from attribute subkind table
					int index;
					AttributeSubKind::Type attributeSubKind = AttributeSubKind::Other;
					if ((index = mAttributeSubKinds.indexOf(attributeSubKindName)) != -1)
						attributeSubKind = static_cast<AttributeSubKind::Type>(index);

					// Check whether attribute kind fits to a postal address
					QString &prefix = postalAddressAttributePrefix;
					if (attributeKindName.startsWith(prefix)) {
						QString key = attributeKindName.mid(prefix.length());
						postalAddresses[attributeSubKind][key] = value;

						qDebug() << key.toLocal8Bit() << "/" <<
									attributeSubKindName.toLocal8Bit() << ": " << value.toLocal8Bit();
					} else {
						// Lookup attribute kind from attribute kind table.
						// If none is found, use the default attribute Note/Other.
						if ((index = mAttributeKinds.indexOf(attributeKindName)) != -1) {
							attributeKind = static_cast<AttributeKind::Type>(index);
						}

						// Add attribute to contact
						contactBuilder.addAttribute(ContactAttributeBuilder()
								.setKind(attributeKind)
								.setSubKind(attributeSubKind)
								.setValue(value));

						qDebug() << attributeKindName.toLocal8Bit() << "(" << attributeKind << ")/" <<
									attributeSubKindName.toLocal8Bit() << "(" << attributeSubKind << "): " << value.toLocal8Bit();
					}
				}
			}

			// Create ContactPostalAddressBuilder for each address
			for (PostalAddressMap::const_iterator it = postalAddresses.constBegin(); it != postalAddresses.constEnd(); ++it) {
				ContactPostalAddressBuilder contactPostalAddressBuilder;
				contactPostalAddressBuilder.setSubKind(it.key());

				for (PostalAddressMap::mapped_type::const_iterator it2 = it->constBegin(); it2 != it->constEnd(); ++it2) {
					const QString &key = it2.key();
					if (key == QString("Line1"))
						contactPostalAddressBuilder.setLine1(*it2);
					else if (key == QString("Line2"))
						contactPostalAddressBuilder.setLine2(*it2);
					else if (key == QString("Code"))
						contactPostalAddressBuilder.setPostalCode(*it2);
					else if (key == QString("City"))
						contactPostalAddressBuilder.setCity(*it2);
					else if (key == QString("Region"))
						contactPostalAddressBuilder.setRegion(*it2);
					else if (key == QString("Country"))
						contactPostalAddressBuilder.setCountry(*it2);
					else if (key == QString("Label"))
						contactPostalAddressBuilder.setLabel(*it2);
					else if (key == QString("Latitude"))
						contactPostalAddressBuilder.setLatitude((*it2).toDouble());
					else if (key == QString("Longitude"))
						contactPostalAddressBuilder.setLongitude((*it2).toDouble());
				}

				ContactPostalAddress contactPostalAddress = contactPostalAddressBuilder;
				if (contactPostalAddress.isValid())
					contactBuilder.addPostalAddress(contactPostalAddress);
			}

			// Import newly created contact to contact service
			Contact contact = contactBuilder;
			ImportResult importResult = importContact(contact);
			switch (importResult) {
			case Invalid:
				invalid += 1; break;
			case Ignored:
				ignored += 1; break;
			case Overwritten:
				overwritten += 1; break;
			case Merged:
				merged += 1; break;
			case Added:
				added += 1; break;
			}

			// Check whether operation was cancelled by user
			Application::instance()->processEvents();
			if (systemProgressDialog->result() == SystemUiResult::CancelButtonSelection)
				break;

			// Update progress bar
			addressRecordIndex += 1;
			systemProgressDialog->setProgress(file.pos() * 100 / fileSize);
			systemProgressDialog->setStatusDetails(QString(tr("%1 contacts imported")).arg(addressRecordIndex));
			systemProgressDialog->show();
		}

		// Remove progress bar
		systemProgressDialog->cancel();
	} else
		Utilities::showSystemToast(this, "CSV file empty, nothing to import!");

	// Show result toast
	QString toast = QString(tr(
			"%1 contact(s) ignored.\n"
			"%2 contact(s) overwritten.\n"
			"%3 contact(s) merged.\n"
			"%4 contact(s) added.\n"
			"%5 contact(s) invalid.")).arg(ignored).arg(overwritten).arg(merged).arg(added).arg(invalid);
	Utilities::showSystemToast(this, toast);
}

ContactsImporter::ImportResult ContactsImporter::importContact(Contact& newContact)
{
	// TODO: Collect contacts in list and add in one operation. Faster?
	bool isExisting = false;
	Contact existingContact;
	ImportResult retval = Invalid;

	if (!newContact.isValid())
		return Invalid;

	qDebug() << "Imported contact: " << newContact.firstName() << newContact.lastName();

	if (mMergePolicy != NoCheck) {
		// Check whether contact is already available:
		// First filter by first and last names
		ContactSearchFilters contactSearchFilters;
		QList<SearchField::Type> searchFieldList;
		searchFieldList << SearchField::FirstName << SearchField::LastName;
		contactSearchFilters.setSearchFields(searchFieldList)
							.setSearchValue(newContact.lastName().isEmpty() ? newContact.firstName() : newContact.lastName());

		// Second iterate over all available contacts
		QList<Contact> contactList = mContactService.searchContacts(contactSearchFilters);
		for (QList<Contact>::const_iterator it = contactList.begin(); it != contactList.end(); ++it) {
			existingContact = mContactService.contactDetails(it->id());

			qDebug() << "Found present contact: " << existingContact.firstName() << existingContact.lastName();

			if (existingContact.lastName() == newContact.lastName() &&
				existingContact.firstName() == newContact.firstName())
			{
				// Contact is already available -> ignore
				isExisting = true;
				break;
			}
		}
	} else
		retval = Added;

	if (isExisting) {
		if (mMergePolicy == KeepExisting) {
			qDebug() << "Contact is already existing: Ignoring imported contact";

			retval = Ignored;
		} else if (mMergePolicy == OverwriteExisting) {
			qDebug() << "Contact is already existing: Overwriting existing contact";

			mContactService.deleteContact(existingContact.id());
			mContactService.createContact(newContact, false);

			retval = Overwritten;
		} else if (mMergePolicy == Merge) {
			// Create new contact
			Contact createdContact = mContactService.createContact(newContact, false);

			qDebug() << "Contact is already existing: Merging contacts " << existingContact.id() << ", " << createdContact.id();

			// And merge contacts afterwards
			QList<ContactId> contactIds;
			contactIds << existingContact.id();
			contactIds << createdContact.id();
			mContactService.mergeContacts(contactIds);

			retval = Merged;
		}
	} else {
		// Save contact to database
		mContactService.createContact(newContact, false);
		retval = Added;
	}

	return retval;
}
