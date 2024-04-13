#include "WavHeader.h"
#include <fstream>

class WavFileManager
{
public:
    WavFileManager() = default;
    WavFileManager(const WavFileManager&) = delete;
    virtual ~WavFileManager() = default;
    WavFileManager(WavFileManager&&) = delete;
    WavFileManager& operator=(WavFileManager&&) = delete;

    void prepareFile(std::ofstream &file); // write wav header to file
    void writeAsBytes(std::ofstream &file, int value, int byteSize); // use to write wav header and audio data to file with reinterpretcast<char*>
    void finalizeFile(std::ofstream &file);
    
    int getWavHeaderSampleRate() const;
    void setWavHeaderSampleRate(int sr);
    int getWavHeaderBitDepth() const;
    void setWavHeaderBitDepth(int bd);
    void updateByteRate(int newSampleRate);
    
    int getPreAudioPosition();
    void setPreAudioPosition(std::ofstream& file);
    int getPostAudioPosition();
    void setPostAudioPosition(std::ofstream& file);

    WavHeader _wavHeader;

private:

    
    int _preAudioPosition {};
    int _postAudioPosition {};
};