import QtQuick 2.15
import QtQuick.Controls 2.15

import QtQml.Models 2.1

import "qrc:/styling"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// 'MatterFi_TransactionHistoryDisplay.qml'
// Delegate for the entries shown from an AccountActivity model. Provides a 
// clickable drop element that expands/contracts to show details of a transaction.
//
// Roles { // QML usually helps turn these into properties on the model by simular name.
/*
  AmountRole = Qt::UserRole + 0,    // QString
  TextRole = Qt::UserRole + 1,      // QString
  MemoRole = Qt::UserRole + 2,      // QString
  TimeRole = Qt::UserRole + 3,      // QDateTime
  UUIDRole = Qt::UserRole + 4,      // QString
  PolarityRole = Qt::UserRole + 5,  // int, -1, 0, or 1
  ContactsRole = Qt::UserRole + 6,  // QStringList
  WorkflowRole = Qt::UserRole + 7,  // QString
  TypeRole = Qt::UserRole + 8,      // int, opentxs::StorageBox)
}
*/
//----------------------
// Columns {
//  TimeColumn   = 0
//  TextColumn   = 1
//  AmountColumn = 2
//  UUIDColumn   = 3
//  MemoColumn   = 4
// };


//-----------------------------------------------------------------------------
Rectangle {
  id: contextRoot
  width: 400
  height: 400
  color: "transparent"

  property var accountActivityModel: undefined // looking for an AccountActivity model

  // The base url used for linking pkt transactions to their Explorer:
  property var pktExplorerBaseURL: "https://explorer.pkt.cash/tx"

  //-----------------------------------------------------------------------------
  // Populate display element data for delegation
  function getDisplayColumnRowData(for_index) {
    var display_data = {}
    if (accountActivityModel !== undefined) {
      // gather up the display data from the index in provied for Delicagation:
      display_data['timestamp'] = accountActivityModel.data( 
        accountActivityModel.index(for_index, 0) );          // TimeColumn
      display_data['text']      = accountActivityModel.data(
        accountActivityModel.index(for_index, 1) );          // TextColumn
      display_data['amount']    = accountActivityModel.data( 
        accountActivityModel.index(for_index, 2) );          // AmountColumn
      display_data['uuid']      = accountActivityModel.data( 
        accountActivityModel.index(for_index, 3) );          // UUIDColumn
      display_data['memo']      = accountActivityModel.data( 
        accountActivityModel.index(for_index, 4) );          // MemoColumn
      // debugger:
      //console.log("Set display delegate data for:", for_index)
      //QML_Debugger.listEverything(display_data)
    } else {
      console.log("Could not locate Row model data from AccountActivity for index:", for_index)
    }
    //QML_Debugger.listEverything(display_data)
    // return the gathered display data:
    return display_data
  }

  //-----------------------------------------------------------------------------
  // What each entry in the list looks like when delegated from OT AccountActivity
  DelegateModel {
    id: accountActivityModelDelegator
    model: (contextRoot.accountActivityModel === null ? [] : contextRoot.accountActivityModel)

    // what each entry will look like when displayed onto the screen:
    delegate: Column {
      id: rowModelDelegate
      width: contextRoot.width - 64
      height: 32
      z: 100
      padding: 16
      leftPadding: 28
      smooth: true
      // state changed to 'dropDown' due to a Mouse click
      signal comboClicked()

      property var display_data: {}

      //----------------------
      Component.onCompleted: {
        // set the display data from the Column and Row information held at the model
        // index inside the AccountActivity model
        rowModelDelegate.display_data = contextRoot.getDisplayColumnRowData(index)
        // debugger:
        //console.log("\nAccountActivity History model: ", accountActivityModel)
        //QML_Debugger.listEverything(accountActivityModel)
        //console.log("\nTransaction Entry: ", index)
        //QML_Debugger.listEverything(model)
      }

      //-----------------------------------------------------------------------------
      // Display the transaction's header details
      // this is a single line summary of the history entry
      Rectangle {
        id: transactionHeaderDisplayRectangle
        radius: 4
        width: parent.width
        height: 32
        color: CustStyle.accent_fill
        z: 2
        smooth: true
        clip: true
        //----------------------
        Row {
          id: clickableHistoryHeader
          width: parent.width
          spacing: 32

          anchors {
            margins: 8;
            top: parent.top;
            left: parent.left;
            verticalCenter: parent.verticalCenter;
          }
          //----------------------
          Text {
            id: thTransactionTypeText
            width: parent.width / 2
            text: display_data['text']
            font.pixelSize: CustStyle.fsize_button
            smooth: true
            clip: true
          }
          //----------------------
          Text {
            id: thAmountText
            width: parent.width / 8
            text: display_data['amount']
            font.pixelSize: CustStyle.fsize_button
            smooth: true
            horizontalAlignment: Text.AlignLeft
          }
          //----------------------
          Text {
            id: thTimestampText
            width: parent.width / 4
            text: display_data['timestamp'].toLocaleString(Qt.locale(), "ddd MMM d yyyy")
            font.pixelSize: CustStyle.fsize_button
            smooth: true
            horizontalAlignment: Text.AlignRight
          }
        }
        //----------------------
        // Create an interaction area for the 'transactionHeaderDisplayRectangle'
        // and tie that into the 'rowModelDelegate' display's state
        MouseArea {
          anchors.fill: parent;
          onClicked: {
            rowModelDelegate.state = (rowModelDelegate.state === "dropDown" ? "" : "dropDown")
          }
        }

      }//end 'transactionHeaderDisplayRectangle'

      //-----------------------------------------------------------------------------
      // This is what is drawn when the 'rowModelDelegate' state becomes
      // the "dropDown" state.
      Rectangle {
        id: dropDownDisplayDetailsRectangle
        width: rowModelDelegate.width
        height: 0
        color: "lightgray"
        radius: 4.0
        // Covers the round top so header is placed over details display
        Rectangle {
          id: squareTopDetailsDrop
          color: parent.color
          z: 1
          width: parent.width
          visible: (parent.height > 2 ? true : false)
          y: (transactionHeaderDisplayRectangle.y - transactionHeaderDisplayRectangle.height + 4)
          height: 16
          anchors.horizontalCenter: parent.horizontalCenter
        }
        //----------------------
        Column {
          id: detailsBodyColumn
          width: parent.width
          height: parent.height
          clip: true
          padding: 12
          spacing: 12
          //----------------------
          Row {
            id: txidAndPKTexplorerRow
            width: parent.width
            height: uuidDisplayText.height
            spacing: width - uuidDisplayText.width - pktOpenExplorerBut.width - 24
            // display transaction uuid:
            Text {
              id: uuidDisplayText
              text: qsTr("TXID: ") + display_data['uuid']
              font.pixelSize: CustStyle.fsize_normal
            }
            // open system web browser and take user to PKT explorer:
            MatterFi_Button_Accent {
              id: pktOpenExplorerBut
              width: 180
              height: 28
              displayText: qsTr("View in PKT Explorer")
              onClicked: {
                var web_url = contextRoot.pktExplorerBaseURL + "/" + display_data['uuid']
                api.openSystemBrowserLink(web_url)
              }
            }
          }
          //----------------------
          Text {
            text: display_data['timestamp'].toLocaleString(Qt.locale(), "dddd,  MMMM dd yyyy  HH:mm:ss")
            font.pixelSize: CustStyle.fsize_normal
          }
          //----------------------
          Text {
            text: qsTr("Amount: ") + display_data['amount']
            font.pixelSize: CustStyle.fsize_normal
          }
          //----------------------
          /*
          Text {
            text: qsTr("Memo:")
            font.pixelSize: CustStyle.fsize_normal
          }
          
          Text {
            id: detailsMemoText
            text: display_data['memo']
            font.pixelSize: CustStyle.fsize_normal
          }
          */
        }
      }

      //----------------------
      // Is in a "dropDown" State?
      // What changed on the display:
      states: State {
        name: "dropDown"
        PropertyChanges {
          target: dropDownDisplayDetailsRectangle
          height: 120
        }
        PropertyChanges {
          target: rowModelDelegate
          height: 156
        }
      }
      //----------------------
      // What does "dropDown" look like
      // when its changing its display?
      transitions: Transition {
        NumberAnimation {
          target: dropDownDisplayDetailsRectangle
          properties: "height"
          duration: 1000
          easing.type: Easing.OutExpo
        }
        NumberAnimation {
          target: rowModelDelegate
          properties: "height"
          duration: 1000
          easing.type: Easing.OutExpo
        }
      }

    }//end 'rowModelDelegate'

  }//end 'accountActivityModelDelegator'

  //-----------------------------------------------------------------------------
  // Create 'body'
  ListView {
    id: body
    width: contextRoot.width - 6
    height: contextRoot.height - 8
    y: 4
    spacing: 12
    model: accountActivityModelDelegator
    focus: false
    clip: true
    smooth: true
    // scroll bar:
    boundsBehavior: Flickable.StopAtBounds
    ScrollBar.vertical: ScrollBar {
      id: verticalScrollBar
      hoverEnabled: true
      height: 10
      visible: (size !== 1.0)
      active: (hovered || pressed)
      policy: ScrollBar.AsNeeded
      orientation: Qt.Vertical

      contentItem: Rectangle {
        implicitWidth: 10
        implicitHeight: 100
        anchors.leftMargin: 50
        radius: width / 2
        z: 2
        color: (verticalScrollBar.pressed ? CustStyle.pkt_logo : CustStyle.accent_normal)
      }

      Rectangle {
        visible: verticalScrollBar.visible
        width: 1
        height: body.height - 32
        color: CustStyle.dm_outline
        z: 0
        anchors.centerIn: parent
      }
    }

    anchors {
      margins: 8;
      topMargin: 8;
      top: parent.anchors.TopAnchor;
      left: parent.anchors.LeftAnchor;
      bottom: parent.anchors.BottomAnchor;
      right: parent.anchors.RightAnchor;
      horizontalCenter: parent.horizontalCenter;
    }

    footer: Rectangle {
      id: transactionsEndFooter
      width: body.width
      height: 96
      color: "transparent"
    }

  //-----------------------------------------------------------------------------
  }//end 'body'

  // Foreground object:
  OutlineSimple {
    outline_color: CustStyle.accent_outline
    radius: 10
  }

//-----------------------------------------------------------------------------
}//end 'contextRoot'