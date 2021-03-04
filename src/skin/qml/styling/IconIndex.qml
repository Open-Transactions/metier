pragma Singleton
import QtQuick 2.15

//-----------------------------------------------------------------------------
// An index of the font awesome unicode icons used for better Fonts usage.
// Synopsis:
/*
    Text {
        text: IconIndex.eye_slash
        font.family: Fonts.icons_reg
        font.pixelSize: <size of font icon>
    }
*/
//-----------------------------------------------------------------------------
// Notes:
//   *Does not include slashed keyboard or slashed camera icons.
//   *Not every icon met image specs. See attached comments.
//   *Setting font.weight to anything but normal can break some characters.
//-----------------------------------------------------------------------------
// Unicode registry is for the Free version of Font Awesome v5.
QtObject {
    // Both Solid and Regular:  Fonts.icons_reg or Fonts.icons_solid
    property string eye_slash:       "\uf070"
    property string paper_plane:     "\uf1d8"
    property string question_circle: "\uf059"
    property string keyboard:        "\uf11c"
    property string comments:        "\uf086"
    property string envelope:        "\uf0e0"
    property string building:        "\uf1ad"  // *is single building

    // Solid Only: Fonts.icons_solid
    property string slash:           "\uf715"
    property string address_card:    "\uf2bb"
    property string camera:          "\uf030"
    property string info_circle:     "\uf05a"
    property string user_alt:        "\uf406"
    property string wallet:          "\uf555"
    property string list:            "\uf03a"
    property string user_friends:    "\uf500"
    property string cog:             "\uf013"
    property string qrcode:          "\uf029"
    property string bars:            "\uf0c9"
    property string ellipsis_v:      "\uf142"  // three virticle dots  ef8582
    property string caret_down:      "\uf0d7"
    property string search:          "\uf002"
    property string right_arrowhead: "\u02c3"
    property string left_arrowhead:  "\u02c2"
    property string down_arrowhead:  "\u02c5"
    property string up_arrowhead:    "\u02c4"
    property string chevron_right:   "\uf054"
    property string chevron_left:    "\uf053"
    property string chevron_up:      "\uf077"  // *some reason is missing?
    property string chevron_down:    "\uf078"  // *some reason is missing?
    property string paperclip:       "\uf0c6"  // *not virtical
    property string portrait:        "\uf3e0"
    property string user_plus:       "\uf234"
    property string plus:            "\uf067"
    property string check:           "\uf00c"
    property string link:            "\uf0c1"  // *not horizontal
    property string phone_alt:       "\uf879"
    property string home:            "\uf015"  // *has chimney
    property string copy:            "\uf0c5"

    property string arrow_alt_circle_left: "\uf359"
}

//-----------------------------------------------------------------------------
