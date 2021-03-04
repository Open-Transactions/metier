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
    rootAppPage.popPage(); // navigate back
  }

  //-----------------------------------------------------------------------------
  // The main display 'body' object:
  Column {
    id: body
    width: pageRoot.width
    anchors.horizontalCenter: parent.horizontalCenter

    Label {
      id: headLiner
      text: qsTr("Product Version")
      color: CustStyle.accent_text
      smooth: true
      font.capitalization: Font.AllUppercase
      font.weight: Font.Bold
      font.pixelSize: CustStyle.fsize_large
      anchors.horizontalCenter: parent.horizontalCenter
      horizontalAlignment: Text.AlignHCenter
      bottomPadding: 24
      topPadding: 18
    }

    /*
    Text {
      id: descriptionText
      text: qsTr("We're version: ")
      width: body.displayWidth - 24
      bottomPadding: 24
      color: CustStyle.theme_fontColor
      wrapMode: Text.WordWrap
      elide: Text.ElideMiddle
      horizontalAlignment: Text.AlignHCenter
      font.pixelSize: CustStyle.fsize_normal
      anchors.horizontalCenter: body.horizontalCenter
    }
    */

    Text {
      id: selectionForReceivingTypeTextDescription
      text: (api.versionString(CustStyle.qml_release_version))
      bottomPadding: 24
      font.weight: Font.DemiBold
      font.pixelSize: CustStyle.fsize_title
      color: CustStyle.theme_fontColor
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      anchors.horizontalCenter: body.horizontalCenter
    }

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