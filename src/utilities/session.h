#pragma once

#include "../application.h"

namespace Sonetta {

/**
 * @brief getCurrentSession Simple utility function for
 *          returning the current Spotinetta::Session
 * @return A pointer to the session or nullptr if none exists
 */

inline Spotinetta::Session * getCurrentSession() {
    Spotinetta::Session * session = nullptr;
    Application * application = Application::instance();

    if (application != nullptr)
        session = application->session();

    return session;
}

}
