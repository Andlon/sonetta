#ifndef SPPLAYLIST_H
#define SPPLAYLIST_H

#include "definitions.h"

class QString;

namespace SonataPrivate {
class SpPlaylistPrivate;
class SpFactory;
}

class SpPlaylist
{
public:
    SpPlaylist();
    SpPlaylist(const SpPlaylist &other);
    ~SpPlaylist();

    bool isValid() const;
    bool isLoaded() const;

    QString name() const;
    QString description() const;

    SpTrackList tracks() const;
    int trackCount() const;

    SpTrack trackAt(int index) const;

    SpImage image() const;

    SpPlaylist& operator=(const SpPlaylist& other);

private:
    SonataPrivate::SpPlaylistPrivate * d;
    SpPlaylist(SonataPrivate::SpPlaylistPrivate *);

    friend class SonataPrivate::SpFactory;
    friend class SpLink;
    friend bool operator==(const SpPlaylist &p1, const SpPlaylist &p2);

};

bool operator==(const SpPlaylist &p1, const SpPlaylist &p2);

Q_DECLARE_METATYPE(SpPlaylist)
Q_DECLARE_METATYPE(SpPlaylistList)

#endif // SPPLAYLIST_H
