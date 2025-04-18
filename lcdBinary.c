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

/* this version needs gpio as argument, because it is in a separate file */
void digitalWrite(uint32_t *gpio, int pin, int value) {
  int reg = 0;
  
  asm volatile(
    "\tmov r1, %[pin]\n"        // r1 = pin
    "\tmov r2, %[val]\n"        // r2 = value
    "\tmov r3, #1\n"            
    "\tlsl r3, r3, r1\n"        // r3 = 1 << pin
    "\tcmp r2, #1\n"            
    "\tbne low\n"              
    "\tcmp r1, #32\n"           
    "\tbls set0\n"              
    "\tadd %[reg], #32\n"        // Set register to set1 (8)
    "\tb shift\n"
    "set0:\n"
    "\tadd %[reg], #28\n"        // Set register to set0 (7)
    "\tb shift\n"
    "low:\n"
    "\tcmp r1, #32\n"           
    "\tbls clr0\n"              
    "\tadd %[reg], #44\n"       // Set register to clr1 (11)1
    "\tb shift\n"
    "clr0:\n"
    "\tadd %[reg], #40\n"       // Set register to clr0 (10)
    "shift:\n"
    "\tstr r3, [%[gpio], %[reg]]\n" 
    : [reg] "+r" (reg)
    : [gpio] "r" (gpio), [pin] "r" (pin), [val] "r" (value)
    : "r1", "r2", "r3", "cc"
  );
}

// adapted from setPinMode
void pinMode(uint32_t *gpio, int pin, int mode) {
  int fsel = pin / 10;  
  int shift = (pin % 10) * 3; 

  asm volatile(
    "\tmov r2, %[gpio]\n"          // r2 = gpio.
    "\tadd r2, r2, %[sel], lsl #2\n" // r2 = gpio + fsel.
    "\tldr r3, [r2]\n"             // Load r2.
    "\tmov r4, #7\n"               
    "\tlsl r4, r4, %[shifted]\n"     // r4 = (7 << shift).
    "\tbic r3, r3, r4\n"           // gpio + fsel = gpio + fsel & ~(7 << shift).
    "\tmov r5, %[mode]\n"          
    "\tlsl r5, r5, %[shifted]\n"     // mode << shift.
    "\torr r3, r3, r5\n"           // gpio + fsel | mode << shift.
    "\tstr r3, [r2]\n"             // store value in r2.
    :
    : [gpio] "r" (gpio), [sel] "r" (fsel), [shifted] "r" (shift), [mode] "r" (mode)
    : "r2", "r3", "r4", "r5", "memory"
  );
}

void writeLED(uint32_t *gpio, int led, int value) {
  digitalWrite(gpio, led, value);
  // as per professor's esraa request.
}

int readButton(uint32_t *gpio, int button) {
  uint32_t GPLEV = (*(gpio + 13)); 
  int value = 0;

  asm volatile(
    "\tmov r3, %[gplev]\n"     // r3 = gplev.
    "\tmov r5, #0\n"           
    "\tand %[btn], %[btn], #31\n" // button & 31.
    "\tmov r6, #1\n"           
    "\tlsl r6, r6, %[btn]\n"   // 1 << (button & 31).
    "\tand r6, r3, r6\n"       // gplev & (1 << (button & 31)).
    "\tcmp r6, #0\n"           
    "\tbeq end\n"              
    "\tmov r5, #1\n"           
    "end:\n"
    "\tmov %[val], r5\n"       // value = r5.
    : [val] "=r" (value) 
    : [gplev] "r" (GPLEV), [btn] "r" (button)
    : "r3", "r5", "r6", "cc"
  );

  return value;
}