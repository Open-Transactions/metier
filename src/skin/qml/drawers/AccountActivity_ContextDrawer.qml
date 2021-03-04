import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12


import "qrc:/styling"
import "qrc:/matterfi"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// Drawer used as an input item for pop up control out side of the main window.
Drawer {
  id: drawerRoot
  interactive: false
  edge: Qt.TopEdge
  width: rootAppPage.width
  height: rootAppPage.height
  palette.base: rootAppPage.currentStyle > 0 ? "black" : "white"

  property var currentAccountActivty_OTModel: undefined // OT AccountActivity model
  property var accountList_RowModel: undefined          // OT AccountList RowModel

  //-----------------------------------------------------------------------------
  // on Enter animation of item into view
  enter: Transition {
    NumberAnimation { property: "opacity"; 
      from: 0.0; to: 1.0; duration: 400;
    }
  }

  exit: Transition {
    ParallelAnimation {
      PropertyAnimation { property: "position"; 
        from: 1.0; to: 0.0; duration: 400;
      }
      NumberAnimation { property: "opacity";
        from: 1.0; to: 0.0; duration: 400;
        easing.type: Easing.OutCubic;
      }
    }
  }

  //-----------------------------------------------------------------------------
  // set up a receive funds payment code:
  function on_accountActivity_receiveFunds() {
    drawerRoot.openReceiveContextDrawer(drawerRoot.selectedAccountActivtyModel)
  }

  // send to address:
  function on_accountActivity_walletSend() {
    drawerRoot.openSendContextDrawer(drawerRoot.selectedAccountActivtyModel)
  }

  //-----------------------------------------------------------------------------
  // Button Actions:
  function onCloseDrawer_Button() {
    drawerRoot.close()
  }

  //-----------------------------------------------------------------------------
  // Background styling:
  background: Rectangle {
    id: background
    width: drawerRoot.width
    height: drawerRoot.height
    opacity: 1.0 //0.85
    color: rootAppPage.currentStyle > 0 ? CustStyle.lm_navbar_bg : CustStyle.dm_navbar_bg

    //----------------------
    Rectangle {
      id: interactionAreaBG
      width: drawerRoot.width * 0.68
      height: drawerRoot.height * 0.68
      color: CustStyle.neutral_fill
      radius: 4.0
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.verticalCenter: parent.verticalCenter
    }
  }

  //----------------------
  // main display 'body' layout
  Column {
    id: body
    spacing: 8
    padding: 16
    x: interactionAreaBG.x
    y: interactionAreaBG.y
    width: interactionAreaBG.width
    height: interactionAreaBG.height
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter

    //----------------------
    // Clear Title:
    Text {
      id: contextFunctionTextDescription
      text: qsTr("Account Activity:")
      color: CustStyle.theme_fontColor
      font.weight: Font.DemiBold
      font.pixelSize: CustStyle.fsize_title
      verticalAlignment: Text.AlignVCenter
      anchors.horizontalCenter: parent.horizontalCenter
    }

    //-----------------------------------------------------------------------------
    // display the account transaction history:
    Rectangle {
      id: accountActivityListRect
      width: body.width - 32
      height: body.height - 128
      color: Universal.background // "transparent"
      radius: 4.0
      // bg outline
      OutlineSimple {}
      // account activity history Deligation
      DelegateModel {
        id: accountActivityModelDelegator
        model: (currentAccountActivty_OTModel === null ? [] : drawerRoot.currentAccountActivty_OTModel)
        // itterate threw each index in the model 'delegating' (painting) them
        delegate: Rectangle {
          Text {
            text: "entry index: " + index
            color: CustStyle.neutral_text
            font.pixelSize: CustStyle.fsize_normal
          }
        }
      }
      //----------------------
      // display the activity entries
      ListView { // TODO: possibly TableView instead?  
        id: accountActivitylistView
        model: accountActivityModelDelegator // drawerRoot.currentAccountActivty_OTModel 
        clip: true
        topMargin: 8
        leftMargin: 8
        rightMargin: 8
        bottomMargin: 8
        spacing: 16
        width: parent.width
        height: parent.height

        anchors {
          topMargin: 16
          top: parent.anchors.TopAnchor
          left: parent.anchors.LeftAnchor
          bottom: parent.anchors.BottomAnchor
          right: parent.anchors.RightAnchor
          horizontalCenter: parent.horizontalCenter
        }
      }
    }

    //----------------------
    // Close Drawer button
    MatterFi_Button_Standard {
      id: cancelButton
      displayText: qsTr("Close")
      onClicked: drawerRoot.onCloseDrawer_Button()
    }

  }//end 'body'

//-----------------------------------------------------------------------------
}//end drawerRoot