#include <Sonata/sptrackwatcher.h>
#include <Sonata/spartist.h>

#include "../spcore.h"
#include "../spfactory.h"

using namespace SonataPrivate;

SpTrackWatcher::SpTrackWatcher(QObject *parent)
    :   QObject(parent)
{

}

SpTrack SpTrackWatcher::track() const
{
    return m_track;
}

void SpTrackWatcher::setTrack(const SpTrack &track)
{
    if (track == m_track)
        return;

    m_track = track;
    emit trackChanged();
    emit dataChanged();
    SpHandle core = SpCore::handle();
    if (!core.isNull())
    {
        disconnect(core->factory(), SIGNAL(trackUpdated(SpTrack)),
                   this, SLOT(handleTrackDataUpdated(SpTrack)));

        if (!m_track.isLoaded())
        {
            connect(core->factory(), SIGNAL(trackUpdated(SpTrack)),
                    this, SLOT(handleTrackDataUpdated(SpTrack)));
        }
    }
}

SpArtistList SpTrackWatcher::artists() const
{
    return m_track.artists();
}

SpArtist SpTrackWatcher::artistAt(int index) const
{
    return m_track.artistAt(index);
}

QString SpTrackWatcher::name() const
{
    return m_track.name();
}

int SpTrackWatcher::duration() const
{
    return m_track.duration();
}

int SpTrackWatcher::popularity() const
{
    return m_track.popularity();
}

int SpTrackWatcher::artistCount() const
{
    return m_track.artistCount();
}

void SpTrackWatcher::handleTrackDataUpdated(const SpTrack &track)
{
    if (track == m_track)
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
        {
            disconnect(core->factory(), SIGNAL(trackUpdated(SpTrack)),
                       this, SLOT(handleTrackDataUpdated(SpTrack)));
        }
        emit dataChanged();
    }
}

bool SpTrackWatcher::isValid() const
{
    return m_track.isValid();
}

bool SpTrackWatcher::isLoaded() const
{
    return m_track.isLoaded();
}

bool SpTrackWatcher::isAutoLinked() const
{
    return m_track.isAutoLinked();
}


