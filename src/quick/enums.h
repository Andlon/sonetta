#pragma once

#include <QObject>
#include <Spotinetta/Spotinetta>

/* QML doesn't support enums not defined in QObject-derived classes,
 * so we need to "forward" them through never-instantiated QObjects.
 */

namespace Sonetta {

class AlbumEnums : public QObject {
    Q_OBJECT



public:

};

class TrackEnums : public QObject {
    Q_OBJECT

    Q_ENUMS(Availability)

    // For less typing
    typedef Spotinetta::Track::Availability A;

public:
    enum Availability {
        Unavailable = (unsigned int) A::Unavailable,
        Available = (unsigned int) A::Available,
        NotStreamable = (unsigned int) A::NotStreamable,
        BannedByArtist = (unsigned int) A::BannedByArtist
    };

};

}
