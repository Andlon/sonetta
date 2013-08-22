#include <Sonata/spartistbrowsewatcher.h>

#include "../spcore.h"

SpArtistBrowseWatcher::SpArtistBrowseWatcher(QObject *parent)
    :   QObject(parent)
{
    connect(this, SIGNAL(artistBrowseChanged()),
            this, SIGNAL(dataChanged()));
    connect(this, SIGNAL(loaded()),
            this, SIGNAL(dataChanged()));
}

SpArtistBrowseWatcher::~SpArtistBrowseWatcher()
{

}

bool SpArtistBrowseWatcher::isValid() const
{
    return m_browse.isValid();
}

bool SpArtistBrowseWatcher::isLoaded() const
{
    return m_browse.isLoaded();
}

SpArtistBrowse SpArtistBrowseWatcher::artistBrowse() const
{
    return m_browse;
}

void SpArtistBrowseWatcher::setArtistBrowse(const SpArtistBrowse &browse)
{
    m_browse = browse;
    emit artistBrowseChanged();

    if (browse.isLoaded())
        emit loaded();
    else if (browse.isValid())
    {
        using namespace SonataPrivate;
        SpHandle core = SpCore::handle();

        if (!core.isNull())
        {
            connect(core->factory(), SIGNAL(artistBrowseCompleted(SpArtistBrowse)),
                    this, SLOT(handleArtistBrowseUpdated(SpArtistBrowse)), Qt::UniqueConnection);
        }
    }
}

SpArtist SpArtistBrowseWatcher::artist() const
{
    return m_browse.artist();
}

QString SpArtistBrowseWatcher::biography() const
{
    return m_browse.biography();
}

int SpArtistBrowseWatcher::portraitCount() const
{
    return m_browse.portraitCount();
}

SpImage SpArtistBrowseWatcher::portraitAt(int index) const
{
    return m_browse.portraitAt(index);
}

SpImageList SpArtistBrowseWatcher::portraits() const
{
    return m_browse.portraits();
}

int SpArtistBrowseWatcher::trackCount() const
{
    return m_browse.trackCount();
}

SpTrack SpArtistBrowseWatcher::trackAt(int index) const
{
    return m_browse.trackAt(index);
}

SpTrackList SpArtistBrowseWatcher::tracks() const
{
    return m_browse.tracks();
}

int SpArtistBrowseWatcher::albumCount() const
{
    return m_browse.albumCount();
}

SpAlbum SpArtistBrowseWatcher::albumAt(int index) const
{
    return m_browse.albumAt(index);
}

SpAlbumList SpArtistBrowseWatcher::albums() const
{
    return m_browse.albums();
}

int SpArtistBrowseWatcher::similarArtistCount() const
{
    return m_browse.similarArtistCount();
}

SpArtist SpArtistBrowseWatcher::similarArtistAt(int index) const
{
    return m_browse.similarArtistAt(index);
}

SpArtistList SpArtistBrowseWatcher::similarArtists() const
{
    return m_browse.similarArtists();
}

int SpArtistBrowseWatcher::topTrackCount() const
{
    return m_browse.topTrackCount();
}

SpTrack SpArtistBrowseWatcher::topTrackAt(int index) const
{
    return m_browse.topTrackAt(index);
}

SpTrackList SpArtistBrowseWatcher::topTracks() const
{
    return m_browse.topTracks();
}

void SpArtistBrowseWatcher::handleArtistBrowseUpdated(const SpArtistBrowse &browse)
{
    if (browse == m_browse)
    {
        using namespace SonataPrivate;
        SpHandle core = SpCore::handle();

        Q_ASSERT(!core.isNull()); // Should be safe, as signal originates from factory
        disconnect(core->factory(), SIGNAL(artistBrowseCompleted(SpArtistBrowse)),
                   this, SLOT(handleArtistBrowseUpdated(SpArtistBrowse)));

        emit loaded();
    }
}
