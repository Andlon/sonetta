#ifndef NAVIGATIONNATIVEEVENTFILTER_H
#define NAVIGATIONNATIVEEVENTFILTER_H

#include <QAbstractNativeEventFilter>



class NavigationNativeEventFilter : public QAbstractNativeEventFilter
{
public:
    explicit NavigationNativeEventFilter();

    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
};

#endif // NAVIGATIONNATIVEEVENTFILTER_H
