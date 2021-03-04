import QtQuick 2.15
import QtQml.Models 2.1
import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12

import "qrc:/styling"
import "qrc:/styling/cust_formaters"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// 'MatterFi_AccountActivity.qml'
// Delegate for 'AccountList' model's 'AccountActivity' selection model display. 
// This is the user input for Activity related acctions to a user's Account. It
// also should contain the account's ledger for transaction history. This history
// Will not be avaliable untill the block chain has finished its sync.
// deps/opentxs/include/opentxs/ui/qt/AccountActivity.hpp
//
// Roles { *Note* user Roles start at '256'
//  PolarityRole: 256
//  ContactsRole: 257
//  WorkflowRole: 258
//  TypeRole:     259
// }
//
// Functions:
//    onSendToContactButton
//    validateAmount
//    displayBalance
//    getDestValidator
//    getScaleModel
//-----------------------------------------------------------------------------
Item {
    id: contextRoot

    property var accountActivityModel: undefined // looking for an AccountActivity model
    property var accountListModel: undefined     // looking for AccountList model

    property bool isEnabled: true

    signal contactSend()   // triggered on contact Button
    signal walletSend()    // triggered on send Button
    signal receiveButton() // triggered on receive Button

    //-----------------------------------------------------------------------------
    function onSendToContactButton() {
        //console.log("Send request to Contact: ")
        contextRoot.contactSend()
    }

    function onReceiveFundsButton() {
        contextRoot.receiveButton()
    }

    // send functionality is handeled not in this contextRoot,
    // but by the Page container its utilized in for 
    // getting access to the AccountActivity model.
    function onSendToWalletButton() {
        contextRoot.walletSend()
    }

    // update History when AccountActivity changes
    property var historyCount: 0
    function refreshActivityView() {
        console.log("AccountActivity contextRoot Refreshing: ")
        if (contextRoot.accountActivityModel === null) {
            contextRoot.historyCount = 'syncing'
        } else {
            // read the number of entries listed in the AccountActivity model
            contextRoot.historyCount = contextRoot.accountActivityModel.rowCount()
        }
    }
    
    //-----------------------------------------------------------------------------
    // main influencer in item layout positions, 'body'
    Rectangle {
        id: body
        width: parent.width
        height: parent.height
        color:  "transparent"

        //-----------------------------------------------------------------------------
        Column {
            id: activityOverLedgerColumn
            width: body.width
            height: body.height
            spacing: 8
            topPadding: 8

            //----------------------
            // null selection place holder for when there is
            // no selected accountListModel provided.
            Rectangle {
                id: bg_displayArea
                visible: (contextRoot.accountListModel === undefined)
                height: 128
                width: body.width
                color: Universal.background // "transparent"
                radius: 4.0
                //----------------------
                Text {
                    id: accountSelectionHelpText
                    text: qsTr("Please select an Account from the provided list on the left.")
                    color: CustStyle.theme_fontColor
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: CustStyle.fsize_normal
                    anchors.fill: parent
                }
            }

            //-----------------------------------------------------------------------------
            // Display an account summary of status and place interaction buttons
            Rectangle {
                id: accountActivityInteractions_bg_Rectangle
                visible: (contextRoot.accountListModel !== undefined)
                width: body.width
                y: accountActivityInteractions.topPadding
                height: accountActivityInteractions.height
                color: Universal.background // "transparent"
                // outline the bg area as well.
                OutlineSimple {
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                }
                //----------------------
                // Where the summary is Delegated:
                Column {
                    id: accountActivityInteractions
                    width: parent.width
                    spacing: 4
                    topPadding: 8
                    leftPadding: 8
                    height: 128
                    
                    //----------------------
                    MatterFi_PaymentCode {
                        id: usersPaymentCode
                        displayLable: qsTr("My PKT Payment code: ")
                        displayText: (accountListModel != null ? accountListModel.account : "null")
                    }
                    //----------------------
                    Text {
                        id: accountBalanceText
                        text: qsTr("Balance: ") + 
                            (accountListModel != null ? accountListModel.balance : "null")
                        color: CustStyle.theme_fontColor
                        font.pixelSize: CustStyle.fsize_normal
                        verticalAlignment: Text.AlignVCenter
                    }
                    //----------------------
                    Text {
                        id: accountRowCountText
                        text: qsTr("History Entrys: ") + contextRoot.historyCount
                        color: CustStyle.theme_fontColor
                        font.pixelSize: CustStyle.fsize_normal
                        verticalAlignment: Text.AlignVCenter
                    }
                    //----------------------
                    // Buttons: [send to Contact, refresh list]
                    Row {
                        spacing: 8

                        /*
                        MatterFi_Button_Standard {
                            id: onSendToContactButtonBut
                            width: 150
                            displayText: qsTr("Send To Contact")
                            onClicked: contextRoot.onSendToContactButton()
                        }
                        */

                        MatterFi_Button_Standard {
                            id: onSendToWalletButtonBut
                            width: 150
                            displayText: qsTr("Send Funds")
                            onClicked: contextRoot.onSendToWalletButton()
                        }

                        MatterFi_Button_Standard {
                            id: onReceiveFundsButtonBut
                            width: 150
                            displayText: qsTr("Receive Funds")
                            onClicked: contextRoot.onReceiveFundsButton()
                        }
                        /*
                        // debug force refresh list
                        MatterFi_Button_Standard {
                            id: refreshActivityModel
                            width: 200
                            displayText: qsTr("Refresh Transaction History")
                            onClicked: contextRoot.refreshActivityView()
                        }
                        */
                    }
                }

            }//end Top Column, 'Interactions' area

            //-----------------------------------------------------------------------------
            // display the account transaction history:
            Rectangle {
                id: accountActivityListRect
                width: body.width
                height: contextRoot.height - accountActivityInteractions.height - 32
                color: Universal.background // "transparent"
                radius: 4.0
                // bg outline
                OutlineSimple {
                    //z: 100
                    //outline_color: "white"
                }
                // account activity history Deligation
                DelegateModel {
                    id: accountActivityModelDelegator
                    model: (contextRoot.accountActivityModel === null ? [] : contextRoot.accountActivityModel)
                    // itterate threw each index in the model 'delegating' (painting) them
                    delegate: Rectangle {
                        Text {
                            text: "entry index: " + index
                            color: CustStyle.theme_fontColor
                            font.pixelSize: CustStyle.fsize_normal
                        }
                        /*
                        // debugger
                        Component.onCompleted: {
                            console.log("AccountActivity contextRoot entry: ", index)
                            QML_Debugger.listEverything(model)
                        }
                        */
                    }
                }
                //----------------------
                // display the activity entries
                ListView {
                    id: accountActivitylistView
                    width: parent.width
                    height: parent.height
                    model: accountActivityModelDelegator
                    clip: true
                    spacing: 16
                    
                    anchors {
                        margins: 8;
                        topMargin: 16;
                        top: parent.anchors.TopAnchor;
                        left: parent.anchors.LeftAnchor;
                        bottom: parent.anchors.BottomAnchor;
                        right: parent.anchors.RightAnchor;
                        horizontalCenter: parent.horizontalCenter;
                    }
                }
                //----------------------
                // no activity was listed
                Column {
                    id: noActivityToDisplayColumn
                    spacing: 8

                    anchors {
                        topMargin: 16;
                        top: parent.anchors.TopAnchor;
                        left: parent.anchors.LeftAnchor;
                        bottom: parent.anchors.BottomAnchor;
                        right: parent.anchors.RightAnchor;
                        horizontalCenter: parent.horizontalCenter;
                        verticalCenter: parent.verticalCenter;
                    }

                    Text {
                        visible: (contextRoot.historyCount < 1)
                        id: noAccountHistoryText
                        text: qsTr("There is no Account History to report.")
                        color: CustStyle.theme_fontColor
                        font.pixelSize: CustStyle.fsize_normal
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Text {
                        visible: (contextRoot.historyCount < 1)
                        id: noAccountHistoryTip
                        text: qsTr("Wait for the Blockchain to sync.")
                        color: CustStyle.theme_fontColor
                        font.pixelSize: CustStyle.fsize_normal
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }//end 'accountActivityListRect'

        }//end 'activityOverLedgerColumn'

    //-----------------------------------------------------------------------------
    }//end 'body'
//-----------------------------------------------------------------------------
}//end 'contextRoot'
