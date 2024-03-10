// include libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define OLED parameters
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define BUZZER 5
#define LED_1 15
#define PB_CANCEL 34
#define PB_OK 32
#define PB_UP 33
#define PB_DOWN 35


//declare objects
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire ,OLED_RESET);

//Global variables
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

unsigned long timeNow = 0 ;
unsigned long timeLast = 0 ;

bool alarm_enabled = true;
int n_alarms = 2;
int alarm_hours[] = {0,1};
int alarm_minutes[] = {1,10};
bool alarm_triggered[] = {false,false};

int n_notes = 8 ;
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;
int notes[] = {C,D,E,F,G,A,B,C_H};

int current_mode = 0;
int max_modes = 4;
String modes[]= {"1 - Set Time ","2 - Set Alarm 1","3 - Set Alarm 2","4 - Disable Alarms"};

void setup() {

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(PB_CANCEL, INPUT);
  pinMode(PB_OK, INPUT);
  pinMode(PB_UP, INPUT);
  pinMode(PB_DOWN, INPUT);

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
  update_time_with_check_alarms();
  if (digitalRead(PB_OK)== LOW){
    delay(200);
    go_to_menu();
  }
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


void ring_alarm(){
    display.clearDisplay();
    print_line("Medicine Time :",0,0,2);

    digitalWrite(LED_1, HIGH);

    bool break_happened = false ;

    //ring the buzzer
    while (break_happened == false  && digitalRead(PB_CANCEL) == HIGH){
        for (int i = 0 ; i < n_notes;i++){
          if (digitalRead(PB_CANCEL) == LOW){
            delay(200);
            break_happened = true;
            break;
          }
          tone(BUZZER,notes[i]);
          delay(500);
          noTone(BUZZER);
          delay(3);
    }
    }
    

    digitalWrite(LED_1,LOW);
    display.clearDisplay();

}


void update_time_with_check_alarms(void){
    update_time();
    print_time_now();
    if (alarm_enabled == true) {
      for (int i = 0 ; i < n_alarms ;i++){
        if (alarm_triggered[i]== false && alarm_hours[i]==hours && alarm_minutes[i]==minutes){
          ring_alarm();
          alarm_triggered[i] = true;
        }
      }
    }
}

int wait_for_button_press(){
  while(true){
    if(digitalRead(PB_UP)==LOW){
      delay(200);
      return PB_UP;
    }

    else if(digitalRead(PB_DOWN)==LOW){
      delay(200);
      return PB_DOWN;
    }

    else if(digitalRead(PB_OK)==LOW){
      delay(200);
      return PB_OK;
    }

    else if(digitalRead(PB_CANCEL)==LOW){
      delay(200);
      return PB_CANCEL;
    }
    update_time();
  }
}
void go_to_menu(){
  while(digitalRead(PB_CANCEL) == HIGH){
    display.clearDisplay();
    print_line(modes[current_mode],0,0,2);

    int pressed = wait_for_button_press();
    if (pressed == PB_UP){
      delay(200);
      current_mode += 1 ;
      current_mode = current_mode % max_modes ;
    }

    else if (pressed == PB_DOWN){
      delay(200);
      current_mode -= 1 ;
      if (current_mode < 0)
        current_mode = max_modes -1 ;
    }

    else if (pressed == PB_OK){
      delay(200);
      //run_mode(current_mode);
      Serial.println(current_mode);
    }

    else if (pressed == PB_CANCEL){
      delay(200);
      break;
    }
  }

}

void set_time(){
  int temp_hour = hours ;

  while (true){
    display.clearDisplay();
    print_line("Enter hour : " + String(temp_hour),0,0,2);

    int pressed = wait_for_button_press();
      if (pressed == PB_UP){
        delay(200);
        temp_hour += 1 ;
        temp_hour = temp_hour % 24 ;
      }

      else if (pressed == PB_DOWN){
        delay(200);
        temp_hour -= 1 ;
        if (temp_hour < 0)
          temp_hour = 23;
      }

      else if (pressed == PB_OK){
        delay(200);
        hours = temp_hour ;
        break;
      }

      else if (pressed == PB_CANCEL){
        delay(200);
        break;
      }
    
  }

  int temp_minute = minutes ;

  while (true){
    display.clearDisplay();
    print_line("Enter minute : " + String(temp_minute),0,0,2);

    int pressed = wait_for_button_press();
      if (pressed == PB_UP){
        delay(200);
        temp_minute += 1 ;
        temp_minute = temp_minute % 24 ;
      }

      else if (pressed == PB_DOWN){
        delay(200);
        temp_minute -= 1 ;
        if (temp_minute < 0)
          temp_minute = 23;
      }

      else if (pressed == PB_OK){
        delay(200);
        minutes = temp_minute ;
        break;
      }

      else if (pressed == PB_CANCEL){
        delay(200);
        break;
      }
    
  }

  display.clearDisplay();
  print_line("Time is set",0,0,2);
  delay(1000);
  
}

void set_alarm(int alarm){
  int temp_hour = alarm_hours[alarm] ;

  while (true){
    display.clearDisplay();
    print_line("Enter hour : " + String(temp_hour),0,0,2);

    int pressed = wait_for_button_press();
      if (pressed == PB_UP){
        delay(200);
        temp_hour += 1 ;
        temp_hour = temp_hour % 24 ;
      }

      else if (pressed == PB_DOWN){
        delay(200);
        temp_hour -= 1 ;
        if (temp_hour < 0)
          temp_hour = 23;
      }

      else if (pressed == PB_OK){
        delay(200);
        alarm_hours[alarm] = temp_hour ;
        break;
      }

      else if (pressed == PB_CANCEL){
        delay(200);
        break;
      }
    
  }

  int temp_minute = alarm_minutes[alarm] ;

  while (true){
    display.clearDisplay();
    print_line("Enter minute : " + String(temp_minute),0,0,2);

    int pressed = wait_for_button_press();
      if (pressed == PB_UP){
        delay(200);
        temp_minute += 1 ;
        temp_minute = temp_minute % 24 ;
      }

      else if (pressed == PB_DOWN){
        delay(200);
        temp_minute -= 1 ;
        if (temp_minute < 0)
          temp_minute = 23;
      }

      else if (pressed == PB_OK){
        delay(200);
        alarm_minutes[alarm] = temp_minute ;
        break;
      }

      else if (pressed == PB_CANCEL){
        delay(200);
        break;
      }
    
  }

  display.clearDisplay();
  print_line("Alarm is set",0,0,2);
  delay(1000);

}

void run_mode(int mode){
  if (mode==0){
    set_time();
  }
  else if (mode==1 || mode==2){
     set_alarm(mode-1);
  }
  else if(mode==3){
     alarm_enabled=false;
  }
  //   display.clearDisplay();
  //   print_line("Alarms    Disabled",0,0,2);
  //   delay(1000);
  // }
  // else if(mode==3){
  //   //display_alarm();
  // }
}