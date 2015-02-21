#ifndef BJSON_H_INCLUDED
#define BJSON_H_INCLUDED

#include<functional>
bool parseJson(const unsigned char*	jsonText, int jsonTextLength, std::function<void(int effect, double timing_sec, double effectval, int pos)>  fCallBack);


#endif 
