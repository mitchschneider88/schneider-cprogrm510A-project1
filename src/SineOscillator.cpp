#include "SineOscillator.h"

SineOscillator::SineOscillator(float frequency, float amplitude) : Oscillator(frequency, amplitude) {}

float SineOscillator::renderAudio()
{
    auto sample {getAmplitude() * sin(getAngle())};
    setAngle(getAngle() + getOffset());
    return sample;
}