#include "FileWriter.h"

class TestFileWriter : public FileWriter
{
    void write(std::ostream& file, int value, int byteSize) override;
};