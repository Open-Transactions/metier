import QtQuick 2.15
import QtQuick.Controls.Universal 2.12


import "qrc:/styling"

//-----------------------------------------------------------------------------
// SvgIconButton.qml
// Display a SVG image as an interactable screen Item.
Item {
    id: contextRoot
    // display configuration propterties
    property string iconFile: "Matterfi-normal"
    property bool   isFAicon_solid: false
    property bool   isFAicon_reg: false
    property color  color: rootAppPage.currentStyle > 0 ? CustStyle.dm_icon_but : CustStyle.lm_icon_but
    property color  bgcolor: "transparent"
    property int    iconSize: 18
    property int    iconPadding: 0
    property var    objectName: ""
    property int    radius: 2
    property bool   isEnabled: true
    // padding around icon
    property int bottomPadding: 0
    property int topPadding: 0
    // is square all the time
    width:  iconSize
    height: iconSize

    signal action() // emmited when clicked on.

    //-----------------------------------------------------------------------------
    // You'll need to make sure the icon is in the qrc resources.
    function getFilename() {
        if (isFAicon_solid) {
            return "qrc:/assets/svgs/FA5-Free/solid/" + iconFile + ".svg"
        } else if (isFAicon_reg) {
            return "qrc:/assets/svgs/FA5-Free/reg/" + iconFile + ".svg"
        } else {
            return "qrc:/assets/svgs/" + iconFile + ".svg"
        }
    }

    //-----------------------------------------------------------------------------
    // Maintains the size of the interaction object's footprint while animating.
    Rectangle {
        id: trueBody
        radius: contextRoot.radius
        width: contextRoot.width
        height: contextRoot.height
        color: contextRoot.bgcolor
        anchors.fill: parent
        
        // The insides that get animated when interaction is detected.
        Rectangle {
            id: body
            width: parent.width
            height: parent.height
            radius: contextRoot.radius
            color: contextRoot.bgcolor
            property bool shrunk: false
            scale: body.shrunk ? CustStyle.but_shrink : 1.0
            anchors.centerIn: parent
            // Draw the SVG:
            Image {
                id: iconImage
                width: body.width - contextRoot.iconPadding
                height: body.height - contextRoot.iconPadding
                sourceSize.width: body.width - contextRoot.iconPadding
                sourceSize.height: body.height - contextRoot.iconPadding
                anchors.horizontalCenter: body.horizontalCenter
                anchors.verticalCenter: body.verticalCenter
                source: contextRoot.getFilename()
                fillMode: Image.PreserveAspectFit
                smooth: true
                anchors.topMargin: contextRoot.topPadding
                anchors.bottomMargin: contextRoot.bottomPadding
                // Color the base of the SVG image's display
                //ColorOverlay { // TODO: requires 'GraphicalEffects'
                //    anchors.fill: iconImage
                //    source: iconImage
                //    color: contextRoot.color
                //}
            }
        }//end 'body'

        // Mark area around body as Input area.
        MouseArea {
            id: inputArea
            anchors.fill: parent
            hoverEnabled: true
            enabled: contextRoot.isEnabled

            onPressed: {
                //body.color = contextRoot.colorPress
                body.shrunk = true
            }
            onReleased: {
                //body.color = contextRoot.colorNormal
                body.shrunk = false
            }
            onEntered: {
                //body.color = contextRoot.colorHover
                body.shrunk = true
            }
            onExited: {
                //body.color = contextRoot.colorNormal
                body.shrunk = false
            }

            onClicked: {
                contextRoot.action()
            }
        }//end 'inputArea'

    }//end 'trueBody'
}//end 'contextRoot'
