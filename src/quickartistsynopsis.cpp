#include "quickartistsynopsis.h"

#include <Sonata/SpLink>

#include <QDebug>

QuickArtistSynopsis::QuickArtistSynopsis(QObject *parent) :
    QObject(parent)
{
    m_browseWatcher = new SpArtistBrowseWatcher(this);
    m_artistWatcher = new SpArtistWatcher(this);

    connect(m_browseWatcher, SIGNAL(artistBrowseChanged()),
            this, SIGNAL(artistBrowseChanged()));
    connect(m_browseWatcher, SIGNAL(dataChanged()),
            this, SIGNAL(browseDataChanged()));
    connect(m_artistWatcher, SIGNAL(dataChanged()),
            this, SIGNAL(artistDataChanged()));
    connect(m_artistWatcher, SIGNAL(artistChanged()),
            this, SIGNAL(artistDataChanged()));
}

SpArtistBrowse QuickArtistSynopsis::artistBrowse() const
{
    return m_browseWatcher->artistBrowse();
}

void QuickArtistSynopsis::setArtistBrowse(const SpArtistBrowse &browse)
{
    m_browseWatcher->setArtistBrowse(browse);
    m_artistWatcher->setArtist(browse.artist());
}

bool QuickArtistSynopsis::isValid() const
{
    return m_browseWatcher->isValid();
}

bool QuickArtistSynopsis::isLoaded() const
{
    return m_browseWatcher->isLoaded();
}

QString QuickArtistSynopsis::name() const
{
//    return m_artistWatcher->name();
    return m_browseWatcher->artist().name();
}

QString QuickArtistSynopsis::biography() const
{
    return m_browseWatcher->biography();
}

QString QuickArtistSynopsis::portrait(/*Spotify::ImageSize*/ int size) const
{
//    return SpLink(m_artistWatcher->portrait((Spotify::ImageSize) size)).uri();
    return SpLink(m_browseWatcher->artist().portrait((Spotify::ImageSize) size)).uri();
}

QStringList QuickArtistSynopsis::portraitLinks() const
{
    QStringList links;

    int count = m_browseWatcher->portraitCount();
    for (int i = 0; i < count; ++i)
        links << SpLink(m_browseWatcher->portraitAt(i)).uri();

    return links;
}

SpAlbumList QuickArtistSynopsis::albums() const
{
    return m_browseWatcher->albums();
}

SpArtistList QuickArtistSynopsis::similarArtists() const
{
    return m_browseWatcher->similarArtists();
}

SpTrackList QuickArtistSynopsis::topTracks() const
{
    return m_browseWatcher->topTracks();
}
