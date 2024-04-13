#include "Synthesizer.h"
#include "TestHarness.h"

int main()
{
    // random number generator used in some tests
    srand(::time_t(NULL));
    TestResult tr;
    TestRegistry::runAllTests(tr);

    /*Synthesizer synth(48000, 16);

    synth.initializeTempo();

    std::ofstream audioFile {synth.createAudioFile()};

    synth.finalizeFile(audioFile);*/

}