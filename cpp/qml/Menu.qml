import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "./" as Custom

Rectangle {
    id: menu
    GridLayout{
        anchors.fill: parent
        columns: Math.max(1,Math.min(4, mainWindow.width / mainWindow.buttonSize))
        rowSpacing: 3
        columnSpacing: 3
        anchors.margins: 2

        Button {
            id: country; Layout.fillWidth: true; Layout.fillHeight: true
            Layout.maximumWidth: 270
            text: "Countries"
            font.bold: true
            font.pointSize: 15

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
            font.bold: true
            font.pointSize: 15

            onClicked: {
                filter.filterModel = catalogue.years
                filter.filtered = catalogue.yearsFilter
                filter.backup = catalogue.yearsFilter.slice();
                filter.visible = true
                content.visible = false
                mainWindow.filterType = "year";
            }
        }
        ColumnLayout{
            Layout.fillWidth: true; Layout.fillHeight: true
            Layout.minimumWidth: 200
            spacing: 2
            Button {
                height: parent.height / 2
                width: parent.width
                id: price; Layout.fillWidth: true; Layout.fillHeight: true
                text: "Prices"
                font.bold: true
                font.pointSize: 15

                onClicked: {
                    filter.filterModel = catalogue.prices
                    filter.filtered = catalogue.priceFilter
                    filter.visible = true;
                    content.visible = false;
                    mainWindow.filterType = "price";
                }
            }
            ComboBox {
                Layout.fillWidth: true; Layout.fillHeight: true
                id: wishOwned;
                height: parent.height / 2
                width: parent.width
                displayText: "Show "+currentText
                model: ["all", "owned", "wishlist"]
                currentIndex: 0
                font.bold: true
                font.pointSize: 15

                onCurrentTextChanged: {
                    catalogue.showMode = currentText
                }
            }
        }


        ColumnLayout{
            Layout.fillWidth: true; Layout.fillHeight: true
            Layout.minimumWidth: 200
            spacing: 2
            ComboBox {
                Layout.fillWidth: true; Layout.fillHeight: true
                id: sorting;
                height: parent.height / 2
                width: parent.width
                displayText: "Sort by "+currentText
                model: ["price", "year", "country", "series"]
                currentIndex: 1
                font.bold: true
                font.pointSize: 15

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
                font.bold: true
                font.pointSize: 15

                onCurrentTextChanged: {
                    catalogue.applyOrder(currentText)
                }
            }
        }
    }
}
