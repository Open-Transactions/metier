import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12


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

  property var accountActivity_model: undefined // OT AccountActivity model
  property var accountList_RowModel: undefined // OT AccountList model

  // The base url used for linking pkt Explorer:
  property var pktExplorerBaseURL: "https://explorer.pkt.cash/tx"
  property var headerTextDisplay: qsTr("Send Funds:")

  property var currentBalance_displayText: qsTr("0.0") // Syncing..
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
  // Called on first opening of drawer to configure the attacted AccountActivity:
  function setup() {
    // clear last send noted request index for current transaction status tracking
    drawerRoot.currentRequestIndex = undefined
    drawerRoot.txidReturned = undefined
    // reconnect the signal if exsisting to the new selected AccountActivity
    // model to watch for transaction status reporting, this ensures the connection
    // is current with the selected ActivityModel
    accountActivity_model.transactionSendResult.disconnect(transactionSendResult)
    accountActivity_model.transactionSendResult.connect(transactionSendResult)
    // debugging display layouts?:
    //drawerRoot.currentRequestIndex = "-1"
    //drawerRoot.userHasSentFunds = true
  }

  Component.onDestruction: {
    if (accountActivity_model !== undefined) {
      accountActivity_model.transactionSendResult.disconnect(transactionSendResult)
    }
  }

  // Signal called on result of a send transaction request.
  property var currentRequestIndex: undefined
  property var txidReturned: undefined
  function transactionSendResult(request_index, blockchain_result, txid) {
    //console.log("Transaction Results Detected: ", request_index, blockchain_result, txid)
    if (drawerRoot.currentRequestIndex !== request_index) {
      //console.log("Transaction was not for this instance.", drawerRoot.currentRequestIndex)
    } else {
      //console.log("Transaction was related to this instance.", txid)
      drawerRoot.txidReturned = txid
      if (txid != "0000000000000000000000000000000000000000000000000000000000000000") {
        drawerRoot.headerTextDisplay = qsTr("Transaction Broadcast to Network:")
      } else {
        drawerRoot.headerTextDisplay = qsTr("Send Error:")
      }
      drawerRoot.userHasSentFunds = true
    }
  }

  /*
  // Used to mock a transaction to provide UIUX debugging.
  function displayDummyTransaction() {
    drawerRoot.currentRequestIndex = -1 // mock data index
    dummyTransactionTimer.restart()
  }
  // emulate time for block chain work response:
  Timer {
    id: dummyTransactionTimer
    interval: 2000
    running: false
    // after interval ms mock signal 'transactionSendResult()'
    onTriggered: {
      //drawerRoot.txidReturned = "0000000000000000000000000000000000000000000000000000000000000000"
      drawerRoot.txidReturned = "mock UIUX"
      drawerRoot.userHasSentFunds = true
    }
  }
  */

  //-----------------------------------------------------------------------------
  // Button Actions:
  function onCancel() {
    drawerRoot.onResetForm()
    // close the drawer
    rootAppPage.drawerIsOpen = false
    drawerRoot.close()
  }

  // Clear the last inputs for next time:
  function onResetForm() {
    sendtoAddressTextInput.text = ""
    amountToSendEntered.clearInput()
    drawerRoot.txidReturned = undefined
    drawerRoot.currentRequestIndex = undefined
    drawerRoot.headerTextDisplay = qsTr("Send Funds:")
    drawerRoot.userHasSentFunds = false
  }

  // used when all fields are filled out. Funds will be sent with
  // the input information, after a value is sent, display the transaction ID
  // for it.
  property var userHasSentFunds: false
  // called on button send click action:
  function onSend() {
    //console.log("Send to Wallet requested")
    //sendToAddress(str address, str amount, str memo)
    var address = drawerRoot.get_sendAddress()

    // TODO: QMetaProperty::read: Unable to handle unregistered datatype 'opentxs::ui::DestinationValidator*' 
    // for property 'metier::model::AccountActivity::destValidator'
    var is_address_valid = drawerRoot.accountActivity_model.validateAddress(address)
    if (!is_address_valid) {
      // if payment code to send was invalid, show tooltip notification
      recipientToolTip.visible = true
      paymentCodeWrongToolTipTimer.start()
      return
    }
    // get validated amount
    var amount_validated = amountToSendEntered.realAmount
    // grab memo note
    var memo = drawerRoot.get_sendMemo()
    // make the call to the OT model for a transaction request
    //console.log("W-Request: ", address, amount_validated, memo)
    //console.log("W-Request, was addresss?", is_address_valid, amount_validated)
    if (is_address_valid) {
      drawerRoot.currentRequestIndex = drawerRoot.accountActivity_model.sendToAddress(address, amount_validated, memo)
      //console.log("W-Request 'sendToAddress' request index:", drawerRoot.currentRequestIndex)
    } else {
      console.log("Error: W-Request was not valid inputs: ", is_address_valid, amount_validated)
    }
  }

  //-----------------------------------------------------------------------------
  // return calls for input functionality for manual sending to address. allows
  // for value checking for it's validation
  function get_sendAddress() {
    return sendtoAddressTextInput.text
  }

  function get_sendAmount() {
    return amountToSendEntered.getAmountEntered()
  }

  function get_sendMemo() {
    return ""
    //return transactionMemoTextInput.text
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
    x: (interactionAreaBG.x)
    y: (interactionAreaBG.y)
    width: (interactionAreaBG.width)
    height: (interactionAreaBG.height)
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
      text: (drawerRoot.headerTextDisplay)
      font.weight: Font.DemiBold
      font.pixelSize: CustStyle.fsize_xlarge
      color: CustStyle.accent_text
      verticalAlignment: Text.AlignVCenter
      anchors.horizontalCenter: parent.horizontalCenter
    }

    //----------------------
    // sending from this account summary
    Column {
      id: whichAccountSummaryHeader
      spacing: 12
      topPadding: 8
      anchors.horizontalCenter: parent.horizontalCenter

      Row {
        id: accountBalanceText
        spacing: 8
        anchors.horizontalCenter: parent.horizontalCenter

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
      /*
      MatterFi_PaymentCode {
        id: usersPaymentCode
        displayLable: qsTr("Your PKT Payment code: ")
        displayText: (accountList_RowModel != null ? accountList_RowModel.account : "null")
      }
      */
    }//end 'whichAccountSummaryHeader'

    //-----------------------------------------------------------------------------
    // Area viewed when fillout out a send address. 
    Rectangle {
      id: sendFormDisplayRect
      height: 384
      width: 640
      color: "transparent"
      visible: (drawerRoot.txidReturned === undefined && drawerRoot.currentRequestIndex === undefined)
      anchors.horizontalCenter: parent.horizontalCenter

      // Format the objects vertically with in the defined view parent.
      Column {
        id: sendFormFilloutColumn
        height: parent.height
        width: parent.width
        spacing: 12
        anchors.horizontalCenter: parent.horizontalCenter

        //----------------------
        // recipient account id entry field
        Row {
          id: enterRecepientAddressRow
          spacing: 8
          topPadding: 8
          
          //----------------------
          Text {
            id: enterAddressText
            text: qsTr("Recipient Address:")
            width: 124
            color: CustStyle.theme_fontColor
            font.pixelSize: CustStyle.fsize_normal
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
          }
          //----------------------
          TextInput {
            id: sendtoAddressTextInput
            width: 384
            height: 38
            maximumLength: 64
            color: CustStyle.theme_fontColor
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: CustStyle.fsize_normal
            clip: true
            padding: 8
            // outline the input box
            OutlineSimple {
              outline_color: CustStyle.accent_outline
            }
            // Display notification when incorrect payment code
            MatterFi_ToolTip {
              id: recipientToolTip
              visible: false
              text: qsTr("Please enter a valid PaymentCode.")
              // time that the ToolTip is displayed for
              Timer {
                id: paymentCodeWrongToolTipTimer
                interval: 2000
                running: false
                onTriggered: {
                  recipientToolTip.visible = false
                }
              }
            }
          }
        }
        
        //----------------------
        // Amount to Send
        MatterFi_AmountInput {
          id: amountToSendEntered
          leftPadding: 24

          onValidate: {
            // need to endure that when validating, will tell user details about
            // the issue when a value to send is incorrect. TODO:
            // "When trying to send, if there is not sufficient balance, there is no dialogue box stating what is wrong."
            amountToSendEntered.realAmount = accountActivity_model.validateAmount(rawText)
          }
        }

        //----------------------
        // memo input text area:
        /*
        Text {
          id: enterMemmoText
          text: qsTr("Attach a Memo:")
          color: CustStyle.theme_fontColor
          verticalAlignment: Text.AlignVCenter
          anchors.horizontalCenter: parent.horizontalCenter
        }
        //----------------------
        TextInput {
          id: transactionMemoTextInput
          width: enterRecepientAddressRow.width
          height: body.height / 4
          maximumLength: 128
          color: CustStyle.theme_fontColor
          padding: 8
          // outline the input box
          OutlineSimple {
            outline_color: CustStyle.accent_outline
          }
        }
        */

        //----------------------
        // button controllers
        Row {
          spacing: 32
          height: 48
          width: 256
          anchors.horizontalCenter: parent.horizontalCenter

          MatterFi_Button_Standard {
            id: cancelButton
            displayText: qsTr("Cancel")
            onClicked: drawerRoot.onCancel()
          }

          MatterFi_Button_Standard {
            id: sendButton
            enabled: (
              amountToSendEntered.realAmount.length > 0 && 
              sendtoAddressTextInput.displayText.length > 0
            );
            displayText: qsTr("Send")
            onClicked: {
              drawerRoot.onSend()
              //drawerRoot.displayDummyTransaction() // debugger
            }
          }
        }
      
      //----------------------
      }//end 'sendFormFilloutColumn'
    }//end 'sendFormDisplayRect'

    //-----------------------------------------------------------------------------
    // Display looks like after a transaction request was sent:
    Column {
      id: sendOutcomeDisplayBody
      height: 128
      topPadding: 24
      spacing: 4
      width: body.width
      visible: (drawerRoot.userHasSentFunds || drawerRoot.currentRequestIndex !== undefined)
      anchors.horizontalCenter: parent.horizontalCenter

      //----------------------
      // Display transaction blockchain is pending response.
      Column {
        id: pendingSendColumnDisplay
        height: 96
        width: parent.width
        spacing: 16
        visible: (drawerRoot.userHasSentFunds === false && drawerRoot.txidReturned === undefined)
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
          id: senddingNotificaionText
          text: qsTr("Sending...")
          width: 124
          font.pixelSize: CustStyle.fsize_title
          color: CustStyle.accent_text
          verticalAlignment: Text.AlignVCenter
          horizontalAlignment: Text.AlignHCenter
          anchors.horizontalCenter: parent.horizontalCenter
        }

        MatterFi_BusyIndicator {
          id: pendingBusyIndicator
          scale: 1.0
          visible: parent.visible
          anchors.horizontalCenter: parent.horizontalCenter
        }
      }

      //----------------------
      // Recent send transaction id notice displayed
      Column {
        id: successSendColumnDisplay
        height: 96
        spacing: 16
        visible: (drawerRoot.txidReturned !== undefined &&
          drawerRoot.txidReturned !== "0000000000000000000000000000000000000000000000000000000000000000")
        anchors.horizontalCenter: parent.horizontalCenter
        // send funds accomplishment descriptive text
        Text {
          id: sendSummaryText
          text: qsTr("TXID:")
          width: 124
          font.pixelSize: CustStyle.fsize_large
          color: CustStyle.accent_text
          verticalAlignment: Text.AlignVCenter
          horizontalAlignment: Text.AlignHCenter
          anchors.horizontalCenter: parent.horizontalCenter
        }
        // TXID of PKT transaction.
        Text {
          id: sendTxidText
          text: (drawerRoot.txidReturned !== undefined ? drawerRoot.txidReturned : "")
          width: 124
          font.pixelSize: CustStyle.fsize_normal
          color: CustStyle.theme_fontColor
          verticalAlignment: Text.AlignVCenter
          horizontalAlignment: Text.AlignHCenter
          anchors.horizontalCenter: parent.horizontalCenter
        }
        // view transaction on the pkt explorer
        MatterFi_Button_Accent {
          id: pktOpenExplorerBut
          width: 176
          height: 32
          displayText: qsTr("View in PKT Explorer")
          onClicked: {
            var web_url = drawerRoot.pktExplorerBaseURL + "/" + drawerRoot.txidReturned
            api.openSystemBrowserLink(web_url)
          }
        }
        // done button
        MatterFi_Button_Standard {
          id: doneButton
          leftPadding: parent.spacing
          visible: (drawerRoot.userHasSentFunds)
          displayText: qsTr("Done")
          anchors.horizontalCenter: parent.horizontalCenter
          onClicked: {
            drawerRoot.onCancel()
          }
        }
      }//end 'successSendColumnDisplay'

      //----------------------
      // Insufficient Funds alert.
      Column {
        id: errorSendInsufficientFundsDisplay
        height: 96
        spacing: 32
        visible: (drawerRoot.txidReturned === "0000000000000000000000000000000000000000000000000000000000000000")
        anchors.horizontalCenter: parent.horizontalCenter
        // user inform text reasoning descriptor
        Text {
          id: errorSendInsufficientFundsText
          text: qsTr("Insufficient Balance.")
          height: parent.height
          width: 124
          font.pixelSize: CustStyle.fsize_large
          color: CustStyle.accent_text
          verticalAlignment: Text.AlignVCenter
          horizontalAlignment: Text.AlignHCenter
          anchors.horizontalCenter: parent.horizontalCenter
        }
        // Go Back button
        MatterFi_Button_Standard {
          id: restartButton
          leftPadding: parent.spacing
          visible: (drawerRoot.userHasSentFunds)
          displayText: qsTr("Go Back")
          anchors.horizontalCenter: parent.horizontalCenter
          onClicked: {
            drawerRoot.onResetForm()
          }
        }
      }//end 'errorSendInsufficientFundsDisplay'

    }//end 'sendOutcomeDisplayBody'

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

  //-----------------------------------------------------------------------------
  }//end 'body'


//-----------------------------------------------------------------------------
}//end drawerRoot
