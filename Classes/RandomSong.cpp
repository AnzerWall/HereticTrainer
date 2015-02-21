#include"RandomSong.h"
#include"cocos2d.h"
#include<time.h>
#include<algorithm>
int RanPos(int Fixed) //长条混杂单点的时候的单点随机
{
	int ret;
	if (Fixed < 4)
	{
		ret = mRandomInt(5,8);
	}
	else if(Fixed > 4)
	{
		ret = mRandomInt(0,3);
	}
	else
	{
		int side = mRandomInt(0,1);
		if(side)
			ret = mRandomInt(0,2);
		else
			ret = mRandomInt(6,8);
	}
	return ret;
}
Song Randomize(const Song &Input, int Randomtype)
{
	//旧版随机不动长键，双键，以及长键中间的单押，其他随机0-8
	srand(time(0));
	Song ret = Input;
	if (Randomtype == RANDOM_OLD)
	{
		double lasttime = 0;
		for (int i = 0; i < ret.lstRhythm.size();i++)
		{
			if (ret.lstRhythm[i].type&RHYTHMTYPE_LONG)
			{
				lasttime = std::max(lasttime, ret.lstRhythm[i].endTime);
			}
			if (!((ret.lstRhythm[i].type&RHYTHMTYPE_LONG) || (ret.lstRhythm[i].type&RHYTHMTYPE_SAMETIME)))
			{
				if (ret.lstRhythm[i].beginTime - lasttime>0.1)
					ret.lstRhythm[i].pos = mRandomInt(0, 8);	
			}	
		}
	}
	else
	{
		//新随机，长条双押也随机，长条不随到中间
		//长条中间的单押以及短条将一定会随机到对侧
		//对单押分4个区域0-1,2-4,5-6,7-8,左2区，右2区
		//每个周期，从任一个区域开始，出圈，再随机选择对侧任意未出过圈区域，之后照常换边，当四个区域遍历之后就重新一个循环
		Rhythm LastLong = Rhythm(RHYTHMTYPE_LONG, 0, -2, -1);;
		Rhythm LastPar = Rhythm(RHYTHMTYPE_SAMETIME, 0, -1);
		double last_long_time = -1;	//上个长条结束时间
		int last_long_pos = 0;	//上个长条位置
		int last_par_pos = 9;	//上个双押开始时间
		double last_par_time = -1; 
		int note_limit_single_area = 1; //新随机的单note在一个区域中间出现次数的上限，随机设置，达到之后即重置
		int a[] = {0,0,0,0};
		seq(a);
		int p = 0;
		int note_hit_count = 0;  //已经进入当前区域的note

		for (int i = 0; i < ret.lstRhythm.size();i++)
		{
			if ((ret.lstRhythm[i].type&RHYTHMTYPE_LONG)) //长
			{
				if(ret.lstRhythm[i].beginTime > LastLong.endTime + 0.001)
				{
					if(ret.lstRhythm[i].type & RHYTHMTYPE_SAMETIME)
					{
						//长条带有同时押标记
						if(ret.lstRhythm[i].beginTime != LastPar.beginTime)	// 
						{
							if(ret.lstRhythm[i].beginTime > LastLong.endTime + 0.001)
								ret.lstRhythm[i].pos = mRandomInt(0, 8);
						}	 
						else
						{
							ret.lstRhythm[i].pos = RanPos(LastPar.pos);
						}
						LastPar = ret.lstRhythm[i];
					}
					else
					{
						//当前长条为独立的长条(无同时押标记)，则在0-3以及5-8位随机
						int side = mRandomInt(0,1);
						if(side)
						{
							ret.lstRhythm[i].pos = mRandomInt(0,3);
						}
						else
						{
						ret.lstRhythm[i].pos = mRandomInt(5,8);
						}
					}
				}
				else
				{
					//否则将长条随到当前长条对面
					ret.lstRhythm[i].pos = RanPos(LastLong.pos);;
				}
				//记录上个条位置
				if(LastLong.endTime < ret.lstRhythm[i].endTime)
				{
					LastLong = ret.lstRhythm[i];
				}
			}
			else if (ret.lstRhythm[i].type&RHYTHMTYPE_SAMETIME && !(ret.lstRhythm[i].type & RHYTHMTYPE_LONG)) //纯同时押
			{
				if(ret.lstRhythm[i].beginTime == LastPar.beginTime)
				{
					ret.lstRhythm[i].pos = RanPos(LastPar.pos);
				}
				else
				{
					ret.lstRhythm[i].pos = mRandomInt(0, 8);
				};
				LastPar = ret.lstRhythm[i];
			}
			else if (!((ret.lstRhythm[i].type&RHYTHMTYPE_LONG) || (ret.lstRhythm[i].type&RHYTHMTYPE_SAMETIME)))
			{
				//纯单押
				if (ret.lstRhythm[i].beginTime > LastLong.endTime + 0.001)
				{
					//单押左右交替四个区域随机
					if(note_hit_count >= note_limit_single_area)
					{
						p++;
						//下一次落下的区域不在上一次的地方
						if(p > 3)
						{
							p = 0;
							seq(a);
						}
					}
					switch(a[p])
					{
						case 1:ret.lstRhythm[i].pos = mRandomInt(0,1);break;
						case 2:ret.lstRhythm[i].pos = mRandomInt(2,3);break;
						case 3:ret.lstRhythm[i].pos = mRandomInt(4,6);break;
						case 4:ret.lstRhythm[i].pos = mRandomInt(7,8);break;
					
					}

			//		if(area)
			//		{
			//			ret.lstRhythm[i].pos = mRandomInt(0,4);
			//		}
			//		else
			//		{
			//			ret.lstRhythm[i].pos = mRandomInt(5,8);
			//		}
					note_hit_count++;
				}
				else
				{
					//长单随对面
					ret.lstRhythm[i].pos = RanPos(LastLong.pos);
				}	
			}		
		}
	}
	return ret;
}
int mRandomInt(int min,int max)
{
	return rand() % (max - min + 1) + min;
}
void seq(int* x)
{
	int y[4] = {1,2,3,4};
	int p = mRandomInt(0,3);
	x[0] = y[p];
	y[p] = 0;
	for(int i = 1;i <= 3; i++)
	{
		while(1)
		{
			if(x[i - 1] < 3)
			{
				p = mRandomInt(2,3);
				if(y[p])
				{
					x[i] = y[p];
					y[p] = 0;
					break;
				}
			}
			else
			{
				p = mRandomInt(0,1);	
				if(y[p])
				{
					x[i] = y[p];
					y[p] = 0;
					break;
				}
			}
		}
	}
}