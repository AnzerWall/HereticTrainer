#include"RandomSong.h"
#include"cocos2d.h"
#include<time.h>
#include<algorithm>
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
			if ((ret.lstRhythm[i].type&RHYTHMTYPE_LONG)) //��
			{
				if(ret.lstRhythm[i].beginTime > LastLong.endTime + 0.001)
				{
					if(ret.lstRhythm[i].type & RHYTHMTYPE_SAMETIME)
					{
						//��������ͬʱѺ���
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
						//��ǰ����Ϊ�����ĳ���(��ͬʱѺ���)������0-3�Լ�5-8λ���
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
					//���򽫳����浽��ǰ��������
					ret.lstRhythm[i].pos = RanPos(LastLong.pos);;
				}
				//��¼�ϸ���λ��
				if(LastLong.endTime < ret.lstRhythm[i].endTime)
				{
					LastLong = ret.lstRhythm[i];
				}
			}
			else if (ret.lstRhythm[i].type&RHYTHMTYPE_SAMETIME && !(ret.lstRhythm[i].type & RHYTHMTYPE_LONG)) //��ͬʱѺ
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
				//����Ѻ
				if (ret.lstRhythm[i].beginTime > LastLong.endTime + 0.001)
				{
					//��Ѻ���ҽ����ĸ��������
					if(note_hit_count >= note_limit_single_area)
					{
						p++;
						//��һ�����µ���������һ�εĵط�
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
					//���������
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