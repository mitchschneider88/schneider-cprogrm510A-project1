#include "WavFileManager.h"
#include "SineOscillator.h"
#include <map>
#include <vector>

class Synthesizer : public WavFileManager
{
public:

    Synthesizer(int sampleRate, int bitDepth);
    std::ofstream createAudioFile();
    void initializeTempo();
    void getNotesFromUser();
    size_t calculateNoteLength(int noteType);
    float calculateFrequency(char note, int octave);
    void writeNotesFromUser(std::ofstream& file);

private:

    const int _sampleRate;
    const int _bitDepth;
    int _tempo {};
    SineOscillator osc;
    std::vector<std::pair<float, size_t>> userInput;
};