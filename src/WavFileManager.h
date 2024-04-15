#include "WavHeader.h"
#include <fstream>

class WavFileManager
{
public:
    void prepareFile(std::ofstream &file); // write wav header to file
    static void writeAsBytes(std::ostream &file, int value, int byteSize); // use to write wav header and audio data to file with reinterpretcast<char*>
    void finalizeFile(std::ofstream& file);

    void setWavHeaderSampleRate(int sr);
    void setWavHeaderBitDepth(int bd);
    void updateByteRate(int newSampleRate);
    
    void setPreAudioPosition(std::ostream& file);
    void setPostAudioPosition(std::ostream& file);

    WavHeader _wavHeader; // figure out how to make this private?

private:

    int _preAudioPosition {};
    int _postAudioPosition {};
};