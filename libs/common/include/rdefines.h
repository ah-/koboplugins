#ifndef __RDEFINES_H__
#define __RDEFINES_H__

#ifdef __cplusplus
    #ifndef restrict
        #define restrict
    #endif
#endif

#define DLL_EXPORT

#define WAIT_FOREVER   -1
#define NO_WAIT         0

#define LOWER_DEBUG_LEVEL   0
#define UPPER_DEBUG_LEVEL   9

#define ASCII_CR      '\r'
#define ASCII_LF      '\n'
#define ASCII_CRLF    "\r\n"

#endif // __RDEFINES_H__
