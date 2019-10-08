/* itimerspec_from_str.h

   Header file for itimerspec_from_str.c.
*/
#ifndef ITIMERSPEC_FROM_STR_H
#define ITIMERSPEC_FROM_STR_H

#include <time.h>

void itimerspecFromStr(char *str, struct itimerspec *tsp);

#endif