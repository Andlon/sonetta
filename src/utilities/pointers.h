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

    auto mutableObj = const_cast<std::remove_const<T>::type *>(obj);
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
    ObjectSharedPointer(const ObjectSharedPointer<T> &other) : QSharedPointer(other) { }
    explicit ObjectSharedPointer(T * ptr) : QSharedPointer<T>(ptr, PointerDetails::deleteLaterIfOrphan<T>) { }

    // Note: This is explicit by design in order to be able to cast
    explicit ObjectSharedPointer(const QSharedPointer<T> &other) : QSharedPointer(other) { }

    template <typename U>
    ObjectSharedPointer<U> constCast() const { return ObjectSharedPointer<U>(QSharedPointer<T>::constCast<U>()); }

    void reset(T * ptr) { QSharedPointer<T>::reset(ptr, PointerDetails::deleteLaterIfOrphan<T>); }
};
