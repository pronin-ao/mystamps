import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle{
    id: showStamp
    color: "#202020"
    property var stampData
    Text{
        id: topText
        color: "#eeeeee"
        anchors.top: parent.top
        width: parent.width
        height: parent.height / 10
        text: {
            parent.stampData.country + ", \t" + parent.stampData.year +
                    ": \t" + parent.stampData.capture +
                    ((parent.stampData.spec === "default")?"" : ("\t\t(" +
                    parent.stampData.spec + ")"));
        }
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Image {
        id: stamp
        source: parent.stampData.image
        anchors.top: topText.bottom
        anchors.bottom: bottomText.top
        width: parent.width
        fillMode: Image.PreserveAspectFit
    }
    Text{
        id: bottomText
        color: "#eeeeee"
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height / 10
        text: "id: " + parent.stampData.id +"\t price: "+parent.stampData.price
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