import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12

import "qrc:/"
import "qrc:/styling"
import "qrc:/matterfi"
import "qrc:/qml_shared"
import "qrc:/drawers"

//-----------------------------------------------------------------------------
// Displays user's contact list and a single user selected activity thread.
Page {
	id: pageRoot
	width: rootAppPage.width
	height: rootAppPage.height
	title: qsTr("Contact Threads")
	background: null //Qt.transparent

	property bool hideNavBar: true // hide navigation bar
	//-----------------------------------------------------------------------------
	// Called on button interaction to navigate back in the app Page stack.
	function onNavBackButton() {
		rootAppPage.popPage();
	}
	//----------------------
	// Called when a contact is clicked on in the list.
	function onContactSelection(contact_index, contact_model) {
		selectedContactActivityDisplay.displayBusySignal()
		// get the ActivityThread for that contact
		pageRoot.current_contact_index = contact_index
		pageRoot.currentContact_model = contact_model
		pageRoot.setContactActivityThread(contact_model)
	}

	//-----------------------------------------------------------------------------
	// Manage contact activity thread display for current selection.
	property var contactActivity_model: ({}) // expecting 'ActivityThreadModel'
	property var currentContact_model: ({})  // expecting 'QAbstractItemModel'
	property var filterString: ""
	property bool contactListisEmpty: true
	property var current_contact_index: -1
	//----------------------
	// Run soon as page is ready.
	Component.onCompleted: {
		// populated the known contacts list for activity thread selection
		pageRoot.populateContacts()
	}
	//----------------------
	// Grab the p2p activity between user and contact selected.
	function setContactActivityThread(contactlist_rowdata) {
		//var name = pageRoot.contactList_model.data( contactList_model.index(contact_index, 0) )
		var contact_address = contactlist_rowdata.id
		OTidentity.setContactActivityFocus(contact_address)
		pageRoot.contactActivity_model = OTidentity.contactActivityThread_OTModel

		//debugger:
		//console.log("\nContacts List: contact was clicked on.", contactlist_rowdata)
		//QML_Debugger.listEverything(contactlist_rowdata)
		//console.log("\n\nGrabed p2p thread activity for contact:", contact_address, contactActivity_model)
		//QML_Debugger.listEverything(pageRoot.contactActivity_model)
		//console.log("\n\n")

		// refresh the current contact display information for targeted activity thread
		selectedContactActivityDisplay.refresh(pageRoot.contactActivity_model, pageRoot.currentContact_model);
		// update display of current contact ActivityThread belongs to.
		currentActivityThreadRect.clear_display()
		if (pageRoot.current_contact_index === 0) {
			currentActivityThreadRect.name = pageRoot.currentContact_model.name + " (myself)"
		} else { // is talking to other users
			currentActivityThreadRect.name = pageRoot.currentContact_model.name
		}
		currentActivityThreadRect.image = pageRoot.currentContact_model.image
		
		//TODO: hardcoded PKT blockchain type.
		// grab the matterfi payment code for clipboard copies.
		currentActivityThreadRect.paymentCode = pageRoot.contactActivity_model.paymentCode(9)

		//debugger:
		console.log("Focused Contact Activity", pageRoot.currentContact_model, 
			currentActivityThreadRect.paymentCode);
		//QML_Debugger.listEverything(pageRoot.contactActivity_model)
	}
	//----------------------
	// Make the display list of available user contacts.
	function populateContacts() {
		OTidentity.checkContacts()
		var fetchedContactList = OTidentity.contactList_OTModel
		//debugger:
		//console.log("Contacts List fetched:", fetchedContactList)
		//QML_Debugger.listEverything(fetchedContactList)
		
		// display the list of contacts known for activity thread selection
		if ( fetchedContactList.rowCount() < 1 ) {
			//console.log("Contacts List: No Contacts where found.")
			contactListisEmpty = true
		} else {
			contactListModelDelegator.model = fetchedContactList
			//console.log("Contacts populated:", pageRoot.contactList_model )
			contactListisEmpty = false
		}
	}
	//----------------------
	// Filtering the contact list displayed.
	function applySearchFilter(filter_text) {
		//debugger:
		//console.log("Applying contact list filter string.", filter_text)
		pageRoot.filterString = filter_text
	}
	//----------------------
	// Call UI to add a new contact to the known list.
	function openNewContactUI() {
		rootAppPage.drawerIsOpen = true
		createnewContactDrawer.open()
	}

	//-----------------------------------------------------------------------------
	// ContextUI Drawers:
	CreateContact_ContextDrawer {
		id: createnewContactDrawer
	}

	//-----------------------------------------------------------------------------
	// Main 'body' layout:
	Column {
		id: body
		height: pageRoot.height
		width: pageRoot.width
		spacing: 0
		anchors.horizontalCenter: parent.horizontalCenter

		// Page Header bar:
		Rectangle {
			id: pageHeader
			width: pageRoot.width
			height: 42
			color: "transparent"
			anchors.horizontalCenter: parent.horizontalCenter

			Row {
				id: headerbarDisplay
				width: parent.width
				height: parent.height
				leftPadding: 16
				spacing: 64
				anchors.horizontalCenter: parent.horizontalCenter
				// navigate to prevous page button
				FontIconButton {
					id: navBackIconButton
					iconChar: IconIndex.arrow_alt_circle_left
					//iconSize: 18
					anchors.verticalCenter: parent.verticalCenter
					color: CustStyle.theme_fontColor
					onAction: pageRoot.onNavBackButton()
				}
				// current contact ActivityThread is focused on
				Rectangle {
					id: currentActivityThreadRect
					width: pageHeader.width / 4
					height: pageHeader.height
					color: "transparent"
					property var name: ""
					property var image: undefined
					property var paymentCode: ""
					
					visible: (currentActivityThreadRect.name !== "")
					property bool shrunk: false
					// used to clear the display of current selected contact details
					function clear_display() {
						currentActivityThreadRect.name = ""
						currentActivityThreadRect.image = undefined
						currentActivityThreadRect.paymentCode = ""
					}
					// display which contact is currently in activity with
					Row {
						id: currentContactDetailsRow
						scale: currentActivityThreadRect.shrunk ? CustStyle.but_shrink : 1.0
						width: parent.width
						height: parent.height
						leftPadding: 4
						anchors.centerIn: parent
						// profile photo:
						MatterFi_Avatar {
							id: contactAvatarImage
							width: parent.height
							height: width
							avatarUrl: ( currentActivityThreadRect.image !== undefined ?
								currentActivityThreadRect.image : "" );
							anchors.verticalCenter: parent.verticalCenter
							// display tool tip notification
							MatterFi_ToolTip {
								id: copyCD_PaymentCodeToolTip
								visible: false
								text: qsTr("MatterCode™ Copied!")
								// time that the ToolTip is displayed for
								Timer {
									id: copyCD_paymentcodeCopied
									interval: 2000
									running: false
									onTriggered: {
										copyCD_PaymentCodeToolTip.visible = false
									}
								}
							}
						}
						// profile name:
						Text {
							id: contactNameText
							text: currentActivityThreadRect.name
							color: CustStyle.theme_fontColor
							leftPadding: 12
							font.pixelSize: CustStyle.fsize_normal
							anchors.verticalCenter: parent.verticalCenter
							verticalAlignment: Text.AlignVCenter
						}
						// work around limitation for QClipBoard in QML not being accessible
						TextEdit {
							id: textClipBoard
							visible: false
						}
					}//end 'currentContactDetailsRow'
					
					// mark input area
					MouseArea {
						id: contactDetailsRow_MA
						anchors.fill: parent
						hoverEnabled: true
						enabled: false
						// copy text on user click
						onClicked: {
							currentActivityThreadRect.shrunk = false
							textClipBoard.text = currentActivityThreadRect.paymentCode
							textClipBoard.selectAll()
							textClipBoard.copy()
							copyCD_PaymentCodeToolTip.visible = true
							copyCD_paymentcodeCopied.start()
						}
						onPressed: { currentActivityThreadRect.shrunk = true }
						onReleased: { currentActivityThreadRect.shrunk = false }
						onEntered: { currentActivityThreadRect.shrunk = true }
						onExited: { currentActivityThreadRect.shrunk = false }
					}
				}//end 'currentActivityThreadRect'
			}//end 'headerbarDisplay'
		}//end 'pageHeader'

		//-----------------------------------------------------------------------------
		// Split the page body down the middle.
		Row {
			id: leftRightPageRow
			width: body.width
			height: body.height - pageHeader.height
			anchors.horizontalCenter: parent.horizontalCenter

				//-----------------------------------------------------------------------------
				// Contacts list header bar, filtering is done with in the DelegateModel.
				Column {
				id: contactsListDisplay
				height: leftRightPageRow.height
				width: 256 //leftRightPageRow.width / 2

				Rectangle {
					id: contactsListHeader
					width: parent.width
					height: 42
					color: CustStyle.neutral_fill
					anchors.horizontalCenter: parent.horizontalCenter
					property color search_color: rootAppPage.currentStyle > 0 ? CustStyle.darkmode_text : CustStyle.lightmode_text
					// interactive contact list bar elements
					Row {
						id: searchbarDisplay
						width: parent.width
						height: parent.height
						leftPadding: 6
						spacing: 8
						// open new contact UI
						FontIconButton {
							id: profileContextButton
							iconChar: IconIndex.address_card
							iconSize: 24
							anchors.verticalCenter: parent.verticalCenter
							onAction: {
								pageRoot.openNewContactUI()
							}
						}
						// search icon
						MatterFi_HugeIconImage {
							width: 20
							height: width
							iconChar: IconIndex.search
							isFAicon_solid: true
							color: contactsListHeader.search_color
							anchors.verticalCenter: parent.verticalCenter
						}
						// search string entry field
						TextField {
							id: searchbox
							placeholderText: "Search"
							maximumLength: 32
							focus: (!rootAppPage.drawerIsOpen)
							clip: true
							width: searchbarDisplay.width - 72
							color: contactsListHeader.search_color
							anchors.verticalCenter: parent.verticalCenter
							verticalAlignment: Text.AlignVCenter
							echoMode: TextInput.Normal
							renderType: Text.QtRendering

							background: Item {
								implicitWidth: parent.width
								implicitHeight: parent.height
								Rectangle {
									color: "transparent"
									height: parent.height
									width: parent.width
									anchors.fill: parent
								}
							}
							// update current typed string filter
							onTextChanged: {
								if (!pageRoot.contactListisEmpty) {
									searchingBusyIndicator.running = true
									searchingBusyIndicator.visible = true
									searchTimeoutTimer.restart()
								}
							}
						}
					}//end 'searchbarDisplay'
				}//end 'contactsListHeader'

				//-----------------------------------------------------------------------------
				// Contact list interactive display
				Rectangle {
					id: contactListView
					width: parent.width
					height: parent.height - contactsListHeader.height
					color: "transparent"
					anchors.horizontalCenter: parent.horizontalCenter

					// The display DelegateModel for a contact in the list:
					DelegateModel {
						id: contactListModelDelegator
						model: pageRoot.contactList_model
						// draw all the contacts, also matching search context if any
						delegate: MatterFi_ContactListDelegate {
							width: listView.width
							contact_data: model
							listIndex: index
							filterString: (pageRoot.filterString)
							onAction: {
								pageRoot.onContactSelection(index, contact_data)
							}
						}
					}
					// show the contact list:
					ListView {
						id: listView
						clip: true
						width: parent.width
						height: parent.height
						anchors.topMargin: 16
						model: contactListModelDelegator
						visible: (!searchingBusyIndicator.visible)
						anchors.top: parent.anchors.TopAnchor
						anchors.left: parent.anchors.LeftAnchor
						anchors.bottom: parent.anchors.BottomAnchor
						anchors.right: parent.anchors.RightAnchor
						anchors.horizontalCenter: parent.horizontalCenter
					}
					// Search time out is busy indication:
					Timer {
						id: searchTimeoutTimer
						interval: 500

						onTriggered: {
							searchingBusyIndicator.running = false
							searchingBusyIndicator.visible = false
							pageRoot.applySearchFilter(searchbox.text)
						}
					}
					// is searching/filtering contact list
					MatterFi_BusyIndicator {
						id: searchingBusyIndicator
						scale: 1.0
						anchors.centerIn: parent
					}
					// displayed when no contacts match search context
					Label {
						id: noMatchesLabel
						anchors.centerIn: parent
						visible: false
						text: "No matches"
						color: CustStyle.theme_fontColor
					}
					// displayed when no contacts are avalaible
					Label {
						id: noContactsLabel
						anchors.centerIn: parent
						visible: contactListisEmpty
						text: "No Contacts"
						color: CustStyle.theme_fontColor
					}
				}//end 'contactListView'
			}//end 'contactsListDisplay'

			//-----------------------------------------------------------------------------
			// Contact activity display area
			MatterFi_ContactActivityDelegate {
				id: selectedContactActivityDisplay
				width: leftRightPageRow.width - contactsListDisplay.width
				height: leftRightPageRow.height
			}

		//-----------------------------------------------------------------------------
		}//end 'leftRightPageRow'
	}//end 'body' Column

//-----------------------------------------------------------------------------
}//end 'pageRoot'
