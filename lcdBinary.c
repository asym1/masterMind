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
  // int reg = 0;
  // int copy = pin - 32;
  // if (value == 1) {
  //   (copy <=0) ? (reg = 7) : (reg = 8);
  // }
  // else {
  //   (copy <=0) ? (reg = 10) : (reg = 11);
  // }
  // (*(gpio + reg)) = (1 << pin);
  asm volatile(
    "\tMOV r3, #0b1\n" // set r3 to 1.
    "\tMOV r4, r1\n"
    "\tAND r4, r4, #31\n"
    "\tLSL r3, r3, r4\n"// shift r3 by pin number.
    "\tMOV r5, r1\n" // copy pin into r5
    "\tCMP r2, #1\n" // if it's 1 then set else clr.
    "\tBNE low\n"
    "\tCMP r5, #32\n" // if r5 is <= 32 then set0 else set1.
    "\tBLS set0\n"
    "\tMOV r6, #32\n" // set  to set1 (8) r6 is the bit number to be moved by for gpio
    "\tb shift\n"
    "\tset0:\n"
           "\tMOV r6, #28\n" // set  to set0 (7)
           "\tb shift\n"
    "\tlow:\n"  
          "\tCMP r5, #32\n" // if r5 is <= 32 then clr0 else clr1.
          "\tBLS clr0\n"
          "\tMOV r6, #44\n" // set  to clr1 (11)
          "\tb shift\n"
    "\tclr0:\n"
           "\tMOV r6, #40\n" // set to clr0 (10)
           "\tb shift\n"
    "\tshift:\n"
            "\tstr r3, [r0, r6]\n"
  );

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
  *(gpio + fSel) = (*(gpio + fSel) & ~(7 << shift)) | (1 << shift);
}

void writeLED(uint32_t *gpio, int led, int value) {
  digitalWrite(gpio, led, value);
  /* ***  COMPLETE the code here, using inline Assembler  ***  */
}

int readButton(uint32_t *gpio, int button) {
  // uint32_t GPLEV = (*(gpio + 13)); // GPLEV returns the value of the pin (week 3 tutorial slides, page 6).

  // int value = 0;
  // if(((GPLEV) & (1 << (button & 31))) != 0) { // if gplev and shifting give the same value then 1 else stays as 0 (totally didn't steal it from tutorial 3). 
  //   value = 1;
  // }  
  // return value;
  asm volatile(
    "\tmov r3 [r0, #52]\n" 
    "\tmov r5, #0\n" // r5 is the value given from button. 
    "\tAND r1, r1, #31\n" 
    "\tmov r6, #0b1\n" 
    "\tlsl r6, r1\n" // r6 is the shifted value. 
    "\tAND r3, r6\n" 
    "\tcmp r3, #0\n"  
    "\tbeq end\n"  
    "\tadd r5, #1\n" 
    "\tend:\n"  
         "\tmov r0, r5\n" 
         "\tbx lr\n" 
  );
  /* ***  COMPLETE the code here, using inline Assembler  ***  */
}