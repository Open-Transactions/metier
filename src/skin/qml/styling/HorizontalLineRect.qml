import QtQuick 2.15
import QtQuick.Controls.Universal 2.12

import "qrc:/styling"

//-----------------------------------------------------------------------------
// Used for a styled horizonatal line divider for list elements.
Item {
    id: contextRoot

    property color color: CustStyle.neutral_fill

    property int bottomPadding: 0
    property int topPadding: 0

    Rectangle {
        color: "transparent"
        height: contextRoot.height + contextRoot.topPadding + contextRoot.bottomPadding
        width: contextRoot.width

        Rectangle {
            color: contextRoot.color
            height: 1
            width: parent.width
            anchors.verticalCenter: parent.verticalCenter
            anchors.bottomMargin: contextRoot.bottomPadding
            anchors.topMargin: contextRoot.topPadding
        }
    }
}
