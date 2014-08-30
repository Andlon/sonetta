import QtQuick 2.2

Image {
    property string pattern: "dark"
    visible: pattern != ""

    source: {
        switch (pattern) {
        case "dark":
            return "../images/dark.png"
        case "medium":
            return "../images/medium.png"
        case "light":
            return "../images/light.png"
        case "":
            return ""

        default:
            console.log("Unknown pattern requested.")
            return ""
        }
    }

    fillMode: Image.Tile
}
