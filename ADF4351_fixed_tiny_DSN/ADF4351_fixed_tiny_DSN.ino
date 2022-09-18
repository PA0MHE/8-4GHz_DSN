//  By Maarten PA0MHE 09-09-2022
//******************************************** HARDWARE ********************************************************
//  For ATTINY85 with DSN:
//  PB0   pin 5   
//  PB1   pin 6   ADF DAT
//  PB2   pin 7   ADF CLK
//  PB3   pin 2   ADF LE 
//  PB4   pin 3   
//  PB5   pin 1   
// 
//  Set the system clock to 1MHz internal.
//******************************************************************************************************************

#include <tinySPI.h>                     // https://github.com/JChristensen/tinySPI


#define ADF_LE DDB3                     // PIN2 storage register
#define CLK DDB2
#define DAT DDB1 

uint32_t registers[6] =  {0x640000, 0x8008011, 0x1A004E42, 0x4B3, 0x8A003C, 0x580005} ; // 4000 MHz with ref 10 MHz x2, 5dBm, MUXout digital lock detect

void setup() {
  SPI.begin();                           // Init SPI bus
  pinMode(ADF_LE, OUTPUT);               // latch pin needs to be set up
  digitalWrite(ADF_LE, HIGH);
 
  delay(1000);                            // Give ADF4351 time to setup

  ADFSet(registers, ADF_LE);

  }


void loop(){

} 


void ADFSet(uint32_t reg[6], byte LE) {  // Program all 6 registers ADF4351 
  for (int i = 5; i >= 0; i--) {         // program ADF4351 start with R5
    ADFWriteRegister32(reg[i], LE);
  } 
  delay(1);
  ADFWriteRegister32(reg[0], LE);        // Program R0 again
}


void ADFWriteRegister32(const uint32_t value, byte LE) //Program data into one of the 32bits register, the register that is selected by the four LSBs.
{
  digitalWrite(LE, LOW);
  for (int i = 3; i >= 0; i--)             // loop through 4 x 8bits
  SPI.transfer((value >> 8 * i) & 0xFF);   // shift , mask and send octet via SPI
  digitalWrite(LE, HIGH);                  // Load Enable rising edge the data stored in the shift register
  digitalWrite(LE, LOW);                   // 
}
