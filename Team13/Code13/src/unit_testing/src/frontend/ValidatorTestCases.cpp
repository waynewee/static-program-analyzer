#include <string>
#include "ValidatorTestCases.h"
using namespace std;

extern TestCase cyclic_test_case_1 = { *(new string("procedure A {"
"call B;}"
"procedure B { call C;}"
"procedure C { call A; }"))};