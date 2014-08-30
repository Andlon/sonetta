#pragma once

#include "abstracttrackcollectionmodel.h"
#include "../utilities/pointers.h"

namespace Sonetta {

class AlbumModel : public AbstractTrackCollectionModel
{
    Q_OBJECT

    Q_PROPERTY(Spotinetta::Album album READ album WRITE setAlbum RESET resetAlbum NOTIFY albumChanged)
    Q_PROPERTY(Spotinetta::Artist artist READ artist NOTIFY albumMetadataChanged)
    Q_PROPERTY(Spotinetta::TrackList tracks READ tracks NOTIFY tracksChanged)
    Q_PROPERTY(int count READ count NOTIFY tracksChanged)
    Q_PROPERTY(QString name READ name NOTIFY albumMetadataChanged)
    Q_PROPERTY(QString artistName READ artistName NOTIFY albumMetadataChanged)
    Q_PROPERTY(bool isAvailable READ isAvailable NOTIFY albumMetadataChanged)
    Q_PROPERTY(int year READ year NOTIFY albumMetadataChanged)
    Q_PROPERTY(int totalDuration READ totalDuration NOTIFY totalDurationChanged)

    Q_PROPERTY(QString smallCoverUri READ smallCoverUri NOTIFY albumChanged)
    Q_PROPERTY(QString normalCoverUri READ normalCoverUri NOTIFY albumChanged)
    Q_PROPERTY(QString largeCoverUri READ largeCoverUri NOTIFY albumChanged)

public:
    explicit AlbumModel(ObjectSharedPointer<const Spotinetta::Session> session, QObject * parent = 0);

    QString name() const;
    QString artistName() const;
    bool    isAvailable() const;
    int     year() const;

    Spotinetta::Album::Type type() const;
    Spotinetta::Artist      artist() const;
    Spotinetta::TrackList   tracks() const;

    Spotinetta::Album album() const;
    void setAlbum(const Spotinetta::Album &album);

    QString smallCoverUri() const;
    QString normalCoverUri() const;
    QString largeCoverUri() const;

    int totalDuration() const;
    int count() const;

protected:
    int getTrackCount() const;
    Spotinetta::Track getTrackAt(int index) const;

signals:
    void albumChanged();
    void tracksChanged();
    void albumMetadataChanged();
    void totalDurationChanged();

private slots:
    void onLoaded();

private:
    Q_DISABLE_COPY(AlbumModel)
    void resetAlbum() { setAlbum(Spotinetta::Album()); }
    void recalculateTotalDuration();

    ObjectSharedPointer<const Spotinetta::Session>      m_session;
    ObjectScopedPointer<Spotinetta::AlbumBrowseWatcher> m_watcher;
    Spotinetta::TrackList               m_tracks;

    int m_duration;
};

}
