#include <Sonata/spalbumwatcher.h>
#include "../spcore.h"

#include <QDebug>

using namespace SonataPrivate;

SpAlbumWatcher::SpAlbumWatcher(QObject *parent)
    :   QObject(parent)
{
}

SpAlbumWatcher::SpAlbumWatcher(const SpAlbum &album, QObject *parent)
    :   QObject(parent)
{
    setAlbum(album);
}

SpAlbum SpAlbumWatcher::album() const
{
    return m_album;
}

void SpAlbumWatcher::setAlbum(const SpAlbum &album)
{
    if (album == m_album)
        return;

    m_album = album;
    emit albumChanged();

    SpHandle core = SpCore::handle();

    if (!core.isNull())
    {
        disconnect(core->factory(), SIGNAL(albumUpdated(SpAlbum)),
                   this, SLOT(handleAlbumLoaded(SpAlbum)));

        if (m_album.isValid() && !m_album.isLoaded())
        {
            connect(core->factory(), SIGNAL(albumUpdated(SpAlbum)),
                    this, SLOT(handleAlbumLoaded(SpAlbum)));
        }
    }
}

void SpAlbumWatcher::handleAlbumLoaded(const SpAlbum &album)
{
    qDebug() << "Got album loaded callback!";
    if (album == m_album)
    {
        SpHandle core = SpCore::handle();

        if (!core.isNull())
        {
            disconnect(core->factory(), SIGNAL(albumUpdated(SpAlbum)),
                       this, SLOT(handleAlbumLoaded(SpAlbum)));
        }

        emit loaded();
    }
}
