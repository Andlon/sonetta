#pragma once

#include "basicobject.h"
#include "definitions.h"

namespace Spotinetta {

namespace detail {
// BasicLoadableObject requires functions to be available at compile-time, and thus
// libspotify functions can not be used directly as they're merely dynamically linked.
// Use proxy functions like these to work around this restriction
inline sp_error track_add_ref(sp_track * track) { return sp_track_add_ref(track); }
inline sp_error track_release(sp_track * track) { return sp_track_release(track); }
inline bool track_is_loaded(sp_track * track) { return sp_track_is_loaded(track); }
}

typedef BasicLoadableObject<sp_track, &detail::track_add_ref, &detail::track_release, &detail::track_is_loaded> BasicTrack;

class Track : public BasicTrack {
public:
    Track() { }
    Track(sp_track * track, bool increment = true) : BasicTrack(track, increment) { }

    bool isAutoLinked(const Session &session) const;
    bool isAvailable(const Session &session) const;
    Track playableTrack(const Session &session) const;

    QString name() const;

    int duration() const;
    int popularity() const;

    Artist artistAt(int index) const;
    ArtistList artists() const;
    int artistCount() const;

    Album album() const;
};

}

Q_DECLARE_METATYPE(Spotinetta::Track)
