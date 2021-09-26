import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "./" as Custom

Rectangle {
    id: content
    property bool stampView: showStamp.visible
    property string year: showStamp.stampData.year
    property string country: showStamp.stampData.country
    property string spec: showStamp.stampData.spec
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
                    source:  modelData.image
                    fillMode: Image.PreserveAspectFit
                    property bool showMore: false
                    Timer {
                        id: timer
                        interval: 500
                        running: false
                        repeat: false

                        onTriggered: {
                            image.showMore = !image.showMore
                        }
                    }
                    TapHandler {
                        //grabPermissions: PointerHandler.TakeOverForbidden
                        onTapped: {
                            if(timer.running) {
                                modelData.checked = !modelData.checked;
                                catalogue.checkStamp(modelData);
                               timer.stop()
                            }else {
                                timer.start()
                            }
                        }
                        onLongPressed: {
                            showStamp.visible = true;
                            showStamp.stampData = modelData;
                            grid.visible = false;
                        }
                    }

                    Rectangle {
                        anchors.fill: parent
                        visible: modelData.checked
                        color: "#5088cc88"
                    }
                    Rectangle {
                        anchors.fill: parent
                        visible: (image.showMore && parent.source != "")
                        color: "#aa000000"
                    }
                    Text{
                        visible: {parent.source == "" || image.showMore}
                        anchors.fill: parent
                        color: image.showMore ? "#fff0f0" :"#cccccc"
                        font.bold: true
                        font.pointSize: 16
                        text: {
                             modelData.country + ",   " +
                             modelData.year + "\n" +
                             modelData.color + ",   " +
                             modelData.price + "\n" +
                             (image.showMore?(
                             modelData.series + "\n" +
                             modelData.capture): "")
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
                    font.pointSize: 18

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
