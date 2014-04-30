#include "imageprovider.h"

#include <QThread>
#include <QEvent>
#include <QCoreApplication>

#include <QDebug>

namespace sp = Spotinetta;

namespace Sonetta {

ImageProvider::ImageProvider(ObjectSharedPointer<const Spotinetta::Session> session, QObject *parent)
    :   QObject(parent), QQuickImageProvider(QQmlImageProviderBase::Image, QQmlImageProviderBase::ForceAsynchronousImageLoading),
      m_cancel(false), m_waitCount(0), m_session(session)
{

}

ImageProvider::~ImageProvider()
{
    // Make sure we cancel any pending requests
    {
        QMutexLocker locker(&m_waitMutex);
        m_cancel = true;
    }

    m_waitCondition.wakeAll();

    // Wait until all pending requests have been finished
    QMutexLocker locker(&m_waitMutex);
    while (m_waitCount > 0)
    {
        m_waitCondition.wait(&m_waitMutex);
    }
}

void ImageProvider::loadImage(const QString &uri)
{
    if (!m_session.isNull())
    {
        sp::Image image = m_session->createImageFromLink(sp::Link(uri));

        if (image.isLoaded() || !image.isValid())
        {
            notifyImageLoaded(uri, image);
        }
        else
        {
            // Add image to vector of images waiting to be loaded,
            // and create a watcher that will notify us when the image is loaded
            m_pending.append(qMakePair(uri, image));
            sp::ImageWatcher * watcher = new sp::ImageWatcher(m_session.data(), this);
            connect(watcher, &sp::ImageWatcher::loaded, this, &ImageProvider::onImageLoaded);
            connect(watcher, &sp::ImageWatcher::loaded, watcher, &sp::ImageWatcher::deleteLater);
            watcher->watch(image);
        }
    }
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QMutexLocker locker(&m_waitMutex);
    if (m_cancel)
        return QImage();

    const QString & uri = id;

    if (uri.simplified().isEmpty())
        return QImage();

    QMetaObject::invokeMethod(this, "loadImage", Qt::QueuedConnection,
                              Q_ARG(QString, uri));

    ++m_waitCount;
    while (!imageReady(uri) && !m_cancel)
    {
        m_waitCondition.wait(&m_waitMutex);
    }
    --m_waitCount;

    if (m_cancel)
        return QImage();

    QByteArray data = takeImageData(uri);

    // We have the data, release the lock before decoding/scaling
    // (do NOT access shared member data after this point!)
    locker.unlock();

    QImage image = QImage::fromData(data);
    *size = image.size();

    if (requestedSize.isValid())
    {
        return image.scaled(requestedSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    else
    {
        return image;
    }
}

void ImageProvider::notifyImageLoaded(const QString &uri, const Spotinetta::Image &image)
{
    QMutexLocker locker(&m_waitMutex);
    m_results.insert(uri, image.data());
    m_waitCondition.wakeAll();
}

bool ImageProvider::imageReady(const QString &uri) const
{
    return m_results.contains(uri);
}

void ImageProvider::onImageLoaded()
{
    QVector<UrlImagePair> keep;

    for (const UrlImagePair & pair : m_pending)
    {
        const QString & uri = pair.first;
        const sp::Image & image = pair.second;

        if (image.isLoaded())
        {
            notifyImageLoaded(uri, image);
        }
        else
        {
            keep.append(pair);
        }
    }

    m_pending.swap(keep);
}

QByteArray ImageProvider::takeImageData(const QString &uri)
{
    // Mutex should already be locked at this point
    return m_results.take(uri);
}

}
