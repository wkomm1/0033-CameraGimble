const byte AZ_EN = 15;
const byte EL_EN = 21;
const byte AZ_DIR = 16;
const byte EL_DIR = 20;
const byte AZ_STEP = 22;
const byte EL_STEP = 17;
const byte SERVO_01 = 3;
const byte SERVO_02 = 4;


int AZ_Pos = 0;
int EL_Pos = 0;
int AZ_Target = 0;
int EL_Target = 0;
int stepTime = 1000;
byte serialInput = 0;
int minStepTime = 1000;
int maxStepTime = 10000;
const int STEP_PER_REV_AZ = 2000;
const int STEP_PER_REV_EL = 2000;


void setup() {
  Serial.begin(250000);  //usb serial
  Serial1.begin(115200);  //bluetooth serial
  pinMode(AZ_EN, OUTPUT); digitalWrite(AZ_EN, HIGH);  //active low
  pinMode(EL_EN, OUTPUT); digitalWrite(EL_EN, HIGH);  //active low
  pinMode(AZ_DIR, OUTPUT); digitalWrite(AZ_DIR, LOW);
  pinMode(EL_DIR, OUTPUT); digitalWrite(EL_DIR, LOW);
  pinMode(AZ_STEP, OUTPUT); digitalWrite(AZ_STEP, LOW);
  pinMode(EL_STEP, OUTPUT); digitalWrite(EL_STEP, LOW);
}

unsigned long timerx = 0;

void loop() {
  while (1) {
    if (millis() - timerx < 1500) {
      AZ_Target = 200;
      EL_Target = 200;
    }
    else if (millis() - timerx > 3000) {
      delay(2000);
      timerx = millis();
    }
    else {
      AZ_Target = 0;
      EL_Target = 0;
    }
    updateAZPos(); updateELPos();
  }
  serialInput = Serial.read();
  if (serialInput == 'R' || serialInput == 'r') {
    digitalWrite(AZ_EN, HIGH); digitalWrite(EL_EN, HIGH);
    AZ_Pos = 0; EL_Pos = 0;
    AZ_Target = 0; EL_Target = 0;
  }
  else if (serialInput == 'J' || serialInput == 'j') AZ_Target -= 3;
  else if (serialInput == 'L' || serialInput == 'l') AZ_Target += 3;
  else if (serialInput == 'I' || serialInput == 'i') EL_Target += 3;
  else if (serialInput == 'K' || serialInput == 'k') EL_Target -= 3;
  else if (serialInput == 'A' || serialInput == 'a') AZ_Target = Serial.parseInt();
  else if (serialInput == 'E' || serialInput == 'e') EL_Target = Serial.parseInt();
  if (serialInput == 'F' || serialInput == 'f') {
    stepTime = Serial.parseInt();
    if (stepTime > maxStepTime) stepTime = maxStepTime;
    else if (stepTime < minStepTime) stepTime = minStepTime;
    //else stepTime = map(stepTime, minStepTime, maxStepTime, maxStepTime, minStepTime);
  }
  updateAZPos(); updateELPos();
  Serial.print("AZ POS:"); Serial.print(AZ_Pos);
  Serial.print("  EL POS:"); Serial.print(EL_Pos);
  Serial.print("  StepTime:"); Serial.print(stepTime);
  Serial.println();
}


void updateAZPos() {
  if (AZ_Target > AZ_Pos) {
    digitalWrite(AZ_EN, LOW); digitalWrite(AZ_DIR, LOW);
    digitalWrite(AZ_STEP, HIGH); delayMicroseconds(stepTime);
    digitalWrite(AZ_STEP, LOW); delayMicroseconds(stepTime);
    AZ_Pos++;
  }
  else if (AZ_Target < AZ_Pos) {
    digitalWrite(AZ_EN, LOW); digitalWrite(AZ_DIR, HIGH);
    digitalWrite(AZ_STEP, HIGH); delayMicroseconds(stepTime);
    digitalWrite(AZ_STEP, LOW); delayMicroseconds(stepTime);
    AZ_Pos--;
  }
}


void updateELPos() {
  if (EL_Target > EL_Pos) {
    digitalWrite(EL_EN, LOW); digitalWrite(EL_DIR, HIGH);
    digitalWrite(EL_STEP, HIGH); delayMicroseconds(stepTime);
    digitalWrite(EL_STEP, LOW); delayMicroseconds(stepTime);
    EL_Pos++;
  }
  else if (EL_Target < EL_Pos) {
    digitalWrite(EL_EN, LOW); digitalWrite(EL_DIR, LOW);
    digitalWrite(EL_STEP, HIGH); delayMicroseconds(stepTime);
    digitalWrite(EL_STEP, LOW); delayMicroseconds(stepTime);
    EL_Pos--;
  }
}

