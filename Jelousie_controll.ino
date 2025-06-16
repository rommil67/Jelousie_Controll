#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Piny
const int potPin1 = A0; // potencjometr główny
const int potPin2 = A1; // porównawczy
const int potPin3 = A2; // ustawienie DEADZONE
const int potPin4 = A3; // ustawienie RAMP
const int dirUpPin = 2;
const int dirDownPin = 3;
const int pwmPin = 5; // PWM (analogWrite na pinie PWM)

// Zmienne dynamiczne
float DEADZONE_PERCENT = 3.0;
float PWM_RAMP_PERCENT = 15.0;

// Pomocnicze
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 50;

byte barChars[5][8] = {
  {B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000},
  {B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000},
  {B11100, B11100, B11100, B11100, B11100, B11100, B11100, B11100},
  {B11110, B11110, B11110, B11110, B11110, B11110, B11110, B11110},
  {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111}
};

void setup() {
  lcd.begin();
  lcd.setBacklight(1);
  lcd.clear();

  for (int i = 0; i < 5; i++) {
    lcd.createChar(i, barChars[i]);
  }

  pinMode(dirUpPin, OUTPUT);
  pinMode(dirDownPin, OUTPUT);
}

void drawBar(int value, int row) {
  int steps = map(value, 0, 1023, 0, 80); // 16 * 5
  int fullChars = steps / 5;
  int rem = steps % 5;

  lcd.setCursor(0, row);
  for (int i = 0; i < 16; i++) {
    if (i < fullChars) lcd.write(byte(4));
    else if (i == fullChars && rem > 0) lcd.write(byte(rem - 1));
    else lcd.print(" ");
  }
}

void loop() {
  if (millis() - lastUpdateTime >= updateInterval) {
    lastUpdateTime = millis();

    int val1 = analogRead(potPin1);
    int val2 = analogRead(potPin2);
    int setDead = analogRead(potPin3);
    int setRamp = analogRead(potPin4);

    DEADZONE_PERCENT = map(setDead, 0, 1023, 100, 700) / 100.0; // 1.0–7.0%
    PWM_RAMP_PERCENT = map(setRamp, 0, 1023, 0, 3000) / 100.0;  // 0.0–30.0%

    float diff = val2 - val1;
    float diffAbs = abs(diff);
    float diffPercent = (diffAbs / 1023.0) * 100.0;
    byte pwmOut = 0;

    if (diffPercent < DEADZONE_PERCENT) {
      digitalWrite(dirUpPin, LOW);
      digitalWrite(dirDownPin, LOW);
      pwmOut = 0;
    } else {
      if (diff > 0) {
        digitalWrite(dirUpPin, HIGH);
        digitalWrite(dirDownPin, LOW);
      } else {
        digitalWrite(dirUpPin, LOW);
        digitalWrite(dirDownPin, HIGH);
      }

      if (diffPercent < PWM_RAMP_PERCENT) {
        pwmOut = map(diffPercent * 100, DEADZONE_PERCENT * 100, PWM_RAMP_PERCENT * 100, 0, 255);
        pwmOut = constrain(pwmOut, 0, 255);
      } else {
        pwmOut = 255;
      }
    }

    analogWrite(pwmPin, pwmOut);

    // Ekran LCD
    drawBar(val1, 0); // drawbar pozycji
    lcd.setCursor(0, 1);
    lcd.print("DZ:");
    lcd.print(DEADZONE_PERCENT, 1);
    lcd.print("% R:");
    lcd.print(PWM_RAMP_PERCENT, 1);
    lcd.print("%");
  }
}
