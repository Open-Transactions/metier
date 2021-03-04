import QtQuick 2.15
import QtQml.Models 2.1

import QtQuick.Controls 2.15
import QtQuick.Controls.Universal 2.12


import "qrc:/styling"
import "qrc:/matterfi"
import "qrc:/styling/cust_formater"
import "qrc:/qml_shared"

//-----------------------------------------------------------------------------
Page {
    id: viewContactPage
    width: rootAppPage.width
    height: rootAppPage.height
    //title: set dynamically by contact 'fullusername'
    objectName: "view_contact_profile"
    background: null //Qt.transparent

    // navbar properties
    property bool showConvoButton: true
    property bool showQRcodeButton: true
    //----------------------
    function onConvoButtonAction() {
        console.log("Contact View: convo button clicked.")

    }

    function onQRcodeButtonAction() {
        console.log("Contact View: qr code button clicked.")

    }

    function onPaymentLinkButton() {
        console.log("Contact View: payment link button clicked.")

    }

    function onContactEmail() {
        console.log("Contact View: email button clicked.")

    }

    function onContactPhone() {
        console.log("Contact View: phone button clicked.")

    }

    function onEditContactButton() {
        console.log("Contact View: edit contact button clicked.")

    }

    //----------------------
    // Should be the same fields as MatterFi::UserProfile struct
    property int profile_id: 0
    property var contactDummy: {
        "name_first": 'Demo',
        "name_middle": 'Is',
        "name_last": 'User',
        "emails": [["tested", 'demo@email.com']],
        "social_medias": [[0, '']],
        "phone_numbers": [['Home', '555-555-5555']],
        "website": "Demoweb.com",
        "address": "598 First Demo st.",
        "location": "Austin, TX",
        "job_title": ["MatterFi", "Software Program"],
        "nymid_string": "111222333444555666777888999000111222",
        "payment_code": "333555777999111666444333777999111884",
        "avatar_url": ""
    }
    //----------------------
    // make the display for contact profile view
    function populateFeilds() {
        // not logged in, show no contacts
        // matterfi.isuser_logged_in()
        if (false) {
            var contactInfo = {} //matterfi.get_contact_details(profile_id)
            if (Object.keys(contactInfo) < 1) {
                console.warn("Contact View: No user information was found. MatteFi_ID:", profile_id)
            } else {
                viewContactPage.contactDummy = contactInfo
                console.log("Contact View: User information populated: ", contactInfo)
            }
        } else {
            console.warn("Contact View: No user is Signed in.")
        }
    }
    //----------------------
    // Build complete username string
    property string fullusername: "null-user"
    function viewUsername() {
        var whole_name = contactDummy.name_first
        whole_name = whole_name + " " + contactDummy.name_last
        if (whole_name.length > 1) {
            // will display full name in Navbar title
            viewContactPage.title = whole_name
        }
    }
    //----------------------
    // due to the display nature of the Nym ID, we need to create an
    // array for a Grid
    property var nymgridIditems: []
    function setNymIdGrid() {
        viewContactPage.nymgridIditems = []
        if (contactDummy.nymid_string.length > 0) {
            // break down the string into 3 character chunks
            var stringChars = contactDummy.nymid_string.split("")
            var stringchunk = ""
            var stringLength = stringChars.length
            for (var index = 0; index < stringLength; index++) {
                stringchunk = stringchunk + stringChars[index]
                if (stringchunk.length >= 3) {
                    viewContactPage.nymgridIditems.push(stringchunk)
                    stringchunk = ""
                } else if ( index >= stringLength - 1) {
                    viewContactPage.nymgridIditems.push(stringchunk)
                }
            }
        } else {
            viewContactPage.nymgridIditems.push("Nan")
        }
        // in order tp update the display you have to push to the
        // Repeater model
        nymidDisplayText.model = viewContactPage.nymgridIditems
    }
    //----------------------
    // Make the displayed payment code shorter so it fits in display rect
    property string shorterPaymentCode: "nan"
    function shorterPaymentcode() {
        var stringlength = contactDummy.payment_code.length
        if (stringlength > 0) {
            var shortstring = ""
            shortstring = contactDummy.payment_code.substr(0,15)
            shortstring = shortstring + "..."
            shortstring = shortstring + contactDummy.payment_code.substr(stringlength - 8, stringlength)
            viewContactPage.shorterPaymentCode = shortstring
        } else {
            viewContactPage.shorterPaymentCode = "nan"
        }
    }
    //----------------------
    // run soon as page is ready
    Component.onCompleted: {
        viewContactPage.populateFeilds()
        viewContactPage.viewUsername()
        viewContactPage.setNymIdGrid()
        viewContactPage.shorterPaymentcode()
        console.log("Contact View: profile data populated. ", contactDummy)
    }

    //-----------------------------------------------------------------------------
    // Background styling:
    background: Column {
        id: background
        spacing: 0
        width: parent.width
        property color bgColor: rootAppPage.currentStyle > 0 ? CustStyle.dm_navbar_bg : CustStyle.lm_navbar_bg
        property color fontColor: rootAppPage.currentStyle > 0 ? CustStyle.dm_navbar_text : CustStyle.lm_navbar_text

        Rectangle { // contact avatar, nym:
            id: bgTopSection
            height: 90
            width: parent.width
            color: background.bgColor
        }
        Rectangle { // contact details:
            id: bgBottomSection
            width: parent.width
            height: parent.height - bgTopSection.height
            color: "transparent"
        }
    }

    //-----------------------------------------------------------------------------
    // Avatar header layout:
    Column {
        id: avatarHeaderColumn
        width: parent.width
        spacing: 6
        anchors.horizontalCenter: parent.horizontalCenter

        Row {
            id: avatarDisplayRow
            topPadding: 8
            leftPadding: 16
            spacing: 16

            MatterFi_Avatar {
                id: avatarImage
                width: 70
                height: width
                avatarUrl: contactDummy.avatar_url
            }

            // Nym ID string chuck display Grid
            Column {
                id: nymDisplayColumn
                spacing: 6

                Text {
                    text: qsTr("Nym ID")
                    color: CustStyle.neutral_text
                    leftPadding: 16
                    font.pixelSize: CustStyle.fsize_normal
                    font.weight: Font.Bold
                    font.pixelSize: CustStyle.fsize_title
                }
                //----------------------
                Grid {
                    id: nymidGrid
                    columnSpacing: 6
                    rowSpacing: 4
                    columns: 6
                    leftPadding: 14
                    y: nymDisplayColumn.y
                    x: nymDisplayColumn.x
                    anchors.horizontalCenter: parent.horizontalCenter

                    Repeater {
                        id: nymidDisplayText
                        model: viewContactPage.nymgridIditems
                        Text {
                            font.pixelSize: CustStyle.fsize_normal
                            text: model.modelData
                            color: "darkgray"
                        }
                    }
                }
            }
        }
    }

    //-----------------------------------------------------------------------------
    // Details body layout:
    Column {
        id: contactDetailsColumn
        y: bgTopSection.height
        topPadding: 16
        leftPadding: 8
        spacing: 0
        height: bgBottomSection.height
        width: parent.width
        property int itemlistHeight: 36

        //----------------------
        ListButtonObject {
            id: paymentCodeView
            width: parent.width
            height: parent.itemlistHeight
            //svgiconName: "link"
            iconChar: IconIndex.link
            txtValue: viewContactPage.shorterPaymentCode
            txtLable: qsTr("Payment Code")
            onAction: onPaymentLinkButton()
        }
        // Show cat. divider
        HorizontalLineRect {
            height: contactDetailsColumn.itemlistHeight - 8
            width: parent.width
            color: CustStyle.neutral_fill
            bottomPadding: 12
        }

        //----------------------
        Text {
            text: qsTr("Contact")
            color: CustStyle.theme_fontColor
            font.pixelSize: CustStyle.fsize_normal
            bottomPadding: 12
        }
        Column {
            width: parent.width
            spacing: 16
            bottomPadding: 12
            // Display the phone number
            ListButtonObject {
                id: phoneNumberView
                property variant phonenumber: contactDummy.phone_numbers[0]
                width: parent.width
                height: contactDetailsColumn.itemlistHeight
                //svgiconName: "phone"
                iconChar: IconIndex.phone_alt
                txtValue: phonenumber[1] || "" // is the number value
                txtLable: phonenumber[0] || "" // is the lable to number
                onAction: onContactPhone()
            }
            // Display the email
            ListButtonObject {
                id: emailView
                property variant email: contactDummy.emails[0]
                width: parent.width
                height: contactDetailsColumn.itemlistHeight
                //svgiconName: "envelope-square"
                iconChar: IconIndex.envelope
                txtValue: email[1] || "" // is the address
                txtLable: email[0] || "" // is the associated lable
                onAction: onContactEmail()
            }
        }
        // Show cat. divider
        HorizontalLineRect {
            height: contactDetailsColumn.itemlistHeight - 8
            width: parent.width
            color: CustStyle.neutral_fill
            bottomPadding: 12
        }

        //----------------------
        Text {
            text: qsTr("About")
            color: CustStyle.theme_fontColor
            font.pixelSize: CustStyle.fsize_normal
            bottomPadding: 12
        }
        Column {
            width: parent.width
            spacing: 16
            bottomPadding: 16
            // Display the phone number
            ListButtonObject {
                id: locationView
                property string location: contactDummy.location
                width: parent.width
                height: contactDetailsColumn.itemlistHeight
                //svgiconName: "home"
                iconChar: IconIndex.home
                txtValue: location || "" // is the state town
                txtLable: "home"
                iconIsButton: false
            }
            // Display the email
            ListButtonObject {
                id: occupationView
                property variant occupation: contactDummy.job_title
                width: parent.width
                height: contactDetailsColumn.itemlistHeight
                //svgiconName: "building"
                iconChar: IconIndex.building
                txtValue: occupation[0] || "" // is the company name
                txtLable: occupation[1] || "" // is the job title held
                iconIsButton: false
            }
        }

        //----------------------
        // Edit contact button
        FontIconButton {
            id: editContactButton
            iconChar: IconIndex.plus_circle
            iconSize: 24
            x: rootAppPage.width - 80
            y: avatarHeaderColumn.height + contactDetailsColumn.height - 240
            iconSize: 52
            iconPadding: 36
            radius: width / 2
            color: "white"
            bgcolor: "orange"
            onAction: onEditContactButton()
        }
    }
}



//-----------------------------------------------------------------------------
