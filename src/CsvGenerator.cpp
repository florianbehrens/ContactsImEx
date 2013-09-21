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

#include "CsvGenerator.h"

#include <QIODevice>
#include <QString>

CsvGenerator::CsvGenerator(QIODevice &ioDevice)
  : mIoDevice(ioDevice),
    mLineStart(true)
{}

CsvGenerator& CsvGenerator::operator<<(QString op)
{
	// Escape special characters: '\', '"', newline
	op.replace(QString("\\"), QString("\\\\"));
	op.replace(QString("\""), QString("\\\""));
	op.replace(QString("\r\n"), QString("\\n"));
	op.replace(QString("\n"), QString("\\n"));

	if (!mLineStart)
		mIoDevice.write(",");
	mIoDevice.write("\"");
	mIoDevice.write(op.toUtf8());
	mIoDevice.write("\"");

	mLineStart = false;

	return *this;
}

void CsvGenerator::terminateLine()
{
	mIoDevice.write("\n");
	mLineStart = true;
}
