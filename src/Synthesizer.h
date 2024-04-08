#include "WavFileManager.h"
#include <math.h>

class Synthesizer : public WavFileManager
{
public:
    Synthesizer(int sampleRate, int bitDepth);
    std::ofstream createAudioFile();
    void initializeTempo();
    void writeNotesFromUser(std::ofstream& file);
    float calculateNoteLength();

private:

    const int _sampleRate;
    const int _bitDepth;
    int _tempo {};
};