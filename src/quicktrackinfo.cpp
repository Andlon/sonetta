#include "quicktrackinfo.h"
#include <Sonata/sptrackwatcher.h>
#include <Sonata/spartistwatcher.h>

#include <Sonata/SpAlbum>
#include <Sonata/SpLink>

#include <QVector>

#include <QDebug>

class QuickTrackInfo::QuickTrackInfoPrivate
{
public:
    SpTrackWatcher * trackWatcher;
    QVector<SpArtistWatcher *> artistWatchers;
};

QuickTrackInfo::QuickTrackInfo(QObject *parent)
    :   QObject(parent), d(new QuickTrackInfoPrivate)
{
    d->trackWatcher = new SpTrackWatcher(this);
    connect(d->trackWatcher, SIGNAL(trackChanged()),
            this, SLOT(handleTrackChanged()));
    connect(d->trackWatcher, SIGNAL(dataChanged()),
            this, SLOT(handleTrackUpdated()));

}

QuickTrackInfo::~QuickTrackInfo()
{
    delete d;
}

bool QuickTrackInfo::isValid() const
{
    return d->trackWatcher->isValid();
}

bool QuickTrackInfo::isLoaded() const
{
    return d->trackWatcher->isLoaded();
}

bool QuickTrackInfo::isAutoLinked() const
{
    return d->trackWatcher->isAutoLinked();
}

int QuickTrackInfo::duration() const
{
    return d->trackWatcher->duration();
}

int QuickTrackInfo::popularity() const
{
    return d->trackWatcher->popularity();
}

int QuickTrackInfo::artistCount() const
{
    return d->trackWatcher->artistCount();
}

SpTrack QuickTrackInfo::track() const
{
    return d->trackWatcher->track();
}

void QuickTrackInfo::setTrack(const SpTrack &track)
{
    d->trackWatcher->setTrack(track);
}

QString QuickTrackInfo::name() const
{
    return d->trackWatcher->name();
}

QStringList QuickTrackInfo::artistNames() const
{
    QStringList names;
    foreach(SpArtistWatcher * watcher, d->artistWatchers)
        names << watcher->name();

    return names;
}

SpArtist QuickTrackInfo::artistAt(int index) const
{
    if (index >= 0 && index < artistCount())
        return d->artistWatchers.at(index)->artist();

    return SpArtist();
}

SpArtistList QuickTrackInfo::artists() const
{
    SpArtistList list;
    foreach(SpArtistWatcher * watcher, d->artistWatchers)
        list << watcher->artist();

    return list;
}

QString QuickTrackInfo::albumCoverUri(int size) const
{
    return SpLink::fromAlbumCover(d->trackWatcher->track().album(), static_cast<Spotify::ImageSize>(size)).uri();
}

void QuickTrackInfo::handleTrackChanged()
{
    if (d->trackWatcher->isLoaded() || !d->trackWatcher->isValid())
        configureWatchers();

    emit trackChanged();
    emit artistsChanged();
}

void QuickTrackInfo::handleTrackUpdated()
{
    if (d->trackWatcher->isLoaded())
        configureWatchers();

    emit dataUpdated();
}

void QuickTrackInfo::configureWatchers()
{
    // Reuse watchers if possible

    int newCount = d->trackWatcher->artistCount();
    int oldCount = d->artistWatchers.count();
    int limit = qMin<int>(newCount, oldCount);

    for (int i = limit; i < oldCount; ++i)
        d->artistWatchers[i]->deleteLater();

    d->artistWatchers.resize(newCount);

    for (int i = limit; i < newCount; ++i)
    {
        SpArtistWatcher * watcher = new SpArtistWatcher(this);
        d->artistWatchers[i] = watcher;
        connect(watcher, SIGNAL(artistChanged()), this, SIGNAL(artistsChanged()));
        connect(watcher, SIGNAL(dataChanged()), this, SIGNAL(artistsChanged()));
    }

    // Set artists after creation to avoid slot being called while inserting artists
    for (int i = 0; i < newCount; ++i)
        d->artistWatchers[i]->setArtist(d->trackWatcher->artistAt(i));
}
