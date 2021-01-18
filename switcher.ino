const int btns[] = {2, 4, 6, 8};
const int leds[] = {3, 5, 7, 9};
const int switcherPin = 10;
int activeOutput = 0;
bool manualPush = false;

// the setup function runs once when you press reset or power the board
void setup()
{
    for (int i = 0; i < 4; i++)
    {
        pinMode(btns[i], INPUT_PULLUP);
        pinMode(leds[i], OUTPUT);
        digitalWrite(leds[i], LOW);
    }
    pinMode(switcherPin, OUTPUT);

    digitalWrite(switcherPin, HIGH);
    digitalWrite(leds[activeOutput], HIGH);
}

void manual_mode()
{
    int numActive = 4;

    // turn off all leds
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(leds[i], LOW);
    }

    // wait for buttons to be released
    while (numActive != 0)
    {
        numActive = 4;
        for (int i = 0; i < 4; i++)
        {
            if (digitalRead(btns[i]) == HIGH)
            {
                numActive--;
            }
        }
    }

    // turn on far left and right
    digitalWrite(leds[0], HIGH);
    digitalWrite(leds[3], HIGH);

    while (true)
    {
        bool btn1Pressed = false;
        bool btn4Pressed = false;

        for (int i = 0; i < 4; i++)
        {
            if (digitalRead(btns[i]) == LOW)
            {
                if (i == 0)
                {
                    btn1Pressed = true;
                }
                else if (i == 3)
                {
                    btn4Pressed = true;
                }
            }
        }

        if (btn4Pressed)
        {

            for (int i = 0; i < 4; i++)
            {
                digitalWrite(leds[i], LOW);
            }

            while (digitalRead(btns[3]) == LOW)
            {
            }

            delay(100); // debounce
            activeOutput = 0;
            digitalWrite(leds[activeOutput], HIGH);
            return;
        }
        else if (btn1Pressed)
        {
            digitalWrite(switcherPin, LOW);
            delay(100);
            digitalWrite(switcherPin, HIGH);
            delay(100);

            while (digitalRead(btns[0]) == LOW)
            {
            } // wait for release

            delay(100); // debounce
        }
    }
}

// the loop function runs over and over again forever
void loop()
{
    bool hadKeypress = false;
    int numActive = 0;
    int newActiveOutput = activeOutput;

    for (int i = 0; i < 4; i++)
    {
        if (digitalRead(btns[i]) == LOW)
        {
            hadKeypress = true;
            newActiveOutput = i;
            numActive++;
        }
    }

    if (numActive == 4)
    {
        manual_mode();
    }
    else if (hadKeypress && numActive == 1)
    {
        digitalWrite(leds[activeOutput], LOW);
        digitalWrite(leds[newActiveOutput], HIGH);

        // send pulses
        int i = activeOutput;
        while (i != newActiveOutput)
        {
            digitalWrite(switcherPin, LOW);
            delay(100);
            digitalWrite(switcherPin, HIGH);
            delay(100);

            i = (i + 1) % 4;
        }

        activeOutput = newActiveOutput;
    }
}
