#pragma once

#include "../utilities/mosaicgenerator.h"
#include "../application.h"

namespace Sonetta {

class QuickMosaicGenerator : public MosaicGenerator {
    Q_OBJECT
public:
    explicit QuickMosaicGenerator(QObject * parent = 0)
        : MosaicGenerator(Application::session().constCast<const Spotinetta::Session>(), parent) { }
};

}
