import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12


import "qrc:/styling"
import "qrc:/qml_shared"
import "qrc:/matterfi"

//-----------------------------------------------------------------------------
Page {
    id: pageRoot
    title: qsTr("Recovery Storage Wranings")
    width: rootAppPage.width
    height: rootAppPage.height
    objectName: "store_recovery_phrase"
    background: null //Qt.transparent

    property bool hideNavBar: true // hide navigation bar

    property bool isMakingRecovery: true
    
    //-----------------------------------------------------------------------------
    function onCancelButton() {
        // nav back
        rootAppPage.popPage();
    }
    //----------------------
    function onNextButton() {
        if (isMakingRecovery) {
            rootAppPage.pushPage("pages/newuser/new_recovery_phrase.qml")
        } else {
            // navigate back in StackView
            rootAppPage.popPage();
        }
    }

    //-----------------------------------------------------------------------------
    // popup contents displayed:
    Column {
        id: body
        width: pageRoot.width
        property int displayWidth: 310
        focus: true
        anchors.horizontalCenter: parent.horizontalCenter

        Label {
            id: headLiner
            text: qsTr("Warning")
            color: CustStyle.theme_fontColor
            smooth: true
            font.capitalization: Font.AllUppercase
            font.weight: Font.Bold
            font.pixelSize: CustStyle.fsize_large
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            bottomPadding: 24
            topPadding: 18
        }

        Text {
            id: descriptionText
            text: qsTr("Anyone with access to your recovery phrase can steal your funds.")
            width: body.displayWidth - 24
            anchors.horizontalCenter: body.horizontalCenter
            elide: Text.ElideMiddle
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            color: CustStyle.theme_fontColor
            font.pixelSize: CustStyle.fsize_normal
            bottomPadding: 24
        }

        //----------------------
        Row {
            id: warnOne
            width: body.displayWidth
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 26
            height: 64
            leftPadding: 16
            /*
            MatterFi_SVGimage {
                svgFile: "eye-slash"
                width: 42
                height: width
                color: CustStyle.theme_fontColor
                scale: body.shrunk ? CustStyle.but_shrink : 1.0
                anchors.verticalCenter: parent.verticalCenter
                isFAicon_solid: true
            }
            */
            MatterFi_HugeIconImage {
                width: 42
                height: width
                iconChar: IconIndex.eye_slash
                isFAicon_solid: true
                color: CustStyle.theme_fontColor
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text: qsTr("DO NOT let anyone see your recovery phrase")
                width: parent.width / 4 * 3 - parent.leftPadding
                elide: Text.ElideMiddle
                wrapMode: Text.WordWrap
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: CustStyle.fsize_normal
                color: CustStyle.theme_fontColor
            }
        }

        //----------------------
        Row {
            id: warnTwo
            width: body.displayWidth
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 26
            height: 64
            leftPadding: 16
            y: warnOne.y + warnOne.height + 64
            /*
            MatterFi_SVGimage { // **Needs replacement, needs crossed out version**
                svgFile: "camera"
                width: 42
                height: width
                color: CustStyle.theme_fontColor
                scale: body.shrunk ? CustStyle.but_shrink : 1.0
                anchors.verticalCenter: parent.verticalCenter
                isFAicon_solid: true
            }
            */
            MatterFi_HugeIconImage {
                width: 42
                height: width
                iconChar: IconIndex.camera
                isFAicon_solid: true
                color: CustStyle.theme_fontColor
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text: qsTr("DO NOT let anyone take a screen shot of your recovery phrase")
                width: parent.width / 4 * 3 - parent.leftPadding
                elide: Text.ElideMiddle
                wrapMode: Text.WordWrap
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: CustStyle.fsize_normal
                color: CustStyle.theme_fontColor
            }
        }

        //----------------------
        Row {
            id: warnThree
            width: body.displayWidth
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 26
            height: 64
            leftPadding: 16
            y: warnTwo.y + warnTwo.height + 64
            /*
            MatterFi_SVGimage { // **Needs replacement, needs crossed out version**
                svgFile: "keyboard"
                width: 42
                height: width
                color: CustStyle.theme_fontColor
                scale: body.shrunk ? CustStyle.but_shrink : 1.0
                anchors.verticalCenter: parent.verticalCenter
                isFAicon_solid: true
            }
            */
            MatterFi_HugeIconImage {
                width: 42
                height: width
                iconChar: IconIndex.keyboard
                isFAicon_solid: true
                color: CustStyle.theme_fontColor
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text: qsTr("DO NOT type your recovery phrase into any other program")
                width: parent.width / 4 * 3 - parent.leftPadding
                elide: Text.ElideMiddle
                wrapMode: Text.WordWrap
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: CustStyle.fsize_normal
                color: CustStyle.theme_fontColor
            }
        }

        //-----------------------------------------------------------------------------
        Row {
            id: choiceButtons
            spacing: 120
            padding: 4
            topPadding: 32
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: pageRoot.Bottom

            MatterFi_Button_Standard {
                id: cancel_button
                displayText: qsTr("Cancel")
                onClicked: pageRoot.onCancelButton()
            }

            MatterFi_Button_Standard {
                id: next_button
                displayText: qsTr("Next")
                onClicked: pageRoot.onNextButton()
            }
        }

        //-----------------------------------------------------------------------------
    }// display column
}// page
