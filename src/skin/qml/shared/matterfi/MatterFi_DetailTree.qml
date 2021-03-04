import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQml.Models 2.15

import com.TreeModelAdaptor 1.0
import "qrc:/treeview"

import "qrc:/styling"
import "qrc:/qml_shared"
//-----------------------------------------------------------------------------
// 'MatterFi_DetailTree.qml'
// Provides an interactive list for TreModels.
//
/* OT BlockchainAccountStatusQt
{BlockchainAccountStatusQt::NameRole, "name"},
	{BlockchainAccountStatusQt::SourceIDRole, "sourceid"},
	{BlockchainAccountStatusQt::SubaccountIDRole, "subaccountid"},
	{BlockchainAccountStatusQt::SubaccountTypeRole, "subaccounttype"},
	{BlockchainAccountStatusQt::SubchainTypeRole, "subchaintype"},
	{BlockchainAccountStatusQt::ProgressRole, "progress"},
*/
//-----------------------------------------------------------------------------
Item {
  id: contextRoot
	width: parent.width
	height: parent.height

	property var model: undefined // BlockchainAccountStatusQt

  signal action() // Emited when clicked on an index with in the detail structure.

  //-----------------------------------------------------------------------------
	/*
  Component.onCompleted: {
    // debugger:
    console.log("MatterFi_DetailTree model:", contextRoot.model)
    QML_Debugger.listEverything(contextRoot.model)
		//QML_Debugger.listModelChildren(contextRoot.model)
		console.log("RowCount: " + contextRoot.model.rowCount() + ", ColumnCount: " + contextRoot.model.columnCount())
		contextRoot.getIndexData(0, 0, 0)
		contextRoot.getSibling(0, 0, 0)
  }

 	//----------------------
	// Search the detail tree model for data at a row and column.
	function getIndexData(row = 0, column = 0, role = 0) {
		var entrydata = contextRoot.model.data( contextRoot.model.index(row, column), role + 256)
		console.log("Looked up DetailTree model data for:", row, column, role + 256, entrydata)
		QML_Debugger.listEverything(entrydata)
		return entrydata
	}

	function getSibling(index, row = 0, column = 0) {
		var modelIndex = contextRoot.model.index(row, column)
		var sibling = contextRoot.model.sibling(row, column, modelIndex)
		console.log("Looked up DetailTree sibling for:", row, column, modelIndex, sibling)
		QML_Debugger.listEverything(sibling)
		return sibling
	}
	*/

	//-----------------------------------------------------------------------------
	// Default Outdated QT TreeView:  **Qt5 DEPRECIATED**  //** Replaced  **//
	// https://github.com/hvoigt/qt-qml-treeview/blob/master/treemodel.cpp
	TreeView {
		id: advancedDetailTreeView
		width: parent.width
		height: parent.height
		model: contextRoot.model
		padding: 4
		clip: true
		focus: true
		// styling
		textColor: CustStyle.theme_fontColor
		//frameVisible: true
	  headerVisible: false
		alternatingRowColors: false
		rowFontSize: CustStyle.fsize_normal
		rowHeight: rowFontSize + 4
		//backgroundVisible: false
		//horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
		//verticalScrollBarPolicy: Qt.ScrollBarAsNeeded
		// used to highlight top row depth
		property var is_top_depth_row: true
		// how the rows look
		rowDelegate: Rectangle {
			color: (styleData.selected ? CustStyle.pkt_logo : "transparent")
		}
		// setup view columns
		TableViewColumn {
			role: "name"
			title: "Name"
			width: advancedDetailTreeView.width / 7 * 2
		}
		TableViewColumn {
			role: "sourceID"
			title: "SourceID"
			width: advancedDetailTreeView.width / 7 * 2
		}
		TableViewColumn {
			role: "progress"
			title: "Progress"
			width: advancedDetailTreeView.width / 7 * 3
			horizontalAlignment: Text.AlignRight
		}
		// how the display items look.
		itemDelegate: Item {
			Text {
				height: 32
				color: CustStyle.theme_fontColor
				text: (styleData.value !== undefined ? styleData.value : "")
				font.pixelSize: CustStyle.fsize_normal
			}
		}
	}//end 'advancedDetailTreeView'

	//-----------------------------------------------------------------------------
	// Create a simple border outline:
	OutlineSimple {
		//outline_color: "red"
		radius: 4
	}

  //-----------------------------------------------------------------------------
}//end 'contextRoot'