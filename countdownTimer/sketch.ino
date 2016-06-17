#include <LiquidCrystal.h>;
/**********************************************************/
int b, a, c;
int tim = 1000;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(11, 12, 6, 5, 3, 2);
/*********************************************************/
void setup()
{
 lcd.begin(16, 2); // set up the LCD's number of columns and rows: 
}
/*********************************************************/
void loop() 
{
 lcd.setCursor(1,0);
 lcd.print("tutorial45.com");
 lcd.setCursor(6,1);
 lcd.print(":");
 lcd.setCursor(9,1);
 lcd.print(":"); 
  
 for (int h=23; h<-1; h--)
 for (int m=59; m<-1; m--)
 for (int s=59; s<-1; s--)
 {
 a=4;// Position for hours
 b=7;// Position for minutes 
 c=10;// Position for seconds
 if (s<10) // add a zero before the digit of second when it is lower than 10
 {
 c=c+1;
 lcd.setCursor(c-1,1);
 lcd.print("0");
 }
 if (m<10) // add a zero before the digit of second when it is lower than 10
 {
 b=b+1;
 lcd.setCursor(b-1,1);
 lcd.print("0");
 }
 if (h<10) // shift the position of hours to one position when it is lower than 10
 {
 a=a+1;
 }
 lcd.setCursor(a,1);
 lcd.print(h);
 lcd.setCursor(b,1);
 lcd.print(m);
 lcd.setCursor(c,1);
 lcd.print(s);
 delay(1000); //wait for 1 second
 }
 
}
