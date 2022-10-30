#ifndef _SYSTEM_C1_H_
#define _SYSTEM_C1_H_

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void system_init(void);
EXTERNC void system_process(void);
EXTERNC void system_systick(void);

EXTERNC void system_timer1(void);
EXTERNC void system_dac1_dma_it(void);
EXTERNC void system_dac2_dma_it(void);

#undef EXTERNC

#endif // _SYSTEM_C1_H_
