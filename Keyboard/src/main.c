#include "bcm2835.h"	//Needed for GPIO support
#include <stdio.h>

/*
 * Set columns as outputs, high all the time.
 * Set rows as inputs with pull-up. Will be high by default.
 * If we press a button the connection between the pins will open.
 * If the columns all we're low all the time we wouldn't know which column
 * we we're located in when a button was pressed and hence a low signal returned.
 * Since they're high all the time we can cycle through them and make one
 * column low at a time. When a low signal is returned we've found teh right column.
 * */
 
int columns [3] = {14,4,15};							//The gpio pins connected to the columns
int nrofcolumns = sizeof(columns)/sizeof(int);
	
int rows [4] = {23,22,18,17};							//The gpio pins connected to the rows
int nrofrows = sizeof(rows)/sizeof(int);				//Size of gives nr of bytes, divide by size of one element to get nr of items

char keys [4][3] = {
	{'1','2','3'},
	{'4','5','6'},
	{'7','8','9'},
	{'*','0','#'}};
	
int keySetup () {
	if(!bcm2835_init())
		return -1;
		
	int i;
	for(i = 0; i < nrofcolumns; i++) {
		bcm2835_gpio_fsel(columns[i],BCM2835_GPIO_FSEL_OUTP);	//Set the columns as outputs
		bcm2835_gpio_set(columns[i]);							//Set it high
	}
	
	for(i = 0; i < nrofrows; i++) {
		bcm2835_gpio_fsel(rows[i], BCM2835_GPIO_FSEL_INPT);	//Set the rows to be inputs
		bcm2835_gpio_set_pud(rows[i],BCM2835_GPIO_PUD_UP);	//Set them to pull up
	}
	return 1;
}

char readKey () {
	int i;
	int j;
	while(1) {
		for(i = 0; i < nrofcolumns; i++) {					//Loop over the columns
			bcm2835_gpio_clr(columns[i]);					//Set this columns pin to low
			for(j = 0; j < nrofrows; j++) {					//Loop over the rows
				if(!bcm2835_gpio_lev(rows[j])) {			//Check if the signal for some row is low
					return(keys[j][i]);						//If so, return
				}
			}
			bcm2835_gpio_set(columns[i]);					//Set this columns pin to high again
		}
	}
}

int main () {
	keySetup();
	char c = readKey();
	printf("%c", c);
	return 0;
}



