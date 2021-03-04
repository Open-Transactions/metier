import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12


import "qrc:/styling"
import "qrc:/matterfi"
import "qrc:/qml_shared"
import "qrc:/matterfi"

//// import org.opentransactions.metier 1.0 as OTModels

//-----------------------------------------------------------------------------
// Display all the avaliable BlockChains to allow the user to enable/disable
// the one's they wish to use in their wallet.
Page {
  id: pageRoot
  title: qsTr("Select BlockChain")
  width: rootAppPage.width
  height: rootAppPage.height
  objectName: "select_blockchian"
  background: null //Qt.transparent

  property var testBlockChainList: []

  //----------------------
  // called on next button action to check if user can procceed foreword
  function onNextButton() {
    // can proceed with user block chains?
    api.checkStartupConditions()
    if (rootAppPage.mainIsReady) {
      console.log("Block Chain Chooser: user is ready for account activity.")
    } else {
      console.log("Block Chain Chooser: App is not ready yet.")
    }
    // Checking start up conditions should fire a signal that will tell
    // main.qml to navigate to the next step in the Account set up.
  }
  //----------------------
  // called when a block chian is selected from the list
  function onBlockChainSelection(block_index) {
      var blockChain = pageRoot.testBlockChainList[block_index]
      console.log("Block Chain Chooser: chain was clicked on.", JSON.stringify(blockChain) )
  }
  //----------------------
  // make the display list of available user contacts
  function populateBlockChains() {
    pageRoot.testBlockChainList = api.blockchainChooserModelQML(test) // test blockchains

    console.log("Block Chain Chooser:", Object.keys(pageRoot.testBlockChainList), "\n");

    // iterait threw the model data to fetch attached data
    var modelData = []
    for (var row = 0; row < pageRoot.testBlockChainList.rowCount; row++) {
      for (var column = 0; column < pageRoot.testBlockChainList.columnCount; column++) {
        var index = pageRoot.testBlockChainList.index(row, column)
        if (index.isValid) {
          var indexData = pageRoot.testBlockChainList.data(index)
          console.log("Block Chain Chooser:", Object.keys( indexData ) );
          modelData.push(indexData)
        } else {
          console.log("Block Chain Chooser: index not valid")
          // means end of model list
          break;
        }
      }
    }
  }

  //----------------------
  // detected a user change to the block chain list
  function blockChainChanged() {
    console.log("Block Chain Chooser: block chain changed.")
  }
  //----------------------
  // run soon as page is ready
  Component.onCompleted: {
    // has the user made chain changes?
    api.chainsChanged.connect(pageRoot.blockChainChanged)
    // make the chain selection list
    pageRoot.populateBlockChains()
  }

  Component.onDestruction: {
    api.chainsChanged.disconnect(pageRoot.blockChainChanged)
  }

  //-----------------------------------------------------------------------------
  // page contents displayed:
  Column {
    id: body
    width: pageRoot.width
    property int displayWidth: 320
    anchors.horizontalCenter: parent.horizontalCenter

    //-----------------------------------------------------------------------------
    // Display list of available block chaings to choose from.
    Rectangle {
      id: blockListView
      width: pageRoot.width
      height: pageRoot.height - rootAppPage.toolbar_height() - 32
      color: "transparent"
      //----------------------
      // The display DelegateModel for a block chain in the list:
      DelegateModel {
        id: blockListModelDelegator
        model:  pageRoot.testBlockChainList

        delegate: MatterFi_BlockChainDelegate {
          width: listView.width
          height: 40
          blockchain_data: model
          onAction: pageRoot.onBlockChainSelection(index)
        }
      }

      //----------------------
      // show the activity list:
      ListView {
        id: listView
        model: blockListModelDelegator
        width: blockListView.width / 2
        height: blockListView.height
        clip: true
        // screen position anchors
        anchors {
          top: parent.anchors.TopAnchor;
          topMargin: 16;
          left: parent.anchors.LeftAnchor;
          bottom: parent.anchors.BottomAnchor;
          right: parent.anchors.RightAnchor;
          horizontalCenter: parent.horizontalCenter;
        }
      }
    }
  }//end of display 'body' Column

//-----------------------------------------------------------------------------
}