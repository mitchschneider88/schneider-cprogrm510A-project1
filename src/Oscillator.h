#include <cmath>

class Oscillator
{
public:
    Oscillator() = default;
    Oscillator(const Oscillator& osc) = default;
    Oscillator(Oscillator&& osc) = delete;
    Oscillator& operator=(const Oscillator& osc) = delete;
    Oscillator& operator=(Oscillator&& osc) = delete;
    virtual ~Oscillator() noexcept = default;

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