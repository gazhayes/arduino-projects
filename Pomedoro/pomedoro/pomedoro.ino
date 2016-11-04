#include <stopwatch.h>
#include <SD.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
const int chipSelect = 10;
LiquidCrystal_I2C lcd(0x3F,16,2);

//------Initialize timekeepers--------|
unsigned int XRCISE = 0;
unsigned int GOALS = 0;
unsigned int EDU = 0;
unsigned int CHORES = 0;
unsigned int CHAT = 0;
unsigned int RELAX = 0;

//------Initialize buttons----------|
int XRCISE_b = 8;
int GOALS_b = 9;
int EDU_b = 4;
int CHORES_b = 5;
int CHAT_b = 6;
int RELAX_b = 7;

int POM = 2;
int STOPWATCH = A0;
int END = A1;


//---------Initialize LEDs-----------|
int POM_l = A2;
int STOPWATCH_l = A3;

//---------Initialize variables--------|
String button = "none";
int POMTIME = 0;
int piezoPin = 3;
StopWatch sw(StopWatch::SECONDS);
  File timeLog;


void setup() {
//------Initialize buttons----------|
pinMode(XRCISE_b, INPUT_PULLUP);
pinMode(GOALS_b, INPUT_PULLUP);
pinMode(EDU_b, INPUT_PULLUP);
pinMode(CHORES_b, INPUT_PULLUP);
pinMode(CHAT_b, INPUT_PULLUP);
pinMode(RELAX_b, INPUT_PULLUP);
pinMode(POM, INPUT_PULLUP);
pinMode(STOPWATCH, INPUT_PULLUP);
pinMode(END, INPUT_PULLUP);
pinMode(A2, OUTPUT);

  lcd.init();
  lcd.backlight();
  pinMode(10, OUTPUT);
  if(SD.begin(chipSelect)){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SD CARD OK");
    delay(1000);
    } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SD CARD FAIL");
    delay(1000);
      };
}

void loop () { 
//int GOALS_val = digitalRead(GOALS_b);
//delay(2);
    
  
//First button select
button = "none";
int activity = 0;

unsigned int currentTimer = 0;
while (button == "none") {
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("SELECT ACTIVITY");
while (digitalRead(XRCISE_b) == 1 && digitalRead(GOALS_b) == 1 && digitalRead(EDU_b) == 1 && digitalRead(CHORES_b) == 1 && digitalRead(CHAT_b) == 1 && digitalRead(RELAX_b) == 1 && digitalRead(END) == 1) {};
if (digitalRead(XRCISE_b) == 0) {button = "EXERCISE";currentTimer = XRCISE;};
if (digitalRead(GOALS_b) == 0) {button = "GOALS";currentTimer = GOALS;activity = 2;};
if (digitalRead(EDU_b) == 0) {button = "EDUCATION";currentTimer = EDU;};
if (digitalRead(CHORES_b) == 0) {button = "CHORES";currentTimer = CHORES;};
if (digitalRead(CHAT_b) == 0) {button = "CHAT";currentTimer = CHAT;};
if (digitalRead(RELAX_b) == 0) {button = "RELAX";currentTimer = RELAX;};
if (digitalRead(END) == 0) {delay(500);endofday();};

}

if (button != "none") {
  delay(300);
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACT: " + button);
    lcd.setCursor(0, 1);
    lcd.print("POM / STOPWATCH?");
    while (digitalRead(POM) == 1 && digitalRead(STOPWATCH) == 1 && digitalRead(END) == 1) {
    };
    if (digitalRead(POM) == 0) {
      pomedoro(activity);
    };
    if (digitalRead(STOPWATCH) == 0) {
    stopwatch();
    };
    if (digitalRead(END) == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(button);
    lcd.setCursor(0, 1);
    int hours = floor(sw.elapsed()/3600);
    lcd.print(hours);
    lcd.print("Hours ");
    lcd.print(currentTimer/60%60);
    lcd.print("Mins");
    delay(2000);
    };
    
    }
    
delay(100);
  }

void pomedoro(int activity) {
  int pot = analogRead(A6) / 17.05;
  POMTIME = pot*60;
  if(POMTIME == 0) {POMTIME = 60;}
  if(button == "EXERCISE") {XRCISE = XRCISE + POMTIME;}
  if(button == "GOALS") {GOALS = GOALS + POMTIME;}
  if(button == "EDU") {EDU = EDU + POMTIME;}
  if(button == "CHORES") {CHORES = CHORES + POMTIME;}
  if(button == "CHAT") {CHAT = CHAT + POMTIME;}
  if(button == "RELAX") {RELAX = RELAX + POMTIME;}
  
  sw.reset();
  sw.start();
  
  while(POMTIME-sw.elapsed() >= 1){
    analogWrite(POM_l, 128);
  lcd.clear();
  lcd.setCursor(0, 0);
 lcd.print("]---POMODORO---[");
//lcd.print(POMTIME);
//lcd.print(" ");
//lcd.print(GOALS);
lcd.setCursor(0, 1);
lcd.print(button + ":");
  lcd.setCursor(11, 1);
  if(POMTIME-sw.elapsed() < 600){
     lcd.print("0"); 
    }
 // if(POMTIME-sw.elapsed() < 60){
 //    lcd.print("0"); 
 //   }
  lcd.print((POMTIME-sw.elapsed())/60%60);
  lcd.print(":");
  
//  lcd.setCursor(11, 1);
  if(60-sw.elapsed()%60 < 10) {
    lcd.print("0");
    }
  lcd.print(60-sw.elapsed()%60);
  //lcd.print("S");
  delay(1000);

  }
  analogWrite(POM_l, 0);
  tone(piezoPin, 3000, 50);
  delay(100);
  tone(piezoPin, 3000, 50);
  delay(100);
  tone(piezoPin, 3000, 50);
  delay(1000);
  }

void stopwatch() {
  sw.reset();
  sw.start();
  while(digitalRead(END) == 1) {
  analogWrite(STOPWATCH_l, 128);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("]---" + button + "---[");
  //lcd.setCursor(0, 1);
  //lcd.print(button + ":");
  lcd.setCursor(4, 1);
  int hours = floor(sw.elapsed()/3600);
  lcd.print("0");
  lcd.print(hours);
  lcd.print(":");
  if(sw.elapsed()/60%60 < 10){lcd.print("0");}
  lcd.print(sw.elapsed()/60%60);
  lcd.print(":");
  if(sw.elapsed()%60 < 10){lcd.print("0");}
  lcd.print(sw.elapsed()%60);
  delay(1000);
  }
  analogWrite(STOPWATCH_l, 0);
  sw.stop();
  tone(piezoPin, 3000, 50);
  delay(100);
  tone(piezoPin, 3000, 50);
  delay(100);
  tone(piezoPin, 3000, 50);
  delay(1000);
  if(button == "EXERCISE") {XRCISE = XRCISE + sw.elapsed();}
  if(button == "GOALS") {GOALS = GOALS + sw.elapsed();}
  if(button == "EDU") {EDU = EDU + sw.elapsed();}
  if(button == "CHORES") {CHORES = CHORES + sw.elapsed();}
  if(button == "CHAT") {CHAT = CHAT + sw.elapsed();}
  if(button == "RELAX") {RELAX = RELAX + sw.elapsed();}
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("]-----DONE-----[");
  lcd.setCursor(0, 1);
  lcd.print(button + ":");
  lcd.setCursor(9, 1);
  int hours = floor(sw.elapsed()/3600);
  lcd.print(hours);
  lcd.print(":");
  if(sw.elapsed()/60%60 < 10){lcd.print("0");}
  lcd.print(sw.elapsed()/60%60);
  lcd.print(":");
  if(sw.elapsed()%60 < 10){lcd.print("0");}
  lcd.print(sw.elapsed()%60);
  delay(1000);
  }
  
void endofday() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PRESS AGAIN");
  lcd.setCursor(0, 1);
  lcd.print("FOR RESET");
while(digitalRead(END) == 1 && digitalRead(POM) == 1){}
if(digitalRead(END) == 0) {
  if(sdcard() == 1){
XRCISE = 0;
GOALS = 0;
EDU = 0;
CHORES = 0;
CHAT = 0;
RELAX = 0;
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("SD WRITE SUCCESS");
delay(500);
  } else {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SD ERROR");
  }
}
}

int sdcard(){

  if(!SD.exists("timelog.csv")){timeLog = SD.open("timelog.csv", FILE_WRITE);timeLog.println("EXERCISE,GOALS,EDUCATION,CHORES,CHAT,RELAX,TOTAL");timeLog.close();}
  if(SD.exists("timelog.csv")){
    timeLog = SD.open("timelog.csv", FILE_WRITE);
    unsigned int total = XRCISE+GOALS+EDU+CHORES+CHAT+RELAX;
   String toprint = String(String(XRCISE/60%60) + "," + String(GOALS/60%60) + "," + String(EDU/60%60) + "," + String(CHORES/60%60) + "," + String(CHAT/60%60) + "," + String(RELAX/60%60) + "," + String(total/60%60));
   //timeLog.println(String(String(XRCISE) + "," + String(GOALS) + "," + String(EDU) + "," + String(CHORES) + "," + String(CHAT) + "," + String(RELAX)));
   lcd.clear();
lcd.setCursor(0, 0);
lcd.print("WROTE:");
lcd.setCursor(0, 1);
lcd.print(toprint);
timeLog.println(toprint);
timeLog.close();

delay(3000);
    return 1;
  } else {return 0;}
  
  
  
}


