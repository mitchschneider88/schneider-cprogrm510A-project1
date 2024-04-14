#include "WavFileManager.h"
#include "SineOscillator.h"
#include <numbers>
#include <vector>


class Synthesizer : public WavFileManager
{
public:

    Synthesizer(int sampleRate, int bitDepth);
    std::ofstream createAudioFile();
    void initializeTempo();
    void getInputFromUser();
    void updateOctave();
    unsigned int calculateNoteLength(int noteType);
    float calculateFrequency(const char note);
    std::vector<float> parseNoteInput(const std::string& input);
    unsigned int parseRhythmInput(const int input);
    void writeNotesFromUser(std::ofstream& file);

private:

    const int _sampleRate;
    const int _bitDepth;
    unsigned int _octave {4};
    unsigned int _tempo {};
    double _maxAmplitude;

    SineOscillator _osc;
    std::vector<std::pair<std::vector<float>, unsigned int>> _userInput;
};