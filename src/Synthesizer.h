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
    void writeNotesFromUser(std::ostream& file);

    void initializeTempo();
    void updateOctave();

    std::ofstream createAudioFile();
    void prepareFile(std::ostream& file);
    void finalizeFile(std::ostream& file);
    WavFileManager _fileManager;

private:

    std::vector<float> parseNoteInput(const std::string& input);
    unsigned int parseRhythmInput(int input);
    float calculateFrequency(char note);
    unsigned int calculateNoteLength(int noteType);


    SineOscillator _osc;
    std::vector<std::pair<std::vector<float>, unsigned int>> _userInput;

    int _sampleRate;
    int _bitDepth;
    unsigned int _octave {4};
    unsigned int _tempo {};
    double _maxAmplitude;
};

void resetInputBuffer();
bool isValidRhythmInput(int rhythm);
static bool isValidNoteInput(char note);