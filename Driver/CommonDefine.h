#ifndef __COMMONDEFINE_H
#define __COMMONDEFINE_H
//½Ç¶È»¡¶È»¥»»
#define M_PI  (float)3.1415926535f
#define Ang2Rad(m)  (m/180.0f*M_PI)
#define Rad2Ang(n)  (n/M_PI*180.f)


#define VAL_LIMIT(val, min, max)\
if(val<=min)\
{\
	val = min;\
}\
else if(val>=max)\
{\
	val = max;\
}\


#define ABS_VALUE(value)\
if(value>=0)\
{\
	value = value;\
}\
else if(value<0)\
{\
	value=-value;\
}\

#define false 0
#define true 1

#define M_E 2.718281828f



#endif
