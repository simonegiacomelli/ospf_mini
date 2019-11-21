class Example {
  public:
    Example(int apin, int adelay) {
      _delay =  adelay;
      _pin = apin;
    }
    void fasterBlink() {
      pinMode(_pin, OUTPUT);
      while (1) {
        digitalWrite(_pin, HIGH);
        delay(_delay);
        digitalWrite(_pin, LOW);
        delay(_delay);
      }
    }
    void start(){
      _th.start(mbed::callback(this, &Example::fasterBlink));
    }
  private:
    int _pin = -1;
    int _delay = 0;
    rtos::Thread _th;
};

mbed::DigitalOut ledg(digitalPinToPinName(LEDB));

void ledg_oneshot(){
  ledg = !ledg;
}

Example ex(LEDR, 1000);
Example ex2(LEDG, 500);
mbed::Ticker ledTicker;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  //th.start(ex.fasterBlink);
  ex.start();
  ex2.start();
  ledTicker.attach(ledg_oneshot,1.5f);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  Serial.print(millis());
  Serial.println(" hi! ");
}
