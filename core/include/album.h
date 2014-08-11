#pragma once

#include <QString>

namespace Sonetta {

struct AlbumData {

};

class Artist;

class Album {
public:
    QString name() const;
    Artist  artist() const;
};

}
