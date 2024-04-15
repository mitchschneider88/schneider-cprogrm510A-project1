#include "TestFileWriter.h"

void TestFileWriter::write(std::ostream& file, int value, int byteSize)
{
    file.write(reinterpret_cast<const char*>(&value), byteSize);
}