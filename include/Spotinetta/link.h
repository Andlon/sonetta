#pragma once

#include "basicobject.h"
#include "definitions.h"

namespace Spotinetta {

namespace detail {
// BasiceObject requires functions to be available at compile-time, and thus
// libspotify functions can not be used directly as they're merely dynamically linked.
// Use proxy functions like these to work around this restriction
inline sp_error link_add_ref(sp_link * link) { return sp_link_add_ref(link); }
inline sp_error link_release(sp_link * link) { return sp_link_release(link); }
}

typedef BasicObject<sp_link, &detail::link_add_ref, &detail::link_release> BasicLink;

class Link : public BasicLink {
public:
    enum class Type {
        Invalid = SP_LINKTYPE_INVALID,
        Track = SP_LINKTYPE_TRACK,
        Album = SP_LINKTYPE_ALBUM,
        Search = SP_LINKTYPE_SEARCH,
        Playlist = SP_LINKTYPE_PLAYLIST,
        Profile = SP_LINKTYPE_PROFILE,
        Starred = SP_LINKTYPE_STARRED,
        LocalTrack = SP_LINKTYPE_LOCALTRACK,
        Image = SP_LINKTYPE_IMAGE
    };

    Link() { }
    Link(sp_link * link, bool increment = true) : BasicLink(link, increment) { }

    explicit Link(const QString &uri);
    explicit Link(const Image &image);
    explicit Link(const Artist &artist);
    explicit Link(const Track &track, int offset = 0);
    explicit Link(const Album &album);
    explicit Link(const Playlist &playlist);

    Type    type() const;
    QString uri() const;

    Track       track() const;
    Image       image() const;
    Artist      artist() const;
    Album       album() const;
    Playlist    playlist() const;

    //static Link fromAlbumCover(const Album &album, ...)
    //static Link fromArtistPortrait(const Artist &artist, ...)
};

}

Q_DECLARE_METATYPE(Spotinetta::Link)
