import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick 2.15

import "./" as Custom

Rectangle{
    function setCharAt(str,index,chr) {
        if(index > str.length-1) return str;
        return str.substring(0,index) + chr + str.substring(index+1);
    }
    function decade(year){
            var begin = year;
            begin = setCharAt(begin,3,'0');
            var end = year
            end = setCharAt(end,3,'9');
            return begin+"-"+end;
        }
    function getFirst(years){
        return years.split('-')[0].replace(' ','');
    }
    function getSpec(spec){
        console.log(spec);
        return spec.replace(/\+/g, '%20');
    }

    function openUrl(url){
        browserWindow.visible = true;
        browser.url = url;
    }

    function shorter(prefix, country){
        var pre = prefix+(prefix.length>0?" ":"");
        if(country.length < 16){
            return pre + country;
        } else if(country.length < 27 && pre.length === 0){
            return country;
        }else if(pre.length === 0) {
            return country.slice(0,25)+"..";
        } else {
            return prefix;
        }
    }

    RowLayout{
        anchors.fill: parent
        spacing: 5
        anchors.margins: 3
        Button{
            text: "SW"
            visible: !yearArea.visible
            Layout.fillHeight: true
            font.bold: true
            font.pointSize: 16

            onClicked: browserWindow.visible = true;

        }
        Rectangle{
            id: yearArea
            visible: false
            color: "white"
            Layout.minimumWidth: 200
            Layout.fillHeight: true
            Layout.fillWidth: true;
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
            text: shorter("", catalogue.countriesFilter[0])
            visible: (catalogue.countriesFilter.length === 1) && !gotoSeries.visible
            Layout.fillHeight: true
            font.bold: true
            font.pointSize: 16
            Layout.fillWidth: true;

            TapHandler {
                gesturePolicy: TapHandler.ReleaseWithinBounds
                onTapped: {
                   openUrl("https://www.stampworld.com/ru/stamps/"+catalogue.countriesFilter[0]+
                                         (yearArea.visible?("/Postage%20stamps/"+
                                                            decade(yearEdit.text)+"?year="+yearEdit.text):""))
                }
                onLongPressed: {
                    yearArea.visible = !yearArea.visible;

                }
            }
        }

        Button{
            text: shorter("My",catalogue.countriesFilter[0])
            visible: (catalogue.countriesFilter.length === 1) && !gotoSeries.visible
            Layout.fillHeight: true
            font.bold: true
            font.pointSize: 16
            Layout.fillWidth: true;

            TapHandler {
                gesturePolicy: TapHandler.ReleaseWithinBounds
                onTapped: {
                    openUrl("https://www.stampworld.com/ru/stamps/"+catalogue.countriesFilter[0]+"/?view=mycatalogue&user=383347&cid=164812"+
                                         (yearArea.visible?("&year="+yearEdit.text):""))
                }
                onLongPressed: {
                    yearArea.visible = !yearArea.visible
                }
            }
        }
        Button{
            text: shorter("Wish", catalogue.countriesFilter[0])
            visible: (catalogue.countriesFilter.length === 1) && !gotoSeries.visible
            Layout.fillHeight: true
            font.bold: true
            font.pointSize: 16
            Layout.fillWidth: true;

            TapHandler {
                gesturePolicy: TapHandler.ReleaseWithinBounds
                onTapped: {
                    openUrl("https://www.stampworld.com/ru/stamps/"+catalogue.countriesFilter[0]+"/?view=wanted&user=383347"+
                                         (yearArea.visible?("&year="+yearEdit.text):""))
                }
                onLongPressed: {
                    yearArea.visible = !yearArea.visible
                }
            }
        }
        Button{
            id: gotoYear
            text: shorter("", content.country)+": "+getFirst(content.year)
            visible: content.stampView
            Layout.fillHeight: true
            font.bold: true
            font.pointSize: 16
            Layout.fillWidth: true;

            onClicked: {
                openUrl("https://www.stampworld.com/ru/stamps/"+content.country+"/"+
                                    ((content.spec=="default")?"Postage%20stamps":getSpec(content.spec))+"/"+
                                                        decade(getFirst(content.year))+
                                                        "&year="+getFirst(content.year))
            }
        }
        Button{
            id: gotoOwned
            text: shorter("My", content.country)
            visible: content.stampView
            Layout.fillHeight: true
            font.bold: true
            font.pointSize: 16
            Layout.fillWidth: true;

            onClicked: {
                openUrl("https://www.stampworld.com/ru/stamps/"+
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
            Layout.fillWidth: true;

            onClicked: {
                openUrl(content.seriesLink)
            }

            onVisibleChanged: {
                if(visible){
                    yearArea.visible = false;
                }
            }
        }
    }



}
