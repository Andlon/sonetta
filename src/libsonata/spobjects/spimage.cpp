#include <Sonata/spimage.h>

#include "../spfactory.h"
#include "../spcore.h"

#include <QImage>

using namespace SonataPrivate;

SpImage::SpImage()
    :   d(0)
{
}

SpImage::SpImage(SpImagePrivate *priv)
    :   d(priv)
{
    // Note that we don't need to add a reference here, because
    // the sp_image_create function used by SpFactory preincrements the counter
}

SpImage::SpImage(const SpImage &other)
{
    if (other.isValid())
    {
        d = new SpImagePrivate(*other.d);
        sp_image_add_ref(d->handle);
    }
    else
        d = 0;
}

SpImage::~SpImage()
{
    if (isValid())
        sp_image_release(d->handle);

    delete d;
}

SpImage& SpImage::operator=(const SpImage &other)
{
    if (other.isValid())
        sp_image_add_ref(other.d->handle);

    if (isValid())
        sp_image_release(d->handle);

    if (d != other.d)
    {
        delete d;
        d = other.d ? new SpImagePrivate(*other.d) : 0;
    }

    return *this;
}

bool SpImage::isValid() const
{
    return d != 0 && !d->session.isNull();
}

bool SpImage::isLoaded() const
{
    return isValid() ? sp_image_is_loaded(d->handle) : false;
}

bool operator==(const SpImage &image1, const SpImage &image2)
{
    if (!image1.isValid() || !image2.isValid())
        return false;

    return image1.d->handle == image2.d->handle;
}

QImage SpImage::image() const
{
    if (isLoaded())
    {
        size_t size;
        const uchar * imageData = static_cast<const uchar *>(sp_image_data(d->handle, &size));
        return QImage::fromData(imageData, size, "jpg");
    }

    return QImage();
}

QByteArray SpImage::imageData() const
{
    if (isLoaded())
    {
        size_t size;
        const char * imageData = static_cast<const char *>(sp_image_data(d->handle, &size));
        return QByteArray(imageData, size);
    }

    return QByteArray();
}
