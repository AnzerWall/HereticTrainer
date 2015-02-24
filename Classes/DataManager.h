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
struct SongConfig
{
	double rate;
	bool bPlayMusic;
	double baddis;
	double gooddis;//65
	double greatdis;//35
	double perfectdis;//15
	double touchdis;
	double touchwidth;//触摸判定矩形的宽
	double touchheight;//half
	SongConfig()
	{
		rate=1.0;
		bPlayMusic=true;
		baddis = 144;
		gooddis = 120;//65
		greatdis = 80;//35
		perfectdis = 45;//15
		touchdis = 142;
		touchwidth = 80;//触摸判定矩形的宽
		touchheight = 140;//half
	}
	
	
};
struct SongInfo
{
	std::string sDisplayName;
	int iDifficulty;
	std::string sDataPath;
	std::string sMusicPath;
	std::string sTitlePath;
	std::string sBackgroundPath;
	std::string sPressButtonPath[9];
	bool bUsable;
	double dSpeed;
	double dDuration;
	SongInfo()
	{
		sDisplayName="Unknown Song";
		iDifficulty=1;
		sDataPath="";
		sMusicPath="";
		sTitlePath="DefaultTitle.png";
		sBackgroundPath="DefaultBackground.png";
		sPressButtonPath[9];
		bUsable = false;
		dSpeed=1.6;
		dDuration = 0;
		for (int i = 0; i < 9; i++)sPressButtonPath[i] = "umi.png";
	}
};
struct Song
{
	double dDuration;
	double dSpeed;
	bool bUsable;
	std::vector<Rhythm> lstRhythm;
	Song()
	{
		dDuration=0;
		dSpeed=1.6;
		bUsable = false;
	}
};
class DataManager
{
public:

	static Song loadDataFile(std::string path,SongInfo &songinfo);
	static Song getTestSong();

	static std::vector<SongInfo> getSongInfoList();
};
#endif