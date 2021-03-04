import QtQuick.Window 2.13

import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12


import "qrc:/styling"
import "qrc:/matterfi"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// Handles laoding and calling a new Window to be shown in the OS environment.
Window {
  id: newWindowRoot
  // Width constraints
  minimumWidth:  600
  width:         800 //<- starting value
  maximumWidth: 1024
  // Height constraints
  minimumHeight: 400
  height:        600 //<- starting value
  maximumHeight: 768

  // Window visibility
  visible: true
  color: rootAppPage.currentStyle > 0 ? CustStyle.dm_pagebg : CustStyle.lm_pagebg
  //flags: Qt.FramelessWindowHint // draw window with no frame around it.

  property var loaded_qml_file: "qrc:/pages/home.qml" // What qml file to display in popup window.

  //----------------------
  // Called to start loading the qml file into Loader component
  function load_qml_file(qml_file) {
    //debugger:
    console.log("Loading NewWindow Source qml file.", qml_file)
    // load in new source qml file
    loader.source = qml_file
    newWindowRoot.loaded_qml_file = qml_file
  }

  //-----------------------------------------------------------------------------
  // Display 'body':
  Rectangle {
    id: body
    width: parent.width
    height: parent.height
    color: "transparent"
    //----------------------
    // What loads the QML file to display in the new window
    Loader {
      id: loader
      anchors.fill: parent
      source: ""

      onSourceChanged: {
        animation.running = true
      }

      NumberAnimation {
        id: animation
        target: loader.item
        duration: 500
        property: "opacity"
        from: 0
        to: 255
        easing.type: Easing.InExpo
      }
    }
    //----------------------
    // Busy loading qml file:
    MatterFi_BusyIndicator {
      id: pendingBusyIndicator
      scale: 1.0
      visible: (loader.status == Loader.Loading)
      running: (loader.status == Loader.Loading)
      anchors.centerIn: parent
    }

  }//end 'body'
//-----------------------------------------------------------------------------
}//end 'newWindowRoot'