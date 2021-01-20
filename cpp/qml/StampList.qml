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
                color: {
                    modelData.checked
                            ? "green"
                            : (modelData.owned ? "#333355" : "#00000000")
                }
                width: content.width / 2
                height: content.height / 4
                Image{
                    anchors.fill: parent
                    id: image
                    source: {
                        (modelData.image === "") ? "" :
                        "data:image/jpg;base64,"+modelData.image
                    }
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
                    Text{
                        visible: {parent.source == ""}
                        anchors.fill: parent
                        color: "#cccccc"
                        text: {
                             modelData.country + ",   " +
                             modelData.year + "\n" +
                             modelData.color + ",   " +
                             modelData.price
                        }
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Text{
                    anchors.fill: parent
                    visible: modelData.owned
                    color: "#bb3300"
                    font.bold: true
                    text: {
                        ((modelData.conditions.indexOf("mnh")>=0)
                         ? "**\n" : "") +
                        ((modelData.conditions.indexOf("mint")>=0)
                         ? "*\n": "") +
                        ((modelData.conditions.indexOf("used")>=0)
                         ?"#\n":"") +
                        ((modelData.conditions.indexOf("fdc")>=0)
                         ?"[fdc]":"")
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
