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
        title: qsTr("Import contacts from file(s)") + Retranslate.onLocaleOrLanguageChanged
    }
    
    // The ScrollView helps for smaller screens, or when DropDown controls are 
    // expanded.
    ScrollView {
	    Container {
	        topPadding: 30
	        bottomPadding: 30
            leftPadding: 20
            rightPadding: 20

            Label {
                multiline: true
                text: qsTr("Choose between one or more vCards stream (version 3.0) files or a single comma separated values file.") + Retranslate.onLocaleOrLanguageChanged
            }

            DropDown {
                id: filetypeDropDown

                // Enums for import file type (readonly properties seem not to
                // be supported by Blackberry OS 10)
                /*readonly*/ property int vcf: 0
                /*readonly*/ property int csv: 1

                horizontalAlignment: HorizontalAlignment.Fill
                selectedIndex: 0
                title: qsTr("File format") + Retranslate.onLocaleOrLanguageChanged

                Option {
                    text: qsTr("vCard stream") + " (.vcf)" + Retranslate.onLocaleOrLanguageChanged
                }
                Option {
                    text: qsTr("Comma seperated") + " (.csv)" + Retranslate.onLocaleOrLanguageChanged
                }
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
	            text: qsTr("After pressing the Import button you can select the file(s) to import your contacts from.") + Retranslate.onLocaleOrLanguageChanged
	        }
	        
	        Button {
	            horizontalAlignment: HorizontalAlignment.Fill
	            text: qsTr("Import")
	            onClicked: filePicker.open()

	            attachedObjects: [
	                FilePicker {
	                    id: filePicker
	                    filter: filetypeDropDown.selectedIndex == filetypeDropDown.vcf ? [ "*.vcf" ] : [ "*.csv" ]
	                    mode: filetypeDropDown.selectedIndex == filetypeDropDown.vcf ? FilePickerMode.PickerMultiple : FilePickerMode.Picker
	                    title: qsTr("Select file(s)") + Retranslate.onLocaleOrLanguageChanged
	                    directories: [ "/accounts/1000/shared" ]
	                    onFileSelected: filetypeDropDown.selectedIndex == filetypeDropDown.vcf ? contactsImporter.importVcard(selectedFiles) : contactsImporter.importCsv(selectedFiles[0])
	                }
	            ]
	        }

	        attachedObjects: [
		        ContactsImporter {
		            id: contactsImporter
	                mergePolicy: mergePolicyDropDown.selectedIndex
                    onRunningChanged: Application.mainWindow.screenIdleMode = (running ? 1 /*ScreenIdleMode.KeepAwake*/ : 0 /*ScreenIdleMode.Normal*/)
		        }
	        ]
        }
    }
}
