/*
 * (c) Copyright Florian Behrens 2013.
 *
 * This file is part of ContactsImEx.
 *
 * ContactsImEx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ContactsImEx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ContactsImEx.  If not, see <http://www.gnu.org/licenses/>.
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
	Q_PROPERTY(bool running READ running NOTIFY runningChanged)

public:
	ContactsBase();

	MergePolicy mergePolicy() const;
	void setMergePolicy(MergePolicy arg);

	bool running() const { return mRunning; }

signals:
	void mergePolicyChanged(MergePolicy arg);
	void runningChanged(bool running);

protected:
	class RunStateActivator
	{
	public:
		explicit RunStateActivator(ContactsBase *contactsBase);
		~RunStateActivator();

	private:
		RunStateActivator(const RunStateActivator&);
		RunStateActivator& operator=(const RunStateActivator&);

		ContactsBase *mContactsBase;
	};

	friend class RunStateActivator;

	RunStateActivator* setRunning();

	static bb::pim::contacts::ContactService mContactService;
	static bool mInitialized;
	static QVector<QString> mAttributeKinds;
	static QVector<QString> mAttributeSubKinds;
	static QString postalAddressAttributePrefix;

	MergePolicy mMergePolicy;

private:
	bool mRunning;
};

#endif /* CONTACTSBASE_H_ */
