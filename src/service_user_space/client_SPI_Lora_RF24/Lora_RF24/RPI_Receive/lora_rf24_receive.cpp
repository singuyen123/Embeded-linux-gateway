#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <RF24/RF24.h>
#include <RF24/LoRa.h>
#include <unistd.h>
#include <pigpio.h>
#include <pthread.h>

#define gpio_C 21
#define gpio_B 20
#define gpio_A 16

using namespace std;
char rxbuf[255];
LoRa_ctl modem;
// RPi generic:
RF24 radio(22,0);

/********************************/

// Radio pipe addresses for the 2 nodes to communicate.
const uint8_t address[6] = "00001";
pthread_t id1,id2;
pthread_mutex_t mutexsum;
void *loraReceive(void *args);
void *RF24Receive(void *args);

void rx_f(rxData *rx){
    printf("rx done \n");
    //printf("CRC error: %d\n", rx->CRC);
    printf("Data size: %d\n", rx->size);
    printf("string: %s\n", rx->buf);//Data we'v received
    printf("RSSI: %d\n", rx->RSSI);
    printf("SNR: %f\n", rx->SNR);
}

int main(int argc, char** argv){

	int ret1,ret2;

	if (gpioInitialise() < 0)
    {
        printf("Pigpio init error\n");
        return -1;
    }else{
		printf("inital gpio successfully\n");
	}

    //See for typedefs, enumerations and there values in LoRa.h header file
    modem.spiCS = 0;//Raspberry SPI CE pin number
    modem.rx.callback = rx_f;
    modem.rx.data.buf = rxbuf;
    modem.eth.preambleLen=6;
    modem.eth.bw = BW62_5;//Bandwidth 62.5KHz
    modem.eth.sf = SF7;//Spreading Factor 12
    modem.eth.ecr = CR8;//Error coding rate CR4/8
    modem.eth.freq = 433000000;// 434.8MHz
    modem.eth.resetGpioN = 4;//GPIO4 on lora RESET pi
    modem.eth.dio0GpioN = 17;//GPIO17 on lora DIO0 pin to control Rxdone and Txdone interrupts
    modem.eth.outPower = OP20;//Output power
    modem.eth.powerOutPin = PA_BOOST;//Power Amplifire pin
    modem.eth.AGC = 1;//Auto Gain Control
    modem.eth.OCP = 240;// 45 to 240 mA. 0 to turn off protection
    modem.eth.implicitHeader = 0;//Explicit header mode
    modem.eth.syncWord = 0x12;

	pthread_mutex_init(&mutexsum,NULL);
	ret1 = pthread_create(&id1,NULL,&RF24Receive,NULL);
	if(ret1 == 0)
	{
		printf("Thread RFReceive created successfully\n");
	}
	sleep(10);
	ret2 = pthread_create(&id2,NULL,&loraReceive,NULL);
	if(ret2 == 0)
	{
		printf("Thread loraReceive created successfull\n");
	}
	while(1)
	{

	}

  return 0;
}

void *loraReceive(void *args)
{
	while(1)
	{

		pthread_mutex_lock(&mutexsum);
		printf("----------------Start Lora---------------\n");
		gpioSetMode(gpio_A, PI_OUTPUT);
		gpioSetMode(gpio_B, PI_OUTPUT);
		gpioSetMode(gpio_C, PI_OUTPUT);
		gpioWrite(gpio_A, 0);
		gpioWrite(gpio_B, 0);
		gpioWrite(gpio_C, 0);

		LoRa_begin(&modem);
	    LoRa_receive(&modem);
	    sleep(20);
	    printf("end\n");
	    LoRa_end(&modem);
		printf("----------------End Lora---------------\n");
		pthread_mutex_unlock(&mutexsum);
		sleep(10);
	}
}
void *RF24Receive(void *args)
{
	while(1)
	{
		
		pthread_mutex_lock(&mutexsum);

		gpioSetMode(gpio_A, PI_OUTPUT);
		gpioSetMode(gpio_B, PI_OUTPUT);
		gpioSetMode(gpio_C, PI_OUTPUT);
		gpioWrite(gpio_A, 1);
		gpioWrite(gpio_B, 1);
		gpioWrite(gpio_C, 1);

		// Setup and configure rf radio
		radio.begin();

			
		// optionally, increase the delay between retries & # of retries
		radio.setRetries(15,15);
		// Dump the configuration of the rf unit for debugging
		radio.printDetails();
		radio.openReadingPipe(0, address);
		radio.startListening();
		int countRF=0;
		// forever loop
		while (countRF<15)
		{
				countRF++;
				// radio.begin();
				// radio.setRetries(15,15);
				// if there is data ready
				if ( radio.available() )
				{
					// Dump the payloads until we've gotten everything
					char data[50];
					memset(data,'\0',sizeof(data));
					// Fetch the payload, and see if this was the last one.
				//	while(radio.available()){
					//	radio.read( &got_time, sizeof(unsigned long) );
						radio.read( &data, sizeof(data) );
				//	}
					// Spew it
					printf("Data receive (%d) %s...\n",sizeof(data), data);
				}
				sleep(1);
		} // forever loop

		//radio.endTransaction();
		radio.closeReadingPipe(0);
		radio.powerDown();
		printf("*********end RF24*********\n");
		pthread_mutex_unlock(&mutexsum);
		sleep(5);

	}

}