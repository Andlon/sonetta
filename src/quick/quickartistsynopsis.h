#pragma once

#include <QObject>
#include <Spotinetta/Spotinetta>

#include "../models/albumlistmodel.h"
#include "../models/tracklistmodel.h"
#include "../models/artistlistmodel.h"
#include "../utilities/pointers.h"

namespace Sonetta
{

class QuickArtistSynopsis : public QObject {
    Q_OBJECT

    Q_PROPERTY(Spotinetta::Artist artist READ artist WRITE setArtist NOTIFY artistChanged)
    Q_PROPERTY(QString name READ name NOTIFY artistDataChanged)
    Q_PROPERTY(QString biography READ biography NOTIFY browseDataChanged)
    Q_PROPERTY(QString largePortraitUri READ largePortraitUri NOTIFY artistChanged)
    Q_PROPERTY(QString normalPortraitUri READ normalPortraitUri NOTIFY artistChanged)
    Q_PROPERTY(QString smallPortraitUri READ smallPortraitUri NOTIFY artistChanged)
    Q_PROPERTY(QObject * similarArtists READ similarArtists CONSTANT)
    Q_PROPERTY(QObject * albums READ albums CONSTANT)
    Q_PROPERTY(QObject * hits READ hits CONSTANT)
public:
    explicit QuickArtistSynopsis(QObject * parent = 0);

    Spotinetta::Artist artist() const;
    void setArtist(const Spotinetta::Artist & artist);

    QObject * similarArtists() const;
    QObject * albums() const;
    QObject * hits() const;

    QString name() const;
    QString biography() const;

    QString smallPortraitUri() const;
    QString normalPortraitUri() const;
    QString largePortraitUri() const;

signals:
    void artistChanged();
    void artistDataChanged();
    void browseDataChanged();

private slots:
    void onLoaded();

private:
    ObjectSharedPointer<const Spotinetta::Session>          m_session;
    ObjectScopedPointer<Spotinetta::ArtistWatcher>          m_artistWatcher;
    ObjectScopedPointer<Spotinetta::ArtistBrowseWatcher>    m_browseWatcher;
    Spotinetta::ArtistBrowseType                            m_browseType;

    ObjectScopedPointer<AlbumListModel>     m_albums;
    ObjectScopedPointer<ArtistListModel>    m_similarArtists;
    ObjectScopedPointer<TrackListModel>     m_hits;
};

}
