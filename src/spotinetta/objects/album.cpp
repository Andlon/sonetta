#include <Spotinetta/album.h>
#include <Spotinetta/artist.h>

namespace Spotinetta {

bool Album::isAvailable() const
{
    return isValid() ? sp_album_is_available(handle()) : false;
}

QString Album::name() const
{
    return isValid() ? QString::fromUtf8(sp_album_name(handle())) : QString();
}

int Album::year() const
{
    return isValid() ? sp_album_year(handle()) : 0;
}

Album::Type Album::type() const
{
    return isValid() ? static_cast<Album::Type>(sp_album_type(handle())) : Album::Type::Unknown;
}

Artist Album::artist() const
{
    return isValid() ? Artist(sp_album_artist(handle())) : Artist();
}

}
