import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12

import "qrc:/"
import "qrc:/styling"
import "qrc:/qml_shared"
import "qrc:/matterfi"

//-----------------------------------------------------------------------------
Page {
    id: pageRoot
    title: qsTr("Edit Profile")
    width: rootAppPage.width
    height: rootAppPage.height
    objectName: "edit_profile"
    background: null //Qt.transparent

    property bool showSaveButton: true
    //-----------------------------------------------------------------------------
    // container for screen display before saving entry to data container
    property var profileDummy: {
        "emails": [["", '']],
        "social_medias": [[0, '']],
        "website": "",
        "address": ""
    }
    // Populate the profileDummy with existing data if any held onto
    function populateDummy() {
        var activeProfile = OTidentity.profile_OTModel
        console.log("Profile populated: ", JSON.stringify(activeProfile) )
        QML_Debugger.listEverything(activeProfile)
    }
    // run soon as page is ready
    Component.onCompleted: {
        pageRoot.populateDummy()
    }

    //-----------------------------------------------------------------------------
    // called back from navBar save button:
    function onSaveButtonAction() {
        profileDummy.website = websiteInput.text
        profileDummy.address = addressInput.text
        console.log("Profile changes saved.", JSON.stringify(profileDummy))
        //matterfi.update_active_profile(profileDummy)
    }

    //-----------------------------------------------------------------------------
    // called on editing social selections:
    function editSocialMediaList(index, selection = 0, account = "") {
        profileDummy.social_medias[index] = { selection, account }
        console.log("Populating dummy with changes: ", profileDummy.social_medias[index])
    }
    function addtoSocialList() {
        profileDummy.social_medias.push([0, ""])
        console.log("added new social slot")
    }
    function addtoEmailList() {
        profileDummy.emails.push(["", ""])
        console.log("added new email slot")
    }

    //-----------------------------------------------------------------------------
    Column {
        id: accountProfileInformation
        y: 40
        width: 280
        spacing: 2
        anchors.horizontalCenter: parent.horizontalCenter

        //----------------------
        Label {
            text: qsTr("Email")
            color: CustStyle.neutral_text
            smooth: true
            font.pixelSize: CustStyle.fsize_lable
        }

        Repeater {
            id: emailsView
            width: 270
            model: profileDummy.emails // ["label", "address"]

            TextField {
                id: emailInput
                width: parent.width
                leftPadding: 0
                text: model.modelData[1] || ""
                placeholderText: qsTr("Enter email")
                color: rootAppPage.currentStyle > 0 ? CustStyle.darkmode_text : CustStyle.lightmode_text
                background: Item {
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                    Rectangle {
                        color: CustStyle.neutral_fill
                        height: 1
                        width: parent.width
                        y: parent.y + parent.height - 8
                    }
                }
                onAccepted: {
                    profileDummy.emails[index] = ["home", emailInput.text]
                }
            }

        }// user emails

        //----------------------
        Label {
            text: qsTr("Social Media")
            topPadding: 16
            color: CustStyle.neutral_text
            smooth: true
            font.pixelSize: CustStyle.fsize_lable
        }

        Repeater {
            id: socialmediaView
            model: profileDummy.social_medias // [type_int, "account_name"]
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter

            Row {
                id: socialInput
                rightPadding: 8
                height: 32
                width: parent.width

                // https://doc.qt.io/qt-5/qtquickcontrols2-customize.html#customizing-combobox
                ComboBox {
                    id: socialList
                    currentIndex: 0
                    property var allowedmedias: []
                    property color displaycolor: "gray"
                    width: parent.width / 3
                    Component.onCompleted: {
                        /*
                        allowedmedias = matterfi.get_allowed_socialm()
                        if (profileDummy.social_medias) {
                            socialList.currentIndex = allowedmedias.indexOf(
                                        profileDummy.social_medias[socialmediaView.index])
                        }
                        */
                    }
                    model: allowedmedias
                    onAccepted: {
                        pageRoot.editSocialMediaList(socialmediaView.index, currentIndex)
                    }
                    contentItem: Text {
                        anchors.verticalCenter: socialList.verticalCenter
                        leftPadding: 0
                        topPadding: 8
                        color: socialList.displaycolor
                        text: socialList.displayText
                        font.pixelSize: CustStyle.fsize_normal
                    }
                    indicator: Text {
                        anchors.verticalCenter: socialList.verticalCenter
                        color: socialList.displaycolor
                        x: socialList.x + socialList.width - 18
                        topPadding: 10
                        font.pixelSize: CustStyle.fsize_contex
                        text: "\u25BC"   // solid down arrow
                    }
                    background: Rectangle {
                        color: "transparent"
                    }
                }

                TextField {
                    id: socialAccount
                    width: parent.width / 3 * 2
                    leftPadding: 0
                    topPadding: 8
                    text: profileDummy.social_medias[socialmediaView.index] ?
                            profileDummy.social_medias[socialmediaView.index]
                            : ""
                    placeholderText: qsTr("account name")
                    color: CustStyle.theme_fontColor
                    background: Item {
                        implicitWidth: parent.width
                        implicitHeight: parent.height
                        Rectangle {
                            color: CustStyle.neutral_fill
                            height: 1
                            width: parent.width
                            y: parent.y + parent.height - 8
                        }
                    }
                    onAccepted: {
                        pageRoot.editSocialMediaList(
                            socialmediaView.index, socialList.currentIndex, socialAccount.text
                        );
                    }
                }

            }// social row group
        }// user social list

        //----------------------
        Label {
            text: qsTr("Address")
            topPadding: 16
            color: CustStyle.neutral_text
            smooth: true
            font.pixelSize: CustStyle.fsize_lable
        }

        TextField {
            id: addressInput
            placeholderText: qsTr("street, city, state, etc.")
            width: parent.width
            maximumLength: 128
            leftPadding: 0
            color: CustStyle.theme_fontColor
            echoMode: TextInput.Normal
            renderType: Text.QtRendering
            background: Item {
                implicitWidth: addressInput.width
                implicitHeight: addressInput.height
                Rectangle {
                    color: CustStyle.neutral_fill
                    height: 1
                    width: parent.width
                    y: parent.y + parent.height - 8
                }
            }
        }

        //----------------------
        Label {
            text: qsTr("Website")
            topPadding: 16
            color: CustStyle.neutral_text
            smooth: true
            font.pixelSize: CustStyle.fsize_lable
        }

        TextField {
            id: websiteInput
            placeholderText: qsTr("example.com")
            width: parent.width
            maximumLength: 64
            leftPadding: 0
            color: CustStyle.theme_fontColor
            echoMode: TextInput.Normal
            renderType: Text.QtRendering
            background: Item {
                implicitWidth: websiteInput.width
                implicitHeight: websiteInput.height
                Rectangle {
                    color: CustStyle.neutral_fill
                    height: 1
                    width: parent.width
                    y: parent.y + parent.height - 8
                }
            }
        }

        //----------------------
    }// column profile information group
    //-----------------------------------------------------------------------------
}

