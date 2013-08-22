#include <Sonata/spalbum.h>

#include "../spfactory.h"
#include "../spcore.h"

using namespace SonataPrivate;

SpAlbum::SpAlbum()
    :   d(0)
{

}

SpAlbum::SpAlbum(SpAlbumPrivate *priv)
    :   d(priv)
{
    if (isValid())
        sp_album_add_ref(d->handle);
}

SpAlbum::SpAlbum(const SpAlbum &other)
{
    if (other.isValid())
    {
        d = new SpAlbumPrivate(*other.d);
        sp_album_add_ref(d->handle);
    }
    else
        d = 0;
}

SpAlbum::~SpAlbum()
{
    if (isValid())
        sp_album_release(d->handle);

    delete d;
}

SpAlbum& SpAlbum::operator =(const SpAlbum &other)
{
    if (isValid())
        sp_album_release(d->handle);

    if (other.isValid())
        sp_album_add_ref(other.d->handle);

    if (d != other.d)
        delete d;
    d = other.d ? new SpAlbumPrivate(*other.d) : 0;

    return *this;
}

bool SpAlbum::isValid() const
{
    return d != 0 && !d->session.isNull();
}

bool SpAlbum::isLoaded() const
{
    return isValid() ? sp_album_is_loaded(d->handle) : false;
}

bool SpAlbum::isAvailable() const
{
    return isValid() ? sp_album_is_available(d->handle) : false;
}

QString SpAlbum::name() const
{
    return isValid() ? QString::fromUtf8(sp_album_name(d->handle)) : QString();
}

int SpAlbum::year() const
{
    return isValid() ? sp_album_year(d->handle) : 0;
}

SpArtist SpAlbum::artist() const
{
    if (isValid())
    {
        SpHandle core = SpCore::handle();
        if (!core.isNull())
        {
             sp_artist * artist = sp_album_artist(d->handle);
             return core->factory()->createArtist(artist);
        }
    }

    return SpArtist();
}

Spotify::AlbumType SpAlbum::type() const
{
    if (isValid())
    {
        sp_albumtype t = sp_album_type(d->handle);

        switch (t)
        {
        case SP_ALBUMTYPE_ALBUM:
            return Spotify::AlbumAlbumType;
            break;
        case SP_ALBUMTYPE_SINGLE:
            return Spotify::SingleAlbumType;
            break;
        case SP_ALBUMTYPE_COMPILATION:
            return Spotify::CompilationAlbumType;
            break;
        case SP_ALBUMTYPE_UNKNOWN:
            return Spotify::UnknownAlbumType;
            break;

        default:
            return Spotify::UnknownAlbumType;
        }
    }

    return Spotify::UnknownAlbumType;
}

bool operator ==(const SpAlbum &a1, const SpAlbum &a2)
{
    if (!a1.isValid() || !a2.isValid())
        return false;

    return a1.d->handle == a2.d->handle;
}
