#include <Sonata/spplaylistcontainer.h>

#include "../spcore.h"
#include "../spfactory.h"

using namespace SonataPrivate;

SpPlaylistContainer::SpPlaylistContainer()
    :   d(0)
{
}

SpPlaylistContainer::SpPlaylistContainer(SpPlaylistContainerPrivate *priv)
    :   d(priv)
{
    if (isValid())
        sp_playlistcontainer_add_ref(d->handle);
}

SpPlaylistContainer::SpPlaylistContainer(const SpPlaylistContainer &other)
{
    if (other.isValid())
    {
        d = new SpPlaylistContainerPrivate(*other.d);
        sp_playlistcontainer_add_ref(d->handle);
    }
    else
        d = 0;
}

SpPlaylistContainer::~SpPlaylistContainer()
{
    if (isValid())
        sp_playlistcontainer_release(d->handle);

    delete d;
}



SpPlaylistContainer& SpPlaylistContainer::operator=(const SpPlaylistContainer& other)
{
    if (other.isValid())
        sp_playlistcontainer_add_ref(other.d->handle);

    if (isValid())
        sp_playlistcontainer_release(d->handle);   

    if (d != other.d)
    {
        delete d;
        d = other.d ? new SpPlaylistContainerPrivate(*other.d) : 0;
    }

    return *this;
}

bool operator==(const SpPlaylistContainer &c1, const SpPlaylistContainer &c2)
{
    if (!c1.isValid() || !c2.isValid())
        return false;

    return c1.d->handle == c2.d->handle;
}

bool SpPlaylistContainer::isValid() const
{
    return d != 0 && !d->session.isNull();
}

bool SpPlaylistContainer::isLoaded() const
{
    return isValid() ? sp_playlistcontainer_is_loaded(d->handle) : false;
}

int SpPlaylistContainer::playlistCount() const
{
    return isValid() ? sp_playlistcontainer_num_playlists(d->handle) : 0;
}

SpPlaylistList SpPlaylistContainer::playlists() const
{
    SpHandle core = SpCore::handle();
    if (!isValid() || core.isNull())
        return SpPlaylistList();

    SpPlaylistList playlists;

    int count = playlistCount();
    for (int i = 0; i < count; ++i)
        playlists << core->factory()->createPlaylist(
                         sp_playlistcontainer_playlist(d->handle, i));

    return playlists;
}

SpPlaylist SpPlaylistContainer::playlistAt(int index) const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createPlaylist(
                        sp_playlistcontainer_playlist(d->handle, index));
    }

    return SpPlaylist();
}
