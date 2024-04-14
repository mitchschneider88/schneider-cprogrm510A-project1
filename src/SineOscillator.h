#include "Oscillator.h"

class SineOscillator : public Oscillator
{
public:
    double renderAudio() override;
};
