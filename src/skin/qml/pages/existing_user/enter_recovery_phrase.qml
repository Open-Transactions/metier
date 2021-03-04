import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12


import "qrc:/styling"
import "qrc:/qml_shared"
import "qrc:/matterfi"

//-----------------------------------------------------------------------------
// Ask the user to enter their RecoveryPhrase if they have one to Import
// their accounts for local Wallet useage in the App.
Page {
    id: pageRoot
    title: qsTr("Recover Phrase Entry")
    width: rootAppPage.width
    height: rootAppPage.height
    objectName: "enter_recovery_phrase"
    background: null //Qt.transparent

    property bool hideNavBar: true // hide navigation bar
    // api.wordCount(const int type, const int strength)
    property var requiredWordCount: 12 //this is temp, it is set via combo_box
    property var longestWord: 8 //api.longestSeedWord <- can not be notifi-able, is updated at later time
    // on checking word entry, are all the fields proper?
    property var wordsAreValid: true
    property var words: [] // contianer for the on screen words used.

    //-----------------------------------------------------------------------------
    property var passingWords: ""
    property var wordErrorHighlight: undefined
    function onFinishButton() {
        // recovery words are passed as single string 
        // with each word spaced apart
        pageRoot.passingWords = ""
        var index = 0
        var still_need_words = false
        //QML_Debugger.listFunctions(passingWords)
        while (index < pageRoot.requiredWordCount) {
            // make sure all indexes have a value
            if (pageRoot.words[index] === undefined || pageRoot.words[index] === "") {
                still_need_words = true
                pageRoot.wordErrorHighlight = index
                break
            }
            // bring the Phrase together for import usage
            if (index === 0) {
                passingWords += pageRoot.words[index]
            } else {
                passingWords += " " + pageRoot.words[index]
            }
            index += 1
        }
        // if the user still needs to enter more words:
        if (still_need_words) {
            console.log("Still need more recovery words: ", pageRoot.wordErrorHighlight, pageRoot.requiredWordCount, "\n")
            // pageRoot.words,
            moreWordsToolTip.visible = true
            stillNeedWordsToolTipTimer.start()
            return
        }
        // if the user entered all the vector words:
        //console.log("\n\n\n User RecoveryPhrase checking words: '", passingWords, "' \n",
        //    language_CB.currentIndex, encryption_CB.currentIndex, " array:", pageRoot.words, "\n\n\n")
        // pass Recover data collection to OT 'api' to see if 
        // information is correct collect user input for wallet recovery
        //      style [BIP32, BIP39]
        //      lang [none, eng]
        // language_CB.currentIndex, encryption_CB.currentIndex
        // importSeed(int type, int lang, QString input, QString passphrase)
        // api.importSeed(1, 1, passingWords, ""); // BIP39, English <- basic importing concept
        console.log("Import starting.")
        importingBusyIndicator.running = true
        delayWorkStartTimer.restart()
        // signal attached on main.qml will navigate to next page when ready
        //api.importSeed(seedtype_int, seedlang_int, passingWords, passPhraseTextInput.text)
        //api.checkStartupConditions() // importSeed should be calling this when finished
    }

    // move cursor to the next entry location for text
    function trynextEntryField(index, fieldReady) {
        //console.log("User RecoveryPhrase NextBox:", index, pageRoot.words)
        wordEntryBoxes.itemAt(index).active_focus = false
        var nextindex = (index + 1 >= wordEntryBoxes.count) ? 0 : index + 1
        //if (nextindex === 0) {
            // Check is made on finish click to check words.
        //    pageRoot.wordsAreValid = true
        //}
        wordEntryBoxes.itemAt(nextindex).active_focus = true
    }

    // move words entered into a more managable array location
    function concatEntryFields(index, word, fieldReady) {
        // each wordEntryBoxes.model text value, validate
        // the fields have fit their validatiors
        if (fieldReady === true) {
            pageRoot.words[index] = word
        } else {
            // input was not validated
            pageRoot.words[index] = undefined
        }
        //console.log("RecoveryPhrase Concact:", fieldReady, word, pageRoot.words)
        pageRoot.wordErrorHighlight = undefined
    }

    function onCancelButton() {
        rootAppPage.popPage()
    }

    // clear all the user defiend settings and reset the word entry fields
    // this is where the CheckBox settings get applied.
    function recalibrateVectors() {
        pageRoot.wordErrorHighlight = undefined
        encryption_CB.refresh()
        language_CB.refresh()
        seedsize_CB.refresh()
        pageRoot.words = []
        pageRoot.requiredWordCount = seedsize_CB.getWordCount()
        pageRoot.longestWord = api.longestSeedWord
        console.log("SeedSize box changed.", pageRoot.requiredWordCount, pageRoot.longestWord)
    }

    //-----------------------------------------------------------------------------
    Component.onCompleted: {
        console.log("Prompted User to enter Recovery Phrase: ")
        pageRoot.recalibrateVectors()
    }

    //-----------------------------------------------------------------------------
    // Is busy importing seed dispaly:
    Rectangle {
        id: displayIsBusyRect
        visible: (importingBusyIndicator.running == true)
        width: body.width
        height: 256
        color: "transparent"
        anchors.centerIn: parent
        // Display notification of expected pause while importing the seed
        Column {
            width: parent.width
            height: parent.height
            spacing: 16
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                text: qsTr("Importing...")
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
        // delay the start call to give the UI time to notify the user of the work pause
        Timer {
            id: delayWorkStartTimer
            interval: 200
            running: false
            onTriggered: {
                var seedtype_int = encryption_CB.currentIndex + 1
                var seedlang_int = language_CB.currentIndex + 1
                api.importSeed(seedtype_int, seedlang_int, 
                    pageRoot.passingWords, passPhraseTextInput.text
                );
                displayIsBusyRect.opacity = 0.0
            }
        }
    }

    //-----------------------------------------------------------------------------
    // entry box display
    Column {
        id: body
        width: 460
        spacing: 18
        visible: (importingBusyIndicator.running === false)

        anchors.horizontalCenter: parent.horizontalCenter
        
        //----------------------
        Label {
            id: pageTitle
            topPadding: 24
            text: qsTr("Enter Recovery Phrase")
            color: rootAppPage.currentStyle > 0 ? CustStyle.darkmode_text : CustStyle.lightmode_text
            font.weight: Font.DemiBold
            font.pixelSize: CustStyle.fsize_title
            smooth: true
            anchors.horizontalCenter: parent.horizontalCenter
        }

        //----------------------
        // recovery options
        Rectangle {
            id: optionboxes
            height: 32
            width: body.width
            color: "transparent"
            anchors.horizontalCenter: parent.horizontalCenter

            Row {
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                // Show Encryption type selection:
                MatterFi_SeedTypeComboBox {
                    id: encryption_CB
                    currentIndex: 0
                    width: optionboxes.width / 3
                    model: api.seedTypeModelQML()

                    onInteraction: {
                        pageRoot.recalibrateVectors()
                    }
                }
                // Show SeedLang selection:
                MatterFi_SeedLangComboBox {
                    id: language_CB
                    currentIndex: 0
                    //visible: (encryption_CB.currentIndex !== 1)
                    width: optionboxes.width / 4
                    // set lang model selection based from Encryption type index
                    model: api.seedLanguageModelQML(encryption_CB.currentIndex + 1)

                    onInteraction: {
                        pageRoot.recalibrateVectors()
                    }
                }
                // Show SeedSize selection:
                MatterFi_SeedSizeComboBox {
                    id: seedsize_CB
                    //visible: (encryption_CB.currentIndex !== 1)
                    width: optionboxes.width / 4
                    // show seed size settings based on Encryption index and it's avaliable sizes
                    model: api.seedSizeModelQML(encryption_CB.currentIndex + 1)

                    onInteraction: {
                        pageRoot.recalibrateVectors()
                    }
                }
                
            }
        }

        //----------------------
        // Optional password phrase usage for seed importing.
        Row {
            id: enterRecepientAddressRow
            width: body.width - 80
            spacing: 8
            topPadding: 8
            anchors.horizontalCenter: parent.horizontalCenter

            //----------------------
            MatterFi_CheckBox {
                id: enablePassPhrase
                checked: false
                width: 32
                text: ""
                anchors.verticalCenter: parent.verticalCenter
                onToggled: { 
                    passPhraseTextInput.clear()
                }
            }
            //----------------------
            Text {
                id: enterAddressText
                text: qsTr("Uses Password:")
                width: 124
                color: CustStyle.theme_fontColor
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: CustStyle.fsize_normal
                anchors.verticalCenter: parent.verticalCenter
            }
            //----------------------
            TextInput {
                id: passPhraseTextInput
                width: 196
                height: 38
                maximumLength: 32
                color: CustStyle.theme_fontColor
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: CustStyle.fsize_normal
                clip: true
                padding: 8
                enabled: (enablePassPhrase.checked === true)
                visible: (enabled)
                echoMode: TextInput.Password
                // outline the input box
                OutlineSimple {
                    outline_color: CustStyle.accent_outline
                }
            }
        }

        //----------------------
        // recovery words entry spaces
        Rectangle {
            id: enterBackupWords
            width: body.width
            height: wordGridModelView.height
            color: "transparent" //rootAppPage.currentStyle > 0 ? "black" : "white"
            radius: 4.0
            anchors.horizontalCenter: parent.horizontalCenter

            Grid {
                id: wordGridModelView
                rightPadding: 20
                columns: 3
                columnSpacing: 10
                rowSpacing: 12
                anchors.horizontalCenter: parent.horizontalCenter
                property int square_size: parent.width / 4

                Repeater {
                    id: wordEntryBoxes
                    model: (pageRoot.requiredWordCount)

                    MatterFi_RecoveryWordInput {
                        width: wordGridModelView.square_size
                        language:   (language_CB.currentIndex + 1)
                        encryption: (encryption_CB.currentIndex + 1)
                        longestWord: pageRoot.longestWord
                        display_index: index
                        issue_at_index: (pageRoot.wordErrorHighlight)
                        height: 32
                        onChanged: {
                            pageRoot.concatEntryFields(index, text, fieldReady)
                        }
                        onNextBox: {
                            pageRoot.trynextEntryField(index, fieldReady)
                        }
                    }
                }
            }
        }

        //----------------------
        Row {
            id: pageButtons
            spacing: 120
            anchors.horizontalCenter: parent.horizontalCenter

            MatterFi_Button_Standard {
                id: cancel_button
                displayText: qsTr("Cancel")
                onClicked: pageRoot.onCancelButton()
            }

            MatterFi_Button_Standard {
                id: finish_button
                enabled: pageRoot.wordsAreValid
                displayText: qsTr("Finish")
                onClicked: pageRoot.onFinishButton()
                // Display notification when still in need of more words
                MatterFi_ToolTip {
                    id: moreWordsToolTip
                    visible: false
                    text: (qsTr("Please enter all words. Check index ") + (pageRoot.wordErrorHighlight + 1) + ".")
                    // time that the ToolTip is displayed for
                    Timer {
                        id: stillNeedWordsToolTipTimer
                        interval: 2000
                        running: false
                        onTriggered: {
                            moreWordsToolTip.visible = false
                        }
                    }
                }
            }
        }

        //-----------------------------------------------------------------------------
    }
}
