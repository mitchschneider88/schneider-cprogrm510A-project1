#include "Oscillator.h"

void Oscillator::setOffset(double offset)
{
    _phaseOffset = offset;
}

void Oscillator::setAmplitude(double amp)
{
    _amplitude = amp;
}

void Oscillator::setAngle(double angle)
{
    _angle = angle;
}

double Oscillator::getOffset() const
{
    return _phaseOffset;
}

double Oscillator::getAmplitude() const
{
    return _amplitude;
}

double Oscillator::getAngle() const
{
    return _angle;
}