#include "quickartistsynopsis.h"
#include "../application.h"

namespace sp = Spotinetta;

namespace Sonetta {

QuickArtistSynopsis::QuickArtistSynopsis(QObject *parent)
    :   QObject(parent),
      m_session(Application::session().constCast<const sp::Session>()),
      m_artistWatcher(new sp::ArtistWatcher(m_session.data())),
      m_browseWatcher(new sp::ArtistBrowseWatcher(m_session.data())),
      m_browseType(MinimalBrowse),
      m_albums(new AlbumListModel),
      m_similarArtists(new ArtistListModel),
      m_hits(new TrackListModel(m_session, 0))
{
    connect(this, &QuickArtistSynopsis::artistChanged,
            this, &QuickArtistSynopsis::artistDataChanged);
    connect(this, &QuickArtistSynopsis::artistChanged,
            this, &QuickArtistSynopsis::browseDataChanged);
    connect(m_artistWatcher.data(), &sp::ArtistWatcher::loaded,
            this, &QuickArtistSynopsis::artistDataChanged);
    connect(m_browseWatcher.data(), &sp::ArtistBrowseWatcher::loaded,
            this, &QuickArtistSynopsis::browseDataChanged);
    connect(m_browseWatcher.data(), &sp::ArtistBrowseWatcher::loaded,
            this, &QuickArtistSynopsis::onLoaded);

    connectModel(m_albums.data());
    connectModel(m_similarArtists.data());
    connectModel(m_hits.data());
}

Spotinetta::Artist QuickArtistSynopsis::artist() const
{
    return m_artistWatcher->watched();
}

void QuickArtistSynopsis::setArtist(const Spotinetta::Artist &artist)
{
    if (this->artist() != artist)
    {
        m_artistWatcher->watch(artist);
        m_browseWatcher->watch(m_session->browse(artist, static_cast<sp::ArtistBrowseType>(m_browseType)));
        m_albums->clear();
        m_similarArtists->clear();
        m_hits->clear();
        emit artistChanged();
    }
}

QuickArtistSynopsis::BrowseType QuickArtistSynopsis::browseType() const
{
    return m_browseType;
}

void QuickArtistSynopsis::setBrowseType(QuickArtistSynopsis::BrowseType type)
{
    if (type != m_browseType)
    {
        m_browseType = type;
        m_browseWatcher->watch(m_session->browse(artist(), static_cast<sp::ArtistBrowseType>(m_browseType)));
        m_albums->clear();
        m_similarArtists->clear();
        m_hits->clear();
        emit browseTypeChanged();
    }
}

QObject *QuickArtistSynopsis::similarArtists() const
{
    return m_similarArtists.data();
}

QObject *QuickArtistSynopsis::albums() const
{
    return m_albums.data();
}

QObject *QuickArtistSynopsis::hits() const
{
    return m_hits.data();
}

QString QuickArtistSynopsis::name() const
{
    return artist().name();
}

QString QuickArtistSynopsis::biography() const
{
    return m_browseWatcher->watched().biography();
}

QString QuickArtistSynopsis::smallPortraitUri() const
{
    return sp::Link::fromArtistPortrait(artist(), sp::ImageSize::Small).uri();
}

QString QuickArtistSynopsis::normalPortraitUri() const
{
    return sp::Link::fromArtistPortrait(artist(), sp::ImageSize::Normal).uri();
}

QString QuickArtistSynopsis::largePortraitUri() const
{
    return sp::Link::fromArtistPortrait(artist(), sp::ImageSize::Large).uri();
}

void QuickArtistSynopsis::onLoaded()
{
    sp::ArtistBrowse browse = m_browseWatcher->watched();
    m_albums->append(browse.albums());
    m_similarArtists->append(browse.similarArtists());
    m_hits->append(browse.tophitTracks());
}

template <typename T>
void QuickArtistSynopsis::connectModel(T * model)
{
    connect(m_session.data(), &sp::Session::metadataUpdated, model, &T::updateMetadata);
}



}
