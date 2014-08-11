#pragma once

#include <QString>

namespace Sonetta {

class Artist;
class Album;

struct TrackData {

};

class Track {
public:
    QString title() const;
    Artist  artist() const;
    Album   album() const;
    int     albumIndex() const;
};

}
