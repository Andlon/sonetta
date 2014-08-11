#pragma once

#include <QString>
#include <QVariant>

namespace Sonetta {

class User {
public:
    QString canonicalName() const;
    QString displayName() const;

    QVariant id() const;
    QString service() const;
};

}
