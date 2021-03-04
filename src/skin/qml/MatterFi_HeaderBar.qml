import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/styling"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// Used to draw the top interactive menu element displayed on most Pages.
// This is used primarily as a navigation bar for the main stack view.
//   https://doc.qt.io/qt-5.14/qml-qtquick-controls-menu.html
//-----------------------------------------------------------------------------
ToolBar {
    id: rootBar

    contentHeight: 42
    height: contentHeight
    property var stackview: ({}) // used to provide reach into current Page.

    // The 'rootAppPage' takes care of when the toolbar is drawn or not.
    visible: !rootAppPage.hideNavBar || (stackview.depth > 1 && !rootAppPage.hideNavBar)
    
    // bg filler
    background: Rectangle {
        color: rootAppPage.currentStyle > 0 ? CustStyle.dm_navbar_bg : CustStyle.lm_navbar_bg
    }
    //----------------------
    // Menu items to select from:
    Menu {
        title: qsTr("&File")
        //----------------------
        Action { 
            text: qsTr("&Options...")
            onTriggered: {}
        }
        //----------------------
        MenuSeparator {}
        //----------------------
        Action { 
            text: qsTr("&Quit")
            onTriggered: {}
        }
    }

//-----------------------------------------------------------------------------
}