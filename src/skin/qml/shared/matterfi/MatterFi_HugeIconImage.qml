import QtQuick 2.15
import QtQuick.Controls.Universal 2.12

import "qrc:/styling"
//-----------------------------------------------------------------------------
// 'MatterFi_HugeIconImage.qml'
// Used to display icons as large images.
//-----------------------------------------------------------------------------
Item {
  id: contextRoot

  property string iconChar: "" // what icon char to display
  // icons are part Font Awesome (FA) ?
  property bool isFAicon_solid: false
  property bool isFAicon_reg:   false

  property string fontFamilyName: Fonts.icons_solid

  property int imgPadding: 0 // space around the image
  
  property color bgcolor: "transparent"
  property color color: CustStyle.theme_fontColor

  //-----------------------------------------------------------------------------
  Rectangle {
    id: body
    width: contextRoot.width
    height: contextRoot.height
    color: contextRoot.bgcolor
    anchors.fill: parent

    // Draw the icon to fill the BG :
    Text {
      id: svgImage
      text: contextRoot.iconChar
      width: body.width - contextRoot.imgPadding
      height: body.height - contextRoot.imgPadding
      anchors.centerIn: parent
      font.pixelSize: height
      font.family: contextRoot.fontFamilyName
      font.styleName: (contextRoot.isFAicon_solid ? "Solid" : "Regualar")
      color: contextRoot.color
    }
  }
}
