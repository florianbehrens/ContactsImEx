#include <QLocale>
#include <QTranslator>
#include <QtDeclarative>
#include <Qt/qdeclarativedebug.h>

#include <bb/cascades/Application>
#include <bb/cascades/pickers/ContactPicker>
#include <bb/cascades/pickers/ContactSelectionMode>

#include "applicationui.hpp"
#include "ContactsExporter.h"
#include "ContactsImporter.h"

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);

    // Load custom QML components
    ContactsExporter::qmlRegisterType();
    ContactsImporter::qmlRegisterType();

    // TODO: The following is a workaround only, since the ContactPicker class
    // seems not to be exposed to QML.
    qmlRegisterType<bb::cascades::pickers::ContactPicker>("bb.cascades.pickers", 1, 0, "ContactPicker");
    qmlRegisterUncreatableType<bb::cascades::pickers::ContactSelectionMode>("bb.cascades.pickers", 1, 0, "ContactSelectionMode", "ContactSelectionMode cannot be instantiated");
    //qmlRegisterType<bb::cascades::pickers::ContactSelectionMode>("bb.cascades.pickers", 1, 0, "ContactSelectionMode");

    // Create the Application UI object, this is where the main.qml file
    // is loaded and the application scene is set.
    new ApplicationUI(&app);

    // Enter the application main event loop.
    return Application::exec();
}
