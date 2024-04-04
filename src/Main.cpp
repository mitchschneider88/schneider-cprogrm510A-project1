#include "SineOscillator.h"
#include "WavHeader.h"
#include <fstream>

void writeAsBytes(std::ofstream &file, int value, int byteSize) // use this internally in the synthesizer class
{
    file.write(reinterpret_cast<const char*>(&value), byteSize);
}

int main()
{
    SineOscillator osc(440, 0.5);
    WavHeader wavHeader;

    std::ofstream audioFile;
    
    audioFile.open("waveform.wav", std::ios::binary);

    if (audioFile.is_open())
    {
                // header chunk
        audioFile << wavHeader.chunkID;
        audioFile << wavHeader.chunkSize;
        audioFile << wavHeader.format;

        // format chunk
        audioFile << wavHeader.subchunk1ID;
        writeAsBytes(audioFile, wavHeader.subchunk1Size, 4);
        writeAsBytes(audioFile, wavHeader.audioFormat, 2);
        writeAsBytes(audioFile, wavHeader.numChan, 2);
        writeAsBytes(audioFile, wavHeader.sampleRate, 4);
        writeAsBytes(audioFile, wavHeader.byteRate, 4);
        writeAsBytes(audioFile, wavHeader.blockAlign, 2);
        writeAsBytes(audioFile, wavHeader.bitDepth, 2);

        // data chunk
        audioFile << wavHeader.subchunk2ID;
        audioFile << wavHeader.subchunk2Size;

        int preAudioPosition {static_cast<int>(audioFile.tellp())};
    
        auto maxAmplitude {pow(2, BIT_DEPTH - 1) - 1};

        for (size_t i {0}; i < SAMPLE_RATE; ++i)
        {
            auto sample {osc.renderAudio()};
            int intSample = static_cast<int>(sample * maxAmplitude);
            writeAsBytes(audioFile, intSample, 2);
        }

        int postAudioPosition {static_cast<int>(audioFile.tellp())};

        audioFile.seekp(preAudioPosition - 4); // seeking to empty 4 bytes after subchunk2ID

        writeAsBytes(audioFile, postAudioPosition - preAudioPosition, 4);
        audioFile.seekp(4, std::ios::beg); // seeking to empty 4 bytes after first 4 bytes of the file
        writeAsBytes(audioFile, postAudioPosition - 8, 4); 
        // this is writing size of header file, which is the size of the file minus 8 bytes to store ID and size of header file

        audioFile.close();
    }

}