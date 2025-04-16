struct LED : Service::LightBulb {               // First we create a derived class from the HomeSpan LightBulb Service

  int ledPin;                                       // this variable stores the pin number defined for this LED
  SpanCharacteristic *power;                        // here we create a generic pointer to a SpanCharacteristic named "power" that we will use below

  int powerPin;                                     // NEW! pin with pushbutton to turn on/off LED
  
  LED(int ledPin, int powerPin) : Service::LightBulb(){

    power=new Characteristic::On();                 // this is where we create the On Characterstic we had previously defined in setup().  Save this in the pointer created above, for use below
    
    new SpanButton(powerPin);                       // NEW! create new SpanButton to control power using pushbutton on pin number "powerPin"

    this->ledPin=ledPin;                            // don't forget to store ledPin...
    this->powerPin=powerPin;                        // NEW! save power pushbutton pin number

    pinMode(ledPin,OUTPUT);                         // ...and set the mode for ledPin to be an OUTPUT (standard Arduino function)
    
  }

  boolean update(){            

    digitalWrite(ledPin,power->getNewVal());        // use a standard Arduino function to turn on/off ledPin based on the return of a call to power->getNewVal() (see below for more info)
   
    return(true);                                   // return true to indicate the update was successful (otherwise create code to return false if some reason you could not turn on the LED)
  
  }

  void button(int pin, int pressType) override {

    if(pin==powerPin){
      if(pressType==SpanButton::SINGLE){            // if a SINGLE press of the power button...
        power->setVal(1-power->getVal());           // ...toggle the value of the power Characteristic
        digitalWrite(ledPin,power->getNewVal());        // use a standard Arduino function to turn on/off ledPin based on the return of a call to power->getNewVal() (see below for more info)
      };      
    };

  }

};
      
