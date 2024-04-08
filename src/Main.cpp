#include "Synthesizer.h"

int main()
{
    Synthesizer synth(48000, 16);

    synth.initializeTempo();

    std::ofstream audioFile {synth.createAudioFile()};

    synth.finalizeFile(audioFile);

}