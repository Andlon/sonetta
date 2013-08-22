#ifndef SPARTISTBROWSEWATCHER_H
#define SPARTISTBROWSEWATCHER_H

#include "spartistbrowse.h"
#include "spimage.h"
#include "spartist.h"
#include "sptrack.h"
#include "spalbum.h"
#include "spartist.h"
#include <QObject>

class SpArtistBrowseWatcher : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool isValid READ isValid NOTIFY artistBrowseChanged)
    Q_PROPERTY(bool isLoaded READ isLoaded NOTIFY dataChanged)
    Q_PROPERTY(SpArtistBrowse artistBrowse READ artistBrowse WRITE setArtistBrowse NOTIFY artistBrowseChanged)
    Q_PROPERTY(QString biography READ biography NOTIFY dataChanged)
    Q_PROPERTY(SpArtist artist READ artist NOTIFY dataChanged)
    Q_PROPERTY(int portraitCount READ portraitCount NOTIFY dataChanged)
    Q_PROPERTY(SpImageList portraits READ portraits NOTIFY dataChanged)
    Q_PROPERTY(int trackCount READ trackCount NOTIFY dataChanged)
    Q_PROPERTY(SpTrackList tracks READ tracks NOTIFY dataChanged)
    Q_PROPERTY(int albumCount READ albumCount NOTIFY dataChanged)
    Q_PROPERTY(SpAlbumList albums READ albums NOTIFY dataChanged)
    Q_PROPERTY(int similarArtistCount READ similarArtistCount NOTIFY dataChanged)
    Q_PROPERTY(SpArtistList similarArtists READ similarArtists NOTIFY dataChanged)
    Q_PROPERTY(int topTrackCount READ topTrackCount NOTIFY dataChanged)
    Q_PROPERTY(SpTrackList topTracks READ topTracks NOTIFY dataChanged)

public:
    explicit SpArtistBrowseWatcher(QObject * parent = 0);
    ~SpArtistBrowseWatcher();

    bool isValid() const;
    bool isLoaded() const;

    SpArtistBrowse artistBrowse() const;
    void setArtistBrowse(const SpArtistBrowse &browse);

    SpArtist artist() const;
    QString biography() const;

    int portraitCount() const;
    Q_INVOKABLE SpImage portraitAt(int index) const;
    SpImageList portraits() const;

    int trackCount() const;
    Q_INVOKABLE SpTrack trackAt(int index) const;
    SpTrackList tracks() const;

    int albumCount() const;
    Q_INVOKABLE SpAlbum albumAt(int index) const;
    SpAlbumList albums() const;

    int similarArtistCount() const;
    Q_INVOKABLE SpArtist similarArtistAt(int index) const;
    SpArtistList similarArtists() const;

    int topTrackCount() const;
    SpTrack topTrackAt(int index) const;
    SpTrackList topTracks() const;

signals:
    void artistBrowseChanged();
    void dataChanged();
    void loaded();

private slots:
    void handleArtistBrowseUpdated(const SpArtistBrowse &browse);

private:
    SpArtistBrowse m_browse;

};

#endif // SPARTISTBROWSEWATCHER_H
