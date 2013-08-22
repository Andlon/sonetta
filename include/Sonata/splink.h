#ifndef SPLINK_H
#define SPLINK_H

#include "definitions.h"

#include <QString>

namespace SonataPrivate {
class SpLinkPrivate;
class SpFactory;
}

class SpLink
{
public:

    SpLink();
    SpLink(const SpLink &other);
    SpLink(const QString &uri);
    SpLink(const SpImage &image);
    SpLink(const SpArtist &artist);
    SpLink(const SpTrack &track);
    SpLink(const SpAlbum &album);
    SpLink(const SpPlaylist &playlist);
    ~SpLink();

    Spotify::LinkType type() const;
    bool isValid() const;
    QString uri() const;

    SpTrack track() const;
    SpImage image() const;
    SpArtist artist() const;
    SpAlbum album() const;
//    SpPlaylist playlist() const;

    SpLink& operator =(const SpLink &other);

    static SpLink fromAlbumCover(const SpAlbum &album, Spotify::ImageSize size);
    static SpLink fromArtistPortrait(const SpArtist &artist, Spotify::ImageSize size);

private:
    SonataPrivate::SpLinkPrivate * d;
    SpLink(SonataPrivate::SpLinkPrivate *);

    friend class SonataPrivate::SpFactory;
    friend bool operator==(const SpLink &link1, const SpLink &link2);
};

bool operator==(const SpLink &link1, const SpLink &link2);

Q_DECLARE_METATYPE(SpLink)

#endif // SPLINK_H
