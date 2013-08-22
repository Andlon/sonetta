#ifndef SPDEFAULTOUTPUT_H
#define SPDEFAULTOUTPUT_H

#include "spaudiooutput.h"
#include <QAudio>

namespace SonataPrivate {
class SpDefaultOutputPrivate;
}

class SpDefaultOutput : public SpAudioOutput
{
    Q_OBJECT
public:
    explicit SpDefaultOutput(QObject *parent = 0);
    ~SpDefaultOutput();

protected slots:
    virtual void resetBuffer();

protected:
    virtual void startAudio();
    virtual void stopAudio();
    virtual void pauseAudio();

    virtual qreal updateVol(qreal volume);

private slots:
    void push();

    void handleStateChange(QAudio::State);

private:
    SonataPrivate::SpDefaultOutputPrivate * d;
};

#endif // SPDEFAULTOUTPUT_H
