/*
 * ContactsImporter.h
 *
 *  Created on: 29.08.2013
 *      Author: willy
 */

#ifndef CONTACTSIMPORTER_H_
#define CONTACTSIMPORTER_H_

#include "ContactsBase.h"

// Forward declarations
namespace bb
{
namespace pim
{
namespace contacts
{
class Contact;
}
}
}

class QFile;
class QString;

class ContactsImporter : public ContactsBase
{
	Q_OBJECT

public:
	static void qmlRegisterType();

	/// Starts the contacts import procedure.
	Q_INVOKABLE void process(const QString &filename);

private:
	void importVcard(QFile &file);
	void importCsv(QFile &file);

	enum ImportResult {
		Invalid,
		Ignored,
		Overwritten,
		Merged,
		Added
	};

	/// Takes a single contact and processes it according to policy.
	ImportResult importContact(bb::pim::contacts::Contact& contact);
};

#endif /* CONTACTSIMPORTER_H_ */
