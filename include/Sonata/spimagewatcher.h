#ifndef SPIMAGEWATCHER_H
#define SPIMAGEWATCHER_H

#include "definitions.h"
#include "spimage.h"
#include <QObject>

class SpImageWatcher : public QObject {
    Q_OBJECT

    Q_PROPERTY(SpImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QString uri READ uri NOTIFY dataChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY imageChanged)
    Q_PROPERTY(bool isLoaded READ isLoaded NOTIFY dataChanged)

public:
    explicit SpImageWatcher(QObject * parent = 0);
    ~SpImageWatcher();

    bool isValid() const;
    bool isLoaded() const;

    SpImage image() const;
    void setImage(const SpImage &image);

    QString uri() const;

    bool event(QEvent *);

signals:
    void imageChanged();
    void dataChanged();

private:
    Q_DISABLE_COPY(SpImageWatcher)
    bool m_isRegistered;
    SpImage m_image;
};

#endif // SPIMAGEWATCHER_H
