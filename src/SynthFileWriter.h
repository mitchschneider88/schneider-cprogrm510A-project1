#include "FileWriter.h"

class SynthFileWriter : public FileWriter
{
public:
    void write(std::ostream& file, int value, int byteSize) override;
};