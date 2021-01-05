import QtQuick 2.3
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import mystamps.data 1.0

import "qml/" as Custom


Window{
    visible: true
    title: "mystamps"
    width: 1080
    height: 2160
    id: mainWindow
    readonly property int buttonSize: 70
    property string filterType: "none"

    Catalogue{
        id: catalogue
        years: [0]
        countries: ["all"]
    }

    Rectangle {
        id: menu
        width: parent.width
        color: "gray"
        height: Math.max(parent.height/6, buttonSize)

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
            ComboBox {
                id: value; Layout.fillWidth: true; Layout.fillHeight: true
                model: ["price", "year", "country"]
            }
        }
    }

    Custom.Filter {
        id: filter

        onApply: {
            if(mainWindow.filterType == "countries")
                catalogue.applyCountriesFilter(new_filter)
            else if(mainWindow.filterType == "year")
                catalogue.applyYearFilter(new_filter)
        }

        onRestore: {
            if(mainWindow.filterType == "countries")
                catalogue.setCountriesFilter(old_filter)
            else if(mainWindow.filterType == "year")
                catalogue.setYearFilter(old_filter)
        }
    }

    Rectangle {
        id: content
        width: parent.width
        height: parent.height - menu.height
        color: "black"
        anchors.top: menu.bottom

        GridView {
            id: grid
            anchors.fill: parent
            cellWidth: content.width / 2
            cellHeight: content.height / 5

            Component{
                id: stampDelegate
                Rectangle{
                    Image{
                        id: image
                        source: modelData
                    }
                    height: image.contentHeight
                    width: image.contentWidth
                }
            }

            model: catalogue.stamps
            delegate: stampDelegate

        }
    }
}
