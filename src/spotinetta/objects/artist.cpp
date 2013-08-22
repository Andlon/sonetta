#include <Spotinetta/artist.h>

namespace Spotinetta {

QString Artist::name() const
{
    return isValid() ? QString::fromUtf8(sp_artist_name(handle())) : QString();
}

}
