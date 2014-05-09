import QtQuick 2.2

Image {
    property string pattern: "dark"

    source: {
        switch (pattern) {
        case "dark":
            return "../images/dark.png"
        case "medium":
            return "../images/medium.png"
        case "light":
            return "../images/light.png"

        default:
            console.log("Unknown pattern requested.")
            return ""
        }
    }

    fillMode: Image.Tile
}
