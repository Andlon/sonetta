#include <Sonata/spsearchwatcher.h>
#include "../spcore.h"

using namespace SonataPrivate;

SpSearchWatcher::SpSearchWatcher(QObject *parent)
    :   QObject(parent)
{
}

SpSearch SpSearchWatcher::search() const
{
    return m_search;
}

void SpSearchWatcher::setSearch(const SpSearch &search)
{
    if (search == m_search)
        return;

    m_search = search;
    emit searchChanged();

    SpHandle core = SpCore::handle();
    if (!core.isNull())
    {
        disconnect(core->factory(), SIGNAL(searchCompleted(SpSearch)),
                   this, SLOT(handleSearchUpdated(SpSearch)));

        if (!m_search.isLoaded())
        {
            connect(core->factory(), SIGNAL(searchCompleted(SpSearch)),
                    this, SLOT(handleSearchUpdated(SpSearch)));
        }
    }
}

void SpSearchWatcher::handleSearchUpdated(const SpSearch &search)
{
    if (search == m_search)
    {
        SpHandle core = SpCore::handle();

        if (!core.isNull())
        {
            disconnect(core->factory(), SIGNAL(searchCompleted(SpSearch)),
                       this, SLOT(handleSearchUpdated(SpSearch)));
        }

        emit loaded();
    }
}



