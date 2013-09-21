import QtQuick 2.0

Image {

    onSourceChanged: {
        if (source.indexOf("image://sp/") != 0)
            source = "image://sp/" + source
    }
}
