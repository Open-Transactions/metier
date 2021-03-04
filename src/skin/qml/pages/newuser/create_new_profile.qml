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
    title: qsTr("Create New Profile")
    width: rootAppPage.width
    height: rootAppPage.height
    objectName: "create_new_profile"

    background: null //Qt.transparent or "transparent"

    property bool hideNavBar: true // hide navigation bar

    // container for screen display before saving entry to data container
    /*
    property var profileDummy: {
        "name_first": '',
        "name_last": ''
    }
    // Populate the profileDummy with existing data if any held onto
    function populateDummy() {
        // we make a call for new profile data container here
        var newActiveProfile = {} //matterfi.get_active_profile(true)
        //console.log("Profile dummy created: ", JSON.stringify(newActiveProfile) )
        if ( Object.keys(newActiveProfile) < 1 ) {
            // is new, uses template data
            //pageRoot.profileDummy = newActiveProfile
        }
    }
    // run soon as page is ready
    Component.onCompleted: {
        pageRoot.populateDummy()
    }
    */

    //-----------------------------------------------------------------------------
    function onNextButton() {
        if (next_button.enabled) {
            pageRoot.submitName(nameInput.text)
        } else {
            console.log("Error: nameNym does not accept an empty string.", nameInput.text)
        }
    }
    //----------------------
    property string profileNym: ""
    function submitName(name) {
        pageRoot.profileNym = name
        //app.displayMainWindow.connect(pageRoot.display_mainWindow)
        // when the name is sent, a signal is called to navigate to the next step.
        // this will dispose access to rootAppPage.
        //console.log("Profile name submited.", profileDummy.name_full);
        importingBusyIndicator.running = true
        delayWorkStartTimer.restart()
    }
    //----------------------
    function onCancelButton() {
        // nav back to home screen
        rootAppPage.popPage()
    }

    // delay the start call to give the UI time to notify the user of the work pause
    Timer {
        id: delayWorkStartTimer
        interval: 200
        running: false
        onTriggered: {
            api.createNym(pageRoot.profileNym);
            api.checkStartupConditions();
            // anything after this point does not get called. Past here QML is busy
            // destroying this Component. 'rootAppPage' will be watching for the signal to 
            // fire for 'displayMainWindow' this is what the stackView navigator will use 
            // to display the wallet asset page. Here an OT call slot to trigger signal 
            // propagation threw the main.qml Nav stack. But main window, which is listening 
            // to OT 'api' signals will should do this.
        }
    }

    // OT signal is fired when its ready to show the account dashboard.
    //function display_mainWindow() {
    //    rootAppPage.clearStackHome("pages/existing_user/single_crypto_account_display.qml")
    //}

    //Component.onDestruction: {
    //    app.displayMainWindow.disconnect(pageRoot.display_mainWindow)
    //}


    //-----------------------------------------------------------------------------
    // Is busy importing seed dispaly:
    Rectangle {
        id: displayIsBusyRect
        visible: (importingBusyIndicator.running == true)
        width: body.width
        height: 256
        color: "transparent"
        anchors.centerIn: parent
        // Display notification of expected pause while creating the profile
        Column {
            width: parent.width
            height: parent.height
            spacing: 16
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: qsTr("Creating...")
                color: CustStyle.theme_fontColor
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: CustStyle.fsize_normal
            }
            
            MatterFi_BusyIndicator {
                id: importingBusyIndicator
                running: false
                visible: (parent.visible)
                scale: 1.0
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    //-----------------------------------------------------------------------------
    // Define the main display 'body' for layout alignents and positioning.
    Column {
        id: body
        spacing: 16
        visible: (importingBusyIndicator.running === false)
        anchors.horizontalCenter: parent.horizontalCenter

        //----------------------
        // Text Header
        Label {
            id: pageTitle
            topPadding: 20
            text: qsTr("Create New Profile")
            color: CustStyle.theme_fontColor
            font.weight: Font.DemiBold
            font.pixelSize: CustStyle.fsize_title
            smooth: true
            anchors.horizontalCenter: parent.horizontalCenter
        }

        //----------------------
        // Name input area
        Column {
            id: nameEntry
            y: pageTitle.y + pageTitle.height + 40
            topPadding: 20
            bottomPadding: 20
            spacing: 2
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                text: qsTr("Enter Public Wallet Name:")
                color: CustStyle.neutral_text
                smooth: true
                font.pixelSize: CustStyle.fsize_contex
            }

            TextField {
                id: nameInput
                placeholderText: qsTr("Default")
                width: 260
                maximumLength: 64
                leftPadding: 0
                color: CustStyle.theme_fontColor
                echoMode: TextInput.Normal
                renderType: Text.QtRendering
                background: Item {
                    implicitWidth: nameInput.width
                    implicitHeight: nameInput.height
                    Rectangle {
                        color: CustStyle.neutral_fill
                        height: 1
                        width: parent.width
                        anchors.bottom: parent.bottom
                    }
                }
            }
        }

        //----------------------
        // profile name usage details
        Text {
            id: disclaimerText
            width: (body.width / 3 * 2 < 520 ? 520 : body.width / 3 * 2)
            wrapMode: Text.Wrap
            elide: Text.ElideRight
            lineHeight: 1.1
            padding: 8
            color: CustStyle.theme_fontColor
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: CustStyle.fsize_lable
            anchors.horizontalCenter: parent.horizontalCenter

            text: qsTr("Your public wallet name is how other users of MatterCode™ enabled wallets can" +
                       " identity your payment code in plain English. If you wish to remain completely" +
                       " anonymous use a random name or non-descriptive name such as \"Default\"." +
                       " In future releases you will be able to maintain multiple wallets with corresponding" +
                       " names. Your name association to a partiuclar on-chain transaction is only cryptographically" +
                       " computatable in wallets you have transacted with. Only MatterCode™ wallets you have sent" +
                       " money to or received moeny from will see your wallet name in their transaction history.")

            // outline the input box
            OutlineSimple {
            outline_color: CustStyle.accent_outline
            }
        }

        //----------------------
        // Button Row
        Row {
            id: pageButtons
            topPadding: 40
            spacing: 120

            anchors {
                horizontalCenter: parent.horizontalCenter
            }

            /*
            MatterFi_Button_Standard {
                id: cancel_button
                displayText: qsTr("Cancel")
                onClicked: pageRoot.onCancelButton()
            }
            */

            // only allow passage to the Dashboard if user has
            // entered a name longer then 1 letter.
            MatterFi_Button_Standard {
                id: next_button
                enabled: (nameInput.displayText.length > 0)
                displayText: qsTr("Next")
                onClicked: pageRoot.onNextButton()
            }
        }

    }//end 'body'

    //-----------------------------------------------------------------------------
}
