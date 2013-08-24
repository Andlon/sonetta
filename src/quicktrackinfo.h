#pragma once

#include <QObject>
#include <QStringList>

#include <Spotinetta/Spotinetta>

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

public:
    explicit QuickTrackInfo(QObject * parent = 0);
    ~QuickTrackInfo();

    bool isValid() const;
    bool isLoaded() const;
    bool isAutoLinked() const;

    Spotinetta::Track track() const;
    void setTrack(const Spotinetta::Track &track);

    QString name() const;
    QStringList artistNames() const;

    //Q_INVOKABLE QString albumCoverUri(/*Spotinetta::otify::ImageSize*/ int size) const;

    int duration() const;
    int popularity() const;
    int artistCount() const;
    Q_INVOKABLE Spotinetta::Artist artistAt(int index) const;
    Spotinetta::ArtistList artists() const;

signals:
    void trackChanged();
    void dataUpdated();
    void artistsChanged();

private slots:
    void onTrackLoaded();

private:
    void deleteWatchers();
    void setupWatchers();

    Spotinetta::TrackWatcher *              m_trackWatcher;
    QVector<Spotinetta::ArtistWatcher *>    m_artistWatchers;
    QPointer<Spotinetta::Session>           m_session;
};

}
