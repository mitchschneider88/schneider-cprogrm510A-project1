#include "Oscillator.h"

class SquareOscillator : public Oscillator
{
public:

    SquareOscillator(float frequency, float amplitude);
    
    float renderAudio() override;
};