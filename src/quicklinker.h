#ifndef QUICKLINKER_H
#define QUICKLINKER_H

#include <QObject>
#include <Sonata/SpLink>
#include <Sonata/SpTrack>
#include <Sonata/SpArtist>
#include <Sonata/SpAlbum>
#include <Sonata/SpImage>
#include <Sonata/SpPlaylist>

class QQmlEngine;
class QJSEngine;

/*
 * QML Singleton class for easy creation of Spotify URIs from object or vice versa.
 *
 */

class QuickLinker : public QObject
{
    Q_OBJECT
public:
    explicit QuickLinker(QObject *parent = 0);

    Q_INVOKABLE SpArtist createArtist(const QString &uri) const;
    Q_INVOKABLE SpTrack createTrack(const QString &uri) const;
    Q_INVOKABLE SpImage createImage(const QString &uri) const;
    Q_INVOKABLE SpAlbum createAlbum(const QString &uri) const;
//    Q_INVOKABLE SpPlaylist createPlaylist(const QString &uri) const;
    Q_INVOKABLE SpLink createLink(const QString &uri) const;

    Q_INVOKABLE QString createArtistUri(const SpArtist &artist) const;
    Q_INVOKABLE QString createTrackUri(const SpTrack &track) const;
    Q_INVOKABLE QString createImageUri(const SpImage &image) const;
    Q_INVOKABLE QString createAlbumUri(const SpAlbum &album) const;
//    Q_INVOKABLE QString createPlaylistUri(const SpPlaylist &playlist) const;
    Q_INVOKABLE QString createLinkUri(const SpLink &link) const;
};

static QObject * QuickLinkerSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new QuickLinker();
}

#endif // QUICKLINKER_H
