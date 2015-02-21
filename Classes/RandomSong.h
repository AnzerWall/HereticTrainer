#ifndef __RandomSong_H__
#define __RandomSong_H__

#include"DataManager.h"
#define RANDOM_OLD 1
#define RANDOM_NEW 2
Song Randomize(const Song &Input, int Randomtype);
void seq(int* x);
int mRandomInt(int min,int max);
#endif