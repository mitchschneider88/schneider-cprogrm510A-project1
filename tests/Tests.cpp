#include "TestHarness.h"
#include "../src/Synthesizer.h"

TEST(VerifyWavHeader, WavFileManager)
{
    WavFileManager fileManager;

    const int bitDepth {16};
    const int sampleRate {48000};

    fileManager.setWavHeaderBitDepth(bitDepth);
    fileManager.setWavHeaderSampleRate(sampleRate);

    std::ofstream file;

    file.open("wavheadertest.wav", std::ios::binary);

    if (file.is_open())
    {
        fileManager.prepareFile(file);
    }

    std::ifstream testFile ("wavheadertest.wav");

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

    std::remove("wavheadertest.wav");

};

TEST(AudioFileCreation, Synthesizer)
{
    Synthesizer synth (48000, 16);

    std::istringstream input_stream("test");

    std::streambuf* orig_cin = std::cin.rdbuf(input_stream.rdbuf());

    auto audioFile {synth.createAudioFile()};

    std::cin.rdbuf(orig_cin);

    CHECK(audioFile.is_open());

    std::remove("test.wav");
}

TEST(SetTempo, Synthesizer)
{
    Synthesizer synth(48000, 16);

    std::istringstream input_stream("120\nx\n");

    std::streambuf* orig_cin = std::cin.rdbuf(input_stream.rdbuf());

    synth.getInputFromUser();

    std::cin.rdbuf(orig_cin);

    CHECK_EQUAL(120, synth.getTempo());

}

TEST(CreatesCorectNumberOfSamples, Synthesizer)
{
    Synthesizer synth(48000, 16);
    WavFileManager fileManager;

    std::istringstream input_stream("120\na\n4\nx\n");

    std::streambuf* orig_cin = std::cin.rdbuf(input_stream.rdbuf());

    synth.getInputFromUser();

    std::cin.rdbuf(orig_cin);

    std::ostringstream notes;

    fileManager.prepareFile(notes);
    synth.writeNotesFromUser(notes);

    int expectedSize {48044}; // amount of bytes that a quarter note at 120bpm equals, when each sample @ 48000hz takes up 2 bytes + the .wav header

    auto bytes {static_cast<int>(notes.tellp())};

    CHECK_EQUAL(expectedSize, bytes);
}

TEST(CreatesCorectNumberOfSamplesLongerPhrase, Synthesizer)
{
    Synthesizer synth(48000, 16);
    WavFileManager fileManager;

    std::istringstream input_stream("120\na\n4\nc\n2\ne\n1\nx\n");

    std::streambuf* orig_cin = std::cin.rdbuf(input_stream.rdbuf());

    synth.getInputFromUser();

    std::cin.rdbuf(orig_cin);

    std::ostringstream notes;

    fileManager.prepareFile(notes);
    synth.writeNotesFromUser(notes);

    int expectedSize {336044};
    // amount of bytes that a quarter note, half note, wholte note at 120bpm equals
    // when each sample @ 48000hz takes up 2 bytes + the .wav header

    auto bytes {static_cast<int>(notes.tellp())};

    CHECK_EQUAL(expectedSize, bytes);
}

TEST(CreatesCorrectPitch, Synthesizer)
{
    Synthesizer synth(48000, 16);

    std::istringstream input_stream("120\na\n4\nx\nnote\n");

    std::streambuf* orig_cin = std::cin.rdbuf(input_stream.rdbuf());

    synth.getInputFromUser();
    std::ofstream newFile {synth.createAudioFile()};
    synth.writeNotesFromUser(newFile);
    synth.finalizeFile(newFile);

    newFile.close();

    std::cin.rdbuf(orig_cin);

    std::ifstream testFile ("../../../tests/a-48kHz-16bit-120BPM-QuarterNote.wav");
    CHECK(testFile.is_open());

    std::ifstream notesFile ("note.wav");
    CHECK(notesFile.is_open());

    std::istreambuf_iterator<char> testBegin(testFile);
    std::istreambuf_iterator<char> notesBegin(notesFile);
    std::istreambuf_iterator<char> end;
    std::vector<char> testVec (testBegin, end);
    std::vector<char> notesVec (notesBegin, end);

    for (size_t i {0}; i < testVec.size(); ++i)
    {
        CHECK_EQUAL(testVec.at(i), notesVec.at(i));
    }

    std::remove("note.wav");

}

TEST(CreatesCorrectChord, Synthesizer) // this won't write the new files size for some reason
{
    Synthesizer synth(48000, 16);

    std::istringstream input_stream("120\nceg\n4\nx\nchord\n");

    std::streambuf* orig_cin = std::cin.rdbuf(input_stream.rdbuf());

    synth.getInputFromUser();
    std::ofstream newFile {synth.createAudioFile()};
    synth.writeNotesFromUser(newFile);
    synth.finalizeFile(newFile);

    newFile.close();

    std::cin.rdbuf(orig_cin);

    std::ifstream testFile ("../../../tests/ceg-48kHz-16bit-120BPM-QuarterNote.wav");
    CHECK(testFile.is_open());

    std::ifstream notesFile ("chord.wav");
    CHECK(notesFile.is_open());

    std::istreambuf_iterator<char> testBegin(testFile);
    std::istreambuf_iterator<char> notesBegin(notesFile);
    std::istreambuf_iterator<char> end;
    std::vector<char> testVec (testBegin, end);
    std::vector<char> notesVec (notesBegin, end);

    for (size_t i {0}; i < testVec.size(); ++i)
    {
        CHECK_EQUAL(testVec.at(i), notesVec.at(i));
    }

    std::remove("chord.wav");
}

TEST(HandleInvalidNote, Synthesizer)
{
    Synthesizer synth(48000, 16);

    std::string expectedPhrase {"\nWelcome to the command line synthesizer.\nPlease enter your tempo:\n"
                                "\nPlease enter your musical passage using the following notation:"
                                "\n(note(s)) (return) (rhythmic unit) (return) (note(s)) (return) (rhythmic unit) (return)... etc"
                                "\nPlease enter your note(s) as any character a through g\nTo change your octave, press o. The default octave is 4."
                                "\nPlease enter your rhythmic unit as an integer, corresponding the following definitions:"
                                "\n1 = whole note, 2 = half note, 4 = quarter note, 8 = eighth note, 16 = sixteenth note"
                                "\nPress x to stop inputting notes and generate your audio file.\n"
                                "\nEnter note(s):\n"
                                "Your input included an invalid note. Please enter one of the following notes: a b c d e f g\n"
                                "Enter rhythm: \n"
                                "Enter note(s):\n"};

    std::istringstream input_stream("120\nhfgjds4i5\na\n4\nx\n");
    std::ostringstream output_stream;

    std::streambuf* orig_cin = std::cin.rdbuf(input_stream.rdbuf());
    std::streambuf* orig_cout = std::cout.rdbuf(output_stream.rdbuf());

    synth.getInputFromUser();

    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);

    CHECK_EQUAL(expectedPhrase, output_stream.str());

}

TEST(HandleInvalidRhythm, Synthesizer)
{
    Synthesizer synth(48000, 16);

    std::string expectedPhrase {"\nWelcome to the command line synthesizer.\nPlease enter your tempo:\n"
                                "\nPlease enter your musical passage using the following notation:"
                                "\n(note(s)) (return) (rhythmic unit) (return) (note(s)) (return) (rhythmic unit) (return)... etc"
                                "\nPlease enter your note(s) as any character a through g\nTo change your octave, press o. The default octave is 4."
                                "\nPlease enter your rhythmic unit as an integer, corresponding the following definitions:"
                                "\n1 = whole note, 2 = half note, 4 = quarter note, 8 = eighth note, 16 = sixteenth note"
                                "\nPress x to stop inputting notes and generate your audio file.\n"
                                "\nEnter note(s):\n"
                                "Enter rhythm: \n"
                                "Please enter a valid rhythmic unit (1 = whole, 2 = half, 4 = quarter, 8 = eighth, 16 = sixteenth\n"
                                "Enter note(s):\n"};

    std::istringstream input_stream("120\na\n89\n4\nx\n");
    std::ostringstream output_stream;

    std::streambuf* orig_cin = std::cin.rdbuf(input_stream.rdbuf());
    std::streambuf* orig_cout = std::cout.rdbuf(output_stream.rdbuf());

    synth.getInputFromUser();

    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);

    CHECK_EQUAL(expectedPhrase, output_stream.str());

}


TEST(HandleInvalidTempo, Synthesizer)
{
    Synthesizer synth(48000, 16);

    std::string expectedPhrase {"\nWelcome to the command line synthesizer.\nPlease enter your tempo:"
                                "\nInvalid input. Please enter a tempo between 20 & 200.\n"
                                "\nPlease enter your musical passage using the following notation:"
                                "\n(note(s)) (return) (rhythmic unit) (return) (note(s)) (return) (rhythmic unit) (return)... etc"
                                "\nPlease enter your note(s) as any character a through g\nTo change your octave, press o. The default octave is 4."
                                "\nPlease enter your rhythmic unit as an integer, corresponding the following definitions:"
                                "\n1 = whole note, 2 = half note, 4 = quarter note, 8 = eighth note, 16 = sixteenth note"
                                "\nPress x to stop inputting notes and generate your audio file.\n\nEnter note(s):\n"};

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