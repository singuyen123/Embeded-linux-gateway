#ifndef __RF24_CONFIG_H__
#define __RF24_CONFIG_H__

/*** USER DEFINES:  ***/
#define FAILURE_HANDLING
//#define SERIAL_DEBUG
//#define MINIMAL
//#define SPI_UART  // Requires library from https://github.com/TMRh20/Sketches/tree/master/SPI_UART
//#define SOFTSPI   // Requires library from https://github.com/greiman/DigitalIO

/**********************/
#define rf24_max(a, b) (a > b ? a : b)
#define rf24_min(a, b) (a < b ? a : b)

#if defined(SPI_HAS_TRANSACTION) && !defined(SPI_UART) && !defined(SOFTSPI)
#define RF24_SPI_TRANSACTIONS
#endif // defined (SPI_HAS_TRANSACTION) && !defined (SPI_UART) && !defined (SOFTSPI)

// RF modules support 10 Mhz SPI bus speed
const uint32_t RF24_SPI_SPEED = 10000000;

// defined (ARDUINO) && !defined (__arm__) && !defined (__ARDUINO_X86__)
// Define _BV for non-Arduino platforms and for Arduino DUE
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(__arm__) || defined(__ARDUINO_X86__)
#if defined(__arm__) && defined(SPI_UART)
#include <SPI_UART.h>
#define _SPI uspi
#endif // !defined (__arm__) || !defined (SPI_UART)

#define _BV(x) (1 << (x))
#endif // defined (ARDUINO) && !defined (__arm__) && !defined (__ARDUINO_X86__)

#ifdef SERIAL_DEBUG
#define IF_SERIAL_DEBUG(x) ({ x; })
#else
#define IF_SERIAL_DEBUG(x)
#if defined(RF24_TINY)
#define printf_P(...)
#endif // defined(RF24_TINY)
#endif // SERIAL_DEBUG

#if defined(__ARDUINO_X86__)
#define printf_P printf
#define _BV(bit) (1 << (bit))
#endif // defined (__ARDUINO_X86__)

// Progmem is Arduino-specific
#if !defined(ARDUINO) // This doesn't work on Arduino DUE
typedef char const char;

#else // Fill in pgm_read_byte that is used, but missing from DUE
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif // !defined (ARDUINO)

typedef uint16_t prog_uint16_t;
#define PSTR(x) (x)
#define printf_P printf
#define strlen_P strlen
#define PROGMEM
#define pgm_read_word(p) (*(p))
#define pgm_read_ptr(p) (*(p))
#define PRIPSTR "%s"

#endif // __RF24_CONFIG_H__
