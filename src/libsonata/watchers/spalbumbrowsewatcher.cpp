#include <Sonata/spalbumbrowsewatcher.h>
#include "../spcore.h"

using namespace SonataPrivate;

SpAlbumBrowseWatcher::SpAlbumBrowseWatcher(QObject *parent)
    :   QObject(parent)
{
}

SpAlbumBrowse SpAlbumBrowseWatcher::albumBrowse() const
{
    return m_browse;
}

void SpAlbumBrowseWatcher::setAlbumBrowse(const SpAlbumBrowse &albumBrowse)
{
    if (albumBrowse == m_browse)
        return;

    m_browse = albumBrowse;
    emit albumBrowseChanged();

    SpHandle core = SpCore::handle();
    if (!core.isNull())
    {
        disconnect(core->factory(), SIGNAL(albumBrowseCompleted(SpAlbumBrowse)),
                   this, SLOT(handleAlbumBrowseUpdated(SpAlbumBrowse)));

        if (!m_browse.isLoaded())
        {
            connect(core->factory(), SIGNAL(albumBrowseCompleted(SpAlbumBrowse)),
                    this, SLOT(handleAlbumBrowseUpdated(SpAlbumBrowse)));
        }
    }
}

void SpAlbumBrowseWatcher::handleAlbumBrowseUpdated(const SpAlbumBrowse &browse)
{
    if (browse == m_browse)
    {
        SpHandle core = SpCore::handle();

        if (!core.isNull())
        {
            disconnect(core->factory(), SIGNAL(albumBrowseCompleted(SpAlbumBrowse)),
                       this, SLOT(handleAlbumBrowseUpdated(SpAlbumBrowse)));
        }

        emit loaded();
    }
}


