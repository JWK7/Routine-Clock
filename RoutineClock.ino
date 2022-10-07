#define USE_ARDUINO_INTERRUPTS false
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
#include <dht.h>
#include <PulseSensorPlayground.h>

#define DHT11_PIN 7
DS3231  rtc(SDA, SCL);
LiquidCrystal_I2C lcd(0x27, 16, 2);
dht DHT;
Time t;
const int buttonPin = 2;
int buttonState = 0;
const int PulseWire = 0;       
int Threshold = 550; 
PulseSensorPlayground pulseSensor;
int hoursSlept[7] = {-1,-1,-1,-1,-1,-1,-1};
int startSleep = -1;
int endSleep = -1;
int hoursSleptIndex = 0;
void setup() {
  pulseSensor.analogInput(PulseWire); 
  pulseSensor.setThreshold(Threshold);
  pulseSensor.begin();
  pinMode(buttonPin, INPUT);
	lcd.begin();
  lcd.backlight();
  rtc.begin();
  rtc.setDOW(FRIDAY); 
  rtc.setTime(7, 59, 40); 
  rtc.setDate(7, 10, 2022);
}

void showSleep(int start,int end)
{
  if (start != -1)
  {
    hoursSlept[hoursSleptIndex] = end-start;
    hoursSleptIndex+=1;
    if (hoursSleptIndex == 7)
    {
      hoursSleptIndex=0;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Slept ");
    lcd.print(end-start);
    lcd.print(" Hrs");
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Slept N/A Hrs");
  }
  int averageSleep = 0;
  int error = 0;
  for (int i=0 ;i<7;i++)
  {
    if (hoursSlept[i] == -1)
    {
      error=1;
    }
    else
    {
      averageSleep+=hoursSlept[i];
    }
  }
  if (error==1)
  {
    lcd.setCursor(0, 1);
    lcd.print("Avg: N/A Hrs");
  }
  else{
    lcd.setCursor(0, 1);
    lcd.print("Avg: ");
    lcd.print(averageSleep/7);
    lcd.print(" Hrs");
  }
  return;
}

void loop() {
  lcd.clear();
  lcd.setCursor(1, 1);
  lcd.print(rtc.getTimeStr());
  lcd.setCursor(0, 0);
  lcd.print(rtc.getDOWStr(FORMAT_SHORT));
  lcd.print(" ");
  lcd.print(rtc.getDateStr(FORMAT_SHORT));
  lcd.setCursor(0, 1);
  lcd.print(rtc.getTimeStr());
  int chk = DHT.read11(DHT11_PIN);
  lcd.print(" ");
  lcd.print((int)(DHT.temperature* 9/5+32));
  lcd.print((char)223);
  lcd.print(" ");
  lcd.print((int)DHT.humidity);
  lcd.print("%");

  t= rtc.getTime();
  if (t.hour==0 && t.min==0 && t.sec==0)
  {
    buttonState = digitalRead(buttonPin);
    while (buttonState != HIGH)
    {
      buttonState = digitalRead(buttonPin);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sleep");
    }
    t= rtc.getTime();
    startSleep = t.hour+t.min/60;
  }
  if (t.hour==8 && t.min==0 && t.sec==0)
  {
    //Wake up
    buttonState = digitalRead(buttonPin);
    while (buttonState != HIGH)
    {
      buttonState = digitalRead(buttonPin);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wake Up");
    }
    t=rtc.getTime();
    endSleep = t.hour+t.min/60;
    showSleep(startSleep,endSleep);
    endSleep = -1;
    startSleep = -1;
    delay (3000);
    buttonState = digitalRead(buttonPin);
    while (buttonState != HIGH)
    {
      buttonState = digitalRead(buttonPin);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Make Bed");
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nice!");
    delay (3000);
    buttonState = digitalRead(buttonPin);
    while (buttonState != HIGH)
    {
      buttonState = digitalRead(buttonPin);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Check Heartrate");
      lcd.setCursor(0, 1);
      lcd.print("Press to Start");
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Recording...");
    double average=0;
    int i = 0;
    lcd.setCursor(0,1);
    while (i != 16)
    {
      pulseSensor.sawNewSample();
      int myBPM = pulseSensor.getBeatsPerMinute();
      if (pulseSensor.sawStartOfBeat()) {
        lcd.print("-");
        i+=1;
        average+=myBPM/16;
      }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Done!");
    lcd.setCursor(0, 1);
    lcd.print("Average BPM: ");
    lcd.print(average);
    delay (3000);
    buttonState = digitalRead(buttonPin);
    while (buttonState != HIGH)
    {
      buttonState = digitalRead(buttonPin);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Shower");
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nice!");
    delay (3000);
    buttonState = digitalRead(buttonPin);
    while (buttonState != HIGH)
    {
      buttonState = digitalRead(buttonPin);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Eat");
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nice!");
    delay (3000);
    buttonState = digitalRead(buttonPin);
    while (buttonState != HIGH)
    {
      buttonState = digitalRead(buttonPin);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Heading Out");
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("See You!");
    delay (3000);
  }
  delay (1000);
}
