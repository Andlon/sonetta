#include "sonataplayer.h"

#include <QtQml>
#include <QScreen>
#include <QCoreApplication>

SonataPlayer::SonataPlayer(SpotifySession *spotify, QObject *parent) :
    QObject(parent), m_spotify(spotify),
    m_currentIndex(0), m_shuffle(false), m_repeat(true)

{
    connect(m_spotify, SIGNAL(endOfTrack()),
            this, SLOT(next()));
}

SonataPlayer::~SonataPlayer()
{

}

bool SonataPlayer::shuffle() const
{
    return m_shuffle;
}

void SonataPlayer::setShuffle(bool enable)
{
    if (m_shuffle != enable)
    {
        m_shuffle = enable;
        emit shuffleChanged();
    }
}

bool SonataPlayer::repeat() const
{
    return m_repeat;
}

void SonataPlayer::setRepeat(bool enable)
{
    if (m_repeat != enable)
    {
        m_repeat = enable;
        emit repeatChanged();
    }
}

void SonataPlayer::enqueue(const SpTrack &track)
{
    if (m_spotify->currentTrack().isValid())
        m_explicitQueue.append(track);
    else
        m_spotify->play(track);
}

SpAlbumBrowse SonataPlayer::contextAlbum() const
{
    return m_contextAlbumBrowse;
}

SpPlaylist SonataPlayer::contextPlaylist() const
{
    return m_contextPlaylist;
}

void SonataPlayer::setContext(const SpAlbumBrowse &albumContext, int index)
{
    m_contextAlbumBrowse = albumContext;
    m_contextPlaylist = SpPlaylist();
    m_currentIndex = index >= 0 ? index : 0;
}

void SonataPlayer::setContext(const SpPlaylist &playlistContext, int index)
{
    m_contextPlaylist = playlistContext;
    m_contextAlbumBrowse = SpAlbumBrowse();
    m_currentIndex = index >= 0 ? index : playlistContext.trackCount();
}

void SonataPlayer::next()
{
    SpTrack track;

    // Check explicit queue
    if (!m_explicitQueue.isEmpty())
    {
        track = m_explicitQueue.dequeue();
    }
    else if (m_contextAlbumBrowse.isValid())
    {
        // Context is an album
        m_currentIndex = findNextAvailableTrackFromAlbum(m_currentIndex);

        if (m_currentIndex >= m_contextAlbumBrowse.trackCount() && repeat())
        {
            m_currentIndex = 0;
        }

        if (m_currentIndex < m_contextAlbumBrowse.trackCount())
        {
            track = m_contextAlbumBrowse.trackAt(m_currentIndex);
        }
    }
    else if (m_contextPlaylist.isValid())
    {
        // Context is a playlist
        m_currentIndex = findNextAvailableTrackFromPlaylist(m_currentIndex);

        if (m_currentIndex >= m_contextPlaylist.trackCount() && repeat())
        {
            m_currentIndex = 0;
        }

        if (m_currentIndex < m_contextPlaylist.trackCount())
        {
            track = m_contextPlaylist.trackAt(m_currentIndex);
        }
    }

    m_spotify->play(track);
}

void SonataPlayer::prev()
{
    if (--m_currentIndex < 0)
        m_currentIndex = 0;

    SpTrack track;

    if (m_contextAlbumBrowse.isValid() && m_currentIndex < m_contextAlbumBrowse.trackCount())
        track = m_contextAlbumBrowse.trackAt(m_currentIndex);
    else if (m_contextPlaylist.isValid() && m_currentIndex < m_contextPlaylist.trackCount())
        track = m_contextPlaylist.trackAt(m_currentIndex);

    m_spotify->play(track);
}

int SonataPlayer::findNextAvailableTrackFromAlbum(int startIndex) const
{
    /*
     *  Finds next available track index from album browse object, beginning search from
     *  (but not including) startIndex
     */

    int trackCount = m_contextAlbumBrowse.trackCount();

    if (trackCount == 0 || startIndex > trackCount)
        return trackCount;

    int current = startIndex;
    while (++current != startIndex)
    {
        if (current >= trackCount)
        {
            if (repeat())
                current = 0;
            else
                break;
        }

        if (m_contextAlbumBrowse.trackAt(current).isAvailable())
            break;
    }

    if (current == startIndex)
        // We did not find any available tracks to play
        current = trackCount;

    return current;
}

int SonataPlayer::findNextAvailableTrackFromPlaylist(int startIndex) const
{
    /*
     *  Finds next available track index from playlist object, beginning search from
     *  (but not including) startIndex
     */

    int trackCount = m_contextPlaylist.trackCount();

    if (trackCount == 0 || startIndex > trackCount)
        return trackCount;

    int current = startIndex;
    while (++current != startIndex)
    {
        if (current >= trackCount)
        {
            if (repeat())
                current = 0;
            else
                break;
        }

        if (m_contextPlaylist.trackAt(current).isAvailable())
            break;
    }

    if (current == startIndex)
        // We did not find any available tracks to play
        current = trackCount;

    return current;
}
