#include "spotifyimageprovider.h"

#include <QThread>
#include <QEvent>
#include <QCoreApplication>

#include <QDebug>

namespace sp = Spotinetta;

struct ImageDataSync {
    ImageDataSync() : isLoaded(0) { }
    QAtomicInt isLoaded;
    QByteArray data;
    QString uri;
};

struct SyncWatcherPair {
    ImageDataSync * sync;
    QPointer<sp::ImageWatcher> watcher;
};

class SyncEvent : public QEvent {
public:
    SyncEvent(ImageDataSync * sync) : QEvent(QEvent::User), m_sync(sync) { }

    ImageDataSync * sync() const { return m_sync; }

private:
    ImageDataSync * m_sync;
};



ImageProvider::ImageProvider(QObject *parent) :
    QObject(parent), QQuickImageProvider(QQmlImageProviderBase::Image, QQmlImageProviderBase::ForceAsynchronousImageLoading)
{
}

void ImageProvider::customEvent(QEvent *event)
{
    if (event->type() == QEvent::User)
    {
        SyncEvent * syncEvent = static_cast<SyncEvent *>(event);
        ImageDataSync * sync = syncEvent->sync();

        sp::Image link(sync->uri);

        if (link.type() != Spotify::ImageLink)
        {
            sync->data = QByteArray();
            sync->isLoaded.store(1);
        }

        sp::Image image = SpLink(sync->uri).image();

        if (image.isLoaded() || !image.isValid())
        {
            sync->data = image.imageData();
            sync->isLoaded.store(1);
        }
        else
        {
            sp::ImageWatcher * watcher = new sp::ImageWatcher(this);
            watcher->setImage(image);
            connect(watcher, SIGNAL(dataChanged()),
                    this, SLOT(handleImageUpdated()));

            SyncWatcherPair pair;
            pair.sync = sync;
            pair.watcher = QPointer<sp::ImageWatcher>(watcher);

            m_pairs.append(pair);
        }
    }
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
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

void ImageProvider::handleImageUpdated()
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
