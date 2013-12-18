#pragma once

#include "abstracttrackcollectionmodel.h"
#include <QPointer>

namespace Sonetta {

class AlbumModel : public AbstractTrackCollectionModel
{
    Q_OBJECT

    Q_PROPERTY(Spotinetta::Album album READ album WRITE setAlbum NOTIFY albumChanged)
    Q_PROPERTY(Spotinetta::Artist artist READ artist NOTIFY albumMetadataChanged)
    Q_PROPERTY(QString name READ name NOTIFY albumMetadataChanged)
    Q_PROPERTY(bool isAvailable READ isAvailable NOTIFY albumMetadataChanged)
    Q_PROPERTY(int year READ year NOTIFY albumMetadataChanged)

public:
    explicit AlbumModel(Spotinetta::Session * session, QObject * parent = 0);

    QString name() const;
    bool    isAvailable() const;
    int     year() const;

    Spotinetta::Album::Type type() const;
    Spotinetta::Artist      artist() const;

    Spotinetta::Album album() const;
    void setAlbum(const Spotinetta::Album &album);

protected:
    int getTrackCount() const;
    Spotinetta::Track getTrackAt(int index) const;

signals:
    void albumChanged();
    void albumMetadataChanged();

private slots:
    void onLoaded();

private:
    Q_DISABLE_COPY(AlbumModel)
    QPointer<Spotinetta::Session>       m_session;
    Spotinetta::AlbumBrowseWatcher *    m_watcher;
    Spotinetta::TrackList               m_tracks;
};

}
