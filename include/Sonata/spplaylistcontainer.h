#ifndef SPPLAYERLISTCONTAINER_H
#define SPPLAYERLISTCONTAINER_H

#include "definitions.h"

class QString;

namespace SonataPrivate {
class SpPlaylistContainerPrivate;
class SpFactory;
}

class SpPlaylistContainer
{
public:
    SpPlaylistContainer();
    SpPlaylistContainer(const SpPlaylistContainer &other);
    ~SpPlaylistContainer();

    bool isValid() const;
    bool isLoaded() const;

    SpPlaylistList playlists() const;
    int playlistCount() const;

    SpPlaylist playlistAt(int index) const;

    SpPlaylistContainer& operator=(const SpPlaylistContainer& other);

private:
    SonataPrivate::SpPlaylistContainerPrivate * d;
    SpPlaylistContainer(SonataPrivate::SpPlaylistContainerPrivate *);

    friend class SonataPrivate::SpFactory;
    friend class SpLink;
    friend bool operator==(const SpPlaylistContainer &c1, const SpPlaylistContainer &c2);

};

bool operator==(const SpPlaylist &c1, const SpPlaylist &c2);

Q_DECLARE_METATYPE(SpPlaylistContainer)

#endif // SPPLAYERLISTCONTAINER_H
