#include "HomeSpan.h" 
#include "LED.h"     
#include "Identify.h"       

void setup() {

  // Serial.begin(115200);
  
  homeSpan.setApSSID("Apple-LED");
  homeSpan.setApPassword("12345678");
  
  homeSpan.setControlPin(19);
  homeSpan.setStatusPin(18);

  homeSpan.enableAutoStartAP();
  
  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");

 new SpanAccessory();  
   new Service::AccessoryInformation();
     new Characteristic::Identify(); 

 new SpanAccessory();                                                          
   new Service::AccessoryInformation();
     new Characteristic::Identify(); 
     new Characteristic::Name("LED");
   
     new LED(14, 17);

}


void loop(){
  
  homeSpan.poll();
  
}



  
