#pragma once

#include <QObject>
#include <QQuickImageProvider>

#include <QVector>
#include <QString>
#include <QByteArray>
#include <QAtomicInt>
#include <QPointer>

#include <Spotinetta/Spotinetta>

struct SyncWatcherPair;

class ImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    explicit ImageProvider(const Spotinetta::Session * session, QObject *parent = 0);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
    
    void customEvent(QEvent *);

private slots:
    void handleImageUpdated();

private:
    QVector<SyncWatcherPair> m_pairs;
};
