/*
 * ContactsBase.h
 *
 *  Created on: 29.08.2013
 *      Author: willy
 */

#ifndef CONTACTSBASE_H_
#define CONTACTSBASE_H_

#include <QObject.h>
#include <QVector>

#include <bb/pim/contacts/ContactService>

class ContactsBase : public QObject
{
	Q_OBJECT

public:
	/// @brief The merge policy when an imported contact already exists.
	enum MergePolicy {
		KeepExisting,		//< Keep existing contact, discard imported one.
		OverwriteExisting,	//< Overwrite existing contact with imported one.
		Merge,				//< Add attributes from imported to existing contact.
		NoCheck				//< Don't check for local contacts.
	};

	Q_ENUMS(MergePolicy);
	Q_PROPERTY(MergePolicy mergePolicy READ mergePolicy WRITE setMergePolicy NOTIFY mergePolicyChanged)

public:
	ContactsBase();

	MergePolicy mergePolicy() const;
	void setMergePolicy(MergePolicy arg);

signals:
	void mergePolicyChanged(MergePolicy arg);

protected:
	static bb::pim::contacts::ContactService mContactService;
	static bool mInitialized;
	static QVector<QString> mAttributeKinds;
	static QVector<QString> mAttributeSubKinds;
	static QString postalAddressAttributePrefix;

	MergePolicy mMergePolicy;
};

#endif /* CONTACTSBASE_H_ */
