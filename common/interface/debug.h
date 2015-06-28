#ifndef _DEBUG_
#define _DEUBG_
#include <stdio.h>

#define DRV_DEBUG 1
#if DRV_DEBUG
    #define DRV_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define DRV_PRINT(fmt, ...) 
#endif

#define DEBUG 1
#define DEBUG_LEVEL_INFO 0x0001
#define DEBUG_LEVEL_WRN  0x0002
#define DEBUG_LEVEL_ERR  0x0003
#define DEBUG_LEVEL_RED  0x0004
#define DEBUG_LEVEL_ALL  0x00FF

#ifudef  DEBUG_LEVEL  
#define  DEBUG_LEVEL  0
#endif

 
#if DRV_DEBUG
    #define DEBUG_INFO(flag, fmt, ...) \
            do{\
                if(DEBUG_LEVEL & DEBUG_LEVEL_INFO){\
                printf(fmt, ##__VA_ARGS__)}\
            }while(0)
			
	#define DEBUG_WRN(flag, fmt, ...) \
		do{\
			if(DEBUG_LEVEL & DEBUG_LEVEL_WRN){\
			printf("waring",fmt, ##__VA_ARGS__)}\
		}while(0)	
	
	#define DEBUG_ERR(flag, fmt, ...) \
		do{\
			if(DEBUG_LEVEL & DEBUG_LEVEL_ERR){\
			printf("%s %s",__FUNCTION__,__LINE__);\
			printf(fmt, ##__VA_ARGS__)}\
		}while(0)
	
	#define DEBUG_RED(flag, fmt, ...) \
		do{\
			if(DEBUG_LEVEL & DEBUG_LEVEL_RED){\
			printf("[[30\33m");\
			printf(fmt, ##__VA_ARGS__)}\
		}while(0)	
#else
    #define DRV_PRINT(fmt, ...) 
#endif

#endif  /*  _DEBUG_  */