import QtQuick 2.15
import QtQuick.Window 2.12
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12

import "qrc:/"
import "qrc:/qml_shared"
import "qrc:/styling"
import "qrc:/drawers"

//-----------------------------------------------------------------------------
/*
  Main Application entry point. Creates the Qt5 Window Object.

  Navigation:
		Navigation is handled with 'stackView'. Most required functions
  can be found in 'rootAppPage' for page navigation stack management.

  .pushPage(page.qml)    | add a page to the top of the view/activity stack
	.popPage()             | remove the very last page (on on view top) from stack
	.replacePage(page.qml) | replace current with a new page
	.clearStackHome()      | clear stack down to new, push "Home.qml" or page passed.

  Context Buttons:
		Different buttons can be displayed with in the NavBar if the Page being
    displayed sets one of the flags bellow as true.

    page.showSaveButton:   true  | provides 'save'   page.onSaveButtonAction()
    page.showQRcodeButton: true  | provides 'qrcode' page.onQRcodeButtonAction()
    page.showConvoButton:  true  | provides 'convo'  page.onConvoButtonAction()

  In order to connect to the Open-Transactions library wrapper you need to
  utilize the OpenTransaction 'api' rootContext. Sample:

    api.<function>.connect(qml_id.function_name)

*/
//-----------------------------------------------------------------------------

//TODO: application os menubar.
// Since not using ApplicationWindow due to wrapper, need to use equivlent component.
// that will support simular 'header' support and maintain StackView.
// https://doc.qt.io/qt-5/qtquickcontrols2-containers.html
// https://doc.qt.io/archives/qt-5.10/qml-qtquick-controls-applicationwindow.html

Page {
  id: rootAppPage

  visible: true
  title: qsTr("Stack")

  property bool drawerIsOpen: false // up keep for active focus

  // OT signal state variables. These are for judging what start up state the App is in.
  property bool isFirstRun: true      // is the users first run threw app?
  property bool hasProfile: false     // does the user already have a profile?
  property bool mainIsReady: false    // the user has complete all start up requirements?
  property bool stillNeedBlock: false // user still needs to select a blockchain?

  property var passAlongData: ({}) // Shared container for sub page tree navigations.

  //-----------------------------------------------------------------------------
  // Whats the first page to be seen? Helps with debugging layouts.
  function firstpage() {
    if (rootAppPage.mainIsReady) {
      //rootAppPage.clearStackHome("pages/existing_user/user_accounts.qml")
      //rootAppPage.clearStackHome("pages/existing_user/show_recovery_phrase.qml")
      rootAppPage.clearStackHome("pages/existing_user/single_crypto_account_display.qml")
    } else if (rootAppPage.isFirstRun) {
      console.log("Main: App state is first run.", rootAppPage.isFirstRun)
      rootAppPage.clearStackHome() // "home.qml" by default
    } else if (!rootAppPage.hasProfile) {
      console.log("Main: App state is in need of User Profile.")
      rootAppPage.replacePage("pages/newuser/create_new_profile.qml")
    } else if (rootAppPage.stillNeedBlock) {
      console.log("Main: App state is still in need of BlockChain.")
      rootAppPage.replacePage("pages/existing_user/block_chain_chooser.qml")
    } else {
      console.log("Main: App state Un-Known. Went to \"Home.qml\"")
      rootAppPage.clearStackHome()
    }
    //----------------------
    // Display current signal status:
    console.log("Main OT State Signals |",
      "MR:[",  rootAppPage.mainIsReady,     		// Main is Ready
      "] FR:[", rootAppPage.isFirstRun,     		// Is the Frist Run
      "] NP:[", !rootAppPage.hasProfile,    		// User needs Profile
      "] NB:[", rootAppPage.stillNeedBlock, "]" // Needs a BlockChain
    );
  }

  //-----------------------------------------------------------------------------
  // Grab the current applications focused QWindow object pointer.
  // https://docs.huihoo.com/qt/5.x/qml-qtquick-window-window.html
  property var metier_OSwindow: ({})
  function focusWindowChanged(window_in_focus) {
	if (window_in_focus === null) return;
		// change the title used in the window
		rootAppPage.metier_OSwindow = window_in_focus
		rootAppPage.metier_OSwindow.setTitle("pkt-desktop")
		rootAppPage.metier_OSwindow.alert(2500) // mili seconds to flash until active
		// watch for changes in window display:
		rootAppPage.metier_OSwindow.visibilityChanged.connect(rootAppPage.visibility_changed)
		// watch the active focus item with in the window:
		rootAppPage.metier_OSwindow.onActiveFocusItemChanged.connect(rootAppPage.onActiveFocusItemChanged)
		//debugger:
		//console.log("Window changed.", window_in_focus)
		//console.log("GuiApp:", metier) 
		//QML_Debugger.listEverything(metier)
		//console.log("MainWindow: ", window_in_focus)
		//QML_Debugger.listEverything(window_in_focus)
  }

  // If nothing is currently active focus, then set the rootAppPage as 
  // focus item for Key press events.
  function onActiveFocusItemChanged(focus_item) {
		//QML_Debugger.listEverything(focus_item)
		if (rootAppPage === undefined || rootAppPage === null) return; 
		if (focus_item === undefined && !rootAppPage.drawerIsOpen) {
			focus = true
		}
		//console.log("Keyboard Window Focus Item:", focus_item)
  }
	// active focus debugger, which Object is receiving input? -was avaliable in AppWindow
  //onActiveFocusItemChanged: console.log("Item With Focus: ", activeFocusItem)

  // Detected a change in window display modes.
  function visibility_changed(visibility) {
		//console.log("Main Window Visibility Changed.", visibility)
		//if (visibility === 5) {
			// return focus to the StackView if swap was to full screen mode
			focus = true
		//}
  }

  // Return screen to Normal window mode.
  function keyEvent_normal_window_size(key_event) {
		//console.log("Test ESC key.")
		if (Qt.platform.os === "osx" || Qt.platform.os === "unix") {
			if (rootAppPage.metier_OSwindow !== null) {
				//console.log(rootAppPage.metier_OSwindow.visibility)
				// QWindow::FullScreen == 5
				if (rootAppPage.metier_OSwindow.visibility == 5) {
					// if in full screen, return to normal screen
					rootAppPage.metier_OSwindow.showNormal()
					key_event.accepted = true
				} else {
					key_event.accepted = false
				}
			} else {
				key_event.accepted = false
			}
		}
  }

  //-----------------------------------------------------------------------------
  // Link OT connections:
  Component.onCompleted: {
		// create app startup condition signals
		startup.displayBlockchainChooser.connect(rootAppPage.display_blockChainChooser)
		startup.displayFirstRun.connect(rootAppPage.display_firstRun)
		startup.displayMainWindow.connect(rootAppPage.display_mainWindow)
		startup.displayNamePrompt.connect(rootAppPage.display_namePrompt)
		// get the focuse QWindow object for things like MenuBar
		metier.focusWindowChanged.connect(rootAppPage.focusWindowChanged)
		// debugger:
		//console.log("Connected startup signals from OT.");
  }

  // Disconnect any signals:
  Component.onDestruction: {
		if (metier_OSwindow !== null) {
			rootAppPage.metier_OSwindow.visibilityChanged.disconnect(rootAppPage.visibility_changed)
			rootAppPage.metier_OSwindow.onActiveFocusItemChanged.disconnect(rootAppPage.onActiveFocusItemChanged)
			metier.focusWindowChanged.disconnect(rootAppPage.focusWindowChanged)
			//debugger:
      console.log("Cleared Metier signals.")
		}
		if (startup !== null) {
			startup.displayBlockchainChooser.disconnect(rootAppPage.display_blockChainChooser)
			startup.displayFirstRun.disconnect(rootAppPage.display_firstRun)
			startup.displayMainWindow.disconnect(rootAppPage.display_mainWindow)
			startup.displayNamePrompt.disconnect(rootAppPage.display_namePrompt)
			//debugger:
      console.log("Cleared startup signals.")
		}
  }

  //-----------------------------------------------------------------------------
  // function Connections:
  function display_firstRun() {
		console.log("Startup Step 1: Choose New/Recover.");
		// default is to navigate to the Home screen where selection can be made
		rootAppPage.isFirstRun = true
		rootAppPage.mainIsReady = false
		rootAppPage.hasProfile = false
		rootAppPage.stillNeedBlock = true
		rootAppPage.firstpage();
  }

  function display_blockChainChooser() {
		console.log("Startup Step 2: Choose BlockChain.");
		// if a seed is found, the app needs to jump past generating one
		// or it will fail.
		rootAppPage.mainIsReady = false
		rootAppPage.stillNeedBlock = true
		rootAppPage.isFirstRun = false
		rootAppPage.firstpage();
  }
  
  function display_namePrompt() {
		console.log("Startup Step 3: Ask the user to enter their name.");
		// if the user doesnt have a name, they must make a profile with one
		rootAppPage.mainIsReady = false
		rootAppPage.isFirstRun = false
		rootAppPage.hasProfile = false
		rootAppPage.firstpage();
  }

  function display_mainWindow() {
		console.log("Startup Step 4: Show the Main Navigator.");
		// throws user into page navigation for the wallet UI
		rootAppPage.hasProfile = true
		rootAppPage.stillNeedBlock = false
		rootAppPage.isFirstRun = false
		rootAppPage.mainIsReady = true
		rootAppPage.firstpage();
  }

  //-----------------------------------------------------------------------------
  // Navigation
  property bool hideNavBar: true;// hidden at boot time till page is ready.
  property bool displaying_popup: false;

  header: MatterFi_HeaderBar {
		id: headerBar
		stackview: stackView
  }
  // get the actuall header bar high for resizing the display Page in the StackView.
  function toolbar_height() {
		if (hideNavBar != false) {
			return headerBar.contentHeight
		}
		return 0
  }

  // clear the temp data pointer used to pass information with in navigation trees.
  function clear_passAlong() {
		rootAppPage.passAlongData = ({})
  }

  //-----------------------------------------------------------------------------
  // Stack page control:
  function currentPage() {
		return stackView.currentItem.objectName
  }
  // replace current page in stack:
  function replacePage(page) {
		//console.log("Nav Replacing Current Page:")
		//rootAppPage.popPage()
		//rootAppPage.pushPage(page)
		stackView.replace(page)
  }

  // adding page to stack
  function pushPage(page) {
		var pageName = page.split('/')
		pageName = pageName[pageName.length - 1]
		pageName = pageName.replace('.qml', '')
		if (stackView.depth >= 10) {
			console.warning("Nav StackView is getting past 10 pages! ", stackView.depth)
		}
		stackView.push(page)
		//console.log("Nav Push: \"", pageName, "\" Stack depth: ", stackView.depth)
		rootAppPage.hidingNavbar()
  }

  // remove last from stack
  function popPage() {
		if (stackView.currentItem == null) {
			console.log("Nav Pop: \"no page to pop\" Stack depth: ", stackView.depth)
			return;
		}
		var lastpageName = stackView.currentItem.objectName.split('/')
		lastpageName = lastpageName[lastpageName.length - 1]
		lastpageName = lastpageName.replace('.qml', '')
		stackView.pop()
		var pageName = stackView.currentItem.objectName
		//console.log("Nav Pop: \"", pageName, "\" Stack depth: ", stackView.depth)
		rootAppPage.hidingNavbar()
  }

  // clear the stack and return home
  function clearStackHome(page = null) {
		if (stackView.currentItem != null) {
			var pageName = stackView.currentItem.objectName
			console.log("Nav Stack Clearing: \"", pageName, "\" Stack depth was: ", stackView.depth)
			stackView.clear()
		}
		if (page === null) {
			stackView.push("pages/home.qml")
		} else {
			stackView.push(page)
		}
  }

  //----------------------
  // hiding the navebar?
  function hidingNavbar() {
		if (stackView.currentItem != null) {
			// header bar
			if (stackView.currentItem.hideNavBar) {
				rootAppPage.hideNavBar = true
			} else {
				rootAppPage.hideNavBar = false
			}
		} else {
			rootAppPage.hideNavBar = true
		}
		//console.log("NavBar hide: ", rootAppPage.hideNavBar)
  }

  //-----------------------------------------------------------------------------
  // Context Drawer:  '3-dot icon'
  //-----------------------------------------------------------------------------
  function openContextDrawer() {
		//contextDrawer.open()
		console.log("ContextDrawer not in use.")
  }
  // Deligate the context drawer menu object:
  //MatterFi_ContextDrawer {
	//	id: contextDrawer
  //}

  //-----------------------------------------------------------------------------
  // Material Style Theme:
  property int currentStyle: 1 // 0 = light mode, 1 = dark mode
  Universal.theme: currentStyle == 0 ? Universal.Light : Universal.Dark
  function changeStyle(index = 0) {
		currentStyle = index;
  }

  //----------------------
  // Main stack:
  StackView {
    id: stackView
    anchors.fill: parent

    // Signals from OT will set the first page displayed.
    //initialItem: "" // This property is what normally sets first page.
    Component.onCompleted: {
      rootAppPage.firstpage()
    }

    // Theme bg Color fill
    Universal.background: rootAppPage.currentStyle > 0 ? CustStyle.dm_pagebg : CustStyle.lm_pagebg
    background: Rectangle {
      id: page_bg_fill
      color: Universal.background
    }

    //-----------------------------------------------------------------------------
    // Tranisition Animations:
    pushEnter: Transition {
      id: pushEnter
			NumberAnimation {
				property: "opacity";
				from: 0;
				to: 1;
				duration: 600;
				easing.type: Easing.InCubic
			}
 		}

		popEnter: Transition {
			id: popEnter
			ParallelAnimation {
				PropertyAction { property: "x"; value: popEnter.ViewTransition.item.pos }
				NumberAnimation {
					property: "opacity";
					from: 0;
					to: 1;
					duration: 400;
					easing.type: Easing.InCubic
				}
			}
		}

		//----------------------
		popExit: Transition {
			id: popExit
			NumberAnimation {
				property: "opacity";
				from: 1;
				to: 0;
				duration: 400;
				easing.type: Easing.OutCubic
			}
		}

		pushExit: Transition {
			id: pushExit
			ParallelAnimation {
				PropertyAction { property: "x"; value: pushExit.ViewTransition.item.pos }
				NumberAnimation {
					property: "opacity";
					from: 1;
					to: 0;
					duration: 400;
					easing.type: Easing.OutCubic
				}
			}
		}

		//----------------------
		replaceEnter: Transition {
			id: replaceEnter
			ParallelAnimation {
				PropertyAction { property: "x"; value: popEnter.ViewTransition.item.pos }
				NumberAnimation {
					property: "opacity";
					from: 0;
					to: 1;
					duration: 200;
					easing.type: Easing.InCubic
				}
			}
		}

		replaceExit: Transition {
			id: replaceExit
			ParallelAnimation {
				PropertyAction { property: "x"; value: pushExit.ViewTransition.item.pos }
				NumberAnimation {
					property: "opacity";
					from: 1;
					to: 0;
					duration: 200;
					easing.type: Easing.OutCubic
				}
			}
		}

		//----------------------
		// back navigation device button
		// If stackView happens to lose focus, Main window/ active focus item
		// will take over device input actions. **Be aware of this issue.
		focus: true // focus needs to be on StackView for device gesters and key press event detection.

		// if Mac os, and esc key used, exit full screen if current window mode.
		Keys.onEscapePressed: {
			rootAppPage.keyEvent_normal_window_size(event)
		}
  }

  // if esc key used, exit full screen if current window mode.
  Keys.onEscapePressed: {
      rootAppPage.keyEvent_normal_window_size(event)
  }

  // General appication wide key events. Mostly was used for Mobile device back navigation.
	/*
  Keys.onPressed: {
		// if Desktop, navigate back in the current Stack:
		if (Qt.platform.os === "linux" || Qt.platform.os === "windows" || Qt.platform.os === "osx" || Qt.platform.os === "unix") {
			if (event.key !== Qt.Key_Escape) {
				event.accepted = true
				return
			}
		}
		if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
			console.log("Back button Nav captured.")
			if (stackView.depth > 1) {
				event.accepted = true
				if (!rootAppPage.displaying_popup) {
					rootAppPage.popPage()
				} else {
					rootAppPage.displaying_popup = false
				}
			} else {// close app
				event.accepted = false
			}
		}
  }
	*/

	//-----------------------------------------------------------------------------
	// Additional popup window:
	NewWindow {
		id: popupWindow
		visible: false
		onClosing: {
			//debugger:
			console.log("Additional popup window was closed.", popupWindow.loaded_qml_file)
		}
	}

  function popupShowWindow(qml_file) {
		//debugger:
		console.log("Popup Window was opened. file:", qml_file)
		// check if window already has qml file loaded, if so only raise the window.
		if (popupWindow.loaded_qml_file === qml_file) {
			popupWindow.show()
			popupWindow.raise()
			popupWindow.requestActivate()
		} else {
			popupWindow.load_qml_file(qml_file)
			popupWindow.show()
			popupWindow.requestActivate()
		}
  }

  //-----------------------------------------------------------------------------
  // Show the water mark on every page
  WaterMarker {
      visible: true
      // setting the watter mark to be shown on the bottom right corner
      x: rootAppPage.width - 72
      y: rootAppPage.height - 52
  }

  //-----------------------------------------------------------------------------
  // On IOS you get gestures for navigation controls:
  /*
  function onIOSbackGesture() {
		console.log("IOS Gesture Nav 'back' captured.")
		if (stackView.depth > 1) {
			if (!rootAppPage.displaying_popup) {
				rootAppPage.popPage()
			} else {
				rootAppPage.displaying_popup = false
			}
		}
  }
  IOSbackGesture {
		allowedToWork: ( stackView.depth > 1 )
		onBackGestureDetected: rootAppPage.onIOSbackGesture()
		anchors.fill: parent
  }
  */

  //-----------------------------------------------------------------------------
  // Could be a loading indicator:
  /*
  background: Rectangle {
      width: parent.width
      height: parent.height
      color: "yellow"
  }
  */

  //-----------------------------------------------------------------------------
  // Handle when Virtual keyboard is shown:
  // https://doc.qt.io/archives/qt-5.9/qinputmethod.html
  // https://doc.qt.io/archives/qt-5.9/qinputmethod.html#visible-prop
  /*
  Connections {
		target: Qt.inputMethod
		onVisibleChanged: {
			if (!Qt.inputMethod.visible) rootAppPage.y = 0
		}
		onKeyboardRectangleChanged: {
			if (Qt.inputMethod.visible) {
				rootAppPage.y = rootAppPage.y - (Qt.inputMethod.keyboardRectangle.height / 2)
			}
		}
  }
  */

  //-----------------------------------------------------------------------------
  // if focus input on StackView is lost, try to return it before exiting.
  // on desktops this is the 'X' button on the window manager.
  /*
  onClosing: {
		// if Desktop
		if (Qt.platform.os === "linux" || Qt.platform.os === "windows" || Qt.platform.os === "osx" || Qt.platform.os === "unix") {
			console.log("User closed the App.")
			if (close.key !== Qt.Key_Escape) {
				close.accepted = true
				return
			}
		}
		// else, probably on Mobile/Tablet
		if (stackView.depth > 1) {
			console.log("Stack View was returned input focus.")
			close.accepted = false
			if (!rootAppPage.displaying_popup) {
				rootAppPage.popPage()
				stackView.focus = true
			}
		} else {// close app
			console.log("User closed the App.")
			close.accepted = true
		}
  }
  */

//-----------------------------------------------------------------------------
}//end 'rootAppPage'