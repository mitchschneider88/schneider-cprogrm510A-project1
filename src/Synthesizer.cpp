#include "Synthesizer.h"

Synthesizer::Synthesizer(int sampleRate, int bitDepth) : _sampleRate(sampleRate), _bitDepth(bitDepth)
{
    _wavHeader.bitDepth = bitDepth;
    _wavHeader.sampleRate = sampleRate;
}   

void Synthesizer::writeAsBytes(std::ofstream &file, int value, int byteSize)
{
    file.write(reinterpret_cast<const char*>(value), byteSize);
}

void Synthesizer::prepareFile(std::ofstream &file)
{
    file << _wavHeader.chunkID;
    file << _wavHeader.chunkSize;
    file << _wavHeader.format;

    // format chunk
    file << _wavHeader.subchunk1ID;
    writeAsBytes(file, _wavHeader.subchunk1Size, 4);
    writeAsBytes(file, _wavHeader.audioFormat, 2);
    writeAsBytes(file, _wavHeader.numChan, 2);
    writeAsBytes(file, _wavHeader.sampleRate, 4);
    writeAsBytes(file, _wavHeader.byteRate, 4);
    writeAsBytes(file, _wavHeader.blockAlign, 2);
    writeAsBytes(file, _wavHeader.bitDepth, 2);

    // data chunk
    file << _wavHeader.subchunk2ID;
    file << _wavHeader.subchunk2Size;
}