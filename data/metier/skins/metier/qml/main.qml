import QtQuick

Rectangle {
    id: page

    color: "lightgray"
    height: 480
    width: 320

    Text {
        id: helloText

        anchors.horizontalCenter: page.horizontalCenter
        font.bold: true
        font.pointSize: 24
        text: "Hello world!"
        y: 30
    }
}
