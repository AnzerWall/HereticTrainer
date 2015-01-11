#ifndef __DATAMANAGEER__H__
#define __DATAMANAGEER__H__
#include<vector>

#include"cocos2d.h"
#define RHYTHMTYPE_LONG 0x00000001
//长旋律，该符号位不存在表示单次型旋律
#define RHYTHMTYPE_SAMETIME 0x00000002
//同时出现，表示该旋律与其他旋律同时点击

struct Rhythm//旋律就是那些圆环
{
	int type;
	int pos;
	double beginTime;
	double endTime;//仅在长型旋律时有效
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

	//上面两成员暂时无作用

	double duration=0;
	double speed;//注意该成员名虽然叫速度，但是他不表示速度，表示旋律从出生到到达头像处所花时间。
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