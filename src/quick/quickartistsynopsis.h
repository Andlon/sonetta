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
    Q_PROPERTY(BrowseType browseType READ browseType WRITE setBrowseType NOTIFY browseTypeChanged)

    Q_ENUMS(BrowseType)
public:
    enum BrowseType {
        MinimalBrowse = Spotinetta::ArtistBrowseType::NoAlbums,
        StandardBrowse = Spotinetta::ArtistBrowseType::NoTracks
    };

    explicit QuickArtistSynopsis(QObject * parent = 0);

    Spotinetta::Artist artist() const;
    void setArtist(const Spotinetta::Artist & artist);

    BrowseType browseType() const;
    void setBrowseType(BrowseType type);

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
    void browseTypeChanged();

private slots:
    void onLoaded();

private:
    template <typename T>
    void connectModel(T * model);
    ObjectSharedPointer<const Spotinetta::Session>          m_session;
    ObjectScopedPointer<Spotinetta::ArtistWatcher>          m_artistWatcher;
    ObjectScopedPointer<Spotinetta::ArtistBrowseWatcher>    m_browseWatcher;
    BrowseType                                              m_browseType;

    ObjectScopedPointer<AlbumListModel>     m_albums;
    ObjectScopedPointer<ArtistListModel>    m_similarArtists;
    ObjectScopedPointer<TrackListModel>     m_hits;
};

}
