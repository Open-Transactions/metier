import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12

import "qrc:/styling"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// 'MatterFi_ProgressBar.qml'
// Used atm mostly for block sync status.
//-----------------------------------------------------------------------------
Item {
  id: contextRoot
  // set display boundries
  width: 250
  height: 23
  // working properties
  property int minimum: 0
  property int maximum: 100
  property double value: (minimum / maximum)
  property bool isdone: (value >= 1.0)
  //property bool animateStillWorking: true

  //-----------------------------------------------------------------------------
  // The background of the progress bar.
  Rectangle {
    id: bg_fill
    radius: 2.0
    color: 'gray'
    width: contextRoot.width
    height: 8
    anchors.verticalCenter: parent.verticalCenter
  }

  //-----------------------------------------------------------------------------
  // The progress shown majorily threw this Rectangle.
  Rectangle {
    id: highlight
    radius: bg_fill.radius
    // display the progress
    height: bg_fill.height
    width: (contextRoot.value > 1.0 ? bg_fill.width : bg_fill.width * contextRoot.value)
    anchors.verticalCenter: parent.verticalCenter
    // Animation to show while showing progress status
    Behavior on width {
      SmoothedAnimation { velocity: 1200 }
    }
    // Gradient fill used as the Rectangels display color property.
    gradient: Gradient {
      GradientStop {
        position: 0.0;
        color: CustStyle.accent_normal
        // fade color up and down
        SequentialAnimation on color {
          loops: Animation.Infinite
          ColorAnimation {
            from: CustStyle.accent_normal;
            to: CustStyle.accent_active;
            duration: 500
          }
          ColorAnimation {
            from: CustStyle.accent_active;
            to: CustStyle.accent_normal;
            duration: 500
          }
        }
      }
      GradientStop { position: 1.0; color: CustStyle.accent_normal }
    }
  }

//-----------------------------------------------------------------------------
}//end Item