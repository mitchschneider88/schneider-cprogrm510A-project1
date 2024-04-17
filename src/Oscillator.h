#include <cmath>

class Oscillator
{
public:
    Oscillator() = default;
    Oscillator(const Oscillator& osc) = default;
    Oscillator(Oscillator&& osc) noexcept = default;
    Oscillator& operator=(const Oscillator& osc) = default;
    Oscillator& operator=(Oscillator&& osc) noexcept = default;
    virtual ~Oscillator() = default;

    virtual double renderAudio() = 0;

    void setOffset(double offset);
    void setAmplitude(double amp);
    void setAngle(double angle);

    [[nodiscard]] double getOffset() const;
    [[nodiscard]] double getAmplitude() const;
    [[nodiscard]] double getAngle() const;

private:

    double _phaseOffset {};
    double _amplitude {};
    double _angle {};
};