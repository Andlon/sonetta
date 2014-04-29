#pragma once

#include <QObject>
#include <Spotinetta/Spotinetta>
#include <QtQml>

namespace Sonetta {

class QuickFactory : public QObject {
    Q_OBJECT
public:
    explicit QuickFactory(QObject * parent = 0) : QObject(parent) { }

    Q_INVOKABLE Spotinetta::Playlist createPlaylist() const;
    Q_INVOKABLE Spotinetta::Track createTrack() const;
    Q_INVOKABLE Spotinetta::Track createTrackFromUri(const QString &uri) const;
    Q_INVOKABLE Spotinetta::Album createAlbum() const;
    Q_INVOKABLE Spotinetta::Album createAlbumFromUri(const QString &uri) const;
    Q_INVOKABLE Spotinetta::Artist createArtist() const;
    Q_INVOKABLE Spotinetta::Artist createArtistFromUri(const QString &uri) const;
};

inline QObject * quickFactorySingletonProvider(QQmlEngine *, QJSEngine *)
{
    return new QuickFactory;
}

/* Implementation below */
inline Spotinetta::Playlist QuickFactory::createPlaylist() const
{
    return Spotinetta::Playlist();
}

inline Spotinetta::Track QuickFactory::createTrack() const
{
    return Spotinetta::Track();
}

inline Spotinetta::Track QuickFactory::createTrackFromUri(const QString &uri) const
{
    return Spotinetta::Link(uri).track();
}

inline Spotinetta::Album QuickFactory::createAlbum() const
{
    return Spotinetta::Album();
}

inline Spotinetta::Album QuickFactory::createAlbumFromUri(const QString &uri) const
{
    return Spotinetta::Link(uri).album();
}

inline Spotinetta::Artist QuickFactory::createArtist() const
{
    return Spotinetta::Artist();
}

inline Spotinetta::Artist QuickFactory::createArtistFromUri(const QString &uri) const
{
    return Spotinetta::Link(uri).artist();
}

}
