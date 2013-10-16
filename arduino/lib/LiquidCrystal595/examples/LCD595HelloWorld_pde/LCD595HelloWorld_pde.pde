/* -----------------------------------------------------------------------------------
 * Adaption of the LCD4Bit library shipped with Arduino 22
 * for use with 74C595 shift register adapter board found on:
 * http://www.stephenhobley.com
 
 * Code adaption by Steve Hobley - February 2011

 /*---Shift Register 74HC595---
 * [SR Pin 14 (DS)]    to Arduino pin - Yellow wire [datapin]
 * [SR Pin 12 (ST_CP)] to Arduino pin - Green wire  [latchpin]
 * [SR Pin 11 (SH_CP)] to Arduino pin - White wire  [clockpin]
 * Black wire to Ground
 * Red wire to +5v

 -----Shift Reg to LCD--------
 * SR Pin 15  - ENABLE        10000000
 * SR Pin 1   - D4            00000010 
 * SR Pin 2   - D5	      00000100
 * SR Pin 3   - D6	      00001000
 * SR Pin 4   - D7	      00010000
 * SR Pin 5   - MOSFET / LED1 00100000
 * SR Pin 6   - LED 2         01000000
 * SR Pin 7   - RS            00000001
 *
 * ----------------------------------------------------------------------------------- */
// include the library code:
#include <LiquidCrystal595.h>

// initialize the library with the numbers of the interface pins + the row count
// datapin, latchpin, clockpin, num_lines
LiquidCrystal595 lcd(2,3,4);

void setup() 
{
  lcd.begin(20,4);

  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print("This is line 1");
  delay(1000);
  
  lcd.setCursor(0,1);
  lcd.print("This is line 2");
  delay(1000);
  
  lcd.setCursor(0,2);
  lcd.print(127, HEX);
  delay(1000);
  
  lcd.setCursor(0,3);
  lcd.print("This is line ");
  lcd.print(4, BIN); // << Snagged from the Print class
  delay(1000);
  
  
}

void loop() 
{
  
}

