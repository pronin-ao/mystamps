import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "./" as Custom

Rectangle {
    id: content
    GridView {
        id: grid
        anchors.fill: parent
        cellWidth: content.width / 2
        cellHeight: content.height / 4
        visible: true

        Component{
            id: stampDelegate
            Rectangle{
                color: modelData.checked ? "green" : "#00000000"
                width: content.width / 2
                height: content.height / 4
                Image{
                    anchors.fill: parent
                    id: image
                    source: modelData.image
                    fillMode: Image.PreserveAspectFit
                    TapHandler {
                        onDoubleTapped: {
                            showStamp.visible = true;
                            showStamp.stampData = modelData;
                            grid.visible = false;
                        }
                        onLongPressed: {
                            modelData.checked = !modelData.checked;
                            catalogue.checkStamp(modelData);
                        }
                    }

                    Rectangle {
                        anchors.fill: parent
                        visible: modelData.checked
                        color: "#5088cc88"
                    }
                }
            }
        }

        model: catalogue.stamps
        delegate: stampDelegate

    }

    Custom.Stamp{
        id: showStamp
        visible: false
        anchors.fill: parent
    }
}
