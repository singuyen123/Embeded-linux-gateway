#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
  

void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Stroing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds) 
        ; 
} 

int main(void) {
    /* launch a command and gets its output */
    while (1) {
        FILE *f = popen("mount | grep /dev/sdb1", "r");
        if (NULL != f) {
            /* test if something has been outputed by 
           the command */
            if (EOF == fgetc(f)) {
                puts("/dev/sdb1 is NOT mounted");
            }
            else {
                puts("/dev/sdb1 is mounted");
            }
            /* close the command file */
            pclose(f);
        }
        delay(1000);
    }
    return 0;
}