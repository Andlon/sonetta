#pragma once

#include "definitions.h"
#include "basicobject.h"

namespace Spotinetta {

namespace detail {
// BasicLoadableObject requires functions to be available at compile-time, and thus
// libspotify functions can not be used directly as they're merely dynamically linked.
// Use proxy functions like these to work around this restriction
inline sp_error artist_add_ref(sp_artist * artist) { return sp_artist_add_ref(artist); }
inline sp_error artist_release(sp_artist * artist) { return sp_artist_release(artist); }
inline bool artist_is_loaded(sp_artist * artist) { return sp_artist_is_loaded(artist); }
}

typedef BasicLoadableObject<sp_artist, &detail::artist_add_ref, &detail::artist_release, &detail::artist_is_loaded> BasicArtist;

class Artist : public BasicArtist {
public:
    Artist() { }
    Artist(sp_artist * handle, bool increment = true) : BasicArtist(handle, increment) { }

    QString name() const;
    //Image portrait()
};
}

Q_DECLARE_METATYPE(Spotinetta::Artist)
