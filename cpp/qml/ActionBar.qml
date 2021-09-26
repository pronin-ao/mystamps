import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick 2.15

import "./" as Custom

Rectangle{
    function decade(year){
            var begin = year;
            begin=begin.replace(year[3],'0')
            var end = year
            end=end.replace(year[3],'9')
            return begin+"-"+end;
        }
    function getFirst(years){
        return years.split('-')[0].replace(' ','');
    }
    function getSpec(spec){
        console.log(spec);
        return spec.replace(/\+/g, '%20');
    }

    RowLayout{
        anchors.fill: parent
        spacing: 5
        anchors.margins: 3
        Button{
            text: "SW"
            onClicked: Qt.openUrlExternally("https://www.stampworld.com/ru/maps")
            visible: !yearArea.visible
            Layout.fillHeight: true
            font.bold: true
            font.pointSize: 16
        }
        Rectangle{
            id: yearArea
            visible: false
            color: "white"
            Layout.minimumWidth: 200
            Layout.fillHeight: true
            TextEdit{
                id: yearEdit
                anchors.fill: parent
                font.bold: true
                font.pointSize: 16
                text: "1840"
                color: "black"
                inputMethodHints: Qt.ImhDigitsOnly
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
            }
        }
        Button{
            text: catalogue.countriesFilter[0]
            visible: (catalogue.countriesFilter.length === 1) && !gotoSeries.visible
            Layout.fillHeight: true
            font.bold: true
            font.pointSize: 16

            TapHandler {
                gesturePolicy: TapHandler.ReleaseWithinBounds
                onTapped: {
                    Qt.openUrlExternally("https://www.stampworld.com/ru/stamps/"+catalogue.countriesFilter[0]+
                                         (yearArea.visible?("/Postage%20stamps/"+
                                                            decade(yearEdit.text)+"?year="+yearEdit.text):""))
                }
                onLongPressed: {
                    yearArea.visible = !yearArea.visible;

                }
            }
        }

        Button{
            text: "My "+catalogue.countriesFilter[0]
            visible: (catalogue.countriesFilter.length === 1) && !gotoSeries.visible
            Layout.fillHeight: true
            font.bold: true
            font.pointSize: 16

            TapHandler {
                gesturePolicy: TapHandler.ReleaseWithinBounds
                onTapped: {
                    Qt.openUrlExternally("https://www.stampworld.com/ru/stamps/"+catalogue.countriesFilter[0]+"/?view=mycatalogue&user=383347&cid=164812"+
                                         (yearArea.visible?("&year="+yearEdit.text):""))
                }
                onLongPressed: {
                    yearArea.visible = !yearArea.visible
                }
            }
        }
        Button{
            text: "Wish "+catalogue.countriesFilter[0]
            visible: (catalogue.countriesFilter.length === 1) && !gotoSeries.visible
            Layout.fillHeight: true
            font.bold: true
            font.pointSize: 16

            TapHandler {
                gesturePolicy: TapHandler.ReleaseWithinBounds
                onTapped: {
                    Qt.openUrlExternally("https://www.stampworld.com/ru/stamps/"+catalogue.countriesFilter[0]+"/?view=wanted&user=383347"+
                                         (yearArea.visible?("&year="+yearEdit.text):""))
                }
                onLongPressed: {
                    yearArea.visible = !yearArea.visible
                }
            }
        }
        Button{
            id: gotoYear
            text: content.country+": "+getFirst(content.year)
            visible: content.stampView
            Layout.fillHeight: true
            font.bold: true
            font.pointSize: 16

            onClicked: {
                Qt.openUrlExternally("https://www.stampworld.com/ru/stamps/"+content.country+"/"+
                                    ((content.spec=="default")?"Postage%20stamps":getSpec(content.spec))+"/"+
                                                        decade(getFirst(content.year))+
                                                        "&year="+getFirst(content.year))
            }
        }
        Button{
            id: gotoOwned
            text: "My "+content.country
            visible: content.stampView
            Layout.fillHeight: true
            font.bold: true
            font.pointSize: 16

            onClicked: {
                Qt.openUrlExternally("https://www.stampworld.com/ru/stamps/"+
                                     content.country+
                                     ((content.spec=="default")?"":("/"+getSpec(content.spec)))
                                     +"?view=mycatalogue&user=383347&cid=164812"+
                                     "&year="+getFirst(content.year))
            }
        }
        Button{
            id: gotoSeries
            text: "Series"
            visible: content.stampView
            Layout.fillHeight: true
            font.bold: true
            font.pointSize: 16

            onClicked: {
                console.log(content.spec)
            }

            onVisibleChanged: {
                if(visible){
                    yearArea.visible = false;
                }
            }
        }
    }



}
