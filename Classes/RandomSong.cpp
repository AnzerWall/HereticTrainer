#include"RandomSong.h"
#include"cocos2d.h"
#include<time.h>
#include<algorithm>
#include<stdio.h>
int RanPos(int Fixed) //�������ӵ����ʱ��ĵ������
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
	return ret;
}
Song Randomize(const Song &Input, int Randomtype)
{
	//�ɰ��������������˫�����Լ������м�ĵ�Ѻ���������0-8
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
		//�����������˫ѺҲ������������浽�м�
		//�����м�ĵ�Ѻ�Լ�������һ����������Բ�
		//�Ե�Ѻ��4������0-1,2-4,5-6,7-8,��2������2��
		//ÿ�����ڣ�����һ������ʼ����Ȧ�������ѡ��Բ�����δ����Ȧ����֮���ճ����ߣ����ĸ��������֮�������һ��ѭ��
		Rhythm LastLong = Rhythm(RHYTHMTYPE_LONG, 0, -2, -1);;
		Rhythm LastPar = Rhythm(RHYTHMTYPE_SAMETIME, 0, -1);
		double last_long_time = -1;	//�ϸ���������ʱ��
		int last_long_pos = 0;	//�ϸ�����λ��
		int last_par_pos = 9;	//�ϸ�˫Ѻ��ʼʱ��
		double last_par_time = -1; 
		int note_limit_single_area = 1; //������ĵ�note��һ�������м���ִ��������ޣ�������ã��ﵽ֮������
		int a[] = {0,0,0,0};
		seq(a);
		int p = 0;
		int note_hit_count = 0;  //�Ѿ����뵱ǰ�����note

		for (int i = 0; i < ret.lstRhythm.size();i++)
		{
			if ((ret.lstRhythm[i].type&RHYTHMTYPE_LONG && !(ret.lstRhythm[i].type & RHYTHMTYPE_SAMETIME) )) //���ⳤ
			{
				LongNoteRan(ret.lstRhythm[i], LastLong);
			}
			else if (ret.lstRhythm[i].type&RHYTHMTYPE_SAMETIME) //ͬʱѺ
			{
				ParrNoteRan(ret.lstRhythm[i], LastLong, LastPar);
			}
			else if (!((ret.lstRhythm[i].type&RHYTHMTYPE_LONG) || (ret.lstRhythm[i].type&RHYTHMTYPE_SAMETIME)))
			{
				//��Ѻ
				SingleNoteRan(ret.lstRhythm[i],LastLong, note_hit_count, note_limit_single_area, a, p);	
			}	


		}
	}
	return ret;
}
void ParrNoteRan(Rhythm &current, Rhythm &LastLong, Rhythm &LastPar)
{
	current.pos = RanPos(LastPar.pos);
	if (current.type & RHYTHMTYPE_LONG)
	{
		if(LastLong.endTime < current.beginTime)
		{				

			LastLong = current;
		}
	}
	LastPar = current;
}
void SingleNoteRan(Rhythm &current,Rhythm &LastLong, int &note_hit_count, int &note_limit_single_area, int (&a)[4], int &p)
{
	//����Ѻ
	if (current.beginTime > LastLong.endTime + 0.001)
	{
			//��Ѻ���ҽ����ĸ��������
			if(note_hit_count >= note_limit_single_area)
			{
				p++;
			//��һ�����µ���������һ�εĵط�
			if(p > 3)
			{
				p = 0;
				seq(a);					}
			}
			switch(a[p])
			{
				case 1:current.pos = mRandomInt(0,1);break;
				case 2:current.pos = mRandomInt(2,3);break;
				case 3:current.pos = mRandomInt(4,6);break;
				case 4:current.pos = mRandomInt(7,8);break;	
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
			{					//���������
				current.pos = RanPos(LastLong.pos);
			}
}
int mRandomInt(int min,int max)
{
	return rand() % (max - min + 1) + min;
}
void LongNoteRan(Rhythm &current, Rhythm &LastLong)
{
	if(current.beginTime > LastLong.endTime + 0.001)
	{
		current.pos = SingleNoteRan_Exclude4();;
	}
	else
	{
		//���򽫳����浽��ǰ��������
		current.pos = RanPos(LastLong.pos);;
		
			//debug msgs
				
				CCLOG("CurrentLong Pos 5, start from %f, end to %f \n", current.beginTime, current.endTime);
				CCLOG("Lastlong Pos %d, from %f, end to %f\n",LastLong.pos, LastLong.beginTime, LastLong.endTime);
	//			CCLOG("LastPar at %d, begin %f", LastPar.pos, LastPar.beginTime);
			//////
	}
	//��¼�ϸ���λ��
	if(LastLong.endTime < current.endTime)
	{
		LastLong = current;
	}
}
int SingleNoteRan_Exclude4()
{
	int ret;
	int side = mRandomInt(0,1);
	if(side)
	{
		ret = mRandomInt(0,3);
	}
	else
	{
		ret = mRandomInt(5,8);
	}
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