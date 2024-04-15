#include "Synthesizer.h"

Synthesizer::Synthesizer(int sampleRate, int bitDepth, std::unique_ptr<FileWriter> writer)
: _sampleRate(sampleRate), _bitDepth(bitDepth), _writer(std::move(writer))
{
    _maxAmplitude = {pow(2, _bitDepth - 1) - 1};
    _osc.setAmplitude(0.5);
}

void Synthesizer::getInputFromUser()
{
    std::cout << "\nWelcome to the command line synthesizer.\n";

    initializeTempo();

    std::cout << "\nPlease enter your musical passage using the following notation:\n"
              << "(note(s)) (return) (rhythmic unit) (return) (note(s)) (return) (rhythmic unit) (return)... etc\n"
              << "Please enter your note(s) as any character a through g\n"
              << "To change your octave, press o. The default octave is 4.\n"
              << "Please enter your rhythmic unit as an integer, corresponding the following definitions:\n"
              << "1 = whole note, 2 = half note, 4 = quarter note, 8 = eighth note, 16 = sixteenth note "
              << "3 = eighth note triplet.\n"
              << "Press x to stop inputting notes and generate your audio file.\n\n";

    while (true)
    {
        std::string rawInput {};
        int rawRhythmInput {};

        std::pair<std::vector<float>, unsigned int> temp;

        std::cout << "Enter note(s):\n";
        std::cin >> rawInput;

        if (std::tolower(rawInput.at(0)) == 'x' && rawInput.length() == 1)
        {
            break;
        }

        if (std::tolower(rawInput.at(0)) == 'o' && rawInput.length() == 1)
        {
            updateOctave();
            continue;
        }

        temp.first = parseNoteInput(rawInput);

        std::cout << "Enter rhythm: \n";
        std::cin >> rawRhythmInput;

        temp.second = parseRhythmInput(rawRhythmInput);

        _userInput.emplace_back(temp);
    };
}

std::vector<float> Synthesizer::parseNoteInput(const std::string& input) // fix error handling
{
    std::vector<float> frequencies;
    std::string temp {input};

    while(true)
    {
        bool invalidNote {false};

        for (auto& note : temp) // validity check
        {
            char formattedNote = std::tolower(note);

            if (!isValidNoteInput(formattedNote))
            {
                invalidNote = true;
                break;
            }
        }

        if (!invalidNote)
        {
            for (auto& note : temp) {
                frequencies.emplace_back(calculateFrequency(note));
            }
            break;
        }

        else
        {
            std::cout << "Your input included an invalid note. Please enter one of the following notes: a b c d e f g\n";
            resetInputBuffer();
            std::cin >> temp;
        }
    }

    return frequencies;
}

unsigned int Synthesizer::parseRhythmInput(int input)
{
    while (true)
    {
        if (isValidRhythmInput(input))
        {
            return calculateNoteLength(input);
        }
        else
        {
            std::cout << "Please enter a valid rhythmic unit (1 = whole, 2 = half, 4 = quarter, 8 = eighth, 16 = sixteenth, 3 = triplet\n";
            resetInputBuffer();
            std::cin >> input;
        }
    }
}

float Synthesizer::calculateFrequency(char note)
{
    float freq {};

    switch (note)
    {
        case 'a':
            freq = 27.50f; // A4 (concert pitch)
            break;
        case 'b':
            freq = 30.867f; // B4
            break;
        case 'c':
            freq = 32.7037f; // C4
            break;
        case 'd':
            freq = 36.708f; // D4
            break;
        case 'e':
            freq = 41.203f; // E4
            break;
        case 'f':
            freq = 43.653f; // F4
            break;
        case 'g':
            freq = 48.999f; // G4
            break;
        default:
            freq = 0;
    }


    freq *= std::pow(2.0f, _octave);
    return freq;
}

unsigned int Synthesizer::calculateNoteLength(int noteType)
{
    float wholeNoteDuration = 240000.0f / _tempo;

    unsigned int durationInMs = std::round(wholeNoteDuration / noteType);

    unsigned int durationInSamples = (durationInMs * _sampleRate) / 1000;

    return durationInSamples;
}

void Synthesizer::updateOctave()
{
    unsigned int temp;
    std::cout << "Please enter an octave between 0 - 8\n";

    while (true)
    {
        std::cin >> temp;

        if (temp >= 0 && temp <= 8)
        {
            break;
        }
        else
        {
            std::cout << "Invalid input. Please enter an octave between 0 - 8\n";
            resetInputBuffer();
        }
    }

    _octave = temp;
}

void Synthesizer::initializeTempo()
{
    int input {};
    std::cout << "Please enter your tempo:\n";

    while (true)
    {
        std::cin >> input;

        if (input >= 20 && input <= 200)
        {
            _tempo = input;
            break;
        }
        else
        {
            std::cout << "Invalid input. Please enter a tempo between 20 & 200.\n";
            resetInputBuffer();
        }
    }
}

void Synthesizer::writeNotesFromUser(std::ostream& file)
{
    for (auto& pair : _userInput)
    {
        std::vector<int> summedSamples(pair.second, 0);

        for (const auto& note : pair.first)
        {
            _osc.setOffset(2 * std::numbers::pi * note / _sampleRate);

            for (size_t i {0}; i < pair.second; ++i)
            {
                auto sample {_osc.renderAudio()};
                summedSamples.at(i) += static_cast<int>(sample * (_maxAmplitude / pair.first.size())); // scaling output in order to not clip
            }
        }

        for (size_t i {0}; i < pair.second; ++i)
        {
            _writer->write(file, summedSamples.at(i), 2);
        }
    }

    _fileManager.setPostAudioPosition(file);

}

void Synthesizer::finalizeFile(std::ofstream &file)
{
    _fileManager.finalizeFile(file);
}

std::ofstream Synthesizer::createAudioFile()
{
    _fileManager.setWavHeaderBitDepth(_bitDepth);
    _fileManager.setWavHeaderSampleRate(_sampleRate);

    std::cout << "Enter the name for your audio file: \n";

    std::string name;

    std::cin >> name;

    std::ofstream audioFile;

    audioFile.open(name + ".wav", std::ios::binary);

    if (audioFile.is_open())
    {
        _fileManager.prepareFile(audioFile);
    }

    return audioFile;
}

unsigned int Synthesizer::getTempo()
{
    return _tempo;
}

void resetInputBuffer()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool isValidNoteInput(char note)
{
    const std::vector<char> validNotes {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    return std::find(validNotes.begin(), validNotes.end(), note) != validNotes.end();
}

bool isValidRhythmInput(int rhythm)
{
    const std::vector<int> validRhythms {1, 2, 4, 8, 16, 3};
    return std::find(validRhythms.begin(), validRhythms.end(), rhythm) != validRhythms.end();
}
