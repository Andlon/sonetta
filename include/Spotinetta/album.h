#pragma once

#include "definitions.h"
#include "basicobject.h"

namespace Spotinetta {

namespace detail {
// BasicLoadableObject requires functions to be available at compile-time, and thus
// libspotify functions can not be used directly as they're merely dynamically linked.
// Use proxy functions like these to work around this restriction
inline sp_error album_add_ref(sp_album * album) { return sp_album_add_ref(album); }
inline sp_error album_release(sp_album * album) { return sp_album_release(album); }
inline bool album_is_loaded(sp_album * album) { return sp_album_is_loaded(album); }
}

typedef BasicLoadableObject<sp_album, &detail::album_add_ref, &detail::album_release, &detail::album_is_loaded> BasicAlbum;

class Album : public BasicAlbum {
public:
    enum class Type {
        Album = SP_ALBUMTYPE_ALBUM,
        Single = SP_ALBUMTYPE_SINGLE,
        Compilation = SP_ALBUMTYPE_COMPILATION,
        Unknown = SP_ALBUMTYPE_UNKNOWN
    };

    Album() { }
    Album(sp_album * handle, bool increment = true) : BasicAlbum(handle, increment) { }

    bool isAvailable() const;
    QString name() const;

    int year() const;
    Type type() const;

    Artist artist() const;
    //Image cover(Image::Size size) const;
};
}

Q_DECLARE_METATYPE(Spotinetta::Album)
