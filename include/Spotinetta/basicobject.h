#pragma once

#include <libspotify/api.h>

namespace Spotinetta {

template <typename HandleType, sp_error RefFunc(HandleType *), sp_error DerefFunc(HandleType *)>
class BasicObject {
private:
    HandleType * m_handle;

    void ref() const;
    void deref() const;

public:
    BasicObject() : m_handle(0) { }
    BasicObject(HandleType * handle, bool increment = true);
    BasicObject(const BasicObject &other) : BasicObject(other.handle()) { }
    ~BasicObject();

    bool isValid() const;
    HandleType * handle() const;

    BasicObject & operator = (const BasicObject & other);

    bool operator == (const BasicObject &other);
};

template <typename HandleType, sp_error RefFunc(HandleType *), sp_error DerefFunc(HandleType *), bool IsLoadedFunc(HandleType *)>
class BasicLoadableObject : public BasicObject<HandleType, RefFunc, DerefFunc> {
public:
    BasicLoadableObject() { }
    BasicLoadableObject(HandleType * handle, bool increment = true) : BasicObject<HandleType, RefFunc, DerefFunc>(handle, increment) { }

    bool isLoaded() const;
};

/*
 * BasicObject implementation
 */

template <typename HandleType, sp_error RefFunc(HandleType *), sp_error DerefFunc(HandleType *)>
inline void BasicObject<HandleType, RefFunc, DerefFunc>::ref() const
{
    if (isValid())
        RefFunc(m_handle);
}

template <typename HandleType, sp_error RefFunc(HandleType *), sp_error DerefFunc(HandleType *)>
inline void BasicObject<HandleType, RefFunc, DerefFunc>::deref() const
{
    if (isValid())
        DerefFunc(m_handle);
}

template <typename HandleType, sp_error RefFunc(HandleType *), sp_error DerefFunc(HandleType *)>
inline BasicObject<HandleType, RefFunc, DerefFunc>::BasicObject(HandleType * handle, bool increment)
    :   m_handle(handle)
{
    // Increment reference count if required
    if (increment)
        ref();
}

template <typename HandleType, sp_error RefFunc(HandleType *), sp_error DerefFunc(HandleType *)>
inline BasicObject<HandleType, RefFunc, DerefFunc>::~BasicObject()
{
    // Decrement reference count before destruction
    deref();
}

template <typename HandleType, sp_error RefFunc(HandleType *), sp_error DerefFunc(HandleType *)>
inline bool BasicObject<HandleType, RefFunc, DerefFunc>::isValid() const
{
    // We compare with 0 instead of nullptr, because libspotify uses NULL values (though it makes no difference)
    return m_handle != 0;
}

template <typename HandleType, sp_error RefFunc(HandleType *), sp_error DerefFunc(HandleType *)>
inline HandleType * BasicObject<HandleType, RefFunc, DerefFunc>::handle() const
{
    return m_handle;
}

template <typename HandleType, sp_error RefFunc(HandleType *), sp_error DerefFunc(HandleType *)>
inline BasicObject<HandleType, RefFunc, DerefFunc> & BasicObject<HandleType, RefFunc, DerefFunc>::operator = (const BasicObject<HandleType, RefFunc, DerefFunc> &other)
{
    other.ref();
    deref();

    m_handle = other.m_handle;
    return *this;
}

template <typename HandleType, sp_error RefFunc(HandleType *), sp_error DerefFunc(HandleType *)>
inline bool BasicObject<HandleType, RefFunc, DerefFunc>::operator == (const BasicObject<HandleType, RefFunc, DerefFunc> &other)
{
    return isValid() && other.isValid() && handle() == other.handle();
}

/*
 * BasicLoadableObject implementation
 */

template <typename HandleType, sp_error RefFunc(HandleType *), sp_error DerefFunc(HandleType *), bool IsLoadedFunc(HandleType *)>
bool BasicLoadableObject<HandleType, RefFunc, DerefFunc, IsLoadedFunc>::isLoaded() const
{
    return IsLoadedFunc(BasicLoadableObject<HandleType, RefFunc, DerefFunc, IsLoadedFunc>::handle());
}


}
