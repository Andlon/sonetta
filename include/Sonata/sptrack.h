#ifndef SPTRACK_H
#define SPTRACK_H

#include "definitions.h"
#include <QMetaType>

class QString;

namespace SonataPrivate {
class SpTrackPrivate;
class SpFactory;
}

class SpTrack
{
public:
    SpTrack();
    SpTrack(const SpTrack &other);
    ~SpTrack();

    bool isValid() const;
    bool isLoaded() const;
    bool isAutoLinked() const;
    bool isAvailable() const;

    QString name() const;

    int duration() const;
    int popularity() const;

    SpArtistList artists() const;
    int artistCount() const;

    SpArtist artistAt(int index) const;
    SpAlbum album() const;

    SpTrack playableTrack() const;

    SpTrack& operator=(const SpTrack &other);

private:
    SonataPrivate::SpTrackPrivate * d;
    SpTrack(SonataPrivate::SpTrackPrivate *);

    friend class SonataPrivate::SpFactory;
    friend class SpLink;
    friend bool operator==(const SpTrack &t1, const SpTrack &t2);
};

bool operator==(const SpTrack &t1, const SpTrack &t2);

Q_DECLARE_METATYPE(SpTrack)
Q_DECLARE_METATYPE(SpTrackList)

#endif // SPTRACK_H
