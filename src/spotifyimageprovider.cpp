#include "spotifyimageprovider.h"

#include <QThread>
#include <QEvent>
#include <QCoreApplication>

#include <Sonata/SpLink>
#include <Sonata/SpImage>

#include <QDebug>

class SyncEvent : public QEvent {
public:
    SyncEvent(ImageDataSync * sync) : QEvent(QEvent::User), m_sync(sync) { }

    ImageDataSync * sync() const { return m_sync; }

private:
    ImageDataSync * m_sync;
};



SpotifyImageProvider::SpotifyImageProvider(QObject *parent) :
    QObject(parent), QQuickImageProvider(QQmlImageProviderBase::Image, QQmlImageProviderBase::ForceAsynchronousImageLoading)
{
}

void SpotifyImageProvider::customEvent(QEvent *event)
{
    if (event->type() == QEvent::User)
    {
        SyncEvent * syncEvent = static_cast<SyncEvent *>(event);
        ImageDataSync * sync = syncEvent->sync();

        SpLink link(sync->uri);

        if (link.type() != Spotify::ImageLink)
        {
            sync->data = QByteArray();
            sync->isLoaded.store(1);
        }

        SpImage image = SpLink(sync->uri).image();

        if (image.isLoaded() || !image.isValid())
        {
            sync->data = image.imageData();
            sync->isLoaded.store(1);
        }
        else
        {
            SpImageWatcher * watcher = new SpImageWatcher(this);
            watcher->setImage(image);
            connect(watcher, SIGNAL(dataChanged()),
                    this, SLOT(handleImageUpdated()));

            SyncWatcherPair pair;
            pair.sync = sync;
            pair.watcher = QPointer<SpImageWatcher>(watcher);

            m_pairs.append(pair);
        }
    }
}

QImage SpotifyImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    ImageDataSync sync;
    sync.uri = id;

    {
        SyncEvent * syncEvent = new SyncEvent(&sync);
        QCoreApplication::postEvent(this, syncEvent);
    }

    while (sync.isLoaded.load() != 1)
        QThread::msleep(5);

    // Image has been loaded, create QImage from data
    QImage image = QImage::fromData(sync.data);
    *size = image.size();

    if (requestedSize.isValid())
        return image.scaled(requestedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    else
        return image;
}

void SpotifyImageProvider::handleImageUpdated()
{
    QVector<SyncWatcherPair>::Iterator i = m_pairs.begin();
    while (i != m_pairs.end())
    {
        SyncWatcherPair &pair = *i;
        if (pair.watcher->isLoaded())
        {
            pair.sync->data = pair.watcher->image().imageData();
            pair.sync->isLoaded.store(1);
            pair.watcher->deleteLater();
            i = m_pairs.erase(i);
        }
        else
            ++i;
    }
}
