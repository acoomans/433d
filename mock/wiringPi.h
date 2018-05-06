#ifndef INC_433_WIRINGPI_H
#define INC_433_WIRINGPI_H

#include <unistd.h>
#include <sys/time.h>


// Pin modes

#define	INPUT			 0
#define	OUTPUT			 1
#define	PWM_OUTPUT		 2
#define	GPIO_CLOCK		 3
#define	SOFT_PWM_OUTPUT		 4
#define	SOFT_TONE_OUTPUT	 5
#define	PWM_TONE_OUTPUT		 6

#define	LOW			 0
#define	HIGH			 1


// Interrupt levels

#define	INT_EDGE_SETUP		0
#define	INT_EDGE_FALLING	1
#define	INT_EDGE_RISING		2
#define	INT_EDGE_BOTH		3


static int  wiringPiISR         (int pin, int mode, void (*function)(void)) { return 1; }

static          void pinMode             (int pin, int mode) {}
static          void digitalWrite        (int pin, int value) {}

static void         delay             (unsigned int howLong) { sleep(howLong); }
static void         delayMicroseconds (unsigned int howLong) { usleep(howLong); }
static unsigned int millis            (void) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (1000000 * tv.tv_sec + tv.tv_usec ) / 1000;
}
static unsigned int micros            (void) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return 1000000 * tv.tv_sec + tv.tv_usec;
}


#endif //INC_433_WIRINGPI_H
