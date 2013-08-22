#ifndef SPARTISTWATCHER_H
#define SPARTISTWATCHER_H

#include "definitions.h"
#include "spartist.h"
#include "spimage.h"
#include <QObject>

class SpArtistWatcher : public QObject {
    Q_OBJECT

    Q_PROPERTY(SpArtist artist READ artist WRITE setArtist NOTIFY artistChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY artistChanged)
    Q_PROPERTY(bool isLoaded READ isLoaded NOTIFY dataChanged)
    Q_PROPERTY(QString name READ name NOTIFY dataChanged)

public:
    explicit SpArtistWatcher(QObject * parent = 0);

    SpArtist artist() const;
    void setArtist(const SpArtist &artist);

    QString name() const;

    bool isValid() const;
    bool isLoaded() const;

    Q_INVOKABLE SpImage portrait(Spotify::ImageSize size) const;

signals:
    void artistChanged();
    void dataChanged();

private slots:
    void handleArtistDataUpdated(const SpArtist &artist);

private:
    Q_DISABLE_COPY(SpArtistWatcher)
    SpArtist m_artist;
};

#endif // SPARTISTWATCHER_H
