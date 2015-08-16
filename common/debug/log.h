#ifndef _LOG_H_
#define _LOG_H_

/* 
* Normally we strip ALOGV (VERBOSE messages) from release builds.
* You can modify this (for example with "#define LOG_NDEBUG 0"
* at the top of your source file) to change that behavior.
*/
#ifndef LOG_NDEBUG
#ifdef NDEBUG
#define LOG_NDEBUG 1
#else
#define LOG_NDEBUG 0
#endif
#endif

#ifndef LOG_TAG
#define LOG_TAG 
#endif

/*
* Simplified macro to send a verbose log message using the current LOG_TAG.
*/
#include <debug.h>

#ifndef LOGV
#if LOG_NDEBUG
#define LOGV(...) 
#else
#define LOGV(...) DEBUG_VER(DEBUG_LOG_VERBOSE, LOG_TAG,##__VA_ARGS__)
#endif
#endif

#ifndef LOGI
#if LOG_NDEBUG
#define LOGI(...) 
#else
#define LOGI(...) DEBUG_INFO(DEBUG_LOG_INFO, LOG_TAG,##__VA_ARGS__)
#endif
#endif

#ifndef LOGD
#if LOG_NDEBUG
#define LOGD(...) 
#else
#define LOGD(...) DEBUG_LOG(DEBUG_LOG_DEBUG, LOG_TAG,##__VA_ARGS__)
#endif
#endif

#ifndef LOGW
#if LOG_NDEBUG
#define LOGW(...) 
#else
#define LOGW(...) DEBUG_WRN(DEBUG_LOG_WARN, LOG_TAG,##__VA_ARGS__)
#endif
#endif

#ifndef LOGE
#if LOG_NDEBUG
#define LOGE(...) 
#else
#define LOGE(...) DEBUG_ERR(DEBUG_LOG_ERROR, LOG_TAG,##__VA_ARGS__)
#endif
#endif

#endif


