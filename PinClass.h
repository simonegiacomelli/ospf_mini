

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

extern PinClass BuiltinLed