#include "SineOscillator.h"
#include <fstream>
#include "WavHeader.h"

class Synthesizer
{
public:
    Synthesizer(int sampleRate, int bitDepth);
    // initialize wavHeader with correct sample rate / bit depth, etc, then prepare file with wavHeader, then accept audio data

    void writeAsBytes(std::ofstream &file, int value, int byteSize); // use to write wav header and audio data to file with reinterpretcast<char*>
    void prepareFile(std::ofstream &file); // write wav header to file
    void addNote(char noteName, int time); // use to write audio data to file

private:

    SineOscillator _osc {440, 0.5};
    WavHeader _wavHeader;
    const int _sampleRate;
    const int _bitDepth;
};