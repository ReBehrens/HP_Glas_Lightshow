/*
 Name:    Sketch1.ino
 Created: 08.08.2022 14:26:50
 Author:  R. Behrens
*/

const int fotoIn = A7;                // Eingangspin für die Fotodiode
const int house = 10;                 // Ausgangspin für die LED im Haus
const int torch = 11;                 //  Ausgangspin fr die LED in der Fackel
const int testLED = 13;               // Ausgang für die PlatinenLED zum testen
//---
int dark = 800;                       // def. ab wann das licht angehen soll
int sensorWert = 0;                   // Variable für den Startwert (sensor)
//-------------------------------------------------------------
// Werte für die fackel

int minLight = 30;                    // Min. für die zufallauswahl der Helligkeit 0 = LED aus, 255 = LED maximale Helligkeit.
int maxLight = 127;                   // Max. für die zufallauswahl der Helligkeit 0 = LED aus, 255 = LED maximale Helligkeit.
//---
int minD = 30;                        // Min. für die zufallauswahl des Delays 100 = schnell, 800 = langsam.
int maxD = 100;                       // Min. für die zufallauswahl des Delays 100 = schnell, 800 = langsam.
//---
int val = 0;                          // Variable fürs flackern startet bei 0
int delayval = 0;                     // variable für zufall delay startet bei 0

//---------------------------------------------------------------------------------------------------


void setup() {
  Serial.begin(9600);                 //Start der Seriellen Kommunikation
  pinMode(testLED, OUTPUT);           // definition LED auf board als Test/Kontroll LED
  pinMode(house, OUTPUT);             // definition des Pins als LED pin (Ausgang) im Haus
  pinMode(torch, OUTPUT);             // definition des Pins als LED pin (Ausgang) in der Fackel
}

void loop() {
  sensorWert = analogRead(fotoIn);
  Serial.print("Sensorwert = ");
  Serial.println(sensorWert);

  if (sensorWert < dark) {
    digitalWrite(testLED, HIGH);
    analogWrite(house, 255);
    delay(250);
    fackel();    
    
  } else {
    digitalWrite(testLED, LOW);
    analogWrite(house, 0);
    analogWrite(torch, 0);
  }
  delay(100);

}


void fackel() {

  bool reset = false;
      
  while (reset == false) {
    for (int i = 0; i < 20; i++){
      val = random(minLight, maxLight);         // Zufallszahl Helligkeit.
      analogWrite(torch, val);                  // Setzt das PWM Signal auf die Zufallszahl 
      delayval = random(minD,maxD);             // Zufallszahl für den Delay
      delay(delayval);                          // Setze die Wartezeit auf die Zufallszahl
    }

    sensorWert = analogRead(fotoIn);
    Serial.print("<in Funktion> Sensorwert = ");
    Serial.println(sensorWert);
  
    if (sensorWert > dark + 30 ) {
      reset = true;
      analogWrite(torch, 0);
      delay(250);
      analogWrite(house, 0);
    }
  }
}
