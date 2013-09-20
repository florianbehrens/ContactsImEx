// (c) 2013 Florian Behrens.

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
