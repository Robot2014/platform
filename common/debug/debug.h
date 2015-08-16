#ifndef _DEBUG_H_
#define _DEUBG_H_

#include <stdio.h>

#ifndef  DEBUG_LEVEL  
#define  DEBUG_LEVEL  0
#endif

#define  DEBUG_LOG_UNKNOWN 0
#define  DEBUG_LOG_DEFAULT 1   
#define  DEBUG_LOG_VERBOSE 2
#define  DEBUG_LOG_DEBUG   3
#define  DEBUG_LOG_INFO    4
#define  DEBUG_LOG_WARN    5
#define  DEBUG_LOG_ERROR   6    

#define  DEBUG_LEVEL_INFO 0x0001
#define  DEBUG_LEVEL_WRN  0x0002
#define  DEBUG_LEVEL_ERR  0x0003
#define  DEBUG_LEVEL_RED  0x0004
#define  DEBUG_LEVEL_ALL  0x00FF

#define DEBUG_LOG(level, fmt, ...) \
    do{\
        if(level > DEBUG_LEVEL ){\
		printf("D/"##fmt##": "##__VA_ARGS__);}\
    }while(0)
        
#define DEBUG_VER(level, fmt, ...) \
    do{\
        if(level > DEBUG_LEVEL ){\
        printf("V/"##fmt##": "##__VA_ARGS__);}\
    }while(0)

#define DEBUG_INFO(level, fmt, ...) \
    do{\
        if(level > DEBUG_LEVEL){\
        printf("I/"##fmt##": "##__VA_ARGS__);}\
    }while(0)
		
#define DEBUG_WRN(level, fmt, ...) \
	do{\
		if(level > DEBUG_LEVEL){\
		printf("W/"##fmt##": "##__VA_ARGS__);}\
	}while(0)	

#define DEBUG_ERR(level, fmt, ...) \
	do{\
		if(level > DEBUG_LEVEL){\
		printf("[error][%s:%d]",__FUNCTION__,__LINE__);\
		printf("E/"##fmt##": "##__VA_ARGS__);}\
	}while(0)

#define DEBUG_RED(flag, fmt, ...) \
	do{\
		if(DEBUG_LEVEL & DEBUG_LEVEL_RED){\
		printf("[[30\33m");\
		printf(fmt##__VA_ARGS__);}\
		printf("\033[0m");\
	}while(0)	

#endif  /*  _DEBUG_  */
