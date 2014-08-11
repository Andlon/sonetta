#pragma once

#include <QSharedPointer>

namespace Sonetta {

class Playlist : public QObject {

};

typedef QSharedPointer<Playlist> SharedPlaylist;

}
