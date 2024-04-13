#include "Oscillator.h"

void Oscillator::setOffset(float offset)
{
    _phaseOffset = offset;
}

void Oscillator::setAmplitude(float amp)
{
    _amplitude = amp;
}

void Oscillator::setAngle(float angle)
{
    _angle = angle;
}

void Oscillator::setFrequency(float freq)
{
    _frequency = freq;
}

float Oscillator::getOffset() const
{
    return _phaseOffset;
}

float Oscillator::getAmplitude() const
{
    return _amplitude;
}

float Oscillator::getAngle() const
{
    return _angle;
}

float Oscillator::getFrequency() const
{
    return _frequency;
}