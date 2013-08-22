#include <Spotinetta/image.h>

namespace Spotinetta {

QByteArray Image::data() const
{
    if (isValid())
    {
        size_t size;
        const char * imageData = static_cast<const char *>(sp_image_data(handle(), &size));
        return QByteArray(imageData, size);
    }

    return QByteArray();
}

Image::Format Image::format() const
{
    return isValid() ? static_cast<Image::Format>(sp_image_format(handle())) : Format::Unknown;
}

}
