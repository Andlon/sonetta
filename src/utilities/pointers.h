#pragma once

#include <QScopedPointer>
#include <QSharedPointer>
#include <QObject>

#include <type_traits>

namespace PointerDetails {
template <typename T>
void deleteLaterIfOrphan(T * obj)
{
    if (obj == nullptr)
        return;

    auto mutableObj = const_cast<typename std::remove_const<T>::type *>(obj);
    if (mutableObj->parent() == nullptr)
    {
        mutableObj->deleteLater();
    }
}

template <typename T>
struct ScopedObjectDeleter
{
    static void cleanup(T * obj = 0)
    {
        deleteLaterIfOrphan(obj);
    }
};
}

/*
 *  Note: These classes are not complete. I should finish them at some point.
 *  Also note that ObjectScopedPointer can be replaced by a template alias,
 *  but VC++2012 (which I unfortunately have to support for now) doesn't support
 *  this.
 */

template <typename T>
class ObjectScopedPointer : public QScopedPointer<T, PointerDetails::ScopedObjectDeleter<T> >
{
public:
    ObjectScopedPointer(T * p = 0) : QScopedPointer<T, PointerDetails::ScopedObjectDeleter<T> >(p) { }
};

template <typename T>
class ObjectSharedPointer : public QSharedPointer<T>
{
public:
    ObjectSharedPointer() { }
    ObjectSharedPointer(const ObjectSharedPointer<T> &other) : QSharedPointer<T>(other) { }
    explicit ObjectSharedPointer(T * ptr) : QSharedPointer<T>(ptr, PointerDetails::deleteLaterIfOrphan<T>) { }

    // Note: This is explicit by design in order to be able to cast
    explicit ObjectSharedPointer(const QSharedPointer<T> &other) : QSharedPointer<T>(other) { }

    template <typename X>
    ObjectSharedPointer<X> constCast() const { return ObjectSharedPointer<X>(QSharedPointer<T>::template constCast<X>()); }

    void reset(T * ptr) { QSharedPointer<T>::reset(ptr, PointerDetails::deleteLaterIfOrphan<T>); }
};
