#include "SineOscillator.h"

float SineOscillator::renderAudio()
{
    auto sample {getAmplitude() * sin(getAngle())};
    setAngle(getAngle() + getOffset());
    return sample;
}