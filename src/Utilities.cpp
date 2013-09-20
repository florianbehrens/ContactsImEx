/*
 * Utilities.cpp
 *
 *  Created on: 27.08.2013
 *      Author: willy
 */

#include "Utilities.h"

#include <QObject>
#include <QString>
#include <bb/system/SystemToast>

void Utilities::showSystemToast(QObject *parent, const QString &text)
{
	bb::system::SystemToast *toast = new bb::system::SystemToast(parent);
	toast->setBody(text);
	toast->setPosition(bb::system::SystemUiPosition::MiddleCenter);
	toast->show();
}
