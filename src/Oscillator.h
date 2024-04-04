#include <cmath> // use <numbers> for pi

int SAMPLE_RATE {44100};
int BIT_DEPTH {16};

class Oscillator
{
public:
    
    Oscillator(float frequency, float amplitude);
    virtual float renderAudio() = 0;
    
    void setOffset(float offset);
    void setFrequency(float frequency);
    void setAmplitude(float amplitude);
    void setAngle(float angle);
    
    float getAmplitude();
    float getAngle();
    float getOffset();

private:    

    float _frequency {};
    float _amplitude {};
    float _angle {};
    float _offset {};
};