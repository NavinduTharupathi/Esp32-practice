// include libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define OLED parameters
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

//declare objects
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire ,OLED_RESET);

//Global variables
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

unsigned long timeNow = 0 ;
unsigned long timeLast = 0 ;

void setup() {
  //Initialize serial monitor and OLED display
  Serial.begin(115200);
  if (! display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }


  //turn on OLED display
  display.display();
  delay(2000);

  //clear OLED display
  display.clearDisplay();

  print_line("Welcome to medibox",10,20,2);
  display.clearDisplay();
  //print_line();
  //display a custom message
//   display.setTextSize(1);
//   display.setTextColor(SSD1306_WHITE);
//   display.setCursor(0,0);
//   display.println(F("Welcome Medibox!"));
//   display.display();
//   delay(2000);

}

void loop() {
  // put your main code here, to run repeatedly
  update_time();
  print_time_now();
}
void print_line(String text , int column , int row , int text_size){
//void print_line(void){  
    display.setTextSize(text_size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(column,row);
    display.println(text);
    display.display();
    delay(2000);
    // display.clearDisplay();
    // display.setTextSize(2);
    // display.setTextColor(SSD1306_WHITE);
    // display.setCursor(0,0);
    // display.println(F("Welcome to medibox"));
    // display.display();
    // delay(2000);
    
}

void print_time_now(void){
  display.clearDisplay();
  print_line(String(days),0,0,2);
  print_line(":",20,0,2);
  print_line(String(hours),30,0,2);
  print_line(":",50,0,2);
  print_line(String(minutes),60,0,2);
  print_line(":",80,0,2);
  print_line(String(seconds),90,0,2);

}

void update_time(){
  timeNow = millis()/1000; // seconds past after bootup
  seconds = timeNow-timeLast ;

  if(seconds >= 60){
    minutes += 1 ;
    timeLast += 60 ;
  }

  if(minutes == 60){
    hours += 1;
    minutes = 0;
  }

  if (hours == 24){
    days +=1 ;
    hours = 0;
  }
}