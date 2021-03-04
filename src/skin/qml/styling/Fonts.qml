pragma Singleton
import QtQuick 2.15

//-----------------------------------------------------------------------------
Item {
    id: fonts
    // Load the external font files:
    readonly property FontLoader fontAwesomeRegular: FontLoader {
        id: regular
        source: "qrc:/assets/fonts/fontAwesome/FA5-r400.otf"
    }
    readonly property FontLoader fontAwesomeSolid: FontLoader {
        id: solid
        source: "qrc:/assets/fonts/fontAwesome/FA5-s900.otf"
    }
    readonly property FontLoader fontAwesomeBrands: FontLoader {
        id: brands
        source: "qrc:/assets/fonts/fontAwesome/FA5-b400.otf"
    }
    // Make loaded fonts accessible by string name, used for Font.family
    readonly property string icons_reg:   regular.name
    readonly property string icons_solid: solid.name
    readonly property string icons_brand: brands.name
}
