#include "WavHeader.h"
#include <fstream>

class WavFileManager
{
public:
    WavFileManager() = default;
    virtual ~WavFileManager() = default;

    void prepareFile(std::ofstream &file); // write wav header to file
    void writeAsBytes(std::ofstream &file, int value, int byteSize); // use to write wav header and audio data to file with reinterpretcast<char*>
    void finalizeFile(std::ofstream &file);
    
    int getWavHeaderSampleRate();
    void setWavHeaderSampleRate(int sr);
    int getWavHeaderBitDepth();
    void setWavHeaderBitDepth(int bd);
    
    int getPreAudioPosition();
    void setPreAudioPosition(std::ofstream& file);
    int getPostAudioPosition();
    void setPostAudioPosition(std::ofstream& file);

    WavHeader _wavHeader;
private:

    
    int _preAudioPosition {};
    int _postAudioPosition {};
};