#include "PatrolRoute.h"
atomic<int> PatrolRoute::distinctPlacement{ 0 };
mutex PatrolRoute::mtx;
atomic<int> PatrolRoute::progress{ 0 };
mutex PatrolRoute::progressMtx;
const char PatrolRoute::face[]{ '^','>','v','<' };
const  unordered_map<char, int> PatrolRoute::faceId{
    { '^',0 },
    { '>',1 },
    { 'v',2 },
    { '<',3 }
};
const int PatrolRoute::dx[]{ -1,0,1,0 };
const int PatrolRoute::dy[]{ 0,1,0,-1 };