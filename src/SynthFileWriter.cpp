#include "SynthFileWriter.h"

void SynthFileWriter::write(std::ostream& file, int value, int byteSize)
{
    file.write(reinterpret_cast<const char*>(&value), byteSize);
}