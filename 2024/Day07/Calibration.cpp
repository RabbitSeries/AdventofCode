#include "Calibration.h"
const char Calibration::operators[3] = { '+','*','|' };
atomic<int> Calibration::rowCnt{ 0 };
mutex Calibration::progress;