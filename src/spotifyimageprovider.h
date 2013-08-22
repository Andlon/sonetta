#ifndef SPOTIFYIMAGEPROVIDER_H
#define SPOTIFYIMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>

#include <QVector>
#include <QString>
#include <QByteArray>
#include <QAtomicInt>
#include <QPointer>

#include <Sonata/SpImageWatcher>

struct ImageDataSync {
    ImageDataSync() : isLoaded(0) { }
    QAtomicInt isLoaded;
    QByteArray data;
    QString uri;
};

struct SyncWatcherPair {
    ImageDataSync * sync;
    QPointer<SpImageWatcher> watcher;
};



class SpotifyImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    explicit SpotifyImageProvider(QObject *parent = 0);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
    
    void customEvent(QEvent *);

private slots:
    void handleImageUpdated();

private:
    QVector<SyncWatcherPair> m_pairs;
};

#endif // SPOTIFYIMAGEPROVIDER_H
