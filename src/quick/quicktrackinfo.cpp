#include "quicktrackinfo.h"

#include <QVector>
#include <QDebug>

#include "../utilities/pointers.h"
#include "../application.h"

namespace sp = Spotinetta;

namespace Sonetta {

QuickTrackInfo::QuickTrackInfo(QObject *parent)
    :   QObject(parent), m_session(Application::session()),
      m_trackWatcher(new sp::TrackWatcher(m_session.data(), this)),
      m_albumWatcher(new sp::AlbumWatcher(m_session.data(), this))
{
    Q_ASSERT(!m_session.isNull());

    connect(m_trackWatcher.data(), &sp::TrackWatcher::loaded,
            this, &QuickTrackInfo::onTrackLoaded);
    connect(m_albumWatcher.data(), &sp::AlbumWatcher::loaded,
            this, &QuickTrackInfo::dataUpdated);
    connect(this, &QuickTrackInfo::trackChanged,
            this, &QuickTrackInfo::dataUpdated);
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
    foreach(auto watcher, m_artistWatchers)
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
    foreach(auto watcher, m_artistWatchers)
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

void QuickTrackInfo::setupWatchers()
{
    sp::Track track = this->track();
    m_albumWatcher->watch(track.album());
    int count = artistCount();

    m_artistWatchers.clear();

    for (int i = 0; i < count; ++i)
    {
        sp::Artist artist = track.artistAt(i);
        ObjectSharedPointer<sp::ArtistWatcher> watcher(new sp::ArtistWatcher(m_session.data(), this));
        watcher->watch(artist);
        connect(watcher.data(), &sp::ArtistWatcher::loaded, this, &QuickTrackInfo::artistsChanged);
        m_artistWatchers.append(watcher);
    }

    emit artistsChanged();

    if (m_albumWatcher->watched().isLoaded())
        emit dataUpdated();
}

}
