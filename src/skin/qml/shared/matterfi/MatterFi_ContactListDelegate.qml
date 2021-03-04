import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/styling"
import "qrc:/matterfi"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// 'MatterFi_ContactListDelegate.qml'
// Display delegate for OT ContactList model. *not complete list.
// Roles { "id" "section", "image", "name" }
// Functions:
//
//-----------------------------------------------------------------------------
Item {
    id: contextRoot

    property var listIndex: 0
    property var contact_data: ({})
    // called on click interactions
    signal action()

    //-----------------------------------------------------------------------------
    // Filtering the display:  Qt::CaseInsensitive
    property var filterString: ""
    visible: (filterString.length === 0 || (
        contact_data !== undefined && (
            contact_data.name.indexOf(filterString) !== -1 ||
            contact_data.id.indexOf(filterString) !== -1
        )
    ));
    property bool isEnabled: (contextRoot.visible)
    height: (isEnabled ? 40 : 0)

    //-----------------------------------------------------------------------------
    // Run soon as page is ready.
    /*
    Component.onCompleted: {
        console.log("Contact List entry:", contextRoot.contact_data)
        QML_Debugger.listEverything(contextRoot.contact_data)
    }
    */

    //-----------------------------------------------------------------------------
    Rectangle {
        id: body
        scale: shrunk ? CustStyle.but_shrink : 1.0
        width: parent.width
        height: (contextRoot.visible ? parent.height : 0)
        color: (inputArea.containsMouse ? CustStyle.pkt_logo_highlight : "transparent")
        property bool shrunk: false

        Row {
            width: body.width
            height: body.height
            leftPadding: 4

            MatterFi_Avatar {
                id: avatarImage
                width: parent.height
                height: width
                //avatarUrl: contact_data.image
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: nameText
                text: (contextRoot.listIndex === 0 ? (contact_data.name + " (myself)") : contact_data.name)
                color: CustStyle.theme_fontColor
                leftPadding: 12
                font.pixelSize: CustStyle.fsize_normal
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
            }

        }

        //-----------------------------------------------------------------------------
        // Mark area around body as Input area.
        MouseArea {
            id: inputArea
            width: body.width
            height: body.height
            hoverEnabled: true
            enabled: contextRoot.isEnabled

            onClicked: {
                body.shrunk = true
                animationTimer.restart()
            }

            onPressed: {
                //body.color = contextRoot.colorPress
                //body.shrunk = true
            }
            onReleased: {
                //body.color = contextRoot.colorNormal
                //body.shrunk = false
            }
            onEntered: {
                //body.color = contextRoot.colorHover
                //body.shrunk = true
            }
            onExited: {
                //body.color = contextRoot.colorNormal
                //body.shrunk = false
            }

            Timer {
                id: animationTimer
                interval: 100

                onTriggered: {
                    body.shrunk = false
                    contextRoot.action()
                }
            }
        }

    //-----------------------------------------------------------------------------
    }//end 'body'
}//end 'contextRoot'





//-----------------------------------------------------------------------------
