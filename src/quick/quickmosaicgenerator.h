#pragma once

#include "../utilities/mosaicgenerator.h"
#include "../utilities/session.h"

namespace Sonetta {

class QuickMosaicGenerator : public MosaicGenerator {
    Q_OBJECT
public:
    explicit QuickMosaicGenerator(QObject * parent = 0)
        : MosaicGenerator(getCurrentSession(), parent) { }
};

}
