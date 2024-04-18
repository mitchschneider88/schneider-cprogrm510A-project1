#include "WavFileManager.h"
#include "SineOscillator.h"
#include <numbers>
#include <vector>


class Synthesizer
{
public:

    Synthesizer(int sampleRate, int bitDepth);
    Synthesizer(const Synthesizer& synth) = default;
    Synthesizer(Synthesizer&& synth) = default;
    Synthesizer& operator=(const Synthesizer& other) = default;
    Synthesizer& operator=(Synthesizer&& other) = default;
    ~Synthesizer() = default;

    void initializeTempo();
    void updateOctave();

    void welcomeUser();
    void getInputFromUser();
    [[nodiscard]] unsigned int getTempo() const;
    [[nodiscard]] unsigned int getOctave() const;

    std::ofstream createAudioFile();
    void prepareFile(std::ostream& file);
    void writeNotesFromUser(std::ostream& file);
    void finalizeFile(std::ostream& file);


    WavFileManager _fileManager;

private:

    std::vector<float> parseNoteInput(const std::string& input);
    unsigned int parseRhythmInput(int input);
    float calculateFrequency(char note, char modifier);
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
static bool isValidRhythmInput(int rhythm);
static bool isValidNoteInput(char note);
static bool isValidModifier(char modifier);
