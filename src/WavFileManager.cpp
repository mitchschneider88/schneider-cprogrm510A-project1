#include "WavFileManager.h"

void WavFileManager::prepareFile(std::ofstream& file)
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
    setPreAudioPosition(file);
}

void WavFileManager::writeAsBytes(std::ofstream& file, int value, int byteSize)
{
    file.write(reinterpret_cast<const char*>(&value), byteSize);
}

void WavFileManager::finalizeFile(std::ofstream& file)
{
    file.seekp(_preAudioPosition - 4); // seeking to empty 4 bytes after subchunk2ID

    writeAsBytes(file, _postAudioPosition - _preAudioPosition, 4);
    
    file.seekp(4, std::ios::beg); // seeking to empty 4 bytes after first 4 bytes of the file
    
    writeAsBytes(file, _postAudioPosition - 8, 4); 
    // this is writing size of header file, which is the size of the file minus 8 bytes to store ID and size of header file

    file.close();
}

int WavFileManager::getWavHeaderSampleRate() const
{
    return _wavHeader.sampleRate;
}
void WavFileManager::setWavHeaderSampleRate(int sr)
{
    _wavHeader.sampleRate = sr;
    updateByteRate(sr);
}

void WavFileManager::updateByteRate(int newSampleRate)
{
    _wavHeader.byteRate = (newSampleRate * _wavHeader.numChan * (_wavHeader.subchunk1Size / 8));
}

int WavFileManager::getWavHeaderBitDepth() const
{
    return _wavHeader.bitDepth;
}
void WavFileManager::setWavHeaderBitDepth(int bd)
{
    _wavHeader.bitDepth = bd;
}

int WavFileManager::getPreAudioPosition()
{
    return _preAudioPosition;
}
void WavFileManager::setPreAudioPosition(std::ofstream& file)
{
    _preAudioPosition = {static_cast<int>(file.tellp())};
}
int WavFileManager::getPostAudioPosition()
{
    return _postAudioPosition;
}
void WavFileManager::setPostAudioPosition(std::ofstream& file)
{
    _postAudioPosition = {static_cast<int>(file.tellp())};
}