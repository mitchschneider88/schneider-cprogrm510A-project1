#include "WavFileManager.h"
#include "SineOscillator.h"
#include <numbers>
#include <vector>
#include "SynthFileWriter.h"


class Synthesizer
{
public:

    Synthesizer(int sampleRate, int bitDepth, std::unique_ptr<FileWriter> writer);

    unsigned int getTempo();

    void getInputFromUser();
    std::ofstream createAudioFile();
    void writeNotesFromUser(std::ostream& file);

    void initializeTempo();
    void updateOctave();
    void finalizeFile(std::ofstream& file);

private:

    std::vector<float> parseNoteInput(const std::string& input);
    unsigned int parseRhythmInput(int input);
    float calculateFrequency(char note);
    unsigned int calculateNoteLength(int noteType);

    const int _sampleRate;
    const int _bitDepth;
    unsigned int _octave {4};
    unsigned int _tempo {};
    double _maxAmplitude;

    WavFileManager _fileManager;
    std::unique_ptr<FileWriter> _writer;

    SineOscillator _osc;
    std::vector<std::pair<std::vector<float>, unsigned int>> _userInput;
};

void resetInputBuffer();
bool isValidRhythmInput(int rhythm);
static bool isValidNoteInput(char note);