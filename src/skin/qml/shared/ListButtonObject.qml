import QtQuick 2.15
import QtQuick.Controls.Universal 2.12


import "qrc:/styling"

//-----------------------------------------------------------------------------
// Creates a display of an icon next to a Lable text over top a desctiption.
// Its a way to present a list of items with additional description elements avaliable.
Item {
    id: contextRoot
    height: 36

    property string txtValue: "Large Top"    // displays a large text value
    property string txtLable: "Small Bottom" // displays a small detail value
    //property string svgiconName: ""          // displays a svg icon to left of text
    property string iconChar: "\uf06a"       // single unichar used for displaying an icon.
    property bool iconIsButton: true         // the icon can be clicked on as well as the list item

    property color accentfontColor: rootAppPage.currentStyle > 0 ? CustStyle.dm_text_accent : CustStyle.lm_text_accent

    property int bottomPadding: 0
    property int topPadding: 0

    signal action() // emits was clicked/interacted with

    //-----------------------------------------------------------------------------
    // main 'body' display
    Row {
        id: body
        height: contextRoot.height
        width: parent.width
        leftPadding: 16
        anchors.fill: parent
        bottomPadding: contextRoot.bottomPadding
        topPadding: contextRoot.topPadding

        // make link action button
        FontIconButton {
            id: paymentLinkButton
            iconChar: contextRoot.iconChar
            iconSize: 22
            color: CustStyle.theme_fontColor
            anchors.verticalCenter: parent.verticalCenter
            isEnabled: contextRoot.iconIsButton
            onAction: contextRoot.iconIsButton ? contextRoot.action() : {}
        }
        // Show payment code
        Column {
            leftPadding: 32
            height: contextRoot.height
            spacing: 0
            anchors.verticalCenter: parent.verticalCenter

            Text {
                id: paymentlinkText
                text: contextRoot.txtValue
                color: CustStyle.theme_fontColor
                font.pixelSize: CustStyle.fsize_button
                smooth: true
            }

            Text {
                text: contextRoot.txtLable
                topPadding: 4
                color: contextRoot.accentfontColor
                font.pixelSize: CustStyle.fsize_lable
                smooth: true
            }
        }

    }//end 'body'
}//end 'contextRoot'
