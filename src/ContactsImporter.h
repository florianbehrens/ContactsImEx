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

	Q_INVOKABLE void importVcard(const QStringList &filenames);
	Q_INVOKABLE void importCsv(const QString &filename);

private:
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
