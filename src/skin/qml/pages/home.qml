import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12


import "qrc:/styling"
import "qrc:/qml_shared"
import "qrc:/matterfi"

//-----------------------------------------------------------------------------
Page {
    id: pageRoot
    width: rootAppPage.width
    height: rootAppPage.height
    title: qsTr("Home")
    objectName: "home"
    
    background: null //Qt.transparent

    property bool hideNavBar: true // hide navigation bar

    //-----------------------------------------------------------------------------
    // Button Callbacks:
    function on_create_newButton() {
        //api.checkStartupConditions();
        // will check OT 'api' signals for proper account creation step required
        if (rootAppPage.isFirstRun) {
            console.log("Home: App state is first run.")
            rootAppPage.pushPage("pages/newuser/account_recovery_storing.qml")
        } else if (!rootAppPage.hasProfile) {
            console.log("Home: App state is in need of User Profile.")
            rootAppPage.pushPage("pages/newuser/create_new_profile.qml")
        } else if (rootAppPage.stillNeedBlock) {
            console.log("Home: App state is still in need of BlockChain.")
            rootAppPage.pushPage("pages/existing_user/block_chain_chooser.qml")
        } else if (rootAppPage.mainIsReady) {
            console.log("Home: App state is rootAppPage ready.")
            //rootAppPage.replacePage("pages/existing_user/user_accounts.qml") // << default
            rootAppPage.clearStackHome("pages/existing_user/single_crypto_account_display.qml")
        } else { // un-known
            console.log("Home: App state is un-known can not continue.")
        }
    }
    //----------------------
    function on_try_recoveryButton() {
        rootAppPage.pushPage("pages/existing_user/enter_recovery_phrase.qml")
    }

    //-----------------------------------------------------------------------------
    // Logo:
    Image {
        id: iconImage
        width: 250
        height: 250
        sourceSize.width: 250
        sourceSize.height: 250
        anchors.horizontalCenter: parent.horizontalCenter
        source: "qrc:/assets/svgs/pkt-logo.svg"
        fillMode: Image.PreserveAspectFit
        smooth: true
    }

    //-----------------------------------------------------------------------------
    MatterFi_Button_Standard {
        id: newWalletBut
        width: 230
        y: parent.height - 130
        anchors.horizontalCenter: parent.horizontalCenter
        displayText: qsTr("Create New Profile")
        onClicked: pageRoot.on_create_newButton()
    }

    MatterFi_Button_Standard {
        id: recoverWalletBut
        width: 230
        y: parent.height - 70
        anchors.horizontalCenter: parent.horizontalCenter
        displayText: qsTr("Restore From Backup")
        onClicked: pageRoot.on_try_recoveryButton()
    }

    //-----------------------------------------------------------------------------
}
