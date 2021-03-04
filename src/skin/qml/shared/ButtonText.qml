import QtQuick 2.15

import "qrc:/styling"

//-----------------------------------------------------------------------------
// For use with in a Button element, allows fine tuning text displayed with in the button.
Text {
    property color fontColor: rootAppPage.currentStyle > 0 ? CustStyle.buttxt_norm_dm : CustStyle.buttxt_norm_lm
    padding: 8
    color: parent.enabled ? fontColor : CustStyle.buttxt_disabled
    anchors.fill: parent
    smooth: true
    font.weight: Font.DemiBold
    font.pixelSize: CustStyle.fsize_button
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    elide: Text.ElideMiddle
    wrapMode: Text.WordWrap
}
