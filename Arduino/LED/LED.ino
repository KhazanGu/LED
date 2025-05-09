#include "HomeSpan.h" 
#include "LED.h"     
#include "Identify.h"       

void setup() {

  // Serial.begin(115200);
  
  homeSpan.setApSSID("Apple-LED");
  homeSpan.setApPassword("12345678");

  homeSpan.setStatusPin(18);
  homeSpan.setControlPin(19);

  homeSpan.enableAutoStartAP();

  homeSpan.setStatusAutoOff(120);

  homeSpan.begin(Category::Lighting,"LED");

  new SpanAccessory();                                                          
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      new Characteristic::Name("LED");
      
    new LED(14, 17);

}


void loop(){
  
  homeSpan.poll();
  
}



  
