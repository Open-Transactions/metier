import QtQuick 2.15

//-----------------------------------------------------------------------------
// Check for swip gestures, but only if on IOS
MouseArea {
    id: gestureRoot
    hoverEnabled: true
    scrollGestureEnabled: false
    propagateComposedEvents: true
    enabled: Qt.platform.os === "ios"

    signal backGestureDetected()

    property int xPrev: 0
    property int xStart: 0
    property real velocity: 0.0
    property bool tracing: false
    property bool allowedToWork: true

    //----------------------
    onPressed: {
        //console.log("IOS Gesture pressed.")
        if (allowedToWork) {
            xStart = mouse.x
            xPrev = mouse.x
            velocity = 0
            tracing = true
            mouse.accepted = true
            //console.log("IOS Gesture detected, start.")
        } else {
            mouse.accepted = false
        }
    }
    //----------------------
    onPositionChanged: {
        if (!tracing) {
            mouse.accepted = false
            return
        }
        var currVel = (mouse.x - xPrev)
        velocity = (velocity + currVel) / 2.0
        xPrev = mouse.x
        mouse.accepted = false
        //console.log("IOS Gesture detected, tracing.", velocity)
    }
    //----------------------
    onReleased: {
        if (!tracing) {
            mouse.accepted = false
            return
        }
        tracing = false
        if (velocity > 15 && mouse.x > parent.width * 0.2) {
            console.log("IOS Gesture detected, triggered.")
            backGestureDetected()
        }
        mouse.accepted = false
        //console.log("IOS Gesture detected, finished.")
    }
}
