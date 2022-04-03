import QtQuick 2.6
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle{
    id: root
    property var filterModel: []
    property var filtered: []

    property var backup: []

    signal apply(var new_filter)
    signal restore(var old_filter)
    signal filterText(var filter)

    ListView {
        id: list
        anchors.bottom: parent.bottom
        height: parent.height * 3 / 4
        width: parent.width
        cacheBuffer: 0

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
                background: Rectangle {
                    visible: true
                    color: checked ? "#aaaaff": "#00000000"
                }
            }
        }
        model: parent.filterModel
        delegate: filterDelegate
    }


    RowLayout {
        id: result
        anchors.top: parent.top
        height: parent.height / 16
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
                content.setDefaultView();
            }
        }
    }

    Button {
        id: clear
        anchors.top: result.bottom
        anchors.left: parent.left
        height: parent.height / 16
        width: parent.width/10
        text:  "X"
        background: Rectangle {
            color: "#ff6655"
        }

        onClicked: {
            search.text = ""
        }
    }

    TextEdit{
        id: search
        anchors.top: result.bottom
        anchors.left: clear.right
        height: parent.height / 16
        width: parent.width*0.8
        inputMethodHints: Qt.ImhUrlCharactersOnly
    }

    Button {
        anchors.top: result.bottom
        anchors.left: search.right
        height: parent.height / 16
        width: parent.width/10
        text: "Ok"
        background: Rectangle {
            color: "#33ff44"
        }

        onClicked: {
            if(search.text.length > 0)
                root.filterText(search.text)
        }

    }

    RowLayout{
        id: quickers
        anchors.top: search.bottom
        height: parent.height / 8
        width: parent.width
        spacing: 2

        Button {
            id: all
            text: "all"
            Layout.fillWidth: true; Layout.fillHeight: true
            Layout.topMargin: 2; Layout.leftMargin: 2
            onClicked: {
                filtered = filterModel;
            }
        }
        Button {
            id: inv
            text: "inv"
            Layout.fillWidth: true; Layout.fillHeight: true
            Layout.topMargin: 2
            onClicked: {
                var new_filtered = filterModel.slice();
                var value;
                for(value of filtered){
                    var ind = new_filtered.indexOf(value);
                    if(ind >= 0){
                        new_filtered.splice(ind,1);
                    }
                }
                filtered = []
                filtered = new_filtered.slice()
            }
        }
        Button {
            id: none
            text: "none"
            Layout.fillWidth: true; Layout.fillHeight: true
            Layout.topMargin: 2; Layout.rightMargin: 2
            onClicked: {
                filtered = [];
            }
        }
    }

}

