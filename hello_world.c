
#include <alt_types.h>
#include <sys/alt_alarm.h>
#include <sys/alt_irq.h>
#include "address_map.h"
#include <stdbool.h>
#include <stdio.h>

//page 14

volatile int *btn_ptr = (int *) PUSHBUTTON_BASE;
volatile int *switch_ptr = (int *) SLIDER_SWITCH_BASE;

//int HEX3to0_display (int value, unsigned char * table);
volatile int * HEX3_HEX0_ptr	= (int *) HEX3_HEX0_BASE;	// HEX3_HEX0 address


void onButtonPress(void *context, alt_u32 id);
void onSwitch(void *context, alt_u32 id);

int main(){
	*(HEX3_HEX0_ptr) = 0x3f3f;

	int r2 = alt_irq_register(-1, NULL, onSwitch);
	int r1 = alt_irq_register(1, NULL, onButtonPress);	//0 if successful

	printf("Hello from Nios II. got %d and %d!\n", r1, r2);

	// Addresses to enable interrupts for
    *(btn_ptr + 2) = 0xe;	// buttons 1-3
    *(switch_ptr + 2) = 0x0;
    *(switch_ptr) = 0x0;



	while(1){}
	return 0;
}


void onButtonPress(void *context, alt_u32 id){
    int buttonsPressed = *(btn_ptr + 3);
    *(btn_ptr + 3) = 0;
    usleep(150000);

	printf("H!\n");

}

void onSwitch(void *context, alt_u32 id){
	printf("Hal9000\n");
}


/* The function below takes two arguments: value which is the value in decimal
 * to be displayed by the seven-segment displays and a character array that have the encoding
 * to display the provided value in decimal on the seven-segment displays. The provided value
 * can range from 0 - 999. The seven-segment displays must output the entire range correctly.
 */
void HEX3to0_display (int ones){
	unsigned char table[10] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x7, 0x7f, 0x6f};
	int hundereds = ones / 100;
	int tens = (ones-=(100*hundereds)) / 10;
	int hex_bits = 0x0000000F;
	bool noHundereds  = hundereds == 0;
	bool noTens = tens == 0;
	hex_bits = (noHundereds ? 0x0 : table[hundereds]);
	hex_bits = (noHundereds && noTens ? 0x0 : (hex_bits << 8)| table[tens]);
	*(HEX3_HEX0_ptr) = (hex_bits << 8)| table[ones - (tens * 10)];
}
