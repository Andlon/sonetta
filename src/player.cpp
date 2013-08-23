#include "player.h"

#include <QCoreApplication>

namespace sp = Spotinetta;


namespace Sonetta {

Player::Player(sp::Session *session, QObject *parent)
    :   QObject(parent), m_session(session), m_shuffle(false), m_repeat(false)
{
    Q_ASSERT(session != nullptr);

    m_watcher = new sp::TrackWatcher(session, this);

    // Set up connections
    connect(session, &sp::Session::endOfTrack, this, &Player::next);

    // This ensures a track is eventually played whether it's loaded
    // or not
    connect(m_watcher, &sp::TrackWatcher::loaded, [this] {
        play(m_watcher->watched());
    });;
}

bool Player::shuffle() const
{
    return m_shuffle;
}

bool Player::repeat() const
{
    return m_repeat;
}

sp::Track Player::track() const
{
    return m_watcher->watched();
}

void Player::setShuffle(bool enable)
{
    if (m_shuffle != enable)
    {
        m_shuffle = enable;
        emit shuffleChanged();
    }
}

void Player::setRepeat(bool enable)
{
    if (m_repeat != enable)
    {
        m_repeat = enable;
        emit repeatChanged();
    }
}

void Player::play(const Spotinetta::Track &track)
{
    m_watcher->watch(track);

    if (track.isLoaded())
    {
        if (m_session->load(track))
        {
            m_session->play();
            m_session->seek(0);
        }
    }

    // If track is not loaded, it will be played when it is loaded,
    // as the watcher's loaded signal is connected to play(track)
}

void Player::enqueue(const Spotinetta::Track &track)
{
    if (m_explicitQueue.isEmpty())
    {
        // If queue is empty, just play it immediately
        play(track);
    }
    else
    {
        // If it's not empty, add it to queue
        m_explicitQueue.enqueue(track);
    }
}

void Player::play()
{
    m_session->play();
}

void Player::playPause()
{
    if (m_session->playbackState() == sp::Session::PlaybackState::Playing)
    {
        play();
    }
    else
    {
        pause();
    }
}

void Player::pause()
{
    m_session->pause();
}

void Player::next()
{
    if (!m_explicitQueue.isEmpty())
    {
        play(m_explicitQueue.dequeue());
    }

    emit trackChanged();
}


}
