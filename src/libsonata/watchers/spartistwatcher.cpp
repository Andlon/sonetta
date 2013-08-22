#include <Sonata/spartistwatcher.h>

#include "../spcore.h"
#include "../spfactory.h"

using namespace SonataPrivate;

SpArtistWatcher::SpArtistWatcher(QObject *parent)
    :   QObject(parent)
{

}

SpArtist SpArtistWatcher::artist() const
{
    return m_artist;
}

void SpArtistWatcher::setArtist(const SpArtist &artist)
{
    if (artist == m_artist)
        return;

    m_artist = artist;
    emit artistChanged();
    emit dataChanged();
    SpHandle core = SpCore::handle();
    if (!core.isNull())
    {
        disconnect(core->factory(), SIGNAL(artistUpdated(SpArtist)),
                   this, SLOT(handleArtistDataUpdated(SpArtist)));

        if (!m_artist.isLoaded())
        {
            connect(core->factory(), SIGNAL(artistUpdated(SpArtist)),
                    this, SLOT(handleArtistDataUpdated(SpArtist)), Qt::UniqueConnection);
        }
    }
}

QString SpArtistWatcher::name() const
{
    return m_artist.name();
}

bool SpArtistWatcher::isValid() const
{
    return m_artist.isValid();
}

bool SpArtistWatcher::isLoaded() const
{
    return m_artist.isLoaded();
}

void SpArtistWatcher::handleArtistDataUpdated(const SpArtist &artist)
{
    if (artist == m_artist)
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
        {
            disconnect(core->factory(), SIGNAL(trackUpdated(SpTrack)),
                       this, SLOT(handleArtistDataUpdated(SpArtist)));
        }
        emit dataChanged();
    }
}

SpImage SpArtistWatcher::portrait(Spotify::ImageSize size) const
{
    return m_artist.portrait(size);
}

//SpImage SpArtistWatcher::portraitSmall() const
//{
//    return m_artist.portrait(SpImage::ImageSizeSmall);
//}

//SpImage SpArtistWatcher::portraitNormal() const
//{
//    return m_artist.portrait(SpImage::ImageSizeNormal);
//}

//SpImage SpArtistWatcher::portraitLarge() const
//{
//    return m_artist.portrait(SpImage::ImageSizeLarge);
//}


