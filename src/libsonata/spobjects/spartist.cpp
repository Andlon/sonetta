#include <Sonata/spartist.h>

#include "../spfactory.h"
#include "../spcore.h"

using namespace SonataPrivate;

SpArtist::SpArtist()
    :   d(0)
{
}

SpArtist::SpArtist(SpArtistPrivate *priv)
    :   d(priv)
{
    if (isValid())
        sp_artist_add_ref(d->handle);
}

SpArtist::SpArtist(const SpArtist &other)
{
    if (other.isValid())
    {
        d = new SpArtistPrivate(*other.d);
        sp_artist_add_ref(d->handle);
    }
    else
        d = 0;
}

SpArtist::~SpArtist()
{
    if (isValid())
        sp_artist_release(d->handle);

    delete d;
}

SpArtist& SpArtist::operator=(const SpArtist &other)
{
    if (other.isValid())
        sp_artist_add_ref(other.d->handle);

    if (isValid())
        sp_artist_release(d->handle);

    if (d != other.d)
    {
        delete d;
        d = other.d ? new SpArtistPrivate(*other.d) : 0;
    }

    return *this;
}

bool SpArtist::isValid() const
{
    return d != 0 && !d->session.isNull();
}

bool SpArtist::isLoaded() const
{
    return isValid() ? sp_artist_is_loaded(d->handle) : false;
}

QString SpArtist::name() const
{
    return isValid() ? QString::fromUtf8(sp_artist_name(d->handle)) : QString();
}

bool operator ==(const SpArtist &a1, const SpArtist &a2)
{
    if (!a1.isValid() || !a2.isValid())
        return false;

    return a1.d->handle == a2.d->handle;
}

SpImage SpArtist::portrait(Spotify::ImageSize size) const
{
    SpHandle core = SpCore::handle();
    if (isValid() && !core.isNull())
    {
        sp_image_size spsize;
        switch (size)
        {
        case Spotify::ImageSizeSmall:
            spsize = SP_IMAGE_SIZE_SMALL;
            break;
        case Spotify::ImageSizeNormal:
            spsize = SP_IMAGE_SIZE_NORMAL;
            break;
        case Spotify::ImageSizeLarge:
            spsize = SP_IMAGE_SIZE_LARGE;
            break;
        }

        return core->factory()->createImageFromId(sp_artist_portrait(
                                                      d->handle, spsize));
    }

    return SpImage();
}
