import bb.cascades 1.0
import bb.cascades.pickers 1.0
import CustomComponents 1.0

Page {
    titleBar: TitleBar {
        title: qsTr("Export contacts to file") + Retranslate.onLocaleOrLanguageChanged
    }
    
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
            
            horizontalAlignment: HorizontalAlignment.Fill
            selectedIndex: 0
            title: qsTr("File format") + Retranslate.onLocaleOrLanguageChanged
            
            Option {
                property string filenameExtension: "vcf" 
                text: qsTr("vCard stream") + " (.vcf)" + Retranslate.onLocaleOrLanguageChanged
            }
            Option { 
                property string filenameExtension: "csv" 
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
                    defaultSaveFileNames: {
                        return new Array("ContactsExport.".concat(filetypeDropDown.options[filetypeDropDown.selectedIndex].filenameExtension)); 
                    }
                    filter: { 
                        return new Array("*.".concat(filetypeDropDown.options[filetypeDropDown.selectedIndex].filenameExtension));
                    }
                    mode: FilePickerMode.Saver
                    directories: [ "/accounts/1000/shared" ]
                    title: qsTr("Select contacts file") + Retranslate.onLocaleOrLanguageChanged

                    onFileSelected: contactsExporter.exportContacts(selectedFiles[0])
                }
            ]
        }

        attachedObjects: [
            ContactsExporter {
                id: contactsExporter
                filetype: filetypeDropDown.selectedIndex
            }
        ]
    }    
}
