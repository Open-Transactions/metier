import QtQuick 2.15
import QtQuick.Controls.Universal 2.12

import "qrc:/styling"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
Item {
    id: contextRoot
    visible: true
    width: 60
    height: 60

    Image {
        id: imageObject
        width: contextRoot.width
        height: contextRoot.height
        sourceSize.width: width
        sourceSize.height: height
        anchors.horizontalCenter: parent.horizontalCenter
        source: "qrc:/assets/svgs/pkt-icon.svg"
        fillMode: Image.PreserveAspectFit
        smooth: true
    }
}
