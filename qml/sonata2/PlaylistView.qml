import QtQuick 2.0
import com.bitfraction.sonata 0.1

ListView {
    id: root

    currentIndex: 0
    onCountChanged: {
        if (currentIndex >= count)
        {
            currentIndex = 0;
            positionViewAtBeginning();
        }
    }

    Navigation.onNavigationEvent: {
        if (event.key === Navigation.Up)
        {
            if (currentIndex > 0)
            {
                decrementCurrentIndex();
                event.accepted = true;
            }
        }
        else if (event.key === Navigation.Down)
        {
            if (currentIndex < count - 1)
            {
                incrementCurrentIndex();
                event.accepted = true;
            }
        }
    }

    highlight: activeFocus ? highlightDelegate : null

    Component {
        id: highlightDelegate
        Rectangle {
            color: "#30ffffff"
            width: root.width
            height: 40
//            radius: 5
        }
    }

    clip: true
    boundsBehavior: Flickable.StopAtBounds
    interactive: false
}
