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
USING_NS_CC;
Song SongManager::loadjson(const char *file)
{
	//该方法缺少验证，json出错时会崩溃
	auto fullFilePath = FileUtils::getInstance()->fullPathForFilename(file);
	Song ret;

	if (!FileUtils::getInstance()->isFileExist(fullFilePath))return ret;
	
	auto contentStr = FileUtils::getInstance()->getStringFromFile(fullFilePath);
	//log("%s",contentStr.c_str());
	rapidjson::Document doc;

	doc.Parse<0>(contentStr.c_str());
	if (doc.HasParseError())return ret;
	rapidjson::Value& valSpeed = doc["speed"];
	rapidjson::Value& valOffsettime = doc["offsettime"];
	rapidjson::Value& valLane = doc["lane"];
	rapidjson::Value& valAudiofile = doc["audiofile"];
	ret.ok = true;
	ret.name = valAudiofile.GetString();
	ret.speed = 128 / valSpeed.GetDouble();
	ret.duration = 0;
	double offsettime = valOffsettime.GetDouble();

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
				ret.data.push_back(tmp);
			}
			
		}

			
	}
	sort(ret.data.begin(), ret.data.end());
	if (ret.data.size() > 0);
	ret.duration = ret.data[ret.data.size() - 1].beginTime+2.0;
	return ret;

}
Song SongManager::load(const char*file)
{
	std::string str = FileUtils::getInstance()->getStringFromFile(file);
	std::stringstream buf; buf.str(str);
	//读取原格式slist
	Song ret;

	int ver;
	buf >> ver;
	if (ver != 2951){  return ret; }//2951是魔数随便定的
	if (buf >> ret.duration >> ret.speed)
	{
		ret.ok = true;
	}

	Rhythm tmp;
	while (buf >> tmp.type >> tmp.pos)
	{
		
		if (tmp.type&RHYTHMTYPE_LONG)
		{
			buf  >> tmp.beginTime >> tmp.endTime;
		}
		else 
		{
			buf >> tmp.beginTime;
		}
		ret.data.push_back(tmp);
	}

	return ret;
}
Song SongManager::testData()
{
	Song ret;
	ret.name = "Test";
	ret.duration = 13;
	ret.ok = true;
	ret.speed = 1;
	ret.data.push_back(Rhythm(0,1,1));
	ret.data.push_back(Rhythm(0, 1, 2));
	ret.data.push_back(Rhythm(0, 1, 3));
	ret.data.push_back(Rhythm(0, 1, 4));
	ret.data.push_back(Rhythm(0, 1, 5));
	ret.data.push_back(Rhythm(0, 1, 6));
	return ret;
}

std::vector<std::string> getList(const char* path)
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
				if(strstr(dmsg->d_name,".json")==dmsg->d_name+(len-5))//验证后缀
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

std::vector<std::string>  SongManager::getFileList()
{
	std::vector<std::string> ret;

#if (CC_TARGET_PLATFORM ==CC_PLATFORM_ANDROID)
	if (FileUtils::getInstance()->isDirectoryExist("/sdcard/heretictrainer/"))
	{
		ret=getList("/sdcard/heretictrainer");
	}
	else if(FileUtils::getInstance()->isDirectoryExist("/storage/emulated/1/heretictrainer/"))
	{
		ret = getList("/storage/emulated/1/heretictrainer");
	}
	else if(FileUtils::getInstance()->isDirectoryExist("/storage/emulated/0/heretictrainer/"))
	{
		ret = getList("/storage/emulated/0/heretictrainer");
	}
	else if (FileUtils::getInstance()->isDirectoryExist("/storage/extSdCard/heretictrainer/"))
	{
		ret = getList("/storage/extSdCard/heretictrainer");
	}
	else if (FileUtils::getInstance()->isDirectoryExist("/mnt/sdcard/heretictrainer/"))
	{
		ret = getList("/mnt/sdcard/heretictrainer");
	}
#endif
#if (CC_TARGET_PLATFORM ==CC_PLATFORM_WIN32)//待更新
	std::string filedata;
	if(!FileUtils::getInstance()->isFileExist("slist.txt"))return ret;

	filedata = FileUtils::getInstance()->getStringFromFile("slist.txt");
	std::stringstream buf; buf.str(filedata);
	std::string str;
	while (std::getline(buf, str))
	{
		auto pos = str.find("\n");
		if (pos != std::string::npos)str.erase(pos, str.size()-pos);
		pos = str.find("\r");
		if (pos != std::string::npos)str.erase(pos, str.size() - pos);
		if (FileUtils::getInstance()->isFileExist(str+".slist"))
			ret.push_back(FileUtils::getInstance()->fullPathForFilename(str + ".slist"));
	}
#endif
	return ret;
}

