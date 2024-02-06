#include <DHTesp.h>

const int DHT_PIN = 15 ;

DHTesp dhtSensor;

void setup() {
  Serial.begin(115200);
}