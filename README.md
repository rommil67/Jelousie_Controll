# Jelousie_Controll
Kontrola _ Jelousje
Opis:
Funkcja / Element	Pin Arduino Uno	Pin ATmega328P	Opis
Potencjometr główny (val1)	A0	PC0 (pin 23)	Pomiar głównego napięcia
Potencjometr porównawczy (val2)	A1	PC1 (pin 24)	Napięcie odniesienia do porównania
Potencjometr DEADZONE (setDead)	A2	PC2 (pin 25)	Ustawienie zakresu martwego (1–7%)
Potencjometr RAMP (setRamp)	A3	PC3 (pin 26)	Ustawienie progu rampowania PWM (0–30%)
Sygnał kierunku w górę	D2	PD2 (pin 4)	HIGH gdy różnica dodatnia
Sygnał kierunku w dół	D3	PD3 (pin 5)	HIGH gdy różnica ujemna
Wyjście PWM	D5 (PWM)	PD5 (pin 11)	Sygnał PWM proporcjonalny do różnicy potencjometrów
I2C - SDA (LCD 2x16 z modułem I2C)	A4	PC4 (pin 27)	Linia danych I2C
I2C - SCL (LCD 2x16 z modułem I2C)	A5	PC5 (pin 28)	Linia zegara I2C
