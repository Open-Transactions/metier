import QtQuick 2.15
import QtQuick.Controls 2.15



import "qrc:/styling"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// 'MatterFi_BlockChainDelegate.qml'
// Delegate for displaying blockchain selections.
// Roles { "type" "testnet" }
// Functions:
//
//-----------------------------------------------------------------------------
Item {
    id: contextRoot

    property var blockchain_data: {}

    property bool isEnabled: true

    signal action()

    //-----------------------------------------------------------------------------
    Component.onCompleted: {
        //console.log("Block Chain Delegate: ", blockchain_data.objectName, blockchain_data );
        console.log("Block Chain Delegate: ", JSON.stringify( blockchain_data ), "\n" );
        //console.log("Block Chain Delegate: ", Object.keys(blockchain_data) );
        //console.log("Block Chain Delegate: ", Object.values( blockchain_data.data ) );
        //console.log("Block Chain Delegate: ", blockchain_data.data[0] );
        //console.log("Block Chain Delegate: ", Object.values( blockchain_data.children ) );
        
        QML_Debugger.listEverything(blockchain_data);
    }

    //-----------------------------------------------------------------------------
    Rectangle {
        id: body
        scale: shrunk ? CustStyle.but_shrink : 1.0
        width: parent.width
        height: parent.height
        color: "transparent"
        property bool shrunk: false

        Row {
            width: body.width
            height: body.height
            leftPadding: 4

            Text {
                id: nameText
                text: blockchain_data.type
                color: CustStyle.theme_fontColor
                font.pixelSize: CustStyle.fsize_normal
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
            }

        }

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

    }
}





//-----------------------------------------------------------------------------
