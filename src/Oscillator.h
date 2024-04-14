#include <cmath>

class Oscillator
{
public:
    // implement rule of 5
    virtual ~Oscillator() = default;

    virtual double renderAudio() = 0;

    void setOffset(double offset);
    void setAmplitude(double amp);
    void setAngle(double angle);

    double getOffset() const;
    double getAmplitude() const;
    double getAngle() const;

private:

    double _phaseOffset {};
    double _amplitude {};
    double _angle {};
};