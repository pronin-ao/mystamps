import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle{
    id: showStamp
    color: "#101010"
    property var stampData
    Text{
        id: topText
        color: "#eeeeee"
        anchors.top: parent.top
        width: parent.width
        height: parent.height / 10
        text: {
            parent.stampData.country + ", \t" + parent.stampData.year +
                    ": \t" + parent.stampData.series +
                    ((parent.stampData.spec === "default")?"" : ("\t\t(" +
                    parent.stampData.spec + ")"));
        }
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Image {
        id: stamp
        source: showStamp.stampData.image
        anchors.top: topText.bottom
        anchors.bottom: bottomText.top
        width: parent.width
        fillMode: Image.PreserveAspectFit
        Text{
            visible: true
            anchors.fill: parent
            text: (parent.source == "")
                  ?(showStamp.stampData.country + ", \t" +
                 showStamp.stampData.year + "\n" +
                 showStamp.stampData.color + ", \t" +
                 showStamp.stampData.price + "\n\n" +
                 showStamp.stampData.comments)
                 : showStamp.stampData.comments

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "#777777"
        }
    }
    Text{
        id: bottomText
        color: "#eeeeee"
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height / 10
        text: {
            "id: " + parent.stampData.id + ":" + parent.stampData.code +
                    " \t\t price: "+parent.stampData.price +
                    "\t " + parent.stampData.capture + "\t "+
                    "color: " + parent.stampData.color
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }   

    TapHandler {
        onLongPressed: {
            showStamp.visible = false;
            grid.visible = true;
        }
    }
}
