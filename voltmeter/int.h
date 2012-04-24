#include <avr/interrupt.h>
#ifndef _INTERRUPT_H
#define _INTERRUPT_H 1
#define EICRA_INIT (1<<ISC01)
#define EIMSK_INIT (1<<INT0)
void int_init(void);
#endif
