#include </home/pi/bcm2835-1.50/src/bcm2835.h>	//Needed for GPIO support
#include <stdio.h>
#include <string.h>

//Define pin 11 (really GPIO pin 17) to be the one used for sending the dots and dashes
#define PIN_SIGNAL RPI_GPIO_P1_11
//Define pin 12 (really GPIO pin 18) to be the one for signaling a new letter is coming
#define PIN_NEW_LETTER RPI_GPIO_P1_12
//Maximum nr of words you want to allow
#define MAX_WORDS 4
//Maximum length of a word
#define MAX_LENGTH 10

char morse_alfabet [26][4] = {	{0,1,2,2}, //A	0 = short (dot), 1 = long (dash), 2 = not used
								{1,0,0,0}, //B
								{1,0,1,0}, //C
								{1,0,0,2}, //D
								{0,2,2,2}, //E
								{0,0,1,0}, //F
								{1,1,0,2}, //G
								{0,0,0,0}, //H
								{0,0,2,2}, //I
								{0,1,1,1}, //J
								{1,0,1,2}, //K
								{0,1,0,0}, //L
								{1,1,2,2}, //M
								{1,0,2,2}, //N
								{1,1,1,2}, //O
								{0,1,1,0}, //P
								{1,1,0,1}, //Q
								{0,1,0,2}, //R
								{0,0,0,2}, //S
								{1,2,2,2}, //T
								{0,0,1,2}, //U
								{0,0,0,1}, //V
								{0,1,1,2}, //W
								{1,0,0,1}, //X
								{1,0,1,1}, //Y
								{1,1,0,0}, //Z
								};

//Initiates the GPIO pin. Success = 1, fail = 0.
int morse_init (void) {
	if(!bcm2835_init())	//Used to initialize the library. If fail, returns 0.
		return 0;
	
	//Set the pins to be outputs
	bcm2835_gpio_fsel(PIN_SIGNAL, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN_NEW_LETTER, BCM2835_GPIO_FSEL_OUTP);
	
	return 1;
}

//Causes a short signal (a "dot") on the lamp connected to the GPIO pin
void morse_dot (void) {
	
	bcm2835_gpio_write(PIN_SIGNAL, HIGH);
	bcm2835_delay(700);
	bcm2835_gpio_write(PIN_SIGNAL, LOW);
	bcm2835_delay(700);		
}

//Causes a long signal (a "dash") on the lamp connected to the GPIO pin 
void morse_dash (void) {
	
	bcm2835_gpio_write(PIN_SIGNAL, HIGH);
	bcm2835_delay(1400);
	bcm2835_gpio_write(PIN_SIGNAL, LOW);
	bcm2835_delay(1400);		
}

//Sends the letter provided as morse
void morse_letter (char a) {
	int index = a - 65;		//Input is in ASCII, subtract 65 to get index in array
	
	int i;
	for(i = 0; i < 4; i++) {
		if(morse_alfabet[index][i] == 0)		//Short
			morse_dot();
		else if (morse_alfabet[index][i] == 1) 	//Long
			morse_dash();						//Reached end of word
		else 										
			break;
	}
}

//Signals that a new letter is coming
void morse_new_letter (void) {
	bcm2835_gpio_write(PIN_NEW_LETTER, HIGH);
	bcm2835_delay(2000);
	bcm2835_gpio_write(PIN_NEW_LETTER, LOW);
	bcm2835_delay(2000);		
}

//Sends a whole word in morse
void morse_word (char a []) {
	int i;
	for(i = 0; i < MAX_LENGTH; i++) {
		if(a[i] != 0) {
			morse_letter(a[i]);
			morse_new_letter();
		}
		else
			break;
	}
}

int main (void) {
	
	//Initiate the GPIO pins, exits the program if it fails
	if(!morse_init())
		return 0;
	
	
	//An array of that will hold the words entered
	//Index [i] holds the i'th word
	char words [MAX_WORDS][MAX_LENGTH];
	
	int nrofwords = 0;		//Variable for how many names that will be entered
	while(1) {
		printf("How many words will you enter?\n");
		scanf("%d", &nrofwords);	//Save nr of words in nrofwords
		
		if(nrofwords <= MAX_WORDS)	//Break from infinite loop if the value is ok
			break;
		else 						//If the value is to high..	
			printf("The value has to be below" + MAX_WORDS);
	}
	
	int i;						//Counter for the loop
	char temp [MAX_LENGTH];		//Temporarily holds the input
	
	for(i = 0; i < nrofwords; i++) {
		scanf("%s", temp);		//Read the input
			
		int j = 0;		//Counter for the letters
		while(1) {		//Count nr of letters in the word
			if(temp[j] != 0)
				j++;
			else
				break;
		}
		
		if (j > MAX_LENGTH) {	//Check if the word is to long
			printf("Word is too long!");
			break;
		}
		else 				//If not, add to the array
			strncpy(words[i],temp,MAX_LENGTH);
	}
	
	for(i = 0; i < nrofwords; i++) {
		morse_word(words[i]);
	}
	
	return 0;
}
