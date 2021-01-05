import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "./" as Custom

Rectangle {
    id: menu
    GridLayout{
        anchors.fill: parent
        columns: Math.max(1,Math.min(3, mainWindow.width / mainWindow.buttonSize))
        rowSpacing: 2
        columnSpacing: 2
        Button {
            id: country; Layout.fillWidth: true; Layout.fillHeight: true
            text: "Countries"

            onClicked: {
                filter.filtered = catalogue.countriesFilter;
                filter.backup = catalogue.countriesFilter.slice();
                filter.filterModel = catalogue.countries;
                filter.visible = true;
                content.visible = false;
                mainWindow.filterType = "countries";
            }
        }
        Button {
            id: year; Layout.fillWidth: true; Layout.fillHeight: true
            text: "Year"

            onClicked: {
                filter.filterModel = catalogue.years
                filter.filtered = catalogue.yearsFilter
                filter.visible = true
                content.visible = false
                mainWindow.filterType = "year";
            }
        }
        ColumnLayout{
            Layout.fillWidth: true; Layout.fillHeight: true
            spacing: 1
            ComboBox {
                Layout.fillWidth: true; Layout.fillHeight: true
                id: sorting;
                height: parent.height / 2
                width: parent.width
                displayText: "Sort by "+currentText
                model: ["price", "year", "country", "series"]
                currentIndex: 1

                onCurrentTextChanged: {
                    catalogue.applySort(currentText)
                }
            }
            ComboBox {
                Layout.fillWidth: true; Layout.fillHeight: true
                id: order;
                height: parent.height / 2
                width: parent.width
                displayText: "Order: "+currentText
                model: ["raise", "fall"]
                currentIndex: 0

                onCurrentTextChanged: {
                    catalogue.applyOrder(currentText)
                }
            }
        }
    }
}
