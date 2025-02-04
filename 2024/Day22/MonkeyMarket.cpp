#include "MonkeyMarket.h"
unsigned long long MonkeyMarket::threadAcc = 0;
mutex MonkeyMarket::accMutex;
map<string, int> MonkeyMarket::threadAcc2;
mutex MonkeyMarket::accMutex2;