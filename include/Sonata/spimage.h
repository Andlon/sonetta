#ifndef SPIMAGE_H
#define SPIMAGE_H

#include "definitions.h"

class QImage;
class QString;

namespace SonataPrivate {
class SpImagePrivate;
class SpFactory;
}

class SpImage {
public:

    SpImage();
    SpImage(const SpImage &other);
    ~SpImage();

    bool isValid() const;
    bool isLoaded() const;

    QImage image() const;
    QByteArray imageData() const;

    SpImage& operator=(const SpImage& image);

private:
    SonataPrivate::SpImagePrivate * d;
    SpImage(SonataPrivate::SpImagePrivate *);

    friend class SonataPrivate::SpFactory;
    friend class SpLink;
    friend bool operator==(const SpImage &image1, const SpImage &image2);
};

bool operator==(const SpImage &image1, const SpImage &image2);

Q_DECLARE_METATYPE(SpImage)
Q_DECLARE_METATYPE(SpImageList)

#endif // SPIMAGE_H
