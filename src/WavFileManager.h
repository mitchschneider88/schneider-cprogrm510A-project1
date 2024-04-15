#include "WavHeader.h"
#include <fstream>

class WavFileManager
{
public:
    void prepareFile(std::ofstream &file); // write wav header to file
    static void writeAsBytes(std::ofstream &file, int value, int byteSize); // use to write wav header and audio data to file with reinterpretcast<char*>
    void finalizeFile(std::ofstream &file);
    
    void setWavHeaderSampleRate(int sr);
    void setWavHeaderBitDepth(int bd);
    void updateByteRate(int newSampleRate);
    
    void setPreAudioPosition(std::ofstream& file);
    void setPostAudioPosition(std::ofstream& file);

    WavHeader _wavHeader;

private:

    
    int _preAudioPosition {};
    int _postAudioPosition {};
};