import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/styling"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// 'MatterFi_AccountListDelegate.qml'
// Delegate for "AccountList" model display. This is where user "AccountActivity"
//  for each Account in their wallet is displayed for selection.
//
// Roles { notary unit account balance polarity accounttype contractid }
/*
enum Columns {
    NotaryNameColumn     = 0
    DisplayUnitColumn    = 1
    AccountNameColumn    = 2
    DisplayBalanceColumn = 3
};
*/
//
//-----------------------------------------------------------------------------
Item {
    id: contextRoot

    property var listEntry: ({}) // an OT AccountList model, this would be a Row from that Model.
    property var indexEntry: 0   // the index of this Row display from the OT AccountList model.

    property bool isEnabled: true

    signal action()

    //-----------------------------------------------------------------------------
    Component.onCompleted: {
        // debugger:
        console.log("user_accounts.qml MatterFi_AccountListDelegate: ", contextRoot.indexEntry)
        QML_Debugger.listEverything(listEntry)
        //QML_Debugger.listEverything( listEntry.index(contextRoot.indexEntry, 0) )
    }

    //-----------------------------------------------------------------------------
    // 'body' is always the main display container to help align items. This means
    // it might not always be a Rectangle, but it works out well for layouts to have
    // a conatainer QObject for anchors and other draw properties.
    Rectangle {
        id: body
        scale: shrunk ? CustStyle.but_shrink : 1.0
        width: parent.width
        height: parent.height
        //color: "transparent"
        gradient: CustStyle.accent_gradient
        radius: 4.0
        property bool shrunk: false
        // bg outline styler
        OutlineSimple {}
        // Account id display, list identifier user display:
        Text {
            id: accountBlockChainText
            text: listEntry.account
            color: CustStyle.theme_fontColor
            font.pixelSize: CustStyle.fsize_normal
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        //-----------------------------------------------------------------------------
        // Mark area around body as Input area.
        MouseArea {
            id: inputArea
            width: body.width
            height: body.height
            hoverEnabled: false
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
                interval: 300

                onTriggered: {
                    body.shrunk = false
                    contextRoot.action()
                }
            }
        }

    //-----------------------------------------------------------------------------
    }//end 'body'
}//end 'contextRoot'


