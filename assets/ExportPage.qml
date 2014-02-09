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
        title: qsTr("Export contacts to file") + Retranslate.onLocaleOrLanguageChanged
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
	            text: qsTr("Choose between vCards stream (version 3.0) or comma separated values file (no contact pictures included).") + Retranslate.onLocaleOrLanguageChanged
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
	            text: qsTr("Select whether to export all local contacts or just a selection.") + Retranslate.onLocaleOrLanguageChanged
	        }
	
	        CheckBox {
	            id: exportAll
	            text: qsTr("Export all local contacts") + Retranslate.onLocaleOrLanguageChanged
	            checked: true
	        }
	
	        Divider {}
	        
	        Label {
	            multiline: true
	            text: exportAll.checked ? 
	            	qsTr("Press the Export button to select the file to export your contacts to.") + Retranslate.onLocaleOrLanguageChanged :
	            	qsTr("Press the Export button to make a selection of your contacts and select the file to export your contacts to.") + Retranslate.onLocaleOrLanguageChanged            	    
	        }
	        
	        Button {
	            horizontalAlignment: HorizontalAlignment.Fill
	            text: qsTr("Export")
	            onClicked: {
	                contactsExporter.contacts = [];
	                if (exportAll.checked)
	            	   filePicker.open();
	            	else 
	            		contactPicker.open();
	            }
	
	            attachedObjects: [
	                ContactPicker {
	                    id: contactPicker
	                    mode: ContactSelectionMode.Multiple
	                    title: qsTr("Select contact(s)") + Retranslate.onLocaleOrLanguageChanged
	                    
	                    onContactsSelected/*(QList<int> contactIds)*/: {
	                        if (contactIds.length > 0) {
	                            contactsExporter.contacts = contactIds;
	                            filePicker.open();
	                        }
	                    }
	                },
	                FilePicker {
	                    id: filePicker
	                    allowOverwrite: true
                        defaultSaveFileNames: filetypeDropDown.selectedIndex == filetypeDropDown.vcf ? "ContactsExport.vcf" : "ContactsExport.csv"
                        filter: filetypeDropDown.selectedIndex == filetypeDropDown.vcf ? "*.vcf" : "*.csv" 
	                    mode: FilePickerMode.Saver
	                    directories: [ "/accounts/1000/shared" ]
	                    title: qsTr("Select file") + Retranslate.onLocaleOrLanguageChanged
	
	                    onFileSelected: contactsExporter.exportContacts(selectedFiles[0])
	                }
	            ]
	        }
	
	        attachedObjects: [
	            ContactsExporter {
	                id: contactsExporter
	                filetype: filetypeDropDown.selectedIndex
                    onRunningChanged: Application.mainWindow.screenIdleMode = (running ? 1 /*ScreenIdleMode.KeepAwake*/ : 0 /*ScreenIdleMode.Normal*/)
	            }
	        ]
	    }    
	}
}
