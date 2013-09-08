#include "quicktrackinfo.h"

#include <QVector>
#include <QDebug>

#include "../utilities/session.h"

namespace sp = Spotinetta;

namespace Sonetta {

QuickTrackInfo::QuickTrackInfo(QObject *parent)
    :   QObject(parent)
{
    sp::Session * session = getCurrentSession();
    Q_ASSERT(session != nullptr);
    m_session = session;

    m_trackWatcher = new sp::TrackWatcher(session, this);
    m_albumWatcher = new sp::AlbumWatcher(session, this);

    connect(m_trackWatcher, &sp::TrackWatcher::loaded,
            this, &QuickTrackInfo::onTrackLoaded);
    connect(m_albumWatcher, &sp::AlbumWatcher::loaded,
            this, &QuickTrackInfo::dataUpdated);
    connect(this, &QuickTrackInfo::trackChanged,
            this, &QuickTrackInfo::dataUpdated);
}

QuickTrackInfo::~QuickTrackInfo()
{
}

bool QuickTrackInfo::isValid() const
{
    return track().isValid();
}

bool QuickTrackInfo::isLoaded() const
{
    return track().isLoaded();
}

bool QuickTrackInfo::isAutoLinked() const
{
    return track().isLoaded();
}

int QuickTrackInfo::duration() const
{
    return track().duration();
}

int QuickTrackInfo::popularity() const
{
    return track().popularity();
}

int QuickTrackInfo::artistCount() const
{
    return track().artistCount();
}

sp::Track QuickTrackInfo::track() const
{
    return m_trackWatcher->watched();
}

void QuickTrackInfo::setTrack(const sp::Track &track)
{
    if (track != this->track())
    {
        m_trackWatcher->watch(track);

        emit trackChanged();

        if (track.isLoaded())
        {
            onTrackLoaded();
        }
    }
}

QString QuickTrackInfo::name() const
{
    return track().name();
}

QStringList QuickTrackInfo::artistNames() const
{
    QStringList names;
    foreach(sp::ArtistWatcher * watcher, m_artistWatchers)
        names << watcher->watched().name();

    return names;
}

sp::Artist QuickTrackInfo::artistAt(int index) const
{
    if (index >= 0 && index < artistCount())
        return m_artistWatchers.at(index)->watched();

    return sp::Artist();
}

sp::ArtistList QuickTrackInfo::artists() const
{
    sp::ArtistList list;
    foreach(sp::ArtistWatcher * watcher, m_artistWatchers)
        list << watcher->watched();

    return list;
}

QString QuickTrackInfo::smallCoverUri() const
{
    if (!m_session.isNull())
    {
        return sp::Link::fromAlbumCover(track().album(), sp::ImageSize::Small).uri();
    }

    return QString();
}

QString QuickTrackInfo::normalCoverUri() const
{
    if (!m_session.isNull())
    {
        return sp::Link::fromAlbumCover(track().album(), sp::ImageSize::Normal).uri();
    }

    return QString();
}

QString QuickTrackInfo::largeCoverUri() const
{
    if (!m_session.isNull())
    {
        return sp::Link::fromAlbumCover(track().album(), sp::ImageSize::Large).uri();
    }

    return QString();
}

void QuickTrackInfo::onTrackLoaded()
{
    setupWatchers();

    emit dataUpdated();
}

void QuickTrackInfo::deleteWatchers()
{
    for (auto & watcher : m_artistWatchers)
        watcher->deleteLater();

    m_artistWatchers.clear();
}

void QuickTrackInfo::setupWatchers()
{
    sp::Track track = this->track();
    m_albumWatcher->watch(track.album());
    int count = artistCount();

    deleteWatchers();

    Q_ASSERT(!m_session.isNull());

    m_artistWatchers.resize(count);

    for (int i = 0; i < count; ++i)
    {
        sp::Artist artist = track.artistAt(i);
        auto watcher = new sp::ArtistWatcher(m_session, this);
        watcher->watch(artist);
        connect(watcher, &sp::ArtistWatcher::loaded, this, &QuickTrackInfo::artistsChanged);
        m_artistWatchers[i] = watcher;
    }

    emit artistsChanged();

    if (m_albumWatcher->watched().isLoaded())
        emit dataUpdated();
}

}
