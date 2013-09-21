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
import bb.cascades.pickers 1.0
import CustomComponents 1.0

Page {
    titleBar: TitleBar {
        title: qsTr("Import contacts from file") + Retranslate.onLocaleOrLanguageChanged
    }
    
    Container {
        topPadding: 30
        bottomPadding: 30
        leftPadding: 20
        rightPadding: 20
        
        Label {
            multiline: true
            text: qsTr("Import contacts from vCards stream (version 3.0) or comma separated values file on device or media card.") + Retranslate.onLocaleOrLanguageChanged
        }

        Label {
            multiline: true
            text: qsTr("To find out about the required CSV header row values export a reference contact first.") + Retranslate.onLocaleOrLanguageChanged
        }
        
        Divider {}
        
        Label {
            multiline: true
            text: qsTr("The merge policy determines what to do with imported contacts that already exist in the local address book.") + Retranslate.onLocaleOrLanguageChanged
        }

        DropDown {
            id: mergePolicyDropDown
            horizontalAlignment: HorizontalAlignment.Fill
            selectedIndex: 0
            title: qsTr("Merge policy") + Retranslate.onLocaleOrLanguageChanged
                
            Option { text: qsTr("Keep existing contact") + Retranslate.onLocaleOrLanguageChanged }
            Option { text: qsTr("Overwrite existing contact") + Retranslate.onLocaleOrLanguageChanged }
            Option { text: qsTr("Merge contacts") + Retranslate.onLocaleOrLanguageChanged }
            Option { text: qsTr("Add new contact") + Retranslate.onLocaleOrLanguageChanged }
        }

        Divider {}
        
        Label {
            multiline: true
            text: qsTr("After pressing the Import button you can select the file to import your contacts from.") + Retranslate.onLocaleOrLanguageChanged
        }
        
        Button {
            horizontalAlignment: HorizontalAlignment.Fill
            text: qsTr("Import")
            onClicked: filePicker.open()

            attachedObjects: [
                FilePicker {
                    id: filePicker
                    filter: [ "*.vcf", "*.csv" ]
                    mode: FilePickerMode.Picker
                    title: qsTr("Select contacts file") + Retranslate.onLocaleOrLanguageChanged
                    directories: [ "/accounts/1000/shared" ]
                    onFileSelected: contactsImporter.process(selectedFiles[0]);
                }
            ]
        }

        attachedObjects: [
	        ContactsImporter {
	            id: contactsImporter
                mergePolicy: mergePolicyDropDown.selectedIndex
	        }
        ]
    }
}
