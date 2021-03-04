import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12

import "qrc:/styling"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// 'MatterFi_SeedLangComboBox.qml'
// Used to style drop down selection Seed Language ComboBox.
//-----------------------------------------------------------------------------
ComboBox {
  id: contextRoot
  model: ["null"] // the options displayed for selection.

  signal interaction() // send signal that QObject has changed.

  //-----------------------------------------------------------------------------
  // Force refreshing:
  property var displayedText: ""
  function refresh() {
    contextRoot.displayedText = contextRoot.model.data( contextRoot.model.index(currentIndex, 0) )
    if (contextRoot.displayedText === undefined) {
      contextRoot.displayedText = "null"
    }
  }

  //-----------------------------------------------------------------------------
  // What the list looks like when selection is active.
  delegate: ItemDelegate {
    id: selectionDisplay
    width: contextRoot.width
    highlighted: contextRoot.highlightedIndex === index
    // Display Text deligator
    contentItem: Text {
      text: model.display
      color: CustStyle.accent_text
      font: contextRoot.font
      elide: Text.ElideRight
      //font.pixelSize: CustStyle.fsize_normal //already sat.
      verticalAlignment: Text.AlignVCenter
      
      /*
      Component.onCompleted: {
        console.log("SeedLang model ComboBox data:", index);
        QML_Debugger.listEverything(model)
        if (QML_Debugger.hasFunction(model, "index")) {
          if ( model.index(index, 0).valid ) {
            text = model.data( model.index(index, 0) );
          }
        } else {
          console.log("SeedLang model lacks an 'index' function.");
        }
      }
      */
    }
  }//end 'selectionDisplay'

  //-----------------------------------------------------------------------------
  // The indicator used when making a selection in the ComboBox.
  indicator: Canvas {
    id: canvas
    x: contextRoot.width - width - contextRoot.rightPadding
    y: contextRoot.topPadding + (contextRoot.availableHeight - height) / 2
    width: 12
    height: 8
    contextType: "2d"
    // Attach to redraw when ever the ComboBox is interacted with
    Connections {
      target: contextRoot
      function onPressedChanged() { 
        canvas.requestPaint()
      }
    }
    // Draw operation for drop indicator cavas
    onPaint: {
      var ctx = getContext("2d");
      ctx.reset();
      ctx.moveTo(0, 0);
      ctx.lineTo(width, 0);
      ctx.lineTo(width / 2, height);
      ctx.closePath();
      ctx.fillStyle = contextRoot.pressed ? CustStyle.accent_active : CustStyle.accent_fill
      ctx.fill();
    }
  }

  //-----------------------------------------------------------------------------
  // Text showing current selection.
  contentItem: Text {
    id: dispalyItemText
    leftPadding: 4
    rightPadding: contextRoot.indicator.width + contextRoot.spacing

    text: (contextRoot.displayedText === undefined ? "null" : contextRoot.displayedText)
    font: contextRoot.font
    //font.pixelSize: CustStyle.fsize_normal //already sat.
    color: contextRoot.pressed ? CustStyle.accent_active : CustStyle.accent_fill
    verticalAlignment: Text.AlignVCenter
    elide: Text.ElideRight
  }

  background: Rectangle {
    implicitWidth: 120
    implicitHeight: 40
    border.color: contextRoot.pressed ? CustStyle.accent_active : CustStyle.accent_fill
    border.width: contextRoot.visualFocus ? 2 : 1
    radius: 2
  }

  //-----------------------------------------------------------------------------
  // Menu that shows when clicked for selection.
  popup: Popup {
    y: contextRoot.height - 1
    width: contextRoot.width
    implicitHeight: contentItem.implicitHeight
    padding: 1
    // What items in the drop list look like
    contentItem: ListView {
      clip: true
      implicitHeight: contentHeight
      model: contextRoot.popup.visible ? contextRoot.delegateModel : null
      currentIndex: contextRoot.highlightedIndex
      // for long display lists provide a scrollbar
      ScrollIndicator.vertical: ScrollIndicator { }
    }
    // BG fill
    background: Rectangle {
      border.color: CustStyle.accent_fill
      radius: 2
    }
    // close
    onClosed: {
      contextRoot.interaction()
    }
  }
}