class Oscillator
{
public:
    virtual float renderAudio() = 0;

    void setOffset(float offset);
    void setAmplitude(float amp);
    void setAngle(float angle);
    void setFrequency(float freq);


    float getOffset() const;
    float getAmplitude() const;
    float getAngle() const;
    float getFrequency() const;

private:

    float _phaseOffset {};
    float _amplitude {};
    float _angle {};
    float _frequency {};
};