
class PinClass
{
private:
    uint8_t pin;
    bool setPinMode = true;
    bool std = true;
    uint8_t mode = OUTPUT;

public:
    PinClass(uint8_t apin)
    {
        pin = apin;
    }
    PinClass setMode(uint8_t amode)
    {
        mode = amode;
        return *this;
    }
    PinClass setup()
    {
        if (setPinMode)
            pinMode(pin, mode);
        setPinMode = false;
        return *this;
    }
    PinClass setStandard(bool val)
    {
        std = val;
        return *this;
    }
    PinClass on()
    {
        setup();
        digitalWrite(pin, std ? HIGH : LOW);
        return *this;
    }

    PinClass off()
    {
        setup();
        digitalWrite(pin, std ? LOW : HIGH);
        return *this;
    }
};

PinClass BuiltinLed = PinClass(LED_BUILTIN).setStandard(false);

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
    //th.start(ex.fasterBlink);
    ex.start();
    ex2.start();
    ledTicker.attach(ledg_oneshot,1.5f);
}

// the loop function runs over and over again forever
void loop() {
    BuiltinLed.on();
    delay(1000);                       // wait for a second
    BuiltinLed.off();
    delay(1000);                       // wait for a second
    Serial.print(millis());
    Serial.println(" hi! ");
}


/*
#include "mbed.h"
using namespace std;
using namespace mbed;

DigitalOut led1(LEDG);
InterruptIn sw(SW2);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;

void rise_handler(void) {
    printf("rise_handler in context %p\r\n", Thread::gettid());
    led1 = !led1;
}

void fall_handler(void) {
    printf("fall_handler in context %p\r\n", Thread::gettid());
    led1 = !led1;
}

int main() {
    // Start the event queue
    t.start(callback(&queue, &EventQueue::dispatch_forever));
    printf("Starting in context %p\r\n", Thread::gettid());
    // The 'rise' handler will execute in IRQ context
    sw.rise(rise_handler);
    // The 'fall' handler will execute in the context of thread 't'
    sw.fall(queue.event(fall_handler));
}
 */