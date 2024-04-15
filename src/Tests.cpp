#include "TestHarness.h"
#include "Synthesizer.h"
#include "TestFileWriter.h"



TEST(VerifyWavHeader, WavFileManager)
{
    WavFileManager fileManager;

    const int bitDepth {16};
    const int sampleRate {48000};

    fileManager.setWavHeaderBitDepth(bitDepth);
    fileManager.setWavHeaderSampleRate(sampleRate);

    std::ofstream file;

    file.open("test.wav", std::ios::binary);

    if (file.is_open())
    {
        fileManager.prepareFile(file);
    }

    std::ifstream testFile ("test.wav");

    std::vector<char> buffer(44);
    testFile.read(buffer.data(), 44);

    //Chunk ID
    CHECK_EQUAL('R', buffer.at(0));
    CHECK_EQUAL('I', buffer.at(1));
    CHECK_EQUAL('F', buffer.at(2));
    CHECK_EQUAL('F', buffer.at(3));

    //Chunk Size - Unfilled
    CHECK_EQUAL('-', buffer.at(4));
    CHECK_EQUAL('-', buffer.at(5));
    CHECK_EQUAL('-', buffer.at(6));
    CHECK_EQUAL('-', buffer.at(7));

    //Format
    CHECK_EQUAL('W', buffer.at(8));
    CHECK_EQUAL('A', buffer.at(9));
    CHECK_EQUAL('V', buffer.at(10));
    CHECK_EQUAL('E', buffer.at(11));

    //Subchunk 1 ID
    CHECK_EQUAL('f', buffer.at(12));
    CHECK_EQUAL('m', buffer.at(13));
    CHECK_EQUAL('t', buffer.at(14));
    CHECK_EQUAL(' ', buffer.at(15));

    /* little endian reconstruction of the int '16' (PCM) stored in 4 bytes, as specified by
    the canonical wav file format */
    int subchunk1Size = 0;
    for (int i = 19; i >= 16; --i)
    {
        subchunk1Size <<= 8;
        subchunk1Size |= static_cast<unsigned char>(buffer.at(i));
    }

    CHECK_EQUAL(16, subchunk1Size);

    /* little endian reconstruction of the int '1' (PCM) stored in 2 bytes, as specified by
    the canonical wav file format */
    int audioFormat = 0;
    for (int i = 21; i >= 20; --i)
    {
        audioFormat <<= 8;
        audioFormat |= static_cast<unsigned char>(buffer.at(i));
    }

    CHECK_EQUAL(1, audioFormat);

    /* little endian reconstruction of the int '1' (mono) stored in 2 bytes, as specified by
    the canonical wav file format */
    int numChannels = 0;
    for (int i = 23; i >= 22; --i)
    {
        numChannels <<= 8;
        numChannels |= static_cast<unsigned char>(buffer.at(i));
    }

    CHECK_EQUAL(1, numChannels);

    /* little endian reconstruction of the sample rate stored in 4 bytes, as specified by
    the canonical wav file format */
    int wavHeaderSampleRate = 0;
    for (int i = 27; i >= 24; --i)
    {
        wavHeaderSampleRate <<= 8;
        wavHeaderSampleRate |= static_cast<unsigned char>(buffer.at(i));
    }

    CHECK_EQUAL(sampleRate, wavHeaderSampleRate);

    /* little endian reconstruction of the byte rate stored in 4 bytes, as specified by
    the canonical wav file format*/
    int byteRate = 0;
    for (int i = 31; i >= 28; --i)
    {
        byteRate <<= 8;
        byteRate |= static_cast<unsigned char>(buffer.at(i));
    }

    CHECK_EQUAL((sampleRate * numChannels * (subchunk1Size/8)), byteRate);

    /* little endian reconstruction of the block align stored in 2 bytes, as specified by
    the canonical wav file format*/
    int blockAlign = 0;
    for (int i = 33; i >= 32; --i)
    {
        blockAlign <<= 8;
        blockAlign |= static_cast<unsigned char>(buffer.at(i));
    }

    CHECK_EQUAL(numChannels * (subchunk1Size/8), blockAlign);

    /* little endian reconstruction of the bit depth stored in 2 bytes, as specified by
    the canonical wav file format */
    int wavHeaderBitDepth = 0;
    for (int i = 35; i >= 34; --i)
    {
        wavHeaderBitDepth <<= 8;
        wavHeaderBitDepth |= static_cast<unsigned char>(buffer.at(i));
    }

    CHECK_EQUAL(bitDepth, wavHeaderBitDepth);

    // Subchunk 2 ID
    CHECK_EQUAL('d', buffer.at(36));
    CHECK_EQUAL('a', buffer.at(37));
    CHECK_EQUAL('t', buffer.at(38));
    CHECK_EQUAL('a', buffer.at(39));

    // Subchunk 2 Size - Unfilled
    CHECK_EQUAL('-', buffer.at(40));
    CHECK_EQUAL('-', buffer.at(41));
    CHECK_EQUAL('-', buffer.at(42));
    CHECK_EQUAL('-', buffer.at(43));

};

/*TEST(AudioFileCreation, Synthesizer)
{
    Synthesizer synth (48000, 16);

    std::istringstream input_stream("test");

    std::streambuf* orig_cin = std::cin.rdbuf(input_stream.rdbuf());

    auto audioFile {synth.createAudioFile()};

    std::cin.rdbuf(orig_cin);

    CHECK(audioFile.is_open());
}*/

/*TEST(ParseInput, Synthesizer)
{
    Synthesizer synth(48000, 16);

    std::istringstream input_stream("120\na\n4\nx\n");

    std::streambuf* orig_cin = std::cin.rdbuf(input_stream.rdbuf());

    synth.getInputFromUser();

    std::cin.rdbuf(orig_cin);

    auto result = getUserData(synth);

    float wholeNoteDuration = 240000.0f / synth.getTempo();
    unsigned int durationInMs = std::round(wholeNoteDuration / 4);
    unsigned int expectedDurationInSamples = (durationInMs * 48000) / 1000;

    CHECK_EQUAL(120, synth.getTempo());
    CHECK_EQUAL(440, result.at(0).first.at(0));
    CHECK_EQUAL(expectedDurationInSamples, result.at(0).second);

}*/

/*
TEST(HandleInvalidTempo, Synthesizer)
{
    Synthesizer synth(48000, 16);

    std::string expectedPhrase {"\nWelcome to the command line synthesizer.\nPlease enter your tempo:\nInvalid input. Please enter a tempo between 20 & 200.\n\nPlease enter your musical passage using the following notation:\n(note(s)) (return) (rhythmic unit) (return) (note(s)) (return) (rhythmic unit) (return)... etc\nPlease enter your note(s) as any character a through g\nTo change your octave, press o. The default octave is 4.\nPlease enter your rhythmic unit as an integer, corresponding the following definitions:\n1 = whole note, 2 = half note, 4 = quarter note, 8 = eighth note, 16 = sixteenth note 3 = eighth note triplet.\nPress x to stop inputting notes and generate your audio file.\n\nEnter note(s):\n"};

    std::istringstream input_stream("0\n120\nx\n");
    std::ostringstream output_stream;

    std::streambuf* orig_cin = std::cin.rdbuf(input_stream.rdbuf());
    std::streambuf* orig_cout = std::cout.rdbuf(output_stream.rdbuf());

    synth.getInputFromUser();

    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);

    CHECK_EQUAL(expectedPhrase, output_stream.str());
    CHECK_EQUAL(120, synth.getTempo());
}
*/

TEST(TestFileWriter, Synthesizer)
{
    auto data = std::make_unique<TestFileWriter>();

    Synthesizer synth(48000, 16, std::move(data));

    std::istringstream input_stream("120\na\n4\nx\n");

    std::streambuf* orig_cin = std::cin.rdbuf(input_stream.rdbuf());

    synth.getInputFromUser();

    std::cin.rdbuf(orig_cin);

    std::ostringstream oss;

    synth.writeNotesFromUser(oss);

    int expectedSize {48000}; // amount of bytes that a quarter note at 120bpm equals when a sample @ 48000hz takes up 2 bytes

    auto bytes {static_cast<int>(oss.tellp())};

    CHECK_EQUAL(expectedSize, bytes);
}
