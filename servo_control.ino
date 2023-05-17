int ledPin = 5;
bool ledState = 0;
int servoPin = 22;
unsigned long count = 0;
int diffBound = 150;
int middle = 1500;
int lowerBound = middle - diffBound;
int upperBound = middle + diffBound;
int dTime = middle;
int mode = 0;
const int OMG_BUTTON_PIN = 36;
const int MODE_BUTTON_PIN = 0;

void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(servoPin, OUTPUT);
    pinMode(OMG_BUTTON_PIN, INPUT);
    pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
    Serial.begin(115200);
}

void cycle() {
  if (dTime != middle) {
    dTime = middle;
  } else {
    dTime = upperBound;
    upperBound = lowerBound;
    lowerBound = dTime;
  }
}

void loop()
{
  while (1) {
    if (digitalRead(MODE_BUTTON_PIN) == LOW) {
        mode = !mode;
        while(digitalRead(MODE_BUTTON_PIN) == LOW) {
          delay(10); // Button pressing will cycle multiple times
        } 
    }
    if (digitalRead(OMG_BUTTON_PIN) == HIGH && (mode == 0)) {
      count = 0;
      digitalWrite(ledPin, ledState);
      ledState = !ledState;
      cycle();
      while(digitalRead(OMG_BUTTON_PIN) == HIGH) {
        delay(10); // Button pressing will cycle multiple times
      }
    }
    if (digitalRead(OMG_BUTTON_PIN) == HIGH && (mode == 1)) {
      mode = 0;
    }
    if (count > 200000) {
      count = 0;
      mode = 1;  // Cycle when ignored
    }
    if ((mode == 1) && (count % 300 == 0)) {
      digitalWrite(ledPin, ledState);
      ledState = !ledState;
      cycle();
      count += 1;
    }
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(dTime);
    digitalWrite(servoPin, LOW);
    delayMicroseconds(dTime);
    count += 1;
    if (count % 1000 == 0) {
        Serial.print("Count: ");
        Serial.println(count);
    }
  } // end while
} // end main