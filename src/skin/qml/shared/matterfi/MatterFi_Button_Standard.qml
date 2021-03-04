import QtQuick 2.15
import QtQuick.Controls 2.15

import "qrc:/styling"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// 'MatterFi_Button_Standard.qml'
// This is used to draw a standard looking Button for use accross the application.
// Allows the styling to be applied and changed in one place.
//-----------------------------------------------------------------------------
Button {
  id: contextRoot
  width: 100
  height: 48
  scale: contextRoot.down ? CustStyle.but_shrink : 1.0

  property string displayText: ""

  //-----------------------------------------------------------------------------
  contentItem: ButtonText {
    text: contextRoot.displayText
    //font.capitalization: Font.AllUppercase
  }

  //-----------------------------------------------------------------------------
  background:  Rectangle {
    width: parent.width
    height: parent.height
    color: (hovered && enabled ? CustStyle.active_highlight : "transparent")

    OutlineSimple {
      implicitWidth: parent.width
      implicitHeight: parent.height
      radius: 6
    }
  }

}