#ifndef QUICKARTISTSYNOPSIS_H
#define QUICKARTISTSYNOPSIS_H

#include <QObject>
#include <QStringList>
#include <Sonata/SpArtistBrowseWatcher>
#include <Sonata/SpArtistWatcher>

/*
 *  Takes an SpArtistBrowse and exposes useful information about the artist
 *  in a QML-friendly fashion
 */

class QuickArtistSynopsis : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isValid READ isValid NOTIFY artistBrowseChanged)
    Q_PROPERTY(SpArtistBrowse artistBrowse READ artistBrowse WRITE setArtistBrowse NOTIFY artistBrowseChanged)
    Q_PROPERTY(bool isLoaded READ isLoaded NOTIFY browseDataChanged)
    Q_PROPERTY(QStringList portraitLinks READ portraitLinks NOTIFY browseDataChanged)
    Q_PROPERTY(QString biography READ biography NOTIFY browseDataChanged)
    Q_PROPERTY(SpAlbumList albums READ albums NOTIFY browseDataChanged)
    Q_PROPERTY(SpArtistList similarArtists READ similarArtists NOTIFY browseDataChanged)
    Q_PROPERTY(SpTrackList topTracks READ topTracks NOTIFY browseDataChanged)
//    Q_PROPERTY(QString name READ name NOTIFY artistDataChanged)
    Q_PROPERTY(QString name READ name NOTIFY browseDataChanged)

public:
    explicit QuickArtistSynopsis(QObject *parent = 0);

    bool isValid() const;
    bool isLoaded() const;

    QString name() const;
    QString biography() const;

    Q_INVOKABLE QString portrait(/*Spotify::ImageSize*/ int size) const;

    QStringList portraitLinks() const;

    SpAlbumList albums() const;
    SpArtistList similarArtists() const;
    SpTrackList topTracks() const;

    SpArtistBrowse artistBrowse() const;
    void setArtistBrowse(const SpArtistBrowse &browse);
    
signals:
    void artistBrowseChanged();
    void artistBrowseLoaded();
    void browseDataChanged();
    void artistDataChanged();

private:
    SpArtistBrowseWatcher * m_browseWatcher;
    SpArtistWatcher * m_artistWatcher;
};

#endif // QUICKARTISTSYNOPSIS_H
