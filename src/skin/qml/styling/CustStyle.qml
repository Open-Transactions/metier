pragma Singleton
// More about how singltons work:
// https://doc.qt.io/qt-5/qtqml-modules-qmldir.html
//-----------------------------------------------------------------------------
import QtQuick 2.15
//-----------------------------------------------------------------------------
// Styling used threw-out the App. For the most part the Theme palette should
// be taking care of this display featuring. Due to limitations with setting
// a palette before loading any qml to change colors, we use a shared color
// property in the display object. This also allows us to "hot" swap colors.
//
// For the most of color usage values, that is set in the "qtquickcontrols2.conf"
// file. This would be the Theme"s Background, Foreground, and Accent colors.
//
// useful Links:
//
//   Known system color Constants and values:
//   https://doc.qt.io/qt-5/qml-color.html
//
//   Qt Quick Controls 2:
//   https://qmlbook.github.io/ch06-controls/controls.html
//
//   What are QtObject "palette" how to use it:
//   https://doc.qt.io/qt-5/qml-palette.html
//   https://doc.qt.io/qt-5/qpalette.html#ColorRole-enum
//
//   palette Limitations/Usage:
//   https://doc.qt.io/qt-5/qml-qtquick-controls2-control.html#palette-prop
//
//   Animations:
//   https://doc.qt.io/archives/qt-5.9/qtquick-animation-example.html
//
//   Behavours:
//   https://doc.qt.io/qt-5/qtquick-statesanimations-behaviors.html
//
//   Gradients:
//   https://doc.qt.io/qt-5/qml-qtquick-gradient.html
//
//   TextInput vs TextFeild *see which is closer to LineEdit*
//   https://forum.qt.io/topic/41348/solved-what-are-the-differences-between-textfield-and-textinput-also-textarea-and-textedit/4
//
//   Mouse Input target area:
//   https://doc.qt.io/qt-5/qml-qtquick-mousearea.html
//
//   Drawing an SVG:
//   https://doc.qt.io/qt-5/qml-qtquick-image.html#fillMode-prop
//
// Setting a system wide application font? Load after QApplication creation in C++
/*
    QFont use_font("file_name", point_size);
    metier.setFont(use_font);
*/
//
//-----------------------------------------------------------------------------
QtObject {
    // The current QML UIUX version/revision number:
    property int qml_release_version: 2

    // Font Sizes
    property int fsize_xlarge:  32
    property int fsize_large:   24
    property int fsize_title:   20
    property int fsize_burger:  18
    property int fsize_normal:  14
    property int fsize_button:  14
    property int fsize_lable:   12
    property int fsize_contex:  10

    // animates shrinking a button on hold when clicking
    property var but_shrink: 0.9 

    //-----------------------------------------------------------------------------
    // Colors used threw out Application"s Dark/Light mode.
    //  Qt.rgba(1,1,1,1.0)
    //----------------------
    // Light Mode
    property color lightmode_text: "black"
    property color lm_navbar_text: "white"
    property color lm_text_accent: "gray"
    property color lm_navbar_bg:   "black"
    property color lm_icon_but:    "black"
    property color lm_outline:     "gray"
    property color lm_pagebg:      "#ffffff"
    //----------------------
    // Dark Mode
    property color darkmode_text:  "white"
    property color dm_navbar_text: "black"
    property color dm_text_accent: "gray"
    property color dm_navbar_bg:   "white"
    property color dm_icon_but:    "white"
    property color dm_outline:     "gray"
    property color dm_pagebg:      "#212121"
    //----------------------
    // Text used on Buttons
    property color buttxt_norm_lm:  "black"
    property color buttxt_norm_dm:  "white"
    property color buttxt_disabled: "gray"
    //----------------------
    // Used for Accent color styling
    property color accent_normal:  "royalblue"  // typical base accent color
    property color accent_active:  "steelblue"  // color when element active
    property color accent_text:    "steelblue"  // color for accent text
    property color accent_outline: "steelblue"  // color for accent OutlineSimple
    property color accent_fill:    "royalblue"  // color used for accent color fill
    //----------------------
    // Neuteral colors that never change with theme
    property color neutral_fill:    "gray"
    property color neutral_text:    "gray"
    property color neutral_correct: "green"
    property color neutral_error:   "red"
    //----------------------
    // Company Specific colors
    property color pkt_logo: "#3cadef"
    property color pkt_logo_highlight: "#333cadef"
    //----------------------
    // Gradients used:
    property var accent_gradient: Gradient {
        GradientStop { position: 0.0; color: CustStyle.accent_normal }
        GradientStop { position: 1.0; color: CustStyle.accent_active }
    }
        
    //-----------------------------------------------------------------------------
    // Current Applied theme:
    property color theme_fontColor: darkmode_text

    property color active_highlight: "#223cadef"
}
