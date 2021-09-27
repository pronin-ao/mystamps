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

        onGetFilter: {
            filter.filterModel = list;
        }
    }

    Custom.StampList{
        id: content
        width: parent.width
        height: parent.height - menu.height - actionBar.height
        color: "black"
        anchors.bottom: actionBar.top
    }

    Custom.Menu {
        id: menu
        width: parent.width
        color: "gray"
        height: Math.max(parent.height/8, buttonSize)
    }

    Custom.Filter {
        id: filter
        visible: false
        color: "#3300ff00"
        width: parent.width
        height: parent.height - menu.height - actionBar.height
        anchors.top: menu.bottom

        onApply: {
            if(mainWindow.filterType == "countries")
                catalogue.applyCountriesFilter(new_filter)
            else if(mainWindow.filterType == "year")
                catalogue.applyYearFilter(new_filter)
            else if(mainWindow.filterType == "price")
                catalogue.applyPriceFilter(new_filter)
        }

        onRestore: {
            if(mainWindow.filterType == "countries")
                catalogue.setCountriesFilter(old_filter)
            else if(mainWindow.filterType == "year")
                catalogue.setYearFilter(old_filter)
            else if(mainWindow.filterType == "price")
                catalogue.setPriceFilter(old_filter)
        }

        onFilterText: {
            catalogue.filter(filter, mainWindow.filterType)
        }
    }

    Custom.ActionBar {
        id: actionBar
        height: 110
        width: parent.width
        anchors.bottom: parent.bottom
        color: "darkgrey"
    }


}
