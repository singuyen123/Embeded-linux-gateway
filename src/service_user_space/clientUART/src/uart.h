#ifndef LIB_UART_H
#define LIB_UART_H

#define BAUD 9600
static const char* DEVICE = "/dev/ttyS2";
int serialOpen  (const char *device, const int baud);
void serialClose (const int fd);
int serialDataAvail (const int fd);
void serialPutchar (const int fd, const unsigned char c);
void serialPuts (const int fd, const char *s);
int serialGetchar (const int fd);
#endif /*LIB_UART_H*/