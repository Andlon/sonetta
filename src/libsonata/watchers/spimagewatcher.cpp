#include <Sonata/spimagewatcher.h>
#include <Sonata/splink.h>

using namespace SonataPrivate;

#include "../spcore.h"
#include "../spfactory.h"

#include <QDebug>



static void unregisterWatcher(SpImageWatcher * watcher)
{
    SpHandle core = SpCore::handle();
    if (!core.isNull())
        core->factory()->unregisterImageWatcher(watcher);
}

SpImageWatcher::SpImageWatcher(QObject *parent)
    :   QObject(parent), m_isRegistered(false)
{
}

SpImageWatcher::~SpImageWatcher()
{
    if (m_image.isValid())
        unregisterWatcher(this);
}

bool SpImageWatcher::isValid() const
{
    return m_image.isValid();
}

bool SpImageWatcher::isLoaded() const
{
    return m_image.isLoaded();
}

SpImage SpImageWatcher::image() const
{
    return m_image;
}

void SpImageWatcher::setImage(const SpImage &image)
{
    if (image == m_image)
        return;

    SpHandle core = SpCore::handle();
    if (core.isNull())
        return;

    // Unregister old image
    if (m_image.isValid())
        core->factory()->unregisterImageWatcher(this);

    // Swap images
    m_image = image;

    // Register new image
    if (m_image.isValid())
        core->factory()->registerImageWatcher(this);

    emit imageChanged();
    emit dataChanged();
}

bool SpImageWatcher::event(QEvent *event)
{
    if (event->type() == (QEvent::Type) ImageLoadedEvent)
    {
        emit dataChanged();
        return true;
    }

    return QObject::event(event);
}

QString SpImageWatcher::uri() const
{
    return SpLink(m_image).uri();
}
