/*
 * extensions.c:
 *	Part of the GPIO program to test, peek, poke and otherwise
 *	noodle with the GPIO hardware on the Raspberry Pi.
 *	Copyright (c) 2012-2013 Gordon Henderson
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>

#include <wiringPi.h>


#include "extensions.h"

extern int wiringPiDebug ;

#ifndef TRUE
#  define	TRUE	(1==1)
#  define	FALSE	(1==2)
#endif

// Local structure to hold details

struct extensionFunctionStruct
{
  const char *name ;
  int	(*function)(char *progName, int pinBase, char *params) ;
} ;


/*
 * extractInt:
 *	Check & return an integer at the given location (prefixed by a :)
 *********************************************************************************
 */

static char *extractInt (char *progName, char *p, int *num)
{
  if (*p != ':')
  {
    fprintf (stderr, "%s: colon expected\n", progName) ;
    return NULL ;
  }

  ++p ;

  if (!isdigit (*p))
  {
    fprintf (stderr, "%s: digit expected\n", progName) ;
    return NULL ;
  }

  *num = strtol (p, NULL, 0) ;
  while (isdigit (*p))
    ++p ;

  return p ;
}


/*
 * extractStr:
 *	Check & return a string at the given location (prefixed by a :)
 *********************************************************************************
 */

static char *extractStr (char *progName, char *p, char **str)
{
  char *q, *r ;

  if (*p != ':')
  {
    fprintf (stderr, "%s: colon expected\n", progName) ;
    return NULL ;
  }

  ++p ;

  if (!isprint (*p))
  {
    fprintf (stderr, "%s: character expected\n", progName) ;
    return NULL ;
  }

  q = p ;
  while ((*q != 0) && (*q != ':'))
    ++q ;

  *str = r = calloc (q - p + 2, 1) ;	// Zeros it

  while (p != q)
    *r++ = *p++ ;
    
  return p ;
}



/*
 * doExtensionMcp23008:
 *	MCP23008 - 8-bit I2C GPIO expansion chip
 *	mcp23002:base:i2cAddr
 *********************************************************************************
 */



/*
 * doExtensionMcp23016:
 *	MCP230016- 16-bit I2C GPIO expansion chip
 *	mcp23016:base:i2cAddr
 *********************************************************************************
 */




/*
 * doExtensionMcp23017:
 *	MCP230017- 16-bit I2C GPIO expansion chip
 *	mcp23017:base:i2cAddr
 *********************************************************************************
 */






/*
 * doExtensionSr595:
 *	Shift Register 74x595
 *	sr595:base:pins:data:clock:latch
 *********************************************************************************
 */




/*
 * doExtensionPcf8574:
 *	Digital IO (Crude!)
 *	pcf8574:base:i2cAddr
 *********************************************************************************
 */




/*
 * doExtensionPcf8591:
 *	Analog IO
 *	pcf8591:base:i2cAddr
 *********************************************************************************
 */



/*
 * doExtensionMax31855:
 *	Analog IO
 *	max31855:base:spiChan
 *********************************************************************************
 */



/*
 * doExtensionMcp3002:
 *	Analog IO
 *	mcp3002:base:spiChan
 *********************************************************************************
 */


/*
 * doExtensionMcp3004:
 *	Analog IO
 *	mcp3004:base:spiChan
 *********************************************************************************
 */



/*
 * doExtensionMax5322:
 *	Analog O
 *	max5322:base:spiChan
 *********************************************************************************
 */


/*
 * doExtensionMcp4802:
 *	Analog IO
 *	mcp4802:base:spiChan
 *********************************************************************************
 */


/*
 * doExtensionSn3218:
 *	Analog Output (LED Driver)
 *	sn3218:base
 *********************************************************************************
 */


/*
 * doExtensionMcp3422:
 *	Analog IO
 *	mcp3422:base:i2cAddr
 *********************************************************************************
 */


/*
 * doExtensionDrcS:
 *	Interface to a DRC Serial system
 *	drcs:base:pins:serialPort:baud
 *********************************************************************************
 */

static int doExtensionDrcS (char *progName, int pinBase, char *params)
{
  char *port ;
  int pins, baud ;

  if ((params = extractInt (progName, params, &pins)) == NULL)
    return FALSE ;

  if ((pins < 1) || (pins > 100))
  {
    fprintf (stderr, "%s: pins (%d) out of range (2-100)\n", progName, pins) ;
    return FALSE ;
  }
  
  if ((params = extractStr (progName, params, &port)) == NULL)
    return FALSE ;

  if (strlen (port) == 0)
  {
    fprintf (stderr, "%s: serial port device name required\n", progName) ;
    return FALSE ;
  }

  if ((params = extractInt (progName, params, &baud)) == NULL)
    return FALSE ;

  if ((baud < 1) || (baud > 4000000))
  {
    fprintf (stderr, "%s: baud rate (%d) out of range\n", progName, baud) ;
    return FALSE ;
  }

  drcSetupSerial (pinBase, pins, port, baud) ;

  return TRUE ;
}



/*
 * Function list
 *********************************************************************************
 */

struct extensionFunctionStruct extensionFunctions [] = 
{
  
  { "drcs",		&doExtensionDrcS	},
  { NULL,		NULL		 	},
} ;


/*
 * doExtension:
 *	Load in a wiringPi extension
 *********************************************************************************
 */

int doExtension (char *progName, char *extensionData)
{
  char *p ;
  char *extension = extensionData ;
  struct extensionFunctionStruct *extensionFn ;
  int pinBase = 0 ;

// Get the extension extension name by finding the first colon

  p = extension ;
  while (*p != ':')
  {
    if (!*p)	// ran out of characters
    {
      fprintf (stderr, "%s: extension name not terminated by a colon\n", progName) ;
      return FALSE ;
    }
    ++p ;
  }

  *p++ = 0 ;

  if (!isdigit (*p))
  {
    fprintf (stderr, "%s: pinBase number expected after extension name\n", progName) ;
    return FALSE ;
  }

  while (isdigit (*p))
  {
    if (pinBase > 1000000000)
    {
      fprintf (stderr, "%s: pinBase too large\n", progName) ;
      return FALSE ;
    }

    pinBase = pinBase * 10 + (*p - '0') ;
    ++p ;
  }

  if (pinBase < 64)
  {
    fprintf (stderr, "%s: pinBase (%d) too small. Minimum is 64.\n", progName, pinBase) ;
    return FALSE ;
  }

// Search for extensions:

  for (extensionFn = extensionFunctions ; extensionFn->name != NULL ; ++extensionFn)
  {
    if (strcmp (extensionFn->name, extension) == 0)
      return extensionFn->function (progName, pinBase, p) ;
  }

  fprintf (stderr, "%s: extension %s not found\n", progName, extension) ;
  return FALSE ;
}
