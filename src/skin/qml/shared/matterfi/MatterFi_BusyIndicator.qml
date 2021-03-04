// MatterFi_BusyIndicator.qml
import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12

import "qrc:/styling"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// 'MatterFi_BusyIndicator.qml'
// Shows activity to let the user things are working in the background.
BusyIndicator {
  id: contextRoot
  scale: 1.0
  smooth: true
  visible: false

  Universal.accent: CustStyle.pkt_logo
}//end 'contextRoot'