#include "quicktrackinfo.h"

#include <QVector>
#include <QDebug>

#include "application.h"

namespace sp = Spotinetta;

namespace Sonetta {

QuickTrackInfo::QuickTrackInfo(QObject *parent)
    :   QObject(parent)
{
    Application * application = Application::instance();
    Q_ASSERT(application != nullptr);
    sp::Session * session = application->session();
    Q_ASSERT(session != nullptr);
    m_session = session;

    m_trackWatcher = new sp::TrackWatcher(session, this);

    connect(m_trackWatcher, &sp::TrackWatcher::loaded,
            this, &QuickTrackInfo::onTrackLoaded);

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

//QString QuickTrackInfo::albumCoverUri(int size) const
//{
//    return sp::Link::fromAlbumCover(m_trackWatcher->track().album(), static_cast<sp::ImageSize>(size)).uri();
//}

void QuickTrackInfo::onTrackLoaded()
{
    if (isLoaded())
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
    int count = artistCount();

    deleteWatchers();

    Q_ASSERT(!m_session.isNull());

    m_artistWatchers.resize(count);

    for (int i = 0; i < count; ++i)
    {
        sp::Artist artist = track().artistAt(i);
        auto watcher = new sp::ArtistWatcher(m_session, this);
        watcher->watch(artist);
        connect(watcher, &sp::ArtistWatcher::loaded, this, &QuickTrackInfo::artistsChanged);
        m_artistWatchers[i] = watcher;
    }

        emit artistsChanged();
}

}
