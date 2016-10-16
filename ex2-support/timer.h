#ifndef TIMER_H
#define TIMER_H

void setupTimer(uint16_t period);

void startTimer0();
void stopTimer0();

void startTimer1();
void stopTimer1();

void wait(uint16_t delay_ms);

#endif				/* TIMER_H */
