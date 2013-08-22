#include <Sonata/sptrack.h>
#include <Sonata/splink.h>
#include <Sonata/spimage.h>

#include "../spfactory.h"
#include "../spcore.h"

#include <QDebug>

using namespace SonataPrivate;

SpLink::SpLink()
    :   d(0)
{
}

SpLink::SpLink(SpLinkPrivate * p)
    :   d(p)
{
    if (isValid())
        sp_link_add_ref(d->handle);
}

SpLink::SpLink(const SpImage &image)
    :   d(0)
{
    if (image.isValid())
    {
        d = new SpLinkPrivate();
        d->handle = sp_link_create_from_image(image.d->handle);
        d->session = image.d->session;
    }
}

SpLink::SpLink(const QString &uri)
    :   d(0)
{
    SpHandle core = SpCore::handle();
    if (!core.isNull() && core->sessionIsValid())
    {
        sp_link * handle = sp_link_create_from_string(uri.toUtf8());

        if (handle != NULL)
        {
            d = new SpLinkPrivate();
            d->handle = handle;
            d->session = core->session();
        }

    }
}

SpLink::SpLink(const SpArtist &artist)
    :   d(0)
{
    if (artist.isValid())
    {
        d = new SpLinkPrivate();
        d->handle = sp_link_create_from_artist(artist.d->handle);
        d->session = artist.d->session;
    }
}

SpLink::SpLink(const SpTrack &track)
    :   d(0)
{
    if (track.isValid())
    {
        d = new SpLinkPrivate();
        d->handle = sp_link_create_from_track(track.d->handle, 0);
        d->session = track.d->session;
    }
}


SpLink::SpLink(const SpAlbum &album)
    :   d(0)
{
    if (album.isValid())
    {
        d = new SpLinkPrivate();
        d->handle = sp_link_create_from_album(album.d->handle);
        d->session = album.d->session;
    }
}

SpLink::SpLink(const SpPlaylist &playlist)
    :   d(0)
{
    if (playlist.isValid())
    {
        d = new SpLinkPrivate();
        d->handle = sp_link_create_from_playlist(playlist.d->handle);
        d->session = playlist.d->session;
    }
}


SpLink& SpLink::operator =(const SpLink &other)
{
    if (isValid())
        sp_link_release(d->handle);

    if (other.isValid())
        sp_link_release(d->handle);

    if (d != other.d)
    {
        delete d;
        d = other.d ? new SpLinkPrivate(*other.d) : 0;
    }

    return *this;
}

SpLink::~SpLink()
{
    if (isValid())
        sp_link_release(d->handle);

    delete d;
}

SpLink::SpLink(const SpLink &other)
{
    d = other.d ? new SpLinkPrivate(*other.d) : 0;
}

bool SpLink::isValid() const
{
    return d != 0 && !d->session.isNull();
}

Spotify::LinkType SpLink::type() const
{
    return isValid() ? (Spotify::LinkType) sp_link_type(d->handle) : Spotify::InvalidLink;
}

QString SpLink::uri() const
{
    if (isValid())
    {
        char buffer[100];
        int count = sp_link_as_string(d->handle, &buffer[0], 100);

        QString result = QString::fromUtf8(buffer, count);
        return result;
    }

    return QString();
}

SpTrack SpLink::track() const
{
    if (isValid())
    {
        sp_track * trackHandle = sp_link_as_track(d->handle);

        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createTrack(trackHandle);
    }

    return SpTrack();
}

SpImage SpLink::image() const
{
    if (isValid())
    {
        sp_image * imageHandle = sp_image_create_from_link(d->session.data(), d->handle);

        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createImage(imageHandle);
    }

    return SpImage();
}

SpArtist SpLink::artist() const
{
    if (isValid())
    {
        sp_artist * artistHandle = sp_link_as_artist(d->handle);

        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createArtist(artistHandle);
    }

    return SpArtist();
}

SpAlbum SpLink::album() const
{
    if (isValid())
    {
        sp_album * albumHandle = sp_link_as_album(d->handle);

        SpHandle core = SpCore::handle();
        if (!core.isNull())
            return core->factory()->createAlbum(albumHandle);
    }

    return SpAlbum();
}

//SpPlaylist SpLink::playlist() const
//{
//    if (isValid())
//    {
//        sp_playlist * playlistHandle = sp_playlist_
//    }
//}

bool operator ==(const SpLink &link1, const SpLink &link2)
{
    if (!link1.isValid() || !link2.isValid())
        return false;

    return link1.d->handle == link2.d->handle;
}

SpLink SpLink::fromAlbumCover(const SpAlbum &album, Spotify::ImageSize size)
{
    if (album.isValid())
    {
        sp_image_size spSize;

        switch(size)
        {
        case Spotify::ImageSizeSmall:
            spSize = SP_IMAGE_SIZE_SMALL;
            break;
        case Spotify::ImageSizeNormal:
            spSize = SP_IMAGE_SIZE_NORMAL;
            break;
        case Spotify::ImageSizeLarge:
            spSize = SP_IMAGE_SIZE_LARGE;
            break;
        }

        sp_link * handle = sp_link_create_from_album_cover(album.d->handle, spSize);

        if (handle)
        {
            // Note: This WILL cause wrong refcount (+1). Fix when redoing
            // object creation system
            SpLinkPrivate * p = new SpLinkPrivate();
            p->handle = handle;
            p->session = album.d->session;

            return SpLink(p);
        }
    }

    return SpLink();
}

SpLink SpLink::fromArtistPortrait(const SpArtist &artist, Spotify::ImageSize size)
{
    if (artist.isValid())
    {
        // Replace this switch with a cast once enums are synced

        sp_image_size spSize = SP_IMAGE_SIZE_NORMAL;

        switch (size)
        {
        case Spotify::ImageSizeSmall:
            spSize = SP_IMAGE_SIZE_SMALL;
            break;
        case Spotify::ImageSizeLarge:
            spSize = SP_IMAGE_SIZE_LARGE;
            break;
        default:
            break;
        }

        sp_link * handle = sp_link_create_from_artist_portrait(artist.d->handle, spSize);

        if (handle)
        {
            // Note: This WILL cause wrong refcount (+1). Fix when redoing
            // object creation system
            SpLinkPrivate * p = new SpLinkPrivate();
            p->handle = handle;
            p->session = artist.d->session;

            return SpLink(p);
        }
    }

    return SpLink();
}
