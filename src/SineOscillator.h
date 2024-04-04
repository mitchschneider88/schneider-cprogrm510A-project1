#include "Oscillator.h"

class SineOscillator : public Oscillator
{
public:

    SineOscillator(float frequency, float amplitude);

    float renderAudio() override;
};