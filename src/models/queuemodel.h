#pragma once

#include "abstracttrackcollectionmodel.h"
#include "../utilities/persistentplaylistindex.h"

#include <QQueue>

namespace Sonetta {

class QueueContext {
public:
    enum class Type {
        Playlist,
        AlbumBrowse,
        TrackList
    };

    QueueContext() : m_type(Type::TrackList) { }
    explicit QueueContext(const Spotinetta::Playlist &playlist);
    explicit QueueContext(const Spotinetta::TrackList &tracks);
    explicit QueueContext(const Spotinetta::AlbumBrowse &browse);

    bool isValid() const;
    bool isLoaded() const;

    Spotinetta::Playlist    playlist() const;
    Spotinetta::AlbumBrowse albumBrowse() const;
    Spotinetta::TrackList   tracks() const;
    Type                    type() const;

private:
    Type                    m_type;
    Spotinetta::Playlist    m_playlist;
    Spotinetta::TrackList   m_tracks;
    Spotinetta::AlbumBrowse m_browse;
};

class QueueModel : public AbstractTrackCollectionModel
{
    Q_OBJECT
public:
    enum Role {
        Explicit = AbstractTrackCollectionModel::LastTrackCollectionModelRole + 1
    };

    explicit QueueModel(ObjectSharedPointer<const Spotinetta::Session> session, QObject *parent = 0);

    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role) const;

    QueueContext context() const;
    void setContext(const QueueContext &context, int index);

    bool isEmpty() const;

    Spotinetta::Track dequeue();

public slots:
    void enqueue(const Spotinetta::Track &track);
    void pop(int index);

    void clearContext();
    void updateContext(const Spotinetta::Playlist &playlist, int index);
    void updateContext(const Spotinetta::TrackList &tracks, int index);
    void updateContext(const Spotinetta::Album &album, int index);

protected:
    int getTrackCount() const;
    Spotinetta::Track getTrackAt(int index) const;

private slots:
    void onLoaded();

private:
    QueueContext m_context;
    int          m_index;
    bool         m_loaded;

    QQueue<Spotinetta::Track> m_explicit;
    QQueue<Spotinetta::Track> m_implicit;

    ObjectSharedPointer<const Spotinetta::Session>      m_session;
    ObjectScopedPointer<Spotinetta::AlbumBrowseWatcher> m_albumBrowseWatcher;
    ObjectScopedPointer<Spotinetta::PlaylistWatcher>    m_playlistWatcher;
};

}
