#include "WavFileManager.h"
#include "SineOscillator.h"
#include <numbers>
#include <vector>


class Synthesizer
{
public:

    Synthesizer(int sampleRate, int bitDepth);

    unsigned int getTempo();

    void getInputFromUser();
    std::ofstream createAudioFile();
    void writeNotesFromUser(std::ofstream& file);

    void initializeTempo();
    void updateOctave();

    std::vector<float> parseNoteInput(const std::string& input);
    unsigned int parseRhythmInput(int input);
    float calculateFrequency(char note);
    unsigned int calculateNoteLength(int noteType);

private:

    friend std::vector<std::pair<std::vector<float>, unsigned int>> getUserData(Synthesizer s);

    const int _sampleRate;
    const int _bitDepth;
    unsigned int _octave {4};
    unsigned int _tempo {};
    double _maxAmplitude;
    WavFileManager fileManager;

    SineOscillator _osc;
    std::vector<std::pair<std::vector<float>, unsigned int>> _userInput;
};



void resetInputBuffer();
bool isValidRhythmInput(int rhythm);
static bool isValidNoteInput(char note);