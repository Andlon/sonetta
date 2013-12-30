#pragma once

#include "navigation.h"
#include "windowsnavigationfilter.h"
#include "lircclient.h"
#include "../utilities/pointers.h"

class NavigationController {
public:
    NavigationController();
    ~NavigationController();

    void initialize();

private:
    ObjectScopedPointer<LircClient> m_lirc;
    WindowsNavigationFilter         m_filter;

    bool m_initialized;
};
