#include "Synthesizer.h"

Synthesizer::Synthesizer(int sampleRate, int bitDepth) : _sampleRate(sampleRate), _bitDepth(bitDepth)
{

}

void Synthesizer::initializeTempo()
{
    std::cout << "Please enter your tempo: \n";
    std::cin >> _tempo;
}


std::ofstream Synthesizer::createAudioFile()
{
    setWavHeaderBitDepth(_bitDepth);
    setWavHeaderSampleRate(_sampleRate);
    
    std::cout << "Enter the name for your audio file: \n";

    std::string name;

    std::cin >> name;

    std::ofstream audioFile;

    audioFile.open(name + ".wav", std::ios::binary);

    if (audioFile.is_open())
    {
        prepareFile(audioFile);
    }

    return audioFile;
}

void Synthesizer::writeNotesFromUser(std::ofstream& file)
{
    auto maxAmplitude {pow(2, _bitDepth - 1) - 1};

    for (size_t i {0}; i < _sampleRate; ++i)
    {
        //auto sample {osc.renderAudio()};
        //int intSample = static_cast<int>(sample * maxAmplitude);
        //writeAsBytes(audioFile, intSample, 2);
    }

    setPostAudioPosition(file);
}
