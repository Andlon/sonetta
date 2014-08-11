#pragma once

#include "user.h"
#include "playlist.h"
#include <QObject>
#include <QList>
#include <QVariant>

namespace Sonetta {

class AuthenticationDetails {
public:
    QString     username() const;
    QString     password() const;
    QVariant    blob() const;

    // TODO: Figure out how to do OAuth stuff
};

class UserDataProvider : public QObject {
public:
    void authenticate(const AuthenticationDetails &details) = 0;

    void fetchPlaylistContainerForUser(const User & user) = 0;


signals:
    void authenticated(User user, QVariant blob);


};

}
