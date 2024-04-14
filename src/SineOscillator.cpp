#include "SineOscillator.h"

double SineOscillator::renderAudio()
{
    auto sample {getAmplitude() * sin(getAngle())};
    setAngle(getAngle() + getOffset());
    return sample;
}