import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12


import "qrc:/styling"
import "qrc:/matterfi"
import "qrc:/qml_shared"
import "qrc:/drawers"

// import org.opentransactions.metier 1.0 as OTModels

//-----------------------------------------------------------------------------
// Displays the user's wallet with all the accounts/payment codes with held.
Page {
    id: pageRoot
    title: qsTr("User Accounts")
    width: rootAppPage.width
    height: rootAppPage.height
    objectName: "accounts"

    background: null //Qt.transparent

    property bool hideNavBar: true // hide navigation bar
    
    property var accountListModel_OT: api.accountListModelQML()
    //-----------------------------------------------------------------------------
    // Create connections:
    Component.onCompleted: {
        // display block sync notification
        //var accountList_model = api.accountListModelQML()
        accountListModelDelegator.model = api.accountListModelQML()

        QML_Debugger.listEverything(accountListModelDelegator.model)

        accountListModel_OT.dataChanged.connect(pageRoot.on_accountlist_datachanged)
        console.log("PageRoot is loaded.")
    }

    // Disconnect any account update signal slots:
    Component.onDestruction: {
        accountListModel_OT.dataChanged.disconnect(pageRoot.on_accountlist_datachanged)
        if (selectedAccountActivty_OT !== undefined) {
            selectedAccountActivty_OT.syncProgressUpdated.disconnect(sync_status)
        }
    }

    //-----------------------------------------------------------------------------
    // Context Drawers:
    SendFunds_ContextDrawer {
        id: sendContextDrawer
    }
    function openSendContextDrawer(accountActivity_model) {
        sendContextDrawer.accountActivity_model = accountActivity_model
        sendContextDrawer.accountList_RowModel = pageRoot.selectedAccountListRow
        sendContextDrawer.open()
    }
    
    ReceiveFunds_ContextDrawer {
        id: receiveContextDrawer
    }
    function openReceiveContextDrawer(accountActivity_model) {
        receiveContextDrawer.accountActivity_model = accountActivity_model
        receiveContextDrawer.accountList_RowModel = pageRoot.selectedAccountListRow
        receiveContextDrawer.refresh()
        receiveContextDrawer.open()
    }

    //-----------------------------------------------------------------------------
    // AccountActivity interactions:
    function on_accountActivity_contactSend() {
        // nav to contact list for selection
        rootAppPage.pushPage("pages/contacts/contact_list.qml")
    }
    

    function on_accountActivity_receiveFunds() {
        pageRoot.openReceiveContextDrawer(pageRoot.selectedAccountActivty_OT)
    }

    // send to address
    function on_accountActivity_walletSend() {
        // open context draw manager
        pageRoot.openSendContextDrawer(pageRoot.selectedAccountActivty_OT)
    }

    //-----------------------------------------------------------------------------
    // Update the acccount model data sets
    function on_accountlist_datachanged(index, bottomRight, roles) {
        console.log("signal received for accountList model") //, index, bottomRight, roles)
        //accountActivityDisplayDelegate.refreshActivityView()
        accountListModelDelegator.model = api.accountListModelQML()
    }

    function onclick_refresh_account_modellist() {
        accountListModelDelegator.model = api.accountListModelQML()
        console.log("updated accountList model")
    }

    //-----------------------------------------------------------------------------
    // Managing active models used in the page and it's widgets.
    property var selectedAccountActivty_OT: undefined
    property var selectedAccountListRow: undefined
    // account list item was selected from list
    function onAction_accountList(selection) {
        // selection is the model from the AccountList for that user's wallets
        console.log("Account selection made: ", selection)
        // pass along the slected accountlist model for the accounts balance summary
        //pageRoot.selectedAccountListRow = selection
        pageRoot.selectedAccountListRow = selection

        var activityModel = api.accountActivityModelQML(selection.account)
        // if the AccountList model changes it 
        // will void this activity model pointer value
        // take the block chain model for the associated List account selection
        // and pass the users wallet id to it to populate the account transaction history

        //console.log("Activity Model displayed:")
        //QML_Debugger.listEverything(activityModel)
        // TODO: 'Progress' is not defined qml type. *only happens when displaying all properties*
        //console.log("Activity list length:", activityModel.rowCount() )
        pageRoot.selectedAccountActivty_OT = activityModel

        // sync with active account activity's BlockChain from the AccountList
        if (selectedAccountActivty_OT != null) {
            // disconnect from current block chain
            selectedAccountActivty_OT.syncProgressUpdated.disconnect(sync_status)
            // then reconnect the signal to the new selected
            // AccountActivity model
            selectedAccountActivty_OT.syncProgressUpdated.connect(sync_status)
            // BlockChain sync status for slected AccountActivity model
            // should be connected to sync progress bar display.
        }
    }
    
    //-----------------------------------------------------------------------------
    // Block chain syncronization progress display.
    property var syncprogressLow:  0
    property var syncprogressHigh: 100

    function sync_status(current, total) {
        pageRoot.syncprogressLow = current
        pageRoot.syncprogressHigh = total
        //console.log("Sync Progress: ", current, total)
        bsyncPB.value = (current / total)
    }
    // display the current sync status of the BlockChain:
    Row {
        id: bsyncPBStatus
        x: accountListRect.x + accountListRect.width
        y: pageRoot.height - 32
        width: 200
        spacing: 8
        // ui hint text
        Text {
            text: qsTr("Syncing BlockChain:")
            color: CustStyle.theme_fontColor
            font.pixelSize: CustStyle.fsize_normal
            anchors.verticalCenter: parent.verticalCenter
        }
        // bar display
        MatterFi_ProgressBar {
            id: bsyncPB
            minimum: pageRoot.syncprogressLow
            maximum: pageRoot.syncprogressHigh
            width: parent.width
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    //-----------------------------------------------------------------------------
    // Side by side account selection and activity display:
    Row {
        id: displayAccounts
        y: 8
        leftPadding: 16
        height: pageRoot.height - 52
        spacing: 8
        anchors.horizontalCenter: parent.horizontalCenter
        width: pageRoot.width

        //----------------------
        // User Account Selection list
        Rectangle {
            id: accountListRect
            width: parent.width / 3 - 16 //340
            height: parent.height
            color: "transparent"
            radius: 2.0
            // out line the list rect:
            OutlineSimple {}
            // The display DelegateModel for account details from list,
            // how each of the list items are drawn for display:
            DelegateModel {
                id: accountListModelDelegator
                model: [] // this gets set via: api.accountListModelQML()
                // draw each account entry the wallet is managing
                // where it itterates over each RowCount with in the provided model
                delegate: MatterFi_AccountListDelegate {
                    listEntry: model
                    indexEntry: index
                    height: 36
                    width: accountListRect.width - 16
                    onAction: pageRoot.onAction_accountList(listEntry)
                }
            }
            // show the account selection list:
            ListView {
                id: accountlistView
                model: accountListModelDelegator
                clip: true
                topMargin: 8
                spacing: 8
                width: parent.width - 16
                height: parent.height - 16
                // screen position anchors
                anchors {
                    top: parent.anchors.TopAnchor;
                    topMargin: 16;
                    left: parent.anchors.LeftAnchor;
                    bottom: parent.anchors.BottomAnchor;
                    bottomMargin: 8;
                    right: parent.anchors.RightAnchor;
                    horizontalCenter: parent.horizontalCenter;
                }
            }

        }
        //----------------------
        // AccountList selected AccountActivity model display
        Rectangle {
            id: accountActivityRect
            width: (displayAccounts.width / 3 * 2) - 24
            height: displayAccounts.height //128
            color: "transparent"
            // AccountList display BG Styler:
            Rectangle {
                implicitWidth: parent.width
                implicitHeight: parent.height
                radius: 4.0
                // use a Gradient color filler:
                gradient: CustStyle.accent_gradient
                // bg outline
                OutlineSimple {}   
            }
            // the display DelegateModel for selected AccountActivity:
            MatterFi_AccountActivity {
                id: accountActivityDisplayDelegate
                accountActivityModel: pageRoot.selectedAccountActivty_OT
                accountListModel: pageRoot.selectedAccountListRow
                width: parent.width - 16
                height: parent.height + 8
                anchors.horizontalCenter: parent.horizontalCenter
                //onContactSend: pageRoot.on_accountActivity_contactSend()
                onReceiveButton: pageRoot.on_accountActivity_receiveFunds()
                onWalletSend: pageRoot.on_accountActivity_walletSend()
            }
        }

    }//end list and activity display

    //-----------------------------------------------------------------------------v
}
