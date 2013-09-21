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

import bb.cascades 1.0

TabbedPane {
    showTabsOnActionBar: true
    
    Tab {
        title: qsTr("Import") + Retranslate.onLocaleOrLanguageChanged
        imageSource: "asset:///images/contacts_import.png"
        
        ImportPage {}
    }

    Tab {
        title: qsTr("Export") + Retranslate.onLocaleOrLanguageChanged
        imageSource: "asset:///images/contacts_export.png"
        
        ExportPage {}
    }

    Tab {
        title: qsTr("About") + Retranslate.onLocaleOrLanguageChanged
        imageSource: "asset:///images/info.png"
        
        InfoPage {}
    }
}
