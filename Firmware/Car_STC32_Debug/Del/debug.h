#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG_ENABLE 1

#if DEBUG_ENABLE
#define Debug_Printf printf
#else
#define Debug_Printf /##/
#endif

void Debug_Init(void);

#endif
