#include "TestHarness.h"

int main()
{
    srand(::time_t(NULL));
    TestResult tr;
    TestRegistry::runAllTests(tr);
}
