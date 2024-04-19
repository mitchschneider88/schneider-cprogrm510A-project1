#include "Synthesizer.h"

int main()
{
    Synthesizer synth(48000, 16);

    synth.getInputFromUser();
    std::ofstream audioFile {synth.createAudioFile()};
    synth.writeNotesFromUser(audioFile);
    synth.finalizeFile(audioFile);
}