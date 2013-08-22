#include "artistlistmodel.h"

#include <QMetaMethod>
#include <QDebug>

ArtistListModel::ArtistListModel(QObject *parent) :
    AbstractArtistCollectionModel(parent)
{
}

void ArtistListModel::append(const SpArtistList &artists)
{
    if (artists.count() > 0)
    {
        int first = m_artists.count();
        int last = m_artists.count() + artists.count() - 1;

        beginInsertRows(QModelIndex(), first, last);
        m_artists.append(artists);
        endInsertRows();
    }
}

bool ArtistListModel::canFetchMore(const QModelIndex &parent) const
{
    // We can only fetch more if something's receiving notifications when we need more
    return !parent.isValid() && isSignalConnected(QMetaMethod::fromSignal(&ArtistListModel::needMore));
}

void ArtistListModel::fetchMore(const QModelIndex &parent)
{
    if (!parent.isValid())
        emit needMore();
}

void ArtistListModel::clear()
{
    beginResetModel();
    m_artists.clear();
    endResetModel();
}

SpArtist ArtistListModel::getArtistAt(int index) const
{
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < m_artists.count());
    return m_artists[index];
}

int ArtistListModel::getArtistCount() const
{
    return m_artists.count();
}
