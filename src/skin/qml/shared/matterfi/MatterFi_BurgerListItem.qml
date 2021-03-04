import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12


import "qrc:/styling"
import "qrc:/matterfi"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// 'MatterFi_BurgerListItem.qml'
// A '0' svgType will look in the root svg resource folder. 'qrc:/assets/svgs/'
/*
    isFAicon_solid: (contextRoot.svgType == 1)  'qrc:/assets/svgs/FA5-Free/solid/'
    isFAicon_reg:   (contextRoot.svgType == 2)  'qrc:/assets/svgs/FA5-Free/reg/'
*/
//-----------------------------------------------------------------------------
Item {
    id: contextRoot

    property string svgImageName: "check"
    property int svgType: 0
    property color bgcolor: "transparent"
    property string lableText: "Null"
    property int spacing: 16
    property color color: rootAppPage.currentStyle > 0 ? CustStyle.dm_icon_but : CustStyle.lm_icon_but
    property var objectName: ""

    signal action()

    //-----------------------------------------------------------------------------
    Rectangle {
        id: body
        width: contextRoot.width
        height: contextRoot.height
        anchors.fill: parent
        color: contextRoot.bgcolor
        property bool shrunk: false

        Row {
            spacing: contextRoot.spacing
            //----------------------
            MatterFi_SVGimage {
                id: svgImageObject
                width: contextRoot.height // makes square
                height: contextRoot.height
                imgPadding: contextRoot.height / 2
                svgFile: contextRoot.svgImageName
                color: contextRoot.color
                scale: body.shrunk ? CustStyle.but_shrink : 1.0
                anchors.verticalCenter: parent.verticalCenter
                // id type index:
                isFAicon_solid: (contextRoot.svgType == 1)
                isFAicon_reg:   (contextRoot.svgType == 2)

            }
            //----------------------
            Text {
                id: toggleDarkLable
                text: contextRoot.lableText
                color: contextRoot.color
                font.weight: Font.DemiBold
                font.pixelSize: CustStyle.fsize_burger
                smooth: true
                scale: body.shrunk ? CustStyle.but_shrink : 1.0
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        //-----------------------------------------------------------------------------
        // Mark area around body as Input area.
        MouseArea {
            id: inputArea
            anchors.fill: parent
            hoverEnabled: true

            onClicked: {
                contextRoot.action()
            }

            onPressed: {
                body.shrunk = true
            }
            onReleased: {
                body.shrunk = false
            }
            onEntered: {
                body.shrunk = true
            }
            onExited: {
                body.shrunk = false
            }
        }

    //-----------------------------------------------------------------------------
    }// end item Body
}
