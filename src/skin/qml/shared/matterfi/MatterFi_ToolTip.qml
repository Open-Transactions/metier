import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12


import "qrc:/styling"
//-----------------------------------------------------------------------------
// 'MatterFi_ToolTip.qml'
// Styles the tool tip pop ups where context is provided.
//-----------------------------------------------------------------------------
ToolTip {
  id: contextRoot
  text: ""

  background: Rectangle {
    color: CustStyle.accent_fill
    opacity: 1.00
    radius: 4
    OutlineSimple {
      outline_color: CustStyle.accent_outline
      radius: parent.radius
    }
  }
}
