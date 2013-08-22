#include "quicklinker.h"

QuickLinker::QuickLinker(QObject *parent) :
    QObject(parent)
{
}

SpArtist QuickLinker::createArtist(const QString &uri) const
{
    return SpLink(uri).artist();
}

SpTrack QuickLinker::createTrack(const QString &uri) const
{
    return SpLink(uri).track();
}

SpImage QuickLinker::createImage(const QString &uri) const
{
    return SpLink(uri).image();
}

SpAlbum QuickLinker::createAlbum(const QString &uri) const
{
    return SpLink(uri).album();
}

SpLink QuickLinker::createLink(const QString &uri) const
{
    return SpLink(uri);
}

QString QuickLinker::createArtistUri(const SpArtist &artist) const
{
    return SpLink(artist).uri();
}

QString QuickLinker::createTrackUri(const SpTrack &track) const
{
    return SpLink(track).uri();
}

QString QuickLinker::createImageUri(const SpImage &image) const
{
    return SpLink(image).uri();
}

QString QuickLinker::createAlbumUri(const SpAlbum &album) const
{
    return SpLink(album).uri();
}

QString QuickLinker::createLinkUri(const SpLink &link) const
{
    return SpLink(link).uri();
}


