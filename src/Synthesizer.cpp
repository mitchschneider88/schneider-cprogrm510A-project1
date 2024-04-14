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

void Synthesizer::getNotesFromUser()
{
    std::cout << "Welcome to the command line synthesizer.\n"
              << "Please enter your musical passage using the following notation:\n"
              << "(note) (return) (rhythmic unit) (return) (note) (return) (rhythmic unit) (return)... etc\n"
              << "Please enter your note as any character A through G\n"
              << "To change your octave, press o\n"
              << "Please enter your rhythmic unit as an integer, corresponding the following definitions:\n"
              << "1 = whole note, 2 = half note, 4 = quarter note, 8 = eighth note, 16 = sixteenth note "
              << "3 = eighth note triplet.\n"
              << "Press x to stop inputting notes and generate your audio file.\n";

    while (true)
    {
        int counter {};

        char rawInput {};

        float frequencyInput {};
        size_t lengthInput {};

        std::cin >> rawInput;

        if (rawInput == 'x' || rawInput == 'X')
        {
            break;
        }

/*       if (rawInput == o)
        {

        }
*/

        else if (counter % 2)
        {
            if (rawInput == 'a' || rawInput == 'A') // add all cases
            {
                frequencyInput = calculateFrequency(rawInput, 1); // un hard code this
            }
        }

        else
        {
            if (rawInput == 1 || rawInput == 2 || rawInput == 4 || rawInput == 8 || rawInput == 16 || rawInput == 3)
            {
                lengthInput = calculateNoteLength(rawInput);
            }
        }

        userInput.emplace_back(frequencyInput, lengthInput);
        ++counter;
    };
}

float Synthesizer::calculateFrequency(char note, int octave)
{
    // convert note & octave to ms based on tempo
}

size_t Synthesizer::calculateNoteLength(int noteType)
{
    // convert noteType to ms based on tempo
}

void Synthesizer::writeNotesFromUser(std::ofstream& file)
{
    auto maxAmplitude {pow(2, _bitDepth - 1) - 1};

    for (auto& value : userInput)
    {
        osc.setFrequency(value.first);

        for (size_t i {0}; i < value.second; ++i)
        {
            auto sample {osc.renderAudio()};
            int intSample = static_cast<int>(sample * maxAmplitude);
            writeAsBytes(file, intSample, 2);
        }
    }

    setPostAudioPosition(file);
}
