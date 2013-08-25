#pragma once

#include <QObject>
#include <QQuickImageProvider>

#include <QPointer>
#include <QMutex>
#include <QWaitCondition>
#include <QMultiMap>
#include <QPair>
#include <QVector>

#include <Spotinetta/Spotinetta>

namespace Sonetta {

class ImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    explicit ImageProvider(const Spotinetta::Session * session, QObject *parent = 0);
    ~ImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

private slots:
    void loadImage(const QString &uri);
    void onImageLoaded();

private:
    typedef QPair<QString, Spotinetta::Image> UrlImagePair;

    bool imageReady(const QString &uri) const;
    void notifyImageLoaded(const QString &uri, const Spotinetta::Image &image);
    QByteArray takeImageData(const QString &uri);

    QMutex          m_waitMutex;
    QWaitCondition  m_waitCondition;
    bool            m_cancel;

    QVector<UrlImagePair>       m_pending;
    QMultiMap<QString, QByteArray>   m_results;

    QPointer<const Spotinetta::Session>       m_session;


};

}
