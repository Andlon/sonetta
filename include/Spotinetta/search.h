#pragma once

#include "basicobject.h"
#include "definitions.h"

namespace Spotinetta {

namespace detail {
// BasicLoadableObject requires functions to be available at compile-time, and thus
// libspotify functions can not be used directly as they're merely dynamically linked.
// Use proxy functions like these to work around this restriction
inline sp_error search_add_ref(sp_search * search) { return sp_search_add_ref(search); }
inline sp_error search_release(sp_search * search) { return sp_search_release(search); }
inline bool search_is_loaded(sp_search * search) { return sp_search_is_loaded(search); }
}

typedef BasicLoadableObject<sp_search, &detail::search_add_ref, &detail::search_release, &detail::search_is_loaded> BasicSearch;

class Search : public BasicSearch {
public:
    Search() { }
    Search(sp_search * search, bool increment = true) : BasicSearch(search, increment) { }

    QString query() const;
    QString didYouMean() const;

    int trackCount() const;
    Track trackAt(int index) const;
    TrackList tracks() const;

    int albumCount() const;
    Album albumAt(int index) const;
    AlbumList albums() const;

    int artistCount() const;
    Artist artistAt(int index) const;
    ArtistList artists() const;

    int playlistCount() const;
//    SpPlaylist playlistAt(int index) const;
//    SpPlaylistList playlists() const;
};
}

Q_DECLARE_METATYPE(Spotinetta::Search)
