#pragma once

#include <QObject>
#include <QScopedPointer>

class QObjectScopedPointerDeleter {
public:
    static void cleanup(QObject * object)
    {
        object->deleteLater();
    }
};

// Gief template typedef :(
