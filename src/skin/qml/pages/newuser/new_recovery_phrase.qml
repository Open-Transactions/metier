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
    title: qsTr("New Recovery Phrase")
    width: rootAppPage.width
    height: rootAppPage.height
    objectName: "new_recovery_phrase"
    background: null //Qt.transparent

    property bool hideNavBar: true // hide navigation bar
    //-----------------------------------------------------------------------------
    function onCancelButton() {
        rootAppPage.popPage(); // navigate back
    }

    function onFinishButton() {
        if (pageRoot.isUsingRecovery) {
            pageRoot.acceptedRecoveryWords()
        } else {
            console.log("User is skipping for now.")
            pageRoot.onCancelButton()
        }
    }

    //-----------------------------------------------------------------------------
    Component.onCompleted: {
        pageRoot.fetchWords()
    }

    property bool isUsingRecovery: true
    property var recoveryWords: []
    function fetchWords() {
        console.log("Showing recovery words...")

        // TODO: make new words, this can crash if the user was
        // already shown their words, need a way to know if user
        // has done this.
        pageRoot.recoveryWords = api.createNewSeed(1, 1, 128);
        console.log("Made a new set of recovery words.")

        // TODO: find out why App crashes on
        // nav Pop and Push back to this Page.
        //pageRoot.recoveryWords = api.getRecoveryWords();// if already generated.
        //console.log("User recovered startup, showing recovery words.")

    }
    //----------------------
    function acceptedRecoveryWords() {
        console.log("User wrote accepted phrase.")

        // when checking conditions, it should fire the
        // signal which will navigate to the next setup Page
        api.seedBackupFinished();
        
        // A signal should fire here which will tell main.qml that its time
        // for the profile name startup step and handle navigation via signal.
        //rootAppPage.pushPage("pages/newuser/create_new_profile.qml")
    }

    //-----------------------------------------------------------------------------
    // main 'body' display container
    Column {
        id: body
        width: pageRoot.width
        property int displayWidth: 340
        focus: true
        anchors.horizontalCenter: parent.horizontalCenter

        Label {
            id: headLiner
            text: qsTr("Recovery Phrase")
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
            text: qsTr("Write down your 12 words in private and keep them safe in case this device is ever lost or damaged.")
            width: body.displayWidth - 24
            anchors.horizontalCenter: body.horizontalCenter
            elide: Text.ElideMiddle
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            color: CustStyle.theme_fontColor
            font.pixelSize: CustStyle.fsize_normal
            bottomPadding: 24
        }

        //-----------------------------------------------------------------------------
        // recovery words displayed
        Rectangle {
            id: backupWords
            width: wordGridModelView.square_size * 3
            height: wordGridModelView.height
            color: "transparent"
            anchors.horizontalCenter: body.horizontalCenter
            // Display all the words and their indexs:
            Grid {
                id: wordGridModelView
                rightPadding: 20
                columns: 3
                columnSpacing: 10
                rowSpacing: 12
                anchors.horizontalCenter: parent.horizontalCenter
                property int square_size: body.displayWidth / 4
                // using the word model Array, itterate threw the list
                Repeater {
                    id: wordEntryBoxes
                    model: pageRoot.recoveryWords
                    // draw each recovery word along side its index value
                    Row {
                        id: display_word_and_index
                        width: wordGridModelView.square_size
                        spacing: 4
                        height: 32
                        // Display Index value:
                        Text {
                            id: indexIdText
                            width: 18
                            height: parent.height
                            text: (index + 1) + "."
                            color: rootAppPage.currentStyle > 0 ? CustStyle.darkmode_text : CustStyle.lightmode_text
                            font.pixelSize: CustStyle.fsize_normal
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            // wordInputArea BG rect fill, done here
                            // as if it was created in the TextInput object
                            // it will prevent 'focus' for TextInput
                            Item {
                                x: indexText.x
                                y: indexText.y - 6
                                implicitWidth: indexText.width
                                implicitHeight: indexText.height
                                anchors.verticalCenter: parent.verticalCenter
                                Rectangle {
                                    color: CustStyle.neutral_fill
                                    height: parent.height
                                    width: parent.width
                                    radius: 4.0
                                }
                            }
                        }
                        // Display Word value:
                        Text {
                            id: indexText
                            width: wordGridModelView.square_size - indexIdText.width
                            height: parent.height
                            text: model.modelData
                            color: rootAppPage.currentStyle > 0 ? CustStyle.darkmode_text : CustStyle.lightmode_text
                            font.pixelSize: CustStyle.fsize_normal
                            anchors.verticalCenter: parent.verticalCenter
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }//end Row
                }//end Repeater
            }//end 'wordGridModelView'
        }//end recovery word Grid display

        //-----------------------------------------------------------------------------
        // Bottom Page Buttons
        Row {
            id: choiceButtons
            spacing: 120
            padding: 4
            topPadding: 32
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: pageRoot.Bottom

            /*
            MatterFi_Button_Standard {
                id: cancel_button
                displayText: qsTr("Cancel")
                onClicked: pageRoot.onCancelButton()
            }
            */

            MatterFi_Button_Standard {
                id: finish_button
                leftPadding: parent.spacing
                displayText: qsTr("Next")
                onClicked: pageRoot.onFinishButton()
            }
        }


    //-----------------------------------------------------------------------------
    }//end 'body'
}//end 'pageRoot'