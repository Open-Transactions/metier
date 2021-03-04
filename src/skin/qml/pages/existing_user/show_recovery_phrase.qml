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
  function onDoneButton() {
    //rootAppPage.clearStackHome("pages/existing_user/single_crypto_account_display.qml")
    rootAppPage.popPage(); // navigate back
  }

  //-----------------------------------------------------------------------------
  Component.onCompleted: {
    pageRoot.fetchWords()
  }

  property var recoveryWords: []
  function fetchWords() {
    // provide the current user's recovery word list:
    pageRoot.recoveryWords = api.getRecoveryWords();
  }

  //-----------------------------------------------------------------------------
  // popup contents displayed:
  Column {
    id: body
    width: pageRoot.width
    property int displayWidth: 380
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
      text: qsTr("Keep your recovery phrase words private and keep them safe in case this device is ever lost or damaged.")
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
        bottomPadding: 20
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
    MatterFi_Button_Standard {
      id: done_button
      topPadding: 18
      anchors.horizontalCenter: parent.horizontalCenter
      displayText: qsTr("Go Back")
      onClicked: pageRoot.onDoneButton()
    }

  //-----------------------------------------------------------------------------
  }// display column
}// page