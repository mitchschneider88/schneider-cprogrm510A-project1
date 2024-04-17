#include <iostream>

// byte = 8 bits
// one char = one byte

typedef struct WavHeader
{
    std::string chunkID {"RIFF"}; // RIFF Header - 4 bytes
    std::string chunkSize {"----"}; // RIFF Chunk Size - size of file - 4 bytes
    std::string format {"WAVE"}; // WAVE Header - 4 bytes
    std::string subchunk1ID {"fmt "}; // FMT header, Subchunk1ID - 4 bytes
    int subchunk1Size {16}; // Size of the fmt chunk - 4 bytes
    int audioFormat {1}; // Audio format (1=PCM) - 2 bytes
    int numChan {1}; // Number of channels 1=Mono 2=Sterio - 2 bytes
    int sampleRate {}; // Sampling Frequency in Hz - 4 bytes                   
    int byteRate {sampleRate * numChan * (subchunk1Size / 8)}; // bytes per second (essentially bit rate, but bytes) - 4 bytes (un hard code this)
    int blockAlign {numChan * (subchunk1Size / 8)}; // 2=16-bit mono, 4=16-bit stereo  - 2 bytes 
    // used to determine how many bytes need to be available for a playback device...
    // at any given point in time, needs to be one sample of audio of all channels available to playback device
    int bitDepth {}; // Number of bits per sample - 2 bytes
    std::string subchunk2ID {"data"}; // "data" string - 4 bytes
    std::string subchunk2Size {"----"};  // Sampled data length - 4 bytes
} WavHeader;