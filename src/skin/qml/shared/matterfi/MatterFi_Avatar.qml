import QtQuick 2.15
import QtQuick.Controls.Universal 2.12
import QtQuick.Controls 2.15

//import QtGraphicalEffects 1.12 **Deprecated**
// QtGraphicalEffects replaced with:
// Qt5Compact  ->  QtQuick.MultiEffect  *another thing qt is selling that once was free*
// https://www.qt.io/blog/porting-from-qt-5-to-qt-6-using-qt5compat-library

import "qrc:/styling"

//-----------------------------------------------------------------------------
// 'MatterFi_Avatar.qml'
// Used to provide a place holder image while loading an image for a profile.
//-----------------------------------------------------------------------------
Item {
    id: contextRoot

    property string avatarUrl: "http://matterfi.com/assets/images/svg/Matterfi-normal.svg"
    property color bgcolor: "transparent"
    property var objectName: ""

    property int leftPadding: 0
    property int rightPadding: 0

    //-----------------------------------------------------------------------------
    Rectangle {
        id: body
        width: contextRoot.width
        height: contextRoot.height
        color: contextRoot.bgcolor

        Image {
            id: defaultPlaceholderImage
            width: body.width
            height: body.height
            sourceSize.width: body.width
            sourceSize.height: body.height
            source: "qrc:/assets/svgs/Matterfi-normal.svg"
            fillMode: Image.PreserveAspectFit //Image.PreserveAspectCrop
            smooth: true

            layer.enabled: true
            //layer.effect: OpacityMask { // TODO: QtGraphicalEffects
            //    maskSource: mask
            //}

            anchors.leftMargin: contextRoot.leftPadding
            anchors.rightMargin: contextRoot.rightPadding
        }
        //----------------------
        // Retrive the Avatar, then draw it:
        // https://doc.qt.io/qt-5/qml-qtquick-image.html#fillMode-prop
        // https://doc.qt.io/qt-5.12/qtqml-documents-networktransparency.html
        Image {
            id: avatarImage
            property string imgUrl: contextRoot.avatarUrl.trim()
            source: imgUrl
            width: body.width
            height: body.height
            sourceSize.width: body.width
            sourceSize.height: body.height
            fillMode: Image.PreserveAspectFit //Image.PreserveAspectCrop
            smooth: true

            layer.enabled: true
            //layer.effect: OpacityMask { // TODO: QtGraphicalEffects
            //    maskSource: mask
            //}

            onCacheChanged: {
                if (contextRoot.avatarUrl.length > 0) {
                    defaultPlaceholderImage.visible = false
                }
            }
        }
        //----------------------
        // Rounding image mask used
        Rectangle {
            id: mask
            width: 240
            height: 240
            radius: width / 2
            visible: false
        }
    }
}
