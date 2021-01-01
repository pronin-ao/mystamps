import QtQuick 2.3
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window{
    visible: true
    title: "pystamps"
    visibility: "Maximized"
    id: mainWindow
    readonly property int buttonSize: 70
    Rectangle {
        id: menu
        width: parent.width
        color: "gray"
        height: Math.max(parent.height/4, buttonSize*3)

        GridLayout{
            anchors.fill: parent
            columns: Math.max(1,Math.min(3, mainWindow.width / mainWindow.buttonSize))
            rowSpacing: 2
            columnSpacing: 2
            Button {id: country; text: "Country"; Layout.fillWidth: true; Layout.fillHeight: true}
            Button {id: year; text: "Year"; Layout.fillWidth: true; Layout.fillHeight: true}
            Button {id: value; text: "Value"; Layout.fillWidth: true; Layout.fillHeight: true}
        }
    }

    Rectangle {
        id: content
        width: parent.width
        height: parent.height - menu.height
        color: "black"
        anchors.top: menu.bottom

    }
}