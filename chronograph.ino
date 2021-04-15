#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

int firstLaserPin = 2;
int firstReceiverPin = 3;
int secondLaserPin = 4;
int secondReceiverPin = 5;
int ledPin = 7;
int shineDelay = 25;
unsigned long firstTime;
unsigned long secondTime;

unsigned long firstUs;
unsigned long secondUs;
float totalUs;

float totalPeriod;
float vSpeed = 0.0;
float fDistanse = 0.1; // unit: m
bool bReset = false;
bool bFirst = false;
bool bSecond = false;
void setup()
{
    // 初始化Serial
    Serial.begin(9600);

    // 初始化LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Waiting for test");
    lcd.setCursor(0, 1);
    lcd.print("*******  *******");

    // 初始化第一個感測器
    pinMode(firstLaserPin, OUTPUT);
    pinMode(firstReceiverPin, INPUT);
    digitalWrite(firstLaserPin, HIGH);

    // 初始化第二個感測器
    pinMode(secondLaserPin, OUTPUT);
    pinMode(secondReceiverPin, INPUT);
    digitalWrite(secondLaserPin, HIGH);

    // 初始化LED
    pinMode(ledPin, OUTPUT);
}

void loop()
{
    // Serial.println(digitalRead(firstReceiverPin));

    // first = 0, second = 1
    if (firstTime == 0 && !bFirst && !digitalRead(firstReceiverPin) && digitalRead(secondReceiverPin))
    {
        bReset = true;
        bFirst = true;
        firstTime = millis();
        firstUs = micros();
        digitalWrite(ledPin, HIGH);
    }
    // first = 1, second = 0
    if (firstTime != 0 && !bSecond && secondTime == 0 && digitalRead(firstReceiverPin) && !digitalRead(secondReceiverPin))
    {
        bSecond = true;
        secondTime = millis();
        secondUs = micros();
        digitalWrite(ledPin, LOW);
    }
    if (bReset && firstTime != 0 && secondTime != 0 && bFirst && bSecond)
    {
        bReset = false;
        bFirst = false;
        bSecond = false;
        totalPeriod = secondTime - firstTime;
        totalPeriod = totalPeriod / 1000;
        totalUs = secondUs - firstUs;
        totalUs = totalUs / 1000000;
        vSpeed = fDistanse / totalPeriod;        
        String strLcd = "V=" + String(vSpeed) + "m/s" + "T=" + String(totalPeriod) + "s";
        firstTime = 0;
        secondTime = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(strLcd);
        // lcd.setCursor(2, 0);
        // lcd.print(totalPeriod);
        lcd.setCursor(0, 1);
        lcd.print("*******  *******");
    }
    Serial.print("First: ");
    Serial.println(firstTime);
    Serial.print("Second: ");
    Serial.println(secondTime);
    Serial.print("Total: ");
    Serial.println(totalPeriod);
    Serial.print("Total us: ");
    Serial.println(totalUs);

    // if (firstReceiverStatus)
    // {
    //     digitalWrite(ledPin, HIGH);
    //     delay(shineDelay);
    //     digitalWrite(ledPin, LOW);
    //     delay(shineDelay);
    // }
    // else
    // {
    //     digitalWrite(ledPin, HIGH);
    // }
}
