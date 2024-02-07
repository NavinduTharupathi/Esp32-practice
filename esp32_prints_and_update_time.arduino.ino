#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire ,OLED_RESET);

//Global variables
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

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
  print_time_now();
}
void print_line(String text , int column , int row , int text_size){
    display.clearDisplay();
    display.setTextSize(text_size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(column,row);
    display.println(F(text));
    display.display();
    delay(2000);
}

void print_time_now(void){
  print_line(string(days),0,0,2);
  print_line(":",20,0,2);
  print_line(string(hours),30,0,2);
  print_line(":",20,0,2);
  print_line(string(minutes),60,0,2);
  print_line(":",20,0,2);
  print_line(string(seconds),90,0,2);

}