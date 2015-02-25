#ifndef __RandomSong_H__
#define __RandomSong_H__

#include"DataManager.h"
#include"cocos2d.h"
#define RANDOM_OLD 1
#define RANDOM_NEW 2
Song Randomize(const Song &Input, int Randomtype);
void seq(int* x);
int mRandomInt(int min,int max);
int SingleNoteRan_Exclude4();
void SingleNoteRan(Rhythm &current,Rhythm &LastLong, int &note_hit_count, int &note_limit_single_area, int (&a)[4], int &p);
void ParrNoteRan(Rhythm &current, Rhythm &LastLong, Rhythm &LastPar);
void LongNoteRan(Rhythm &current, Rhythm &LastLong);
#endif