const int inMeetingLED = D7;
const int newMeetingSoonLED = D6;
const int pingingWifiLED = D5;

void setupLeds() {
  pinMode(pinoLEDR, OUTPUT);
  pinMode(pinoLEDY, OUTPUT);
  pinMode(pinoLEDG, OUTPUT);

  pinMode(inMeetingLED, OUTPUT);
  pinMode(newMeetingSoonLED, OUTPUT);
  pinMode(pingingWifiLED, OUTPUT);
}

void signalInMeeting() {
  turnEverythingOff();
  turnOn(inMeetingLED);
}

void signalMeetingSoon() {
  turnEverythingOff();
  turnOn(newMeetingSoonLED);
}

void signaWifiPinging() {
  turnOn(pingingWifiLED);
  delay(200);
  turnOff(pingingWifiLED);
}


void turnOn(int pin) {
  digitalWrite(pin, HIGH);
}

void turnOff(int pin) {
  digitalWrite(pin, LOW);
}

void turnEverythingOff() {
  turnOff(inMeetingLED);
  turnOff(newMeetingSoonLED);
  turnOff(pingingWifiLED);
}