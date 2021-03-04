import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12

import "qrc:/"
import "qrc:/styling"
import "qrc:/matterfi"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
// Drawer displayed from NavBar:
Drawer {
    id: drawerRoot
    interactive: drawerRoot.visible // can not swipe open
    edge: Qt.LeftEdge
    width: rootAppPage.width * 0.85
    height: rootAppPage.height
    palette.base: rootAppPage.currentStyle > 0 ? "black" : "white"

    //-----------------------------------------------------------------------------
    // container for screen display before saving entry to data container
    property var profileDummy: {
        "name_first": 'Your-Not',
        "name_last": 'Logged-In'
    }
    property string fullusername: "null-user"
    // Populate the profileDummy with existing data if any held onto
    function populateDummy() {
        var profileModel = OTidentity.profile_OTModel

        console.log("Current Profile populated: ", JSON.stringify(profileModel) )
        //QML_Debugger.listEverything(profileModel)

        // set display model:
        drawerRoot.profileDummy = profileModel
        drawerRoot.fullusername = profileModel.displayName
    }
    // When the drawer is opened:
    onOpened: {
        drawerRoot.populateDummy()
    }

    //-----------------------------------------------------------------------------
    // Button Action:
    function contextButtonCall(but_name) {
        console.log("Nav button OnClick: ", but_name)
        switch (but_name) {
        case "myprofile_nav":

            break;
        case "accounts_nav":

            break;
        case "activity_nav":

            break;
        case "contacts_nav":

            break;
        case "send_payment_nav":

            break;
        case "request_payment_nav":

            break;
        case "settings_nav":

            break;
        case "about_nav":

            break;
        case "profile_context":

            break;
        default:
            console.warn("BurgerMenu context Button objectName is unknown.")
        }
    }

    //-----------------------------------------------------------------------------
    // Background styling:
    background: Column {
        id: background
        spacing: 0
        property color bgColor: rootAppPage.currentStyle > 0 ? "black" : "white"
        Rectangle { // current profile:
            id: bgTopSection
            height: 160  // high impact setting
            opacity: 0.8
            width: drawerRoot.width
            color: background.bgColor
        }
        Rectangle { // navigation menu:
            id: bgBottomSection
            width: drawerRoot.width
            height: drawerRoot.height - bgTopSection.height
            color: background.bgColor
        }
    }

    //-----------------------------------------------------------------------------
    // Current User Profile header:
    Column {
        height: bgTopSection.height
        width: parent.width
        topPadding: 16

        //----------------------
        MatterFi_Avatar {
            id: avatarImage
            x: 12
            width: 80
            height: 80
            avatarUrl: profileDummy.avatar_url || ""
        }

        // User name with profile Edit button
        Row {
            id: profileHeader
            width: drawerRoot.width
            topPadding: 24
            leftPadding: 16
            spacing: parent.width - usernameText.width - 52
            anchors.horizontalCenter: parent.horizontalCenter
            //----------------------
            Text {
                id: usernameText
                text: drawerRoot.fullusername
                color: CustStyle.theme_fontColor
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: CustStyle.fsize_normal
                anchors.verticalCenter: parent.verticalCenter
            }
            //----------------------
            FontIconButton {
                id: profileContextButton
                iconChar: IconIndex.caret_down
                //iconSize: 18
                anchors.verticalCenter: parent.verticalCenter
                objectName: "profile_context"
                onAction: drawerRoot.contextButtonCall(objectName)
            }
        }
    }

    //-----------------------------------------------------------------------------
    // Navigation burger list items:
    Column {
        id: burgerObjectListing
        topPadding: bgTopSection.height
        leftPadding: 16
        spacing: 0
        height: bgBottomSection.height
        width: drawerRoot.width
        property int burgerButlistHeight: 36
        //----------------------
        MatterFi_BurgerListItem {
            id: myProfileSelection
            width: parent.width
            height: parent.burgerButlistHeight
            lableText: qsTr("My Profile")
            svgImageName: "user"
            svgType: 1 // solid
            objectName: "myprofile_nav"
            onAction: drawerRoot.contextButtonCall(objectName)
        }

        Rectangle {
            color: "transparent"
            height: parent.burgerButlistHeight - 8
            x: parent.burgerButlistHeight + burgerObjectListing.leftPadding
            width: parent.width - (rootAppPage.width - parent.width) - parent.leftPadding
            Rectangle {
                color: CustStyle.neutral_fill
                height: 1
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        //----------------------
        MatterFi_BurgerListItem {
            id: accountsSelection
            width: parent.width
            height: parent.burgerButlistHeight
            lableText: qsTr("Accounts")
            svgImageName: "wallet"
            svgType: 1 // solid
            objectName: "accounts_nav"
            onAction: drawerRoot.contextButtonCall(objectName)
        }
        MatterFi_BurgerListItem {
            id: activitySelection
            width: parent.width
            height: parent.burgerButlistHeight
            lableText: qsTr("Activity")
            svgImageName: "list"
            svgType: 1 // solid
            objectName: "activity_nav"
            onAction: drawerRoot.contextButtonCall(objectName)
        }
        MatterFi_BurgerListItem {
            id: contactsSelection
            width: parent.width
            height: parent.burgerButlistHeight
            lableText: qsTr("Contacts")
            svgImageName: "user-friends"
            svgType: 1 // solid
            objectName: "contacts_nav"
            onAction: drawerRoot.contextButtonCall(objectName)
        }

        Rectangle {
            color: "transparent"
            height: parent.burgerButlistHeight - 8
            x: parent.burgerButlistHeight + burgerObjectListing.leftPadding
            width: parent.width - (rootAppPage.width - parent.width) - parent.leftPadding
            Rectangle {
                color: CustStyle.neutral_fill
                height: 1
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        //----------------------
        MatterFi_BurgerListItem {
            id: sendPaymentSelection
            width: parent.width
            height: parent.burgerButlistHeight
            lableText: qsTr("Send Payment")
            svgImageName: "check"
            svgType: 1 // solid
            objectName: "profile_nav"
            onAction: drawerRoot.contextButtonCall(objectName)
        }
        MatterFi_BurgerListItem {
            id: requestPaymentSelection
            width: parent.width
            height: parent.burgerButlistHeight
            lableText: qsTr("Request Payment")
            svgImageName: "check"
            svgType: 1 // solid
            objectName: "request_payment_context"
            onAction: drawerRoot.contextButtonCall(objectName)
        }

        Rectangle {
            color: "transparent"
            height: parent.burgerButlistHeight - 8
            x: parent.burgerButlistHeight + burgerObjectListing.leftPadding
            width: parent.width - (rootAppPage.width - parent.width) - parent.leftPadding
            Rectangle {
                color: CustStyle.neutral_fill
                height: 1
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        //----------------------
        MatterFi_BurgerListItem {
            id: settingsSelection
            width: parent.width
            height: parent.burgerButlistHeight
            lableText: qsTr("Settings")
            svgImageName: "cog"
            svgType: 1 // solid
            objectName: "settings_nav"
            onAction: drawerRoot.contextButtonCall(objectName)
        }
        MatterFi_BurgerListItem {
            id: aboutSelection
            width: parent.width
            height: parent.burgerButlistHeight
            lableText: qsTr("About")
            svgImageName: "info-circle"
            svgType: 1 // solid
            objectName: "about_nav"
            onAction: drawerRoot.contextButtonCall(objectName)
        }
        //----------------------
    }
}
