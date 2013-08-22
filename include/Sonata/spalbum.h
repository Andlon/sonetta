#ifndef SPALBUM_H
#define SPALBUM_H

#include "definitions.h"

class QString;
namespace SonataPrivate {
class SpAlbumPrivate;
class SpFactory;
}

class SpAlbum {
public:
    SpAlbum();
    SpAlbum(const SpAlbum &other);
    ~SpAlbum();

    bool isValid() const;
    bool isLoaded() const;

    bool isAvailable() const;
    QString name() const;
    int year() const;
    Spotify::AlbumType type() const;

    SpArtist artist() const;
    SpImage cover(Spotify::ImageSize size) const;

    SpAlbum& operator=(const SpAlbum &other);

private:
    SonataPrivate::SpAlbumPrivate * d;
    SpAlbum(SonataPrivate::SpAlbumPrivate *);

    friend class SonataPrivate::SpFactory;
    friend class SpLink;
    friend bool operator==(const SpAlbum &a1, const SpAlbum &a2);
};

bool operator==(const SpAlbum &a1, const SpAlbum &a2);

Q_DECLARE_METATYPE(SpAlbum)
Q_DECLARE_METATYPE(SpAlbumList)

#endif // SPALBUM_H
