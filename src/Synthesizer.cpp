#include "Synthesizer.h"

Synthesizer::Synthesizer(int sampleRate, int bitDepth)
: _sampleRate(sampleRate), _bitDepth(bitDepth)
{
    _maxAmplitude = {pow(2, _bitDepth - 1) - 1};
    _osc.setAmplitude(0.5);
}

void Synthesizer::getInputFromUser()
{
    welcomeUser();

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

        std::cout << "Enter rhythm:\n";
        std::cin >> rawRhythmInput;

        temp.second = parseRhythmInput(rawRhythmInput);

        _userInput.emplace_back(temp);
    };
}

void Synthesizer::welcomeUser()
{
    std::cout << "\nWelcome to the command line synthesizer.\n";

    initializeTempo();

    std::cout << "\nPlease enter your musical passage using the following notation:\n"
              << "(note(s)) (return) (rhythmic unit) (return) (note(s)) (return) (rhythmic unit) (return)... etc\n"
              << "Please enter your note(s) as any character a through g\n"
              << "To change your octave, press o. The default octave is 4.\n"
              << "Please enter your rhythmic unit as an integer, corresponding the following definitions:\n"
              << "1 = whole note, 2 = half note, 4 = quarter note, 8 = eighth note, 16 = sixteenth note\n"
              << "To raise or lower a note by a half step, use the . symbol to lower and the # symbol to raise\n"
              << "eg: a. = A Flat, c# = C Sharp\n"
              << "Press x to stop inputting notes and generate your audio file.\n\n";
}

std::vector<float> Synthesizer::parseNoteInput(const std::string& input)
{
    std::vector<float> frequencies;
    std::string temp {input};
    bool validFrequenciesAdded {false};

    while(!validFrequenciesAdded)
    {
        bool invalidValue {false};

        for (auto& value : temp) // validity check
        {
            char formattedValue = std::tolower(value);

            if (!isValidNoteInput(temp[0]) || (!isValidNoteInput(formattedValue) && !isValidModifier(formattedValue)))
            {
                invalidValue = true;
                break;
            }
        }

        if (!invalidValue)
        {
            for (size_t i {0}; i < temp.size(); ++i)
            {
                char value = std::tolower(temp[i]);
                char nextValue = (i < temp.size() - 1) ? temp[i + 1] : 'x';

                // Check if the current character is a valid note
                if (isValidNoteInput(value)) {
                    // Check if the next character is a valid modifier
                    if (i < temp.size() - 1 && isValidModifier(nextValue))
                    {
                        frequencies.emplace_back(calculateFrequency(value, nextValue));
                        ++i; // skip the modifier
                    }
                    else
                    {
                        frequencies.emplace_back(calculateFrequency(value, nextValue));
                    }
                }
                else
                {
                    invalidValue = true;
                    break;
                }
            }
        }

        if (invalidValue)
        {
            std::cout << "Please use only the following notes and modifiers: a b c d e f g . (flat) # (sharp)\n";
            resetInputBuffer();
            std::cin >> temp;
        }

        if (!invalidValue)
        {
            validFrequenciesAdded = true;
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
            std::cout << "Please enter a valid rhythmic unit (1 = whole, 2 = half, 4 = quarter, 8 = eighth, 16 = sixteenth\n";
            resetInputBuffer();
            std::cin >> input;
        }
    }
}

float Synthesizer::calculateFrequency(char note, char modifier)
{
    const float semitoneScalar {1.05946f};

    float freq {};

    switch (note)
    {
        case 'a':
            freq = 27.50f; // A0
            break;
        case 'b':
            freq = 30.867f; // B0
            break;
        case 'c':
            freq = 32.7037f; // C0
            break;
        case 'd':
            freq = 36.708f; // D0
            break;
        case 'e':
            freq = 41.203f; // E0
            break;
        case 'f':
            freq = 43.653f; // F0
            break;
        case 'g':
            freq = 48.999f; // G0
            break;
        default:
            freq = 0;
    }

    if (modifier == '#')
    {
        freq *= semitoneScalar;
    }
    if (modifier == '.')
    {
        freq /= semitoneScalar;
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

        if (temp <= 8)
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
           _fileManager.writeAsBytes(file, summedSamples.at(i), 2);
        }
    }

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
        prepareFile(audioFile);
    }

    return audioFile;
}

void Synthesizer::prepareFile(std::ostream& file)
{
    _fileManager.prepareFile(file);
}

void Synthesizer::finalizeFile(std::ostream &file)
{
    _fileManager.finalizeFile(file);
}

unsigned int Synthesizer::getTempo() const
{
    return _tempo;
}

unsigned int Synthesizer::getOctave() const
{
    return _octave;
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

bool isValidModifier(char modifier)
{
    const std::vector<char> validModifiers {'.', '#'};
    return std::find(validModifiers.begin(), validModifiers.end(), modifier) != validModifiers.end();
}

bool isValidRhythmInput(int rhythm)
{
    const std::vector<int> validRhythms {1, 2, 4, 8, 16};
    return std::find(validRhythms.begin(), validRhythms.end(), rhythm) != validRhythms.end();
}