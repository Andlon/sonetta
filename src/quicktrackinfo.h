#ifndef SPQuickTrackInfo_H
#define SPQuickTrackInfo_H

#include <QObject>
#include <QStringList>

#include <Sonata/SpTrack>
#include <Sonata/SpArtist>

class QuickTrackInfo : public QObject {
    Q_OBJECT

    Q_PROPERTY(SpTrack track READ track WRITE setTrack NOTIFY trackChanged)
    Q_PROPERTY(SpArtistList artists READ artists NOTIFY artistsChanged)
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

    SpTrack track() const;
    void setTrack(const SpTrack &track);

    QString name() const;
    QStringList artistNames() const;

    Q_INVOKABLE QString albumCoverUri(/*Spotify::ImageSize*/ int size) const;

    int duration() const;
    int popularity() const;
    int artistCount() const;
    Q_INVOKABLE SpArtist artistAt(int index) const;
    SpArtistList artists() const;

signals:
    void trackChanged();
    void dataUpdated();
    void artistsChanged();

private slots:
    void handleTrackUpdated();
    void handleTrackChanged();

private:
    void configureWatchers();

    class QuickTrackInfoPrivate;
    QuickTrackInfoPrivate * d;
};

#endif // SPQuickTrackInfo_H
