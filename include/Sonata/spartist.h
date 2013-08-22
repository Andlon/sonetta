#ifndef SPARTIST_H
#define SPARTIST_H

#include "definitions.h"

class QString;

namespace SonataPrivate {
class SpArtistPrivate;
class SpFactory;
}

class SpArtist
{
public:
    SpArtist();
    SpArtist(const SpArtist &other);
    ~SpArtist();

    bool isValid() const;
    bool isLoaded() const;

    QString name() const;
    SpImage portrait(Spotify::ImageSize size) const;

    SpArtist& operator=(const SpArtist &other);

private:
    SonataPrivate::SpArtistPrivate * d;
    SpArtist(SonataPrivate::SpArtistPrivate *);

    friend class SonataPrivate::SpFactory;
    friend class SpLink;
    friend bool operator==(const SpArtist &a1, const SpArtist &a2);
};

bool operator==(const SpArtist &a1, const SpArtist &a2);

Q_DECLARE_METATYPE(SpArtist)
Q_DECLARE_METATYPE(SpArtistList)

#endif // SPARTIST_H
