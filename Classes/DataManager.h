#ifndef __DATAMANAGEER__H__
#define __DATAMANAGEER__H__
#include<vector>

#include"cocos2d.h"
#define RHYTHMTYPE_LONG 0x00000001
//�����ɣ��÷���λ�����ڱ�ʾ����������
#define RHYTHMTYPE_SAMETIME 0x00000002
//ͬʱ���֣���ʾ����������������ͬʱ���

struct Rhythm//���ɾ�����ЩԲ��
{
	int type;
	int pos;
	double beginTime;
	double endTime;//���ڳ�������ʱ��Ч
	Rhythm(){ this->type = 0; this->beginTime = 0; };
	Rhythm(int type, int pos, double beginTime)
	{
		this->pos = pos;
		this->type = type;
		this->beginTime = beginTime;
		this->endTime = 0;
	};
	Rhythm(int type, int pos, double beginTime,  double endTime)
	{
		this->pos = pos;
		this->type = type;
		this->beginTime = beginTime;
		this->endTime = endTime;

	}
	bool operator<(const Rhythm&that) const
	{
		return this->beginTime < that.beginTime;
	}

};
struct Song
{
	std::string name="unknown song";
	bool ok = false;

	//��������Ա��ʱ������

	double duration=0;
	double speed;//ע��ó�Ա����Ȼ���ٶȣ�����������ʾ�ٶȣ���ʾ���ɴӳ���������ͷ������ʱ�䡣
	std::vector<Rhythm> data;
};
class SongManager
{
public:
	static Song load(const char *file);
	static Song loadjson(const char *file);
	static Song testData();
	static std::vector<std::string> getFileList();
};
#endif