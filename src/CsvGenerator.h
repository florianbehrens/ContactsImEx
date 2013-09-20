/*
 * CsvGenerator.h
 *
 *  Created on: 14.09.2013
 *      Author: willy
 */

#ifndef CSVGENERATOR_H_
#define CSVGENERATOR_H_

#include <QString>

class QIODevice;

class CsvGenerator {
public:
	explicit CsvGenerator(QIODevice &ioDevice);

	CsvGenerator &operator<<(QString op);
	void terminateLine();

private:
	QIODevice &mIoDevice;
	bool mLineStart;
};

#endif /* CSVGENERATOR_H_ */
