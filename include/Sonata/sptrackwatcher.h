#ifndef SPTRACKWATCHER_H
#define SPTRACKWATCHER_H

#include "sptrack.h"
#include "spartist.h"
#include <QObject>

class SpTrackWatcher : public QObject
{
    Q_OBJECT

    Q_PROPERTY(SpTrack track READ track WRITE setTrack NOTIFY trackChanged)
    Q_PROPERTY(SpArtistList artists READ artists NOTIFY dataChanged)
    Q_PROPERTY(QString name READ name NOTIFY dataChanged)
    Q_PROPERTY(int duration READ duration NOTIFY dataChanged)
    Q_PROPERTY(int popularity READ popularity NOTIFY dataChanged)
    Q_PROPERTY(int artistCount READ artistCount NOTIFY dataChanged)
    Q_PROPERTY(bool isLoaded READ isLoaded NOTIFY dataChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY trackChanged)
    Q_PROPERTY(bool isAutoLinked READ isAutoLinked NOTIFY dataChanged)
public:
    explicit SpTrackWatcher(QObject * parent = 0);

    SpTrack track() const;
    void setTrack(const SpTrack &track);

    SpArtistList artists() const;
    Q_INVOKABLE SpArtist artistAt(int index) const;

    QString name() const;
    int duration() const;
    int popularity() const;
    int artistCount() const;
    bool isValid() const;
    bool isLoaded() const;
    bool isAutoLinked() const;

signals:
    void trackChanged();
    void dataChanged();

private slots:
    void handleTrackDataUpdated(const SpTrack &track);

private:
    Q_DISABLE_COPY(SpTrackWatcher)
    SpTrack m_track;
};

#endif // SPTRACKWATCHER_H
