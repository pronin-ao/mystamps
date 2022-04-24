import QtQuick 2.15
import QtQuick.Controls 2.15
import QtMultimedia 5.15

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
        VideoOutput {
            id: cameraFrame
            source: camera
            anchors.fill: parent
            visible: false

            MouseArea {
                anchors.fill: parent;
                onClicked: camera.imageCapture.captureToLocation(catalogue.location);
            }

            Camera {
                id: camera
                position: Camera.BackFace

                imageCapture {
                    onImageSaved: {
                        catalogue.getImage(showStamp.stampData, path);
                        cameraFrame.visible = false;
                        showStamp.visible = false;
                        grid.visible = true;
                    }
                }
            }
        }
    }
    Text{
        id: bottomText
        color: "#eeeeee"
        anchors.bottom: parent.bottom
        width: parent.width*9/10
        height: parent.height / 10
        text: {
            "id: " + parent.stampData.id + ":" + parent.stampData.code +
                    " \t price: "+parent.stampData.price +
                    "\t " + parent.stampData.capture + "\t "+
                    "color: " + parent.stampData.color
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Button{
        id: makePhoto
        visible:  QtMultimedia.availableCameras.length !== 0
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: bottomText.height
        height: bottomText.height

        onClicked: {
            cameraFrame.visible = true;
        }
    }

    TapHandler {
        onLongPressed: {
            showStamp.visible = false;
            grid.visible = true;
        }
    }
}
