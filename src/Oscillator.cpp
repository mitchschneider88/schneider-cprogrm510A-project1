#include "Oscillator.h"

Oscillator::Oscillator(float frequency, float amplitude) : _frequency(frequency), _amplitude(amplitude)
{
    _offset = (2 * M_PI * _frequency / SAMPLE_RATE);
}

void Oscillator::setOffset(float offset)
{
    _offset = offset;
}

void Oscillator::setFrequency(float frequency)
{
    _frequency = frequency;
}

void Oscillator::setAmplitude(float amplitude)
{
    _amplitude = amplitude;
}

void Oscillator::setAngle(float angle)
{
    _angle = angle;
}

float Oscillator::getAmplitude()
{
    return _amplitude;
}

float Oscillator::getAngle()
{
    return _angle;
}

float Oscillator::getOffset()
{
    return _offset;
}
