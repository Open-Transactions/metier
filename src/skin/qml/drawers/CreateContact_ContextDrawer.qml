import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12

import "qrc:/"
import "qrc:/styling"
import "qrc:/matterfi"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// Drawer used as an input item for pop up controll out side of the main window.
Drawer {
  id: drawerRoot
  interactive: false
  edge: Qt.TopEdge
  width: rootAppPage.width
  height: rootAppPage.height
  palette.base: rootAppPage.currentStyle > 0 ? "black" : "white"

  //-----------------------------------------------------------------------------
  // on Enter animation of item into view:
  enter: Transition {
    NumberAnimation { property: "opacity"; 
      from: 0.0; to: 1.0; duration: 520;
      easing.type: Easing.InCirc;
    }
  }

  exit: Transition {
    ParallelAnimation {
      PropertyAnimation { property: "position"; 
        from: 1.0; to: 1.0; duration: 0;
      }
      NumberAnimation { property: "opacity";
        from: 1.0; to: 0.0; duration: 520;
        easing.type: Easing.Linear;
      }
    }
  }

  //-----------------------------------------------------------------------------
  // Button Actions:
  function onCancelBut() {
    // close the drawer
    drawerRoot.resetForm()
    rootAppPage.drawerIsOpen = false
    drawerRoot.close()
  }

  // called on add button interaction:
  function onAddBut() {
    drawerRoot.newContact(contactNameTextInput.text, contactAddressTextInput.text)
  }

  // called to reset the the entry form data:
  function resetForm() {
    drawerRoot.created_new_contact = false
    contextFunctionTextDescription.text = qsTr("Create new Contact:")
    contactNameTextInput.clear()
    contactAddressTextInput.clear()
  }

  //----------------------
  // Add contact into the user's list.
  property var created_new_contact: false
  function newContact(contact_name, address) {
      // verify that the address is a propper one
      var addressValid = OTidentity.focusedAccountActivity_OTModel.validateAddress(address)
      if (addressValid) {
        //console.log("Adding Contact into the List:", contact_name, address)
        //api.addContact(contact_name, address)
        OTidentity.contactList_OTModel.addContact(contact_name, address)
        drawerRoot.created_new_contact = true
        contextFunctionTextDescription.text = qsTr("Contact Created:")
      } else {
        console.log("Error: New Contact address is not valid.", address)
        contactAddressToolTip.visible = true
        paymentCodeWrongToolTipTimer.start()
      }
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
    // Defines a bg fill above/in-front of the
    // Background transparancy fill. This also
    // is used for 'body' position in this case.
    Rectangle {
      id: interactionAreaBG
      width: drawerRoot.width * 0.68
      height: drawerRoot.height //* 0.82
      color: "transparent"
      radius: 4.0
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.verticalCenter: parent.verticalCenter
    }
  }

  //----------------------
  // main display 'body' layout 
  //contentItem: 
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
    // BlockChain Logo:
    Image {
      id: walletBrandingImage
      width: 180
      height: 120
      sourceSize.width: walletBrandingImage.width
      sourceSize.height: walletBrandingImage.height
      anchors.horizontalCenter: parent.horizontalCenter
      source: "qrc:/assets/svgs/pkt-icon.svg"
      fillMode: Image.PreserveAspectFit
      smooth: true
      anchors.topMargin: 0
      anchors.bottomMargin: 0
    }

    //----------------------
    // Clear Title:
    Text {
      id: contextFunctionTextDescription
      text: qsTr("Create new Contact:")
      font.weight: Font.DemiBold
      font.pixelSize: CustStyle.fsize_xlarge
      color: CustStyle.accent_text
      verticalAlignment: Text.AlignVCenter
      anchors.horizontalCenter: parent.horizontalCenter
    }

    //-----------------------------------------------------------------------------
    // Area viewed when fillout out a send address. 
    Column {
      id: newContactFormFilloutRect
      height: parent.height
      width: drawerRoot.width
      visible: (drawerRoot.created_new_contact == false)
      spacing: 12
      //----------------------
      // recipient account id entry field
      Row {
        id: enterContactAddressRow
        spacing: 16
        width: body.width - (body.padding * 2)
        topPadding: 8
        rightPadding: 32
        //----------------------
        Text {
          id: enterAddressText
          text: qsTr("Contact's MatterCode™:")
          width: 150
          color: CustStyle.theme_fontColor
          verticalAlignment: Text.AlignVCenter
          font.pixelSize: CustStyle.fsize_normal
          anchors.verticalCenter: parent.verticalCenter
        }
        //----------------------
        TextInput {
          id: contactAddressTextInput
          width: parent.width - enterAddressText.width - enterContactAddressRow.spacing - 80
          height: 38
          maximumLength: 64
          color: CustStyle.theme_fontColor
          verticalAlignment: Text.AlignVCenter
          anchors.verticalCenter: parent.verticalCenter
          font.pixelSize: CustStyle.fsize_normal
          clip: true
          padding: 8
          // outline the input box
          OutlineSimple {
            outline_color: CustStyle.accent_outline
          }
          // Display notification when incorrect payment code
          MatterFi_ToolTip {
            id: contactAddressToolTip
            visible: false
            text: qsTr("Please enter a valid PaymentCode.")
            // time that the ToolTip is displayed for
            Timer {
              id: paymentCodeWrongToolTipTimer
              interval: 2000
              running: false
              onTriggered: {
                contactAddressToolTip.visible = false
              }
            }
          }
        }
      }

      //----------------------
      // Contact name input text area:
      Row {
        id: enterContactNameRow
        spacing: 8
        width: body.width - (body.padding * 2)
        topPadding: 8
        leftPadding: 48
        //----------------------
        Text {
          id: enterContactNameText
          text: qsTr("Contact's Name:")
          width: 108
          color: CustStyle.theme_fontColor
          verticalAlignment: Text.AlignVCenter
          font.pixelSize: CustStyle.fsize_normal
          anchors.verticalCenter: parent.verticalCenter
        }
        //----------------------
        TextInput {
          id: contactNameTextInput
          width: parent.width / 8 * 3 //parent.width - enterContactNameText.width - enterContactNameRow.spacing - 116
          height: 38
          maximumLength: 64
          color: CustStyle.theme_fontColor
          anchors.verticalCenter: parent.verticalCenter
          verticalAlignment: Text.AlignVCenter
          font.pixelSize: CustStyle.fsize_normal
          clip: true
          padding: 8
          // outline the input box
          OutlineSimple {
            outline_color: CustStyle.accent_outline
          }
        }
      }

      //----------------------
      // button controllers
      Row {
        spacing: 64
        height: 48
        width: body.width
        anchors.horizontalCenter: parent.horizontalCenter

        MatterFi_Button_Standard {
          id: cancelButton
          displayText: qsTr("Cancel")
          onClicked: drawerRoot.onCancelBut()
        }

        MatterFi_Button_Standard {
          id: addButton
          enabled: (contactAddressTextInput.displayText.length > 0 &&
            contactNameTextInput.displayText.length > 0)
          displayText: qsTr("Add")
          onClicked: drawerRoot.onAddBut()
        }
      }

    }//end 'newContactFormFilloutRect'

    //-----------------------------------------------------------------------------
    // Area viewed when fillout out a send address. 
    Column {
      id: userActionFollowupColumn
      height: parent.height
      width: drawerRoot.width
      visible: (drawerRoot.created_new_contact)
      spacing: 12
      anchors.horizontalCenter: parent.horizontalCenter

      // button controllers
      Row {
        spacing: 64
        height: 48
        topPadding: 12
        anchors.horizontalCenter: parent.horizontalCenter

        MatterFi_Button_Standard {
          displayText: qsTr("Add Another")
          onClicked: drawerRoot.resetForm()
        }

        MatterFi_Button_Standard {
          displayText: qsTr("Done")
          onClicked: drawerRoot.onCancelBut()
        }
      }
    }//end 'userActionFollowupColumn'

  //-----------------------------------------------------------------------------
  }//end 'body'

//-----------------------------------------------------------------------------
}//end 'drawerRoot'
