import QtQuick 2.15

import "qrc:/styling"
//-----------------------------------------------------------------------------
// Draws a basic outline around a QObject. Is here for quick style changes
// for color attributes or possible back fill options. Just place this in
// the QObject to have an outline drawn around it.
Rectangle {
    id: contextRoot
    // self fills bounds of 'parent' placed within
    implicitWidth: parent.width
    implicitHeight: parent.height
    // the color to use when drawing the outline:
    property color outline_color: ( rootAppPage.currentStyle > 0 ?
        CustStyle.dm_outline : CustStyle.lm_outline )
    // only use the boarder property for supplied color
    color: "transparent"
    border.color: contextRoot.outline_color
    border.width: 1
    radius: 2.0
}
