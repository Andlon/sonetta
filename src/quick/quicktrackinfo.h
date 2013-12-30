#pragma once

#include <QObject>
#include <QStringList>

#include <Spotinetta/Spotinetta>

#include "../utilities/pointers.h"

namespace Sonetta {

class QuickTrackInfo : public QObject {
    Q_OBJECT

    Q_PROPERTY(Spotinetta::Track track READ track WRITE setTrack NOTIFY trackChanged)
    Q_PROPERTY(Spotinetta::ArtistList artists READ artists NOTIFY artistsChanged)
    Q_PROPERTY(QString name READ name NOTIFY dataUpdated)
    Q_PROPERTY(QStringList artistNames READ artistNames NOTIFY artistsChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY trackChanged)
    Q_PROPERTY(bool isLoaded READ isLoaded NOTIFY dataUpdated)
    Q_PROPERTY(bool isAutoLinked READ isAutoLinked NOTIFY dataUpdated)
    Q_PROPERTY(int popularity READ popularity NOTIFY dataUpdated)
    Q_PROPERTY(int duration READ duration NOTIFY dataUpdated)

    Q_PROPERTY(QString smallCoverUri READ smallCoverUri NOTIFY dataUpdated)
    Q_PROPERTY(QString normalCoverUri READ normalCoverUri NOTIFY dataUpdated)
    Q_PROPERTY(QString largeCoverUri READ largeCoverUri NOTIFY dataUpdated)

public:
    explicit QuickTrackInfo(QObject * parent = 0);

    bool isValid() const;
    bool isLoaded() const;
    bool isAutoLinked() const;

    Spotinetta::Track track() const;
    void setTrack(const Spotinetta::Track &track);

    QString name() const;
    QStringList artistNames() const;

    int duration() const;
    int popularity() const;
    int artistCount() const;
    Q_INVOKABLE Spotinetta::Artist artistAt(int index) const;
    Spotinetta::ArtistList artists() const;

    QString smallCoverUri() const;
    QString normalCoverUri() const;
    QString largeCoverUri() const;

signals:
    void trackChanged();
    void dataUpdated();
    void artistsChanged();

private slots:
    void onTrackLoaded();

private:
    void setupWatchers();

    ObjectScopedPointer<Spotinetta::TrackWatcher>               m_trackWatcher;
    ObjectScopedPointer<Spotinetta::AlbumWatcher>               m_albumWatcher;
    QVector<ObjectSharedPointer<Spotinetta::ArtistWatcher> >    m_artistWatchers;
    ObjectSharedPointer<Spotinetta::Session>                    m_session;
};

}
