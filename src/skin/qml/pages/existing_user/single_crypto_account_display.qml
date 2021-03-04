import QtQuick 2.15
import QtQml.Models 2.1
import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12

import "qrc:/"
import "qrc:/styling"
import "qrc:/matterfi"
import "qrc:/qml_shared"
import "qrc:/drawers"

import com.TreeModelAdaptor 1.0
import "qrc:/treeview"

//-----------------------------------------------------------------------------
Page {
	id: pageRoot
	title: qsTr("PKT Asset")
	width: rootAppPage.width
	height: rootAppPage.height
	objectName: "pkt_asset_page"

	background: null //Qt.transparent

	property bool hideNavBar: true // hide navigation bar

	// Current asset details provided by OT AccountList row index, data populated
	property var accountList_selection: undefined // for 'PKT' notaryname match.

	//-----------------------------------------------------------------------------
	// Disconnect blockchain update signals.
	Component.onDestruction: {
		if (pageRoot.focusedAccountActivity_OTModel !== undefined) {
			OTidentity.focusedAccountActivity_OTModel.syncProgressUpdated.disconnect(sync_status)
			//debugger:
      console.log("Cleared focused AcccountActivity signals.")
		}
	}

	// TODO: Due to issues with obtainting Roles as a property on model in Qt,
	// the AccountList model is shoved into a list view to make its roles avaiable.
	Component {
		id: accountListModelDelegator
		Rectangle {
			width: parent.width
			height: 32
			color: "red"

			Component.onCompleted: {
				// Grab the OT AccountList model data at matching notary index for asset display
				if (notaryname == "PKT") {
					pageRoot.accountList_selection = model
					OTidentity.setAccountActivityFocus(account)
					OTidentity.focusedAccountActivity_OTModel.syncProgressUpdated.connect(sync_status) // attach block chain sync status
					//debugger: 
					console.log("Single crypto view, displaying notary:", notaryname)
				}
			}
		}
	}
	// *hidden list
	ListView {
		id: accountlistView
		model: OTidentity.userIdentity_OTModel.accountList
		delegate: accountListModelDelegator
		visible: false
		width: 320
	}

	//-----------------------------------------------------------------------------
	// Context Drawers:
	SendFunds_ContextDrawer {
		id: sendContextDrawer
		currentBalance_displayText: (displayBalanceInUnitsText.text)
	}
	function openSendContextDrawer() {
		sendContextDrawer.accountActivity_model = OTidentity.focusedAccountActivity_OTModel
		sendContextDrawer.accountList_RowModel = OTidentity.accountList_RowModel
		sendContextDrawer.setup()
		rootAppPage.drawerIsOpen = true
		sendContextDrawer.open()
	}
	//----------------------
	ReceiveFunds_ContextDrawer {
		id: receiveContextDrawer
		//currentBalance_displayText: (displayBalanceInUnitsText.text)
	}
	function openReceiveContextDrawer() {
		receiveContextDrawer.accountActivity_model = OTidentity.focusedAccountActivity_OTModel
		receiveContextDrawer.accountList_RowModel = OTidentity.accountList_RowModel
		receiveContextDrawer.refresh()
		rootAppPage.drawerIsOpen = true
		receiveContextDrawer.open()
	}
	
	//-----------------------------------------------------------------------------
	// Block chain syncronization progress display.
	property var syncprogressLow:  0
	property var syncprogressHigh: 100
	property string syncpercentage: "0.0 %"

	function sync_status(current, total) {
		pageRoot.syncprogressLow = current
		pageRoot.syncprogressHigh = total
		bsyncPB.value = (current / total)
		pageRoot.syncpercentage = (bsyncPB.value * 100).toFixed(1)
		//console.log("Sync Progress: ", percentage, current, total)
		percentageTextDisplay.text = "%1 %".arg(pageRoot.syncpercentage)
		if (bsyncPB.value >= 1.0) {
			bsyncPB.isdone = true
		} else {
			bsyncPB.isdone = false
		}
	}

	//-----------------------------------------------------------------------------
	// The main 'body' layout.
	Column {
		id: body
		spacing: 16
		topPadding: 12
		width: pageRoot.width
		height: walletBrandingImage.height
		anchors.horizontalCenter: parent.horizontalCenter

		//----------------------
		// Display the top of the page header
		Row {
			id: topHeaderElementRow
			spacing: 32
			leftPadding: pageRoot.width / 2 - walletBrandingImage.width - 160
			width: walletBrandingImage.width + accountSummaryDisplay.width
			height: walletBrandingImage.height
			//anchors.horizontalCenter: parent.horizontalCenter

			// BlockChain Logo:
			Image {
				id: walletBrandingImage
				width: 300
				height: 170
				sourceSize.width: walletBrandingImage.width
				sourceSize.height: walletBrandingImage.height
				source: "qrc:/assets/svgs/pkt-logo.svg"
				fillMode: Image.PreserveAspectFit
				smooth: true
				anchors {
					topMargin: 0;
					bottomMargin: 0;
				}
			}

			// Display group for the account's summary of current balance and worth.
		  Column {
				id: accountSummaryDisplay
				height: parent.height
				width: 240
				spacing: 8
				topPadding: 8
				//----------------------
				// Display the current BlockChain value held with-in the user's wallet.
				Row {
					id: balanceDisplayRow
					width: parent.width
					height: parent.height
					spacing: 16

					Text {
						id: displayBalanceText
						text: qsTr("Balance: ")
						color: CustStyle.accent_text
						font.bold: true
						font.pixelSize: CustStyle.fsize_large
						anchors.verticalCenter: parent.verticalCenter
					}
					
					Text {
						id: displayBalanceInUnitsText
						// here is where the balance display value is set
						text: (OTidentity.focusedAccountActivity_OTModel !== undefined ?
							OTidentity.focusedAccountActivity_OTModel.displayBalance : qsTr("0.0")
						);
						color: CustStyle.accent_text
						font.bold: true
						font.pixelSize: CustStyle.fsize_xlarge
						anchors.verticalCenter: parent.verticalCenter
					}
				}
			}
		}

		//-----------------------------------------------------------------------------
		// Button Row
		Row {
			id: pageButtons
			spacing: 30
			anchors.horizontalCenter: parent.horizontalCenter
			//----------------------
			MatterFi_Button_Standard {
				id: send_button
				displayText: qsTr("Send")
				onClicked: pageRoot.openSendContextDrawer()
			}
			//----------------------
			MatterFi_Button_Standard {
				id: receive_button
				displayText: qsTr("Receive")
				onClicked: pageRoot.openReceiveContextDrawer()
			}
		}

		//-----------------------------------------------------------------------------
		// display the account transaction history:
		MatterFi_TransactionHistoryDisplay {
			id: transaction_display
			width: pageRoot.width
			height: pageRoot.height - accountSummaryDisplay.height - bsyncPBStatus.height - 116
			accountActivityModel: OTidentity.focusedAccountActivity_OTModel
		}

	}//end 'body'

	//-----------------------------------------------------------------------------
	// Display the current sync status of the BlockChain:
	Rectangle {
		id: bsyncPBStatus
		width: 320
		height: outstandingStatus.height
		color: "transparent"
		y: pageRoot.height - bsyncPBStatus.height - 12
		x: pageRoot.width - width - 152
		// ui hint text
		Text {
			id: syncDeligateAreaText
			text: qsTr("Syncing Blockchain:")
			color: CustStyle.theme_fontColor
			font.pixelSize: CustStyle.fsize_normal
			anchors.verticalCenter: parent.verticalCenter
		}
		// show percentage value
		Text {
			id: percentageTextDisplay
			text: "0.0 %"
			width: 48
			color: CustStyle.theme_fontColor
			font.pixelSize: CustStyle.fsize_normal
			anchors {
				verticalCenter: parent.verticalCenter;
				left: syncDeligateAreaText.right
				leftMargin: 8
			}
		}
		// bar display
		MatterFi_ProgressBar {
			id: bsyncPB
			minimum: pageRoot.syncprogressLow
			maximum: pageRoot.syncprogressHigh
			width: 200
			anchors {
				verticalCenter: parent.verticalCenter;
				left: percentageTextDisplay.right
				leftMargin: 8
			}
		}
		// toggle being shown when sync is done
		visible: (!bsyncPB.isdone)
	}
	// Displayed when the sync is working on Outstanding Blocks
	Row {
		id: outstandingStatus
		width: 128
		spacing: 8
		y: pageRoot.height - 32
		x: pageRoot.width - width - 96
		// display status of blockchain synchronization
		Text {
			text: qsTr("| Blockchain Synced |")
			color: CustStyle.theme_fontColor
			font.pixelSize: CustStyle.fsize_normal
			anchors.verticalCenter: parent.verticalCenter
		}
		// toggle being shown when sync is done
		visible: (bsyncPB.isdone)
	}

	//-----------------------------------------------------------------------------
	// Display the current sync status of the BlockChain:
	MatterFi_BlockChainStatistics {
		id: blockchainStatGroup
		width: parent.width
		y: bsyncPBStatus.y - 8
	}

	//-----------------------------------------------------------------------------
	// Display the context menu
	FontIconButton {
		id: menuContextButton
		iconChar: IconIndex.ellipsis_v
		//iconSize: 18
		x: pageRoot.width - width - 12
		y: 96

		onAction: {
			contextMenu.popup()
		}
		// Create the menu:
		Menu {
			id: contextMenu

			background: Rectangle {
				id: contextItemMenuBG
				implicitWidth: 170
				implicitHeight: 38
				color: CustStyle.theme_fontColor
				border.color: CustStyle.pkt_logo
				radius: 2
			}
			//----------------------
			// Menu Actions:
			Action {
				text: qsTr("View My Contacts")
				onTriggered: {
					rootAppPage.pushPage("pages/contacts/contact_activity_threads.qml")
				}
			}
			// TODO: replacement for TreeView sell out from Controls 1.4 Qt5 into 2.0 Qt6.
			Action { 
				text: qsTr("Advanced Wallet Details")
				onTriggered: {
					//rootAppPage.pushPage("pages/existing_user/advanced_wallet_details.qml")
					rootAppPage.popupShowWindow("qrc:/pages/existing_user/advanced_wallet_details.qml")
				}
			}
			Action { 
				text: qsTr("My Recovery Words")
				onTriggered: {
					rootAppPage.pushPage("pages/existing_user/show_recovery_phrase.qml")
				}
			}
			Action { 
				text: qsTr("Version")
				onTriggered: {
					rootAppPage.pushPage("pages/app_version.qml")
				}
			}
			//----------------------
			// What the menu items look like:
			delegate: MenuItem {
				id: menuItem
				implicitWidth: contextItemMenuBG.implicitWidth
				implicitHeight: contextItemMenuBG.implicitHeight
				arrow: Canvas {}
				indicator: Item {}
				contentItem: Text {
					leftPadding: menuItem.indicator.width
					rightPadding: menuItem.arrow.width
					text: menuItem.text
					font: menuItem.font
					opacity: enabled ? 1.0 : 0.3
					color: menuItem.highlighted ? CustStyle.theme_fontColor : CustStyle.pkt_logo
					horizontalAlignment: Text.AlignLeft
					verticalAlignment: Text.AlignVCenter
					elide: Text.ElideRight
				}
				background: Rectangle {
					implicitWidth: menuItem.implicitWidth
					implicitHeight: menuItem.implicitHeight
					opacity: enabled ? 1 : 0.3
					color: menuItem.highlighted ? CustStyle.pkt_logo : "transparent"
				}
			}
		}
	}

//-----------------------------------------------------------------------------v
}//end 'pageRoot'
