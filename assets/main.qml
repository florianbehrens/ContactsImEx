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
