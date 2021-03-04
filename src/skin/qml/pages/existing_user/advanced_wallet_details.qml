import QtQuick 2.15
import QtQml.Models 2.1
import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12

import "qrc:/"
import "qrc:/styling"
import "qrc:/matterfi"
import "qrc:/qml_shared"
import "qrc:/matterfi"

//-----------------------------------------------------------------------------
/* AccountStatus OT model:
*
* qml: nym:
* qml: chain:
* qml: objectNameChanged:function() { [native code] }
* qml: dataChanged:function() { [native code] }
* qml: headerDataChanged:function() { [native code] }
* qml: layoutChanged:function() { [native code] }
* qml: resetInternalData:function() { [native code] }
* qml: hasIndex:function() { [native code] }
* qml: index:function() { [native code] }
* qml: rowCount:function() { [native code] }
* qml: columnCount:function() { [native code] }
* qml: data:function() { [native code] }
* 
* Roles:
*   NameRole = Qt::UserRole + 0,            // QString
*   SourceIDRole = Qt::UserRole + 1,        // QString
*   SubaccountIDRole = Qt::UserRole + 2,    // QString
*   SubaccountTypeRole = Qt::UserRole + 3,  // int
*   SubchainTypeRole = Qt::UserRole + 4,    // int
*   ProgressRole = Qt::UserRole + 5,        // QString
* 
*/
//-----------------------------------------------------------------------------
// Display an advanced detailed tree of connected peers and sync details for the blockchains.
Page {
  id: pageRoot
  title: qsTr("Advanced Wallet Details")
  objectName: "advanced_details"
  width: rootAppPage.width
  height: rootAppPage.height

  background: Rectangle {
    id: page_bg_fill
    color: rootAppPage.currentStyle > 0 ? CustStyle.dm_pagebg : CustStyle.lm_pagebg
  }

  property bool hideNavBar: true // hide navigation bar

  //-----------------------------------------------------------------------------
  // main display 'body' layout
  Column {
    id: body
    width: parent.width
    height: parent.height
    spacing: 12
    padding: 16
    anchors.centerIn: parent

    //----------------------
    // BlockChain Logo:
    Image {
      id: walletBrandingImage
      source: "qrc:/assets/svgs/pkt-icon.svg"
      width: 180
      height: 120
      smooth: true
      fillMode: Image.PreserveAspectFit
      anchors.topMargin: 0
      anchors.bottomMargin: 0
      sourceSize.width: walletBrandingImage.width
      sourceSize.height: walletBrandingImage.height
      anchors.horizontalCenter: parent.horizontalCenter
    }

    //----------------------
    // Clear Page Title:
    Text {
      id: contextDrawerFunctionTextDescription
      text: qsTr("Advanced Wallet Details:")
      font.weight: Font.DemiBold
      font.pixelSize: CustStyle.fsize_xlarge
      color: CustStyle.accent_text
      verticalAlignment: Text.AlignVCenter
      anchors.horizontalCenter: parent.horizontalCenter
    }

    //----------------------
    // Details TreeView display rect:
    Rectangle {
      id: displayTreeRect
      color: "transparent"
      width: parent.width / 16 * 15
      height: parent.height - (walletBrandingImage.height * 2)
      anchors.horizontalCenter: parent.horizontalCenter

      MatterFi_DetailTree {
        id: accountDetailDelegate
        model: (OTidentity.focusedAccountStatus_OTModel)
      }
    }//end 'displayTreeRect'

  //-----------------------------------------------------------------------------
  }//end 'body'

//-----------------------------------------------------------------------------
}//end 'pageRoot'
