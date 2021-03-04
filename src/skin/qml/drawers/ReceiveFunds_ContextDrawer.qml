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
  closePolicy: Popup.NoAutoClose
  palette.base: rootAppPage.currentStyle > 0 ? "black" : "white"

  property var accountActivity_model: undefined // OT AccountActivity model
  property var accountList_RowModel: undefined  // OT AccountList model

  property var currentBalance_displayText: qsTr("0.0") // Syncing..
  property var userPaymentCode: undefined
  property var qrCodebase64: ""

  //-----------------------------------------------------------------------------
  // Re-Generate the QR Code base64 string for an image deligation.
  function refresh() {
    OTidentity.ensureProfileIsSet()
    // the user's current nym identity's MatterCode™
    var user_OTProfileModel = OTidentity.profile_OTModel
    drawerRoot.userPaymentCode = user_OTProfileModel.paymentCode
    userPayementCodeImage.set_display_string(drawerRoot.userPaymentCode) //accountList_RowModel.account
    // the origional send type, by the blockchain's address
    var blockchain_address = accountActivity_model.getDepositAddress()
    usersBCDepositAddress.displayText = blockchain_address
    userBCDepositAddressImage.set_display_string(usersBCDepositAddress.displayText)
    //debugger:
    //console.log("Received Drawer, forced refresh:", accountList_RowModel.account, accountActivity_model)
    //QML_Debugger.listEverything(accountActivity_model)
  }

  //-----------------------------------------------------------------------------
  // on Enter animation of item into view
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
  function onCancel() {
    rootAppPage.drawerIsOpen = false
    drawerRoot.close()
  }

  function onDoneButton() {
    //console.log("Receive Funds requested", drawerRoot.get_reciveAmount(), drawerRoot.get_reciveMemo())
    rootAppPage.drawerIsOpen = false
    drawerRoot.close()
  }

  //-----------------------------------------------------------------------------
  // Background styling:
  background: Rectangle {
    id: background
    width: drawerRoot.width
    height: drawerRoot.height
    opacity: 1.0 //0.85
    color: "transparent"
    // fade up from transparent
    ColorAnimation on color {
      to: rootAppPage.currentStyle > 0 ? CustStyle.lm_navbar_bg : CustStyle.dm_navbar_bg
      duration: 1000
    }

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
    spacing: 12
    padding: 16
    x: interactionAreaBG.x
    y: interactionAreaBG.y
    width: interactionAreaBG.width
    anchors.horizontalCenter: parent.horizontalCenter

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
      id: contextDrawerFunctionTextDescription
      text: qsTr("Receive Funds:")
      font.weight: Font.DemiBold
      font.pixelSize: CustStyle.fsize_xlarge
      color: CustStyle.accent_text
      verticalAlignment: Text.AlignVCenter
      anchors.horizontalCenter: parent.horizontalCenter
    }
    
    //----------------------
    // Provide a combo box for the options of receiving payments:
    Row {
      id: selectionRowforReceiveType
      spacing: 12
      height: 42
      width: receive_codeType_CB.width + 200
      anchors.horizontalCenter: parent.horizontalCenter

      // what the comboBox is described as providing options for:
      Text {
        id: selectionForReceivingTypeTextDescription
        text: qsTr("Method For Receiving Funds:")
        font.weight: Font.DemiBold
        font.pixelSize: CustStyle.fsize_normal
        color: CustStyle.accent_text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
      }
      // the selection box for the type of Receive code provided on screen
      MatterFi_ComboBox {
        id: receive_codeType_CB
        currentIndex: 0
        width: 240

        model: ListModel {
          id: receiveType_CBmodel
          ListElement { text: "MatterCode™" }
          ListElement { text: "Wallet Address" }
        }
      }
    }

    /*
    //----------------------
    // Used for triggering 'esc' key return to Normal window
    // from Fullscreen mode.
    Item {
      id: esc_normalWindow_trigger
      focus: true

      Keys.onEscapePressed: {
        rootAppPage.keyEvent_normal_window_size(event)
      }
    }
    */

    //----------------------
    // States for transitioning which payment address is shown:
    state: "matterfi_code"
    states: [
      State {
        name: "matterfi_code"
        when: (receive_codeType_CB.currentIndex === 0)
        PropertyChanges { target: whichAccountSummaryHeader_paymentCode; opacity: 1.0 }
      },
      State {
        name: "blockchain_code"
        when: (receive_codeType_CB.currentIndex === 1)
        PropertyChanges { target: whichAccountSummaryHeader_BCRA; opacity: 1.0 }
      }
    ]//end 'states'

  }//end 'body'

  //-----------------------------------------------------------------------------
  // Is user using the Payment Code 'MatterCode' system?
  Column {
    id: whichAccountSummaryHeader_paymentCode
    opacity: 0.0
    visible: (opacity > 0.1)
    spacing: 12
    anchors {
      horizontalCenter: parent.horizontalCenter;
      top: body.bottom
      topMargin: 12
    }

    Behavior on opacity {
      NumberAnimation { duration: 520 }
    }

    Column {
      id: alignLeftBalancePayementTextColumn_PC
      width: 520
      spacing: 12
      anchors.horizontalCenter: parent.horizontalCenter

      /*
      Row {
        id: accountBalanceText_PC
        spacing: 8

        Text {
          text: qsTr("Wallet Balance: ")
          color: CustStyle.theme_fontColor
          font.pixelSize: CustStyle.fsize_normal
          verticalAlignment: Text.AlignVCenter
        }

        Text {
          text: drawerRoot.currentBalance_displayText
          color: CustStyle.theme_fontColor //CustStyle.accent_text
          font.weight: Font.DemiBold
          font.pixelSize: CustStyle.fsize_normal
          verticalAlignment: Text.AlignVCenter
        }
      }
      */

      // display clickable copy text of user's payment code
      MatterFi_PaymentCode {
        id: usersPaymentCode
        displayLable: qsTr("MatterCode™: ")
        displayText: (userPaymentCode != null ? drawerRoot.userPaymentCode : "null")
      }

      // display scanable QR Code image
      MatterFi_QRCodeImage {
        id: userPayementCodeImage
        anchors.horizontalCenter: parent.horizontalCenter
      }
    }

    // Disclaimer:
    Text {
      id: disclaimerText
      width: 520
      wrapMode: Text.Wrap
      elide: Text.ElideRight
      lineHeight: 1.1
      padding: 8
      font.pixelSize: CustStyle.fsize_lable
      color: CustStyle.theme_fontColor
      verticalAlignment: Text.AlignVCenter
      horizontalAlignment: Text.AlignHCenter
      anchors.horizontalCenter: parent.horizontalCenter

      text: qsTr("Share your MatterCode™ with the sender's MatterFi Wallet to create a\nrecurring recipient.  P2P encrypted chat coming soon.")

      /*
      text: qsTr("To automatically receive funds without having to generate payment addresses\n" + 
                  " just share your MatterCode™ with the senders MatterFi Wallet once. After\n" +
                  "that the sender can send you funds anytime. The wallets compute unique onetime\n" + 
                  "HD PKT receiving addresses for you so you can always get to your funds.\n" +
                  "This works with any wallet supporting OBPP-5 payment codes.\n" + 
                  "P2P encrypted chat coming soon!")
      */

      // outline the input box
      OutlineSimple {
        outline_color: CustStyle.accent_outline
      }
    }
  }//end 'whichAccountSummaryHeader'

  //----------------------
  // Is the user instead using the BlockChain Receive Address option?
  Column {
    id: whichAccountSummaryHeader_BCRA
    opacity: 0.0
    visible: (opacity > 0.1)
    spacing: 12
    anchors {
      horizontalCenter: parent.horizontalCenter;
      top: body.bottom
      topMargin: 12
    }

    Behavior on opacity {
      NumberAnimation { duration: 520 }
    }

    Column {
      id: alignLeftBalancePayementTextColumn_BCRA
      width: 520
      spacing: 12
      anchors.horizontalCenter: parent.horizontalCenter
      
      // display clickable copy text of user's payment code
      MatterFi_BlockChainReceiveCode {
        id: usersBCDepositAddress
        displayLable: qsTr("Wallet Address: ")
        displayText: qsTr("Awaiting blockchain response...")
      }

      // display scanable QR Code image
      MatterFi_QRCodeImage {
        id: userBCDepositAddressImage
        visible: (accountList_RowModel !== null)
        anchors.horizontalCenter: parent.horizontalCenter
      }

      Rectangle {
        id: accountBCdepositAddressPlaceholderRect
        width: userBCDepositAddressImage.width
        height: userBCDepositAddressImage.height
        color: "transparent"
        visible: (!userBCDepositAddressImage.visible)
      }
    }
  }

  //----------------------
  // button controllers
  Row {
    spacing: 64
    y: 520
    anchors {
      horizontalCenter: parent.horizontalCenter;
    }

    MatterFi_Button_Standard {
      id: doneButton
      displayText: qsTr("Done")
      onClicked: drawerRoot.onDoneButton()
    }
  }

//-----------------------------------------------------------------------------
}//end drawerRoot
