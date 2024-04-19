#include "WavHeader.h"
#include <fstream>

class WavFileManager
{
public:
    void prepareFile(std::ostream &file);
    void writeAsBytes(std::ostream &file, long long value, int byteSize);
    void finalizeFile(std::ostream& file);

    void setWavHeaderSampleRate(int sr);
    void setWavHeaderBitDepth(int bd);
    void updateByteRate(int newSampleRate);
    
    void setPreAudioPosition(std::ostream& file);
    void setPostAudioPosition(std::ostream& file);

private:

    WavHeader _wavHeader;
    std::streamoff _preAudioPosition {};
    std::streamoff _postAudioPosition {};
};