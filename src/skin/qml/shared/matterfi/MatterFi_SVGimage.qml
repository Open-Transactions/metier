import QtQuick 2.15
import QtQuick.Controls.Universal 2.12

//import Qt5Compat.GraphicalEffects 1.0  TODO: install Qt5Compat package.

import "qrc:/styling"

//-----------------------------------------------------------------------------
// 'MatterFi_SVGimage.qml'
// Used to display SVG images from the 'qrc:/assets/svg/' directory.
Item {
    id: contextRoot
    property var objectName: "" // used to provide context of ownership

    property string svgFile: "Matterfi-normal" // what svg to display
    // Is the svg part of Font Awesome (FA) ?
    property bool isFAicon_solid: false
    property bool isFAicon_reg:   false

    property int imgPadding: 0 // space around the image
    
    property color bgcolor: "transparent"
    // Sets the color overlay value for the svg image,
    // this color should be "transparent" if loading an svg
    // that is not part of Font Awesome
    property color color: rootAppPage.currentStyle > 0 ? CustStyle.dm_icon_but : CustStyle.lm_icon_but

    //-----------------------------------------------------------------------------
    // You'll need to make sure the icon is in the qrc resources if using one of
    // the included FA (Font Awesome) icons. Or you can load an svg from the
    // assets directory by keeping the FA flags to the default 'false' values.
    function getFilename() {
        if (isFAicon_solid) {
            return "qrc:/assets/svgs/FA5-Free/solid/" + svgFile + ".svg"
        } else if (isFAicon_reg) {
            return "qrc:/assets/svgs/FA5-Free/reg/" + svgFile + ".svg"
        } else {
            return "qrc:/assets/svgs/" + svgFile + ".svg"
        }
    }

    //-----------------------------------------------------------------------------
    Rectangle {
        id: body
        width: contextRoot.width
        height: contextRoot.height
        anchors.fill: parent
        color: contextRoot.bgcolor
        // Draw the SVG:
        Image {
            id: svgImage
            width: body.width - contextRoot.imgPadding
            height: body.height - contextRoot.imgPadding
            sourceSize.width: body.width - contextRoot.imgPadding
            sourceSize.height: body.height - contextRoot.imgPadding
            anchors.horizontalCenter: body.horizontalCenter
            anchors.verticalCenter: body.verticalCenter
            source: contextRoot.getFilename()
            fillMode: Image.PreserveAspectFit
            smooth: true
        }
        //ColorOverlay {  // TODO: requires 'GraphicalEffects'
        //    anchors.fill: svgImage
        //    source: svgImage
        //    color: contextRoot.color
        //}
    }
}
