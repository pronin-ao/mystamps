import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle{
    id: root
    color: "#aaaaff"
    width: parent.width
    height: parent.height - menu.height
    anchors.top: menu.bottom
    property var filterModel: []
    property var filtered: []

    property var backup: []

    signal apply(var new_filter)
    signal restore(var old_filter)


    RowLayout {
        id: result
        anchors.top: parent.top
        height: parent.height / 8
        width: parent.width
        spacing: 2

        Button {
            id: cancel
            text: "Back"
            Layout.fillWidth: true; Layout.fillHeight: true
            Layout.topMargin: 2; Layout.leftMargin: 2
            onClicked: {
                filter.visible = false;
                content.visible = true;
                root.restore(backup)
            }
        }
        Button {
            id: apply
            text: "Apply"
            Layout.fillWidth: true; Layout.fillHeight: true
            Layout.topMargin: 2; Layout.rightMargin: 2
            onClicked: {
                root.apply(filtered)
                filter.visible = false;
                content.visible = true;
            }
        }
    }

    RowLayout{
        id: quickers
        anchors.top: result.bottom
        height: parent.height / 8
        width: parent.width
        spacing: 2

        Button {
            id: all
            text: "all"
            Layout.fillWidth: true; Layout.fillHeight: true
            Layout.topMargin: 2; Layout.leftMargin: 2
        }
        Button {
            id: inv
            text: "inv"
            Layout.fillWidth: true; Layout.fillHeight: true
            Layout.topMargin: 2
        }
        Button {
            id: none
            text: "none"
            Layout.fillWidth: true; Layout.fillHeight: true
            Layout.topMargin: 2; Layout.rightMargin: 2
        }
    }

    ListView {
        id: list
        anchors.top: quickers.bottom
        height: parent.height * 3 / 4
        width: parent.width

        Component {
            id: filterDelegate
            CheckDelegate {
                height: list.height / 10
                width: list.width
                text: modelData
                checked: {filtered.indexOf(modelData) >= 0}

                onCheckedChanged: {
                    if(checked) {
                        if(filtered.indexOf(modelData) < 0)
                            filtered.push(modelData);
                    }
                    else {
                        var ind = filtered.indexOf(modelData);
                        filtered.splice(ind, 1);
                    }
                }
            }
        }

        model: parent.filterModel
        delegate: filterDelegate
    }


}

