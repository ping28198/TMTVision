#pragma once
//Error code
typedef long RESULT;
#define RST_OK                                ((RESULT)0L)
#define RST_FALSE                             ((RESULT)1L)
#define ERR_HANDLE_INVALID                    ((RESULT)2L) 
#define ERR_POINT_INVALID                     ((RESULT)3L) 
#define ERR_MEMORY_INVALID                    ((RESULT)4L) 
#define ERR_FILE_INVALID                      ((RESULT)5L) 
#define ERR_PATH_INVALID                      ((RESULT)6L) 
#define ERR_STREAM_INVALID                    ((RESULT)7L) 
#define ERR_FILE_TYPENOTMATCH                 ((RESULT)8L)
#define ERR_OBJECT_INVALID                    ((RESULT)9L) 
#define ERR_MEMORY_NOTMATCH                   ((RESULT)10L) 
#define ERR_PARAMETER_INVALID                 ((RESULT)11L)
//#define ERR_FILE_TYPENOTMATCH                 ((RESULT)12L) 
//Function
#ifndef WIDTHBYTES
#define WIDTHBYTES(i)                         4 * ((i + 31) / 32);
#endif
#ifndef MIN
#define MIN(i,j) i<j?i:j
#endif
#ifndef MAX
#define MAX(i,j) i<j?j:i
#endif
#ifndef MOD
#define MOD(i,j) i%j==j?0:i%j
#endif
#ifndef STR_LEN
#define STR_LEN(x)  (sizeof(x)/sizeof(x[0]))
#endif