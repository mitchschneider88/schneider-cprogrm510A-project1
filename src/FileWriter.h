#include <fstream>
#include <vector>
#pragma once

class FileWriter
{
public:
    // implement rest of 5
    virtual ~FileWriter() = default;
    virtual void write(std::ostream& file, int value, int byteSize) = 0;
};