#include"DataManager.h"
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
#include <dirent.h>
#endif
#include"stdio.h"
#include <stdlib.h>　
#include <string.h>　
#include<string>
#include<vector>
#include<sstream>
#include"json/document.h"
#include<iostream>
#include<algorithm>
#include"LLParser.h"
#include"BJson.h"
USING_NS_CC;
std::string getPathMain(std::string path)
{
	int len = path.length();
	int i,j;
	for (i = len - 1; i >= 0; i--)if (path[i] == '\\' || path[i] == '/')break;
	for (j = len - 1; j>i; j--)if (path[j] == '.')break;
	if (j == i)return path.substr(i+1,len-i-1);
	else return path.substr(i + 1, j- i-1);
}
std::string getPathExtension(std::string path)
{
	for (int i = path.length() - 1; i >= 0; i--)
	{
		if (path[i] == '\\' || path[i] == '/')return "";
		if (path[i] == '.')
		{
			return path.substr(i + 1, path.length() - i - 1);
		}
	}
	return "";
}

SongInfo loadInfoFile(std::string path)
{
	auto sFullPath = FileUtils::getInstance()->fullPathForFilename(path);
	SongInfo ret;
	if (!FileUtils::getInstance()->isFileExist(sFullPath))return ret;
	rapidjson::Document doc;
	auto sContent = FileUtils::getInstance()->getStringFromFile(sFullPath);
	doc.Parse<0>(sContent.c_str());
	if (doc.HasParseError())return ret;
	/*
	{
		"DisplayName":"soldier game",
		"Difficulty":1,
		"Ring":"",
		"DataPath":"sghd.json",
		"MusicPath":"sg.mp3",
		"TitlePath":"sgtittle.png",
		"BackgroundPath":"bk.png",
		"Duration":130,
		"PressButtonPath1":"umi.png",
		"PressButtonPath2":"umi.png",
		"PressButtonPath3":"umi.png",
		"PressButtonPath4":"umi.png",
		"PressButtonPath5":"umi.png",
		"PressButtonPath6":"umi.png",
		"PressButtonPath7":"umi.png",
		"PressButtonPath8":"umi.png",
		"PressButtonPath9":"umi.png",
		"Speed":1.6
	}
	*/
	std::string  sPathMain = getPathMain(sFullPath);

	if (doc.HasMember("DisplayName"))
	{
		rapidjson::Value& valDisplayName = doc["DisplayName"];
		ret.sDisplayName = valDisplayName.GetString();
	}
	else ret.sDisplayName = sPathMain;

	if (doc.HasMember("Difficulty"))
	{
		rapidjson::Value& valDifficulty = doc["Difficulty"];
		ret.iDifficulty = valDifficulty.GetInt();
	}

	if (doc.HasMember("DataPath"))
	{
		rapidjson::Value& valDataPath = doc["DataPath"];
		ret.sDataPath = valDataPath.GetString();
	}
	else ret.sDataPath = sPathMain+".json";

	if (doc.HasMember("MusicPath"))
	{
		rapidjson::Value& valMusicPath = doc["MusicPath"];
		ret.sMusicPath = valMusicPath.GetString();
	}
	else ret.sMusicPath = sPathMain;

	if (doc.HasMember("TitlePath"))
	{
		rapidjson::Value& valTitlePath = doc["TitlePath"];
		if (FileUtils::getInstance()->isFileExist(valTitlePath.GetString()))
			ret.sTitlePath = valTitlePath.GetString();
		
	}

	if (doc.HasMember("BackgroundPath"))
	{
		rapidjson::Value& valBackgroundPath = doc["BackgroundPath"];

		if (FileUtils::getInstance()->isFileExist(valBackgroundPath.GetString()))
		ret.sBackgroundPath = valBackgroundPath.GetString();
	}
	//log("\n%s\n", (ret.sBackgroundPath.c_str()));
	for (int i = 1; i <= 9; i++)
	{
		if (doc.HasMember(cocos2d::String::createWithFormat("PressButtonPath%d",i)->_string.c_str()))
		{
			rapidjson::Value& valPressButtonPath = doc[cocos2d::String::createWithFormat("PressButtonPath%d", i)->_string.c_str()];
			if (FileUtils::getInstance()->isFileExist(valPressButtonPath.GetString()))
				ret.sPressButtonPath[i-1] = valPressButtonPath.GetString();
		}
	}
	if (doc.HasMember("Speed"))
	{
		rapidjson::Value& valSpeed = doc["Speed"];
		ret.dSpeed = valSpeed.GetDouble();
	}
	if (doc.HasMember("Duration"))
	{
		rapidjson::Value& valDuration = doc["Duration"];
		ret.dDuration = valDuration.GetDouble();
	}
	ret.bUsable = true;
	return ret;




}
Song loadLLpracticeJson(std::string sContent, SongInfo &songinfo)
{
	Song ret;
	rapidjson::Document doc;
	doc.Parse<0>(sContent.c_str());
	if (doc.HasParseError())return ret;
	/*
	double dDuration=0;
	bool dSpeed=1.6;
	bool bUsable = false;
	std::vector<Rhythm> lstRhythm;
	*/

	if (doc.HasMember("speed"))
	{
		rapidjson::Value& valSpeed = doc["speed"];
		ret.dSpeed = 128 / valSpeed.GetDouble();
	}

	double offsettime=0;
	if (doc.HasMember("offsettime"))
	{
		rapidjson::Value& valOffsettime = doc["offsettime"];
		offsettime = valOffsettime.GetDouble();
	}
	rapidjson::Value& valLane = doc["lane"];
	

	if (valLane.IsArray())
	{
		for (unsigned int i = 0; i < valLane.Size(); i++)
		{
			
			const rapidjson::Value &valArray = valLane[i];
			for (unsigned int j = 0; j < valArray.Size(); j++)
			{
				const rapidjson::Value &val = valArray[j];
				double begintime = val["starttime"].GetDouble();
				double endtime = val["endtime"].GetDouble();
				bool longnote = val["longnote"].GetBool();
				bool parallel = val["parallel"].GetBool();
				int lane = val["lane"].GetInt();
				Rhythm tmp;
				tmp.beginTime = (begintime + offsettime) / 1000;
				tmp.type = 0;
				if (longnote)tmp.type |= RHYTHMTYPE_LONG;
				if (parallel)tmp.type |= RHYTHMTYPE_SAMETIME;
				tmp.pos = lane;
				tmp.endTime = (endtime + offsettime) / 1000;
				ret.lstRhythm.push_back(tmp);
			}
			
		}

		sort(ret.lstRhythm.begin(), ret.lstRhythm.end());
	}
	if (!ret.lstRhythm.size())return ret;
	ret.bUsable = true;
	ret.dDuration = songinfo.dDuration;
	for (int i = 0; i < (ret.lstRhythm.size()); i++)
	{
		if (ret.lstRhythm[i].type&RHYTHMTYPE_LONG)
		{
			ret.dDuration = std::max(ret.dDuration, std::max(ret.lstRhythm[i].beginTime, ret.lstRhythm[i].endTime));
		}
		else
		{
			ret.dDuration = std::max(ret.dDuration, ret.lstRhythm[i].beginTime);
		}
	}
	ret.dDuration += 0.25;

	return ret;

}
Song loadLoveLiveJson(std::string path, char *data, unsigned len, SongInfo &songinfo)
{
	Song ret;
	ret.bUsable=decryptLLFile(path,data,len);
	if (!ret.bUsable)return ret;
	ret.bUsable = parseJson((unsigned char*)data, len, [&](int effect, double timing_sec, double effectval, int pos)->void
	{
		Rhythm rhTmp;
		if (effect == 1 || effect == 2 || effect == 4)
		{
			rhTmp.beginTime = timing_sec;
			rhTmp.pos = (9 - pos);
			rhTmp.type = 0;

			ret.lstRhythm.push_back(rhTmp);
		}
		else if (effect == 3)
		{
			rhTmp.beginTime = timing_sec;
			rhTmp.endTime = timing_sec + effectval;
			rhTmp.pos = (9 - pos);
			rhTmp.type = RHYTHMTYPE_LONG;
			ret.lstRhythm.push_back(rhTmp);
		}
		else
		{
			ret.bUsable = false;
			return;
		}
	});
	if (!ret.bUsable)return ret;
	if (!ret.lstRhythm.size())return ret;
	ret.bUsable = true;
	sort(ret.lstRhythm.begin(), ret.lstRhythm.end());
	
	for (int i = 0; i < (ret.lstRhythm.size() - 1); i++)
	{
		if (fabs(ret.lstRhythm[i].beginTime - ret.lstRhythm[i+ 1].beginTime) <= 0.001)
		{
			ret.lstRhythm[i].type |= RHYTHMTYPE_SAMETIME;
			ret.lstRhythm[i+1].type |= RHYTHMTYPE_SAMETIME;
		}
	}
	ret.dDuration = songinfo.dDuration;
	for (int i = 0; i < (ret.lstRhythm.size() ); i++)
	{
		if (ret.lstRhythm[i].type&RHYTHMTYPE_LONG)
		{
			ret.lstRhythm[i].beginTime += songinfo.dSpeed;
			ret.lstRhythm[i].endTime += songinfo.dSpeed;
			ret.dDuration = std::max(ret.dDuration,std::max(ret.lstRhythm[i].beginTime, ret.lstRhythm[i].endTime));
		}
		else
		{
			ret.lstRhythm[i].beginTime += songinfo.dSpeed;
			ret.dDuration = std::max(ret.dDuration, ret.lstRhythm[i].beginTime);
		}
	}
	ret.dDuration += 0.25;
	ret.dSpeed = songinfo.dSpeed;
	return ret;
}
Song DataManager::loadDataFile(std::string path, SongInfo &songinfo)
{

	auto sFullPath = FileUtils::getInstance()->fullPathForFilename(path);

	Song ret;
	if (!FileUtils::getInstance()->isFileExist(sFullPath))return ret;

	auto data=FileUtils::getInstance()->getDataFromFile(sFullPath);
	
	if (data.isNull())return ret;
	
	if (isLLFile(sFullPath, (char *)data.getBytes(),data.getSize()))
	{
		ret = loadLoveLiveJson(sFullPath, (char *)data.getBytes(), data.getSize(), songinfo);
	}
	else
	{
		std::string sJsonText((char *)data.getBytes(), data.getSize());
		ret = loadLLpracticeJson(sJsonText, songinfo);
	}

	return ret;

}
std::vector<std::string> getFileListAndroid(const char* path)
{
	std::vector<std::string>  ret;
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
	DIR *dp;
	struct dirent *dmsg;
	int i = 0;
	char addpath[512] = { '\0' };
	if ((dp = opendir(path)) != NULL)
	{
		while ((dmsg = readdir(dp)) != NULL)
		{
			if (!strcmp(dmsg->d_name, ".") || !strcmp(dmsg->d_name, ".."))
				continue;

			if (dmsg->d_type == DT_REG)//是否为普通文件
			{
				addpath[0] = 0;
				int len=strlen(dmsg->d_name);
				if(strstr(dmsg->d_name,".js")==dmsg->d_name+(len-3))//验证后缀
				{
					strcpy(addpath, path);
					strcat(addpath, "/");
					strcat(addpath, dmsg->d_name);
					ret.push_back(addpath);
				}
				else if(strstr(dmsg->d_name,".slist")==dmsg->d_name+(len-6))
				{
					strcpy(addpath, path);
					strcat(addpath, "/");
					strcat(addpath, dmsg->d_name);
					ret.push_back(addpath);
				}


			}
		}
	}

	closedir(dp);
#endif
	return ret;
}
std::vector<std::string> getFileListWindows(const char* path)
{
	std::vector<std::string>  ret;
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32)//待更新
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA("..\\heretictrainer\\*.js", &findFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{

			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{

				ret.push_back(std::string("..\\heretictrainer\\") + findFileData.cFileName);
			}
		} while (FindNextFileA(hFind, &findFileData));
	}

	hFind = FindFirstFileA("..\\heretictrainer\\*.slist", &findFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				ret.push_back(std::string("..\\heretictrainer\\") + findFileData.cFileName);
			}
		} while (FindNextFileA(hFind, &findFileData));
	}

#endif
	return ret;
}
std::vector<SongInfo> DataManager::getSongInfoList()
{
	std::vector<SongInfo> ret;
	std::vector<std::string> vecFileList;
	SongInfo siDefault;
	siDefault.sDisplayName= "Default";
	siDefault.sDataPath = "live_0001.json";
	siDefault.bUsable = true;
	siDefault.dSpeed = 1.6;
	ret.push_back(siDefault);
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
	if (FileUtils::getInstance()->isDirectoryExist("/sdcard/heretictrainer/"))
	{
		vecFileList=getFileListAndroid("/sdcard/heretictrainer");
	}
	else if(FileUtils::getInstance()->isDirectoryExist("/storage/emulated/1/heretictrainer/"))
	{
		vecFileList = getFileListAndroid("/storage/emulated/1/heretictrainer");
	}
	else if(FileUtils::getInstance()->isDirectoryExist("/storage/emulated/0/heretictrainer/"))
	{
		vecFileList = getFileListAndroid("/storage/emulated/0/heretictrainer");
	}
	else if (FileUtils::getInstance()->isDirectoryExist("/storage/extSdCard/heretictrainer/"))
	{
		vecFileList = getFileListAndroid("/storage/extSdCard/heretictrainer");
	}
	else if (FileUtils::getInstance()->isDirectoryExist("/mnt/sdcard/heretictrainer/"))
	{
		vecFileList = getFileListAndroid("/mnt/sdcard/heretictrainer");
	}
#endif
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32)//待更新
	vecFileList = getFileListWindows("");
#endif
	for (auto iter : vecFileList)
	{

		std::string sExt = getPathExtension(iter);
		if (sExt == "js")
		{
			SongInfo siTmp;
			if (FileUtils::getInstance()->isFileExist(iter))
			{
				siTmp.bUsable = true;
				siTmp.sDisplayName = getPathMain(iter);
				siTmp.sDataPath = siTmp.sDisplayName + ".js";
				siTmp.sMusicPath = siTmp.sDisplayName;	//扩展名由MusicPlay.java确认
				ret.push_back(siTmp);
				
			}

		}
		else if (sExt == "slist")
		{
			SongInfo siTmp = loadInfoFile(iter);
			if (siTmp.bUsable)
				ret.push_back(siTmp);
		}
	}
	return ret;
}

