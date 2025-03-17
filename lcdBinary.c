/* ***************************************************************************** */
/* You can use this file to define the low-level hardware control fcts for       */
/* LED, button and LCD devices.                                                  */ 
/* Note that these need to be implemented in Assembler.                          */
/* You can use inline Assembler code, or use a stand-alone Assembler file.       */
/* Alternatively, you can implement all fcts directly in master-mind.c,          */  
/* using inline Assembler code there.                                            */
/* The Makefile assumes you define the functions here.                           */
/* ***************************************************************************** */


#ifndef	TRUE
#  define	TRUE	(1==1)
#  define	FALSE	(1==2)
#endif

#define	PAGE_SIZE		(4*1024)
#define	BLOCK_SIZE		(4*1024)

#define	INPUT			 0
#define	OUTPUT			 1

#define	LOW			 0
#define	HIGH			 1


// APP constants   ---------------------------------

// Wiring (see call to lcdInit in main, using BCM numbering)
// NB: this needs to match the wiring as defined in master-mind.c

#define STRB_PIN 24
#define RS_PIN   25
#define DATA0_PIN 23
#define DATA1_PIN 10
#define DATA2_PIN 27
#define DATA3_PIN 22

// -----------------------------------------------------------------------------
// includes 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>

// -----------------------------------------------------------------------------
// prototypes

int failure (int fatal, const char *message, ...);

// -----------------------------------------------------------------------------
// Functions to implement here (or directly in master-mind.c)

/* this version needs gpio as argument, because it is in a separate file */
void digitalWrite (uint32_t *gpio, int pin, int value) {
  int secondDigit = pin;
  while(secondDigit > 10){
    secondDigit -= 10;
  }
  if (value == 1) {
    if (pin - 32 <= 0) {
      (*(gpio + 7)) = (*(gpio + 7)) | (1 << (secondDigit * 3));
    }
    else {
      (*(gpio + 8)) = (*(gpio + 8)) | (1 << (secondDigit * 3));
    }
  }
  else {
    if (pin - 32 <= 0) {
      (*(gpio + 10)) = (*(gpio + 10)) | (1 << (secondDigit * 3));
    }
    else {
      (*(gpio + 11)) = (*(gpio + 11)) | (1 << (secondDigit * 3));
    }
  }
  /* ***  COMPLETE the code here, using inline Assembler  ***  */
}

// adapted from setPinMode
void pinMode(uint32_t *gpio, int pin, int mode) {
  int fSel = 0, shift = 0;
  for(int i = 10;i <= 60; i+=10) {
    if(pin < i && pin >= i-10) {
      break;
    }
    fSel++;
  }
  shift = (pin%10*3);
  *(gpio + fSel) = (*(gpio + fSel) & ~(7 << shift)) | 1 << shift;
}

void writeLED(uint32_t *gpio, int led, int value) {
  digitalWrite(gpio, led, value);
  /* ***  COMPLETE the code here, using inline Assembler  ***  */
}

int readButton(uint32_t *gpio, int button) {
  /* ***  COMPLETE the code here, using inline Assembler  ***  */
}

void waitForButton(uint32_t *gpio, int button) {
  /* ***  COMPLETE the code here, just C no Assembler; you can use readButton ***  */
}
