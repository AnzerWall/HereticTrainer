#include"BJson.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include<functional>
#include<vector>
#include<string>
//读取lovelive所使用的json文件，实现与yajl库同，我只是把他扒出来而已，详细yajl的文档。
// ======================
//  BJSon Type
// ======================
#define BJSN_END				(0)
#define BJSN_OPEN_OBJ			(1)
#define BJSN_OPEN_ARR			(2)
#define BJSN_CLOSE_OBJ			(3)
#define BJSN_CLOSE_ARR			(4)
#define BJSN_STRING				(5)
#define BJSN_STRING_DIRECT		(6)
#define BJSN_MEMBER				(7)
#define BJSN_MEMBER_DIRECT		(8)
#define BJSN_NUMBER_I64			(9)
#define BJSN_NUMBER_DBL			(10)
#define BJSN_NUMBER_I32			(11)
#define BJSN_NUMBER_I16			(12)
#define BJSN_NUMBER_I8			(13)
#define BJSN_NUMBER_FLT			(14)
#define BJSN_CTE_TRUE			(15)
#define BJSN_CTE_FALSE			(16)
#define BJSN_CTE_NULL			(17)
#define BJSN_NUMBER_I8_RLE		(18)
#define BJSN_NUMBER_I16_RLE		(19)
#define BJSN_NUMBER_I32_RLE		(20)
#define BJSN_NUMBER_I64_RLE		(21)
#define BJSN_CTE_TRUE_RLE		(22)
#define BJSN_CTE_FALSE_RLE		(23)
bool parseJson(const unsigned char*	jsonText, int jsonTextLength, std::function<void(int effect, double timing_sec, double effectval, int pos)>  fCallBack)
{
	const unsigned char* endStream = &jsonText[jsonTextLength];
	const unsigned char* parse = jsonText;

	parse += 4;

	if ((parse[0] != 0xFF) || (parse[1] != 0xFF)) return false;
	parse += 2;
	std::vector<std::string> cp;
	// Parse Pre-CP
	{
		int strSize;
		int cnt = (parse[0] << 24) | (parse[1] << 16) | (parse[2] << 8) | parse[3];
		parse += 4;
		strSize = (parse[0] << 24) | (parse[1] << 16) | (parse[2] << 8) | parse[3];
		parse += 4;


		for (int n = 0; n < cnt; n++)
		{
			int len = (parse[0] << 24) | (parse[1] << 16) | (parse[2] << 8) | parse[3];
			parse += 4;
			std::string tmp((char*)parse, len);
			cp.push_back(tmp);
			parse += len;
		}


	}
	int vEffect = 0;
	double vTiming_sec = 0;
	double vEffectVal = 0;
	int vPos = 0;
	std::string sCurKey = "";
	while (parse < endStream) {

		switch (*parse++) {
		case BJSN_END:
			parse = endStream;
			break;
		case BJSN_OPEN_OBJ:
		{
							  int size = (parse[0] << 24) | (parse[1] << 16) | (parse[2] << 8) | parse[3];
							  parse += 4;
							  //printf("map open %u'{'\n",size);
		}
			break;
		case BJSN_OPEN_ARR:
		{
							  int size = (parse[0] << 24) | (parse[1] << 16) | (parse[2] << 8) | parse[3];
							  parse += 8; // Skip type mask
							  //printf("array open %u'['\n",size);
		}
			break;
		case BJSN_CLOSE_OBJ:
			//printf("map close '}'\n");
			fCallBack(vEffect, vTiming_sec, vEffectVal, vPos);
			break;
		case BJSN_CLOSE_ARR:
			//printf("array close ']'\n");
			break;
		case BJSN_MEMBER:
		{
							int idx = (parse[0] << 24) | (parse[1] << 16) | (parse[2] << 8) | parse[3];
							parse += 4;
							sCurKey = cp[idx];

		}
			break;
		case BJSN_NUMBER_I64:
		{
								// MSB 32/64
								int idx = (parse[0] << 24) | (parse[1] << 16) | (parse[2] << 8) | parse[3];
								unsigned int idx2 = (parse[4] << 24) | (parse[5] << 16) | (parse[6] << 8) | parse[7];
								long long int l64 = (((long long int)idx) << 32) | idx2;
								parse += 8;
								if (sCurKey == "effect")vEffect = (int)l64;
								if (sCurKey == "position")vPos = (int)l64;
								if (sCurKey == "effect_value")vEffectVal = (double)l64;

		}
			break;
		case BJSN_NUMBER_DBL:
		{
								unsigned int idx = (parse[0] << 24) | (parse[1] << 16) | (parse[2] << 8) | parse[3];
								unsigned int idx2 = (parse[4] << 24) | (parse[5] << 16) | (parse[6] << 8) | parse[7];
								unsigned long long int l64 = (((unsigned long long int)idx) << 32) | idx2;
								parse += 8;
								if (sCurKey == "timing_sec")vTiming_sec = (*((double*)&l64));
								if (sCurKey == "effect_value")vEffectVal = (*((double*)&l64));

		}
			break;
		case BJSN_NUMBER_I32:
		{
								int idx = (parse[0] << 24) | (parse[1] << 16) | (parse[2] << 8) | parse[3];
								parse += 4;
								if (sCurKey == "effect")vEffect = (int)idx;
								if (sCurKey == "position")vPos = (int)idx;
								if (sCurKey == "effect_value")vEffectVal = (double)idx;
		}
			break;
		case BJSN_NUMBER_I16:
		{
								int idx = (short)((parse[0] << 8) | parse[1]);
								idx <<= 16;
								idx >>= 16;
								parse += 2;
								if (sCurKey == "effect")vEffect = (int)idx;
								if (sCurKey == "position")vPos = (int)idx;
								if (sCurKey == "effect_value")vEffectVal = (double)idx;
		}
			break;
		case BJSN_NUMBER_I8:
		{
							   int c = *parse++;
							   c <<= 24;
							   c >>= 24;
							   if (sCurKey == "effect")vEffect = (int)c;
							   if (sCurKey == "position")vPos = (int)c;
							   if (sCurKey == "effect_value")vEffectVal = (double)c;
		}
			break;
		case BJSN_NUMBER_FLT:
		{
								int idx = (parse[0] << 24) | (parse[1] << 16) | (parse[2] << 8) | parse[3];
								parse += 4;
								// int --trick--> float -> double
								if (sCurKey == "timing_sec")vTiming_sec = (double)(*((float*)&idx));
								if (sCurKey == "effect_value")vEffectVal = (double)(*((float*)&idx));

		}
			break;

		default:
			return false;
		}
	}

	return true;
}
