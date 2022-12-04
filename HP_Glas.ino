/*
 Name:    Sketch1.ino
 Created: 08.08.2022 14:26:50
 Author:  R. Behrens
*/

const int fotoIn = A7;                // Eingangspin für die Fotodiode
const int house = 10;                 // Ausgangspin für die LED im Haus
const int torch = 11;                 //  Ausgangspin fr die LED in der Fackel
const int testLED = 13;               // Ausgang für die PlatinenLED zum testen
const int lighH = 80;                // Helligkeit der LED im Haus
//---
int dark = 800;                       // def. ab wann das licht angehen soll
int tolerance = 30;                   // tolleranz der erreicht werden muss damit das  licht wieder aus geht
int sensorWert = 0;                   // Variable für den Startwert (sensor)
const long zeitD = 5000;               // X Millisek prüfen ob dunkel schwelle erhalten bleibt
//-------------------------------------------------------------
int beginP = 0;                       // für die überprüfung ob das Programm gerade gestartet wurde
const long interval = 30000;          // inizialisierzeit in Millisek.
unsigned long previousMillis = 0;
//-------------------------------------------------------------
bool darkT = false;                   // boolean für die Dämmmerungsabfrage
//-------------------------------------------------------------
// Werte für die fackel
const long timerF = 14400000;              // mind zeit die das licht nach auslösung an bleibt in Millisek.

int minLight = 1;                    // Min. für die zufallauswahl der Helligkeit 0 = LED aus, 255 = LED maximale Helligkeit.
int maxLight = 20;                   // Max. für die zufallauswahl der Helligkeit 0 = LED aus, 255 = LED maximale Helligkeit.
//---
int minD = 30;                        // Min. für die zufallauswahl des Delays 100 = schnell, 800 = langsam.
int maxD = 150;                       // Min. für die zufallauswahl des Delays 100 = schnell, 800 = langsam.
//---
int val = 0;                          // Variable fürs flackern startet bei 0
int delayval = 0;                     // variable für zufall delay startet bei 0
//---------------------------------------------------------------
const long cooldownT = 21600000;          // zeit für den Cooldown in Millisek.
//---------------------------------------------------------------------------------------------------


void setup() {
  Serial.begin(9600);                 //Start der Seriellen Kommunikation
  pinMode(testLED, OUTPUT);           // definition LED auf board als Test/Kontroll LED
  
  Serial.print("int dark = ");
  Serial.println(dark);

  
}

//___________________________________________________________________________________________________
void loop() {
  unsigned long currentMillis = millis();


  if (beginP == 0) {
    while (currentMillis - previousMillis <= interval) {
    currentMillis = millis();
    
      if (currentMillis - previousMillis >= interval) {
        dark = analogRead(fotoIn);
        Serial.print("initialisierung abgeschlossen = ");
        Serial.println(dark);
        beginP = 1;
      }
      else {
        Serial.println("Wait..");
        Serial.println(currentMillis);
      }
    }
  }

  
    sensorWert = analogRead(fotoIn);
    Serial.print("Sensorwert = ");
    Serial.println(sensorWert);
    int switchD = 0;
    
    if (sensorWert < dark) {
      currentMillis = millis();
      previousMillis = currentMillis;
        
      while (switchD == 0) {
        currentMillis = millis();
        

        sensorWert = analogRead(fotoIn);
        Serial.print("(in messung) Sensorwert = ");
        Serial.println(sensorWert);

        if (sensorWert < dark) {
          if (currentMillis - previousMillis >= zeitD) {
            switchD = 2;
          }
          else {
            delay(100);
          }
        }
        else {
          switchD = 1;
          Serial.println("abbruch");
        }
      }
    }
      
    if (switchD == 2) {
      digitalWrite(testLED, HIGH);
      analogWrite(house, lighH);
      delay(250);
      fackel();    
    
    }
    digitalWrite(testLED, LOW);
    analogWrite(house, 0);
    analogWrite(torch, 0);
    delay(300);
  

}

//___________________________________________________________________________________________________
void fackel() {
  unsigned long currentMillis = millis();
  bool reset = false;
  
  while (reset == false) {
    currentMillis = millis();
    previousMillis = currentMillis;
    
    while (currentMillis - previousMillis <= timerF) {
      currentMillis = millis();
      val = random(minLight, maxLight);         // Zufallszahl Helligkeit.
      analogWrite(torch, val);                  // Setzt das PWM Signal auf die Zufallszahl 
      delayval = random(minD,maxD);             // Zufallszahl für den Delay
      delay(delayval);                          // Setze die Wartezeit auf die Zufallszahl
    }

    sensorWert = analogRead(fotoIn);
    Serial.print("<in Funktion> Sensorwert = ");
    Serial.println(sensorWert);
  
    if (sensorWert > dark + tolerance ) {
      reset = true;
      analogWrite(torch, 0);
      delay(250);
      analogWrite(house, 0);
      digitalWrite(testLED, LOW);
    }
  }
  cooldown();
}

//___________________________________________________________________________________________________
void cooldown(){
  unsigned long currentMillis = millis();
  bool cd = false;
  Serial.println("Cooldown EIN");
  currentMillis = millis();
  previousMillis = currentMillis;

  while (cd == false) {
    currentMillis = millis();
    digitalWrite(testLED, HIGH);
    delay(100);
    digitalWrite(testLED, LOW);
    
      if (currentMillis - previousMillis >= cooldownT) {
        Serial.println("Cooldown ende");
        delay(200);
        cd = true;
         
      }
  }
  
      
}
