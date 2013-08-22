#ifndef SONATA_H
#define SONATA_H

#include <QObject>
#include <QQueue>
#include <QQuickView>
#include <QQueue>

#include <Sonata/Spotify>
#include "../appkey.c"

#include "navigation/navigation.h"

class SonataPlayer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(bool repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)

public:

    explicit SonataPlayer(SpotifySession * spotify, QObject *parent = 0);
    ~SonataPlayer();

    bool shuffle() const;
    void setShuffle(bool enable);

    bool repeat() const;
    void setRepeat(bool enable);

    SpAlbumBrowse contextAlbum() const;
    SpPlaylist contextPlaylist() const;

    Q_INVOKABLE void setContext(const SpAlbumBrowse &albumContext, int index = 0);
    Q_INVOKABLE void setContext(const SpPlaylist &playlistContext, int index = 0);
    
signals:
    void sectionChanged();
    void shuffleChanged();
    void repeatChanged();
    
public slots:
    void enqueue(const SpTrack &track);
    void next();
    void prev();

private:
    int findNextAvailableTrackFromAlbum(int startIndex) const;
    int findNextAvailableTrackFromPlaylist(int startIndex) const;

    SpotifySession * m_spotify;

    QQueue<SpTrack> m_explicitQueue;

    SpAlbumBrowse m_contextAlbumBrowse;
    SpPlaylist m_contextPlaylist;
    int m_currentIndex;

    bool m_shuffle;
    bool m_repeat;
};

#endif // SONATA_H
