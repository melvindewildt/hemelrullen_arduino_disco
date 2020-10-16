//Inputs
const int microphonePin = 0;
int switchPin = 13;           // drukknop pin (aan/uit)  (12 is ingang)
int partyPin = 8;
int potPin = 2;               // potmeter pin

//Outputs
int ledPin = LED_BUILTIN;      // ingebouwde led, indicatie ON/OFF
const int ledPin1 = 24;
const int ledPin2 = 25;
const int ledPin3 = 26;
const int ledPin4 = 27;
const int ledPin5 = 28;
const int ledPin6 = 29;
const int ledPin7 = 30;
const int ledPin8 = 31;
const int ledPin9 = 32;
const int ledPin10 = 33;
const int ledPin11 = 34;
const int ledPin12 = 35;
const int ledPin13 = 36;
const int ledPin14 = 37;
const int ledPin15 = 38;

#define L1 24
#define L2 25
#define L3 26
#define L4 27
#define L5 28
#define L6 29
#define L7 30
#define L8 31
#define L9 32
#define L10 33
#define L11 34
#define L12 35
#define L13 36
#define L14 37
#define L15 38

#define pinOn 0
#define pinOff 1
#define PIN_ON 0
#define PIN_OFF 1

// initial values
int potVal = 0;               // initiele potmetervalue
int switchState = LOW;        // initiele waarde van drukknop


// variables
float BPM;
float BPS;
float BeatTimeMS;

// ledStates is een vector waarbij voor elke LED van de 8 LEDs wordt aangegeven of ie uit(0) of aan(1) staat. Dus ledStates[1] = 1 betekent dat de LED op ledPin2 aan gaat.
int ledStates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Aantal parameters dat je kan tunen
float trigger_threshold_gain = 3;
const long interval = 100;             //Dit is het interval in millisecond dat in ieder geval voorbij moet zijn voordat er weer nieuwe trigger gegeven kan worden
#define WINDOW_SIZE 200                //Dit is de window waarover de moving average wordt genomen

// Initialiseren van een aantal variabelen
unsigned long previousMillis = 100;
unsigned long currentMillis = millis();
float sample;
int   INDEX = 0;
float VALUE = 0;
float SUM = 0;
float READINGS[WINDOW_SIZE];
float AVERAGED = 0;
bool trigger = false;
int stap = 0;
int programStateNumber = 0; // Determines which program to run [0, 1, 2, 3, 4]
int goToNextProgram = 0;
int partyMode = 1;
float BPMfraction = 1.0;
char buffer[30];

// Weer wat dingen intialiseren
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);         // stel seriele monitor in
  //inputs
  pinMode(switchPin, INPUT_PULLUP);  // switch is input
  pinMode(partyPin, INPUT_PULLUP);
  pinMode(potPin, INPUT);     // potmeter is input

  //outputs
  pinMode(ledPin, OUTPUT);  // LED ingebouwd is output
  pinMode (ledPin1, OUTPUT);
  pinMode (ledPin2, OUTPUT);
  pinMode (ledPin3, OUTPUT);
  pinMode (ledPin4, OUTPUT);
  pinMode (ledPin5, OUTPUT);
  pinMode (ledPin6, OUTPUT);
  pinMode (ledPin7, OUTPUT);
  pinMode (ledPin8, OUTPUT);
  pinMode (ledPin9, OUTPUT);
  pinMode (ledPin10, OUTPUT);
  pinMode (ledPin11, OUTPUT);
  pinMode (ledPin12, OUTPUT);
  pinMode (ledPin13, OUTPUT);
  pinMode (ledPin14, OUTPUT);
  pinMode (ledPin15, OUTPUT);
  Serial.begin(9600);
}

// Vanaf hier loopt er constant een loop
void loop() {
  currentMillis = millis();
  partyMode = digitalRead(partyPin);
  //  switchState = digitalRead(switchPin);   // lees switch(LOW/HIGH)
  //Serial.println(switchState);            // Toon waarde seriele monitor (print line)

  potVal = analogRead(potPin);            // lees potmeter (0-1023)
  //Serial.println(potVal);                 // Print waarde seriele monitor

  //Serial.println(currentMillis);
  //  Serial.println(previousMillis);
  //  Serial.println(trigger);

  BPM = 90 + (potVal / 1023.0) * 520;        // BPM between 90 and 200
  BPS = BPM / 60.0; // Beats per second
  //  Serial.println(BPM);
  BeatTimeMS = 1 / BPS * 1000.0;
  //  Serial.println(BeatTimeMS);

  goToNextProgram = digitalRead(switchPin);
  Serial.println(goToNextProgram);
  if (goToNextProgram == 0) {
    delay(200);
    // at end of array so next one is then zero
    if (programStateNumber == 10) { // Nu zijn er 3 programma's, zero, one & two. Als er meer zijn moet je dit mee aanpassen
      programStateNumber = 0; // Reset
    } else {
      programStateNumber = programStateNumber + 1; // Go to next
      stap = 0;
    }
  }

  sprintf(buffer, "Number %d on the screen", programStateNumber); // godverdomme laten staan
  //Serial.println(buffer);
  Serial.println(programStateNumber);

  if (partyMode == 0) {
    stroboscoop();
  } else {

    // programStateNumber bepaalt welke loop er runt, de loop bepaald wat ledStates wordt.
    if (programStateNumber == 0) {
      //    Serial.println("Program zero executed");
      BPMfraction = 1;
      beatsPerMinuteTrigger();
      programZero();
      //Serial.println("programZero");
    } else if (programStateNumber == 1) {
      //Serial.println("programOne");
      BPMfraction = 0.5;
      beatsPerMinuteTrigger();
      programOne();
    } else if (programStateNumber == 2) {
      BPMfraction = 1.0;
      beatsPerMinuteTrigger();
      programTwo();
    } else if (programStateNumber == 3) {
      BPMfraction = 1.0;
      beatsPerMinuteTrigger();
      programThree();
    } else if (programStateNumber == 4) {
      BPMfraction = 0.2;
      beatsPerMinuteTrigger();
      programFour();
    } else if (programStateNumber == 5) {
      BPMfraction = 0.1;
      beatsPerMinuteTrigger();
      programFive();
    } else if (programStateNumber == 6) {
      BPMfraction = 1.0;
      beatsPerMinuteTrigger();
      programSix();
    } else if (programStateNumber == 7) {
      BPMfraction = 0.25;
      beatsPerMinuteTrigger();
      programSeven();
    } else if (programStateNumber == 8) {
      BPMfraction = 0.25;
      beatsPerMinuteTrigger();
      programEight();
    } else if (programStateNumber == 9) {
      BPMfraction = 1.0;
      beatsPerMinuteTrigger();
      programNine();
    } else if (programStateNumber == 10) {
      BPMfraction = 1.0;
      beatsPerMinuteTrigger();
      programTen();
    }
    //  Serial.println(ledStates[30]);

    // Deze for-loop schrijft de waardes uit ledStates naar de daadwerkelijke pinnen van de Arduino, let hier even op de "i+2", die 2 is afhankelijk van met welke ledPin je start!
    for (int i = 24; i <= 40; i++) {
      digitalWrite(i, ledStates[i]);
    }
  }
}

void programZero() {
  if (trigger == true) {
    for (int i = 24; i <= 40; i++) {
      ledStates[i] = pinOn;
    }
  }
  turnOffTheLightsAfterHalfTheBPM();
}

void programOne() {
  if (trigger == true) {
    for (int i = 24; i <= 40; i++) {
      ledStates[i] = PIN_OFF;
    }
    //PROGRAMMA1
    if (stap == 0) {
      ledStates[24] = PIN_ON;
    }
    else if (stap == 1) {
      ledStates[25] = PIN_ON;
    }
    else if (stap == 2) {
      ledStates[26] = PIN_ON;
    }
    else if (stap == 3) {
      ledStates[27] = PIN_ON;
    }
    else if (stap == 4) {
      ledStates[28] = PIN_ON;
    }
    else if (stap == 5) {
      ledStates[29] = PIN_ON;
    }
    else if (stap == 6) {
      ledStates[30] = PIN_ON;
    }
    else if (stap == 7) {
      ledStates[31] = PIN_ON;
    }
    else if (stap == 8) {
      ledStates[32] = PIN_ON;
    }
    else if (stap == 9) {
      ledStates[33] = PIN_ON;
    }
    else if (stap == 10) {
      ledStates[34] = PIN_ON;
    }
    else if (stap == 11) {
      ledStates[35] = PIN_ON;
    }
    else if (stap == 12) {
      ledStates[36] = PIN_ON;
    }
    else if (stap == 13) {
      ledStates[37] = PIN_ON;
    }
    else if (stap == 14) {
      ledStates[38] = PIN_ON;
      stap = -1;
    }
    stap = stap + 1;
  }


}

void programTwo() {
  if (trigger == true) {
    for (int i = 24; i <= 40; i++) {
      ledStates[i] = PIN_OFF;
    }
    //PROGRAMMA2 - rondje x2
    if (stap == 0) {
      ledStates[L1] = PIN_ON;
      ledStates[L2] = PIN_ON;
    }
    else if (stap == 1) {
      ledStates[L2] = PIN_ON;
      ledStates[L3] = PIN_ON;
    }
    else if (stap == 2) {
      ledStates[L3] = PIN_ON;
      ledStates[L6] = PIN_ON;
    }
    else if (stap == 3) {
      ledStates[L6] = PIN_ON;
      ledStates[L9] = PIN_ON;
    }
    else if (stap == 4) {
      ledStates[L9] = PIN_ON;
      ledStates[L12] = PIN_ON;
    }
    else if (stap == 5) {
      ledStates[L12] = PIN_ON;
      ledStates[L15] = PIN_ON;
    }
    else if (stap == 6) {
      ledStates[L15] = PIN_ON;
      ledStates[L14] = PIN_ON;
    }
    else if (stap == 7) {
      ledStates[L14] = PIN_ON;
      ledStates[L13] = PIN_ON;
    }
    else if (stap == 8) {
      ledStates[L13] = PIN_ON;
      ledStates[L10] = PIN_ON;
    }
    else if (stap == 9) {
      ledStates[L10] = PIN_ON;
      ledStates[L7] = PIN_ON;
    }
    else if (stap == 10) {
      ledStates[L7] = PIN_ON;
      ledStates[L4] = PIN_ON;
    }
    else if (stap == 11) {
      ledStates[L4] = PIN_ON;
      ledStates[L1] = PIN_ON;
      stap = -1;
    }
    stap = stap + 1;
  }

}

void programThree() {
  if (trigger == true) {
    for (int i = 24; i <= 40; i++) {
      ledStates[i] = PIN_OFF;
    }
    //PROGRAMMA2 - rondje x3
    if (stap == 0) {
      ledStates[L1] = PIN_ON;
      ledStates[L2] = PIN_ON;
      ledStates[L3] = PIN_ON;
    }
    else if (stap == 1) {
      ledStates[L2] = PIN_ON;
      ledStates[L3] = PIN_ON;
      ledStates[L6] = PIN_ON;
    }
    else if (stap == 2) {
      ledStates[L3] = PIN_ON;
      ledStates[L6] = PIN_ON;
      ledStates[L9] = PIN_ON;
    }
    else if (stap == 3) {
      ledStates[L6] = PIN_ON;
      ledStates[L9] = PIN_ON;
      ledStates[L12] = PIN_ON;
    }
    else if (stap == 4) {
      ledStates[L9] = PIN_ON;
      ledStates[L12] = PIN_ON;
      ledStates[L15] = PIN_ON;
    }
    else if (stap == 5) {
      ledStates[L12] = PIN_ON;
      ledStates[L15] = PIN_ON;
      ledStates[L14] = PIN_ON;
    }
    else if (stap == 6) {
      ledStates[L15] = PIN_ON;
      ledStates[L14] = PIN_ON;
      ledStates[L13] = PIN_ON;
    }
    else if (stap == 7) {
      ledStates[L14] = PIN_ON;
      ledStates[L13] = PIN_ON;
      ledStates[L10] = PIN_ON;
    }
    else if (stap == 8) {
      ledStates[L13] = PIN_ON;
      ledStates[L10] = PIN_ON;
      ledStates[L7] = PIN_ON;
    }
    else if (stap == 9) {
      ledStates[L10] = PIN_ON;
      ledStates[L7] = PIN_ON;
      ledStates[L4] = PIN_ON;
    }
    else if (stap == 10) {
      ledStates[L7] = PIN_ON;
      ledStates[L4] = PIN_ON;
      ledStates[L1] = PIN_ON;
    }
    else if (stap == 11) {
      ledStates[L4] = PIN_ON;
      ledStates[L1] = PIN_ON;
      ledStates[L2] = PIN_ON;
      stap = -1;
    }
    stap = stap + 1;
  }

}

void programFour() {
  if (trigger == true) {
    for (int i = 24; i <= 40; i++) {
      ledStates[i] = PIN_OFF;
    }
    //PROGRAMMA4 - Snake x3
    if (stap == 0) {
      ledStates[L1] = PIN_ON;
      ledStates[L2] = PIN_ON;
      ledStates[L3] = PIN_ON;
    }
    else if (stap == 1 || stap == 23) {
      ledStates[L2] = PIN_ON;
      ledStates[L3] = PIN_ON;
      ledStates[L6] = PIN_ON;
      if (stap == 23) {
        stap = -1;
      }
    }
    else if (stap == 2 || stap == 22) {
      ledStates[L3] = PIN_ON;
      ledStates[L6] = PIN_ON;
      ledStates[L5] = PIN_ON;
    }
    else if (stap == 3 || stap == 21) {
      ledStates[L6] = PIN_ON;
      ledStates[L5] = PIN_ON;
      ledStates[L4] = PIN_ON;
    }
    else if (stap == 4 || stap == 20) {
      ledStates[L5] = PIN_ON;
      ledStates[L4] = PIN_ON;
      ledStates[L7] = PIN_ON;
    }
    else if (stap == 5 || stap == 19) {
      ledStates[L4] = PIN_ON;
      ledStates[L7] = PIN_ON;
      ledStates[L8] = PIN_ON;
    }
    else if (stap == 6 || stap == 18) {
      ledStates[L7] = PIN_ON;
      ledStates[L8] = PIN_ON;
      ledStates[L9] = PIN_ON;
    }
    else if (stap == 7 || stap == 17) {
      ledStates[L8] = PIN_ON;
      ledStates[L9] = PIN_ON;
      ledStates[L12] = PIN_ON;
    }
    else if (stap == 8 || stap == 16) {
      ledStates[L9] = PIN_ON;
      ledStates[L12] = PIN_ON;
      ledStates[L11] = PIN_ON;
    }
    else if (stap == 9 || stap == 15) {
      ledStates[L12] = PIN_ON;
      ledStates[L11] = PIN_ON;
      ledStates[L10] = PIN_ON;
    }
    else if (stap == 10 || stap == 14) {
      ledStates[L11] = PIN_ON;
      ledStates[L10] = PIN_ON;
      ledStates[L13] = PIN_ON;
    }
    else if (stap == 11 || stap == 13) {
      ledStates[L10] = PIN_ON;
      ledStates[L13] = PIN_ON;
      ledStates[L14] = PIN_ON;
    }
    else if (stap == 12) {
      ledStates[L13] = PIN_ON;
      ledStates[L14] = PIN_ON;
      ledStates[L15] = PIN_ON;
    }
    stap = stap + 1;
  }


}

void programFive() {
  if (trigger == true) {
    for (int i = 24; i <= 40; i++) {
      ledStates[i] = PIN_OFF;
    }
    //PROGRAMMA5 - Snake x2
    if (stap == 0) {
      ledStates[L1] = PIN_ON;
      ledStates[L2] = PIN_ON;

    }
    else if (stap == 1 || stap == 25) {
      ledStates[L2] = PIN_ON;
      ledStates[L3] = PIN_ON;

      if (stap == 25) {
        stap = -1;
      }
    }
    else if (stap == 2 || stap == 24) {
      ledStates[L3] = PIN_ON;
      ledStates[L6] = PIN_ON;

    }
    else if (stap == 3 || stap == 23) {
      ledStates[L6] = PIN_ON;
      ledStates[L5] = PIN_ON;

    }
    else if (stap == 4 || stap == 22) {
      ledStates[L5] = PIN_ON;
      ledStates[L4] = PIN_ON;

    }
    else if (stap == 5 || stap == 21) {
      ledStates[L4] = PIN_ON;
      ledStates[L7] = PIN_ON;

    }
    else if (stap == 6 || stap == 20) {
      ledStates[L7] = PIN_ON;
      ledStates[L8] = PIN_ON;

    }
    else if (stap == 7 || stap == 19) {
      ledStates[L8] = PIN_ON;
      ledStates[L9] = PIN_ON;

    }
    else if (stap == 8 || stap == 18) {
      ledStates[L9] = PIN_ON;
      ledStates[L12] = PIN_ON;

    }
    else if (stap == 9 || stap == 17) {
      ledStates[L12] = PIN_ON;
      ledStates[L11] = PIN_ON;

    }
    else if (stap == 10 || stap == 16) {
      ledStates[L11] = PIN_ON;
      ledStates[L10] = PIN_ON;

    }
    else if (stap == 11 || stap == 15) {
      ledStates[L10] = PIN_ON;
      ledStates[L13] = PIN_ON;

    }
    else if (stap == 12 || stap == 14) {
      ledStates[L13] = PIN_ON;
      ledStates[L14] = PIN_ON;

    }
    else if (stap == 13) {
      ledStates[L14] = PIN_ON;
      ledStates[L15] = PIN_ON;

    }

    stap = stap + 1;
  }

}

void stroboscoop() {
  for (int i = 24; i <= 40; i++) {
    ledStates[i] = PIN_OFF;
  }

  int n = 0;
  for (int i = 0; i <= 2; i++) {
    n = random(24, 38);
    ledStates[n] = PIN_ON;
  }

  for (int i = 24; i <= 40; i++) {
    digitalWrite(i, ledStates[i]);
  }

  delay(67);
}

void programSix() {
  if (trigger == true) {
    for (int i = 24; i <= 40; i++) {
      ledStates[i] = PIN_OFF;
    }

    if ( stap == 0) {
      int n = 0;
      n = random(24, 38);
      ledStates[n] = PIN_ON;
      stap = -1;
    }
    stap = stap + 1;
  }
  turnOffTheLightsAfterHalfTheBPM();

}

void programSeven() {
  if (trigger == true) {
    for (int i = 24; i <= 40; i++) {
      ledStates[i] = PIN_OFF;
    }

    if ( stap == 0) {
      int n = 0;
      n = random(24, 38);
      ledStates[n] = PIN_ON;
      stap = -1;
    }
    stap = stap + 1;
  }
  //turnOffTheLightsAfterHalfTheBPM();

}

void programEight() {
  if (trigger == true) {
    for (int i = 24; i <= 40; i++) {
      ledStates[i] = PIN_OFF;
    }

    if ( stap == 0) {
      int n = 0;
      int m = 0;
      n = random(24, 38);
      m = random(24, 38);
      ledStates[n] = PIN_ON;
      ledStates[m] = PIN_ON;
      stap = -1;
    }
    stap = stap + 1;
  }
  //turnOffTheLightsAfterHalfTheBPM();
}

void programNine() {
  if (trigger == true) {
    for (int i = 24; i <= 40; i++) {
      ledStates[i] = PIN_OFF;
    }

    if ( stap == 0) {
      int n = 0;
      int m = 0;
      n = random(24, 38);
      m = random(24, 38);
      ledStates[n] = PIN_ON;
      ledStates[m] = PIN_ON;
      stap = -1;
    }
    stap = stap + 1;
  }
  turnOffTheLightsAfterHalfTheBPM();
}
void programTen() {
  if (trigger == true) {
    for (int i = 24; i <= 40; i++) {
      ledStates[i] = PIN_OFF;
    }

    if ( stap == 0) {
      int n = 0;
      int m = 0;
      n = random(24, 38);
      m = random(24, 38);
      ledStates[n] = PIN_ON;
      ledStates[m] = PIN_ON;
      stap = -1;
    }
    stap = stap + 1;
  }
  turnOffTheLightsAfterQuarterBPM();
}


void turnOffTheLightsAfterHalfTheBPM() {
  //  Serial.println("turnoff");
  //  Serial.println(BPMfraction);
  // After half of the BeatTime in miliseconds, turn off all the lights
  if (currentMillis >= previousMillis + (BeatTimeMS * BPMfraction / 2.0) ) {
    for (int i = 24; i <= 40; i++) {
      ledStates[i] = pinOff;
    }
  }
}
void turnOffTheLightsAfterQuarterBPM() {
  // After half of the BeatTime in miliseconds, turn off all the lights
  if (currentMillis >= previousMillis + (BeatTimeMS * BPMfraction / 4.0) ) {
    for (int i = 24; i <= 40; i++) {
      ledStates[i] = pinOff;
    }
  }
}

// c = 1 for normal BPM, smaller than c is faster and bigger slower
void beatsPerMinuteTrigger() {
  //  Serial.println("bpmtrigger");
  //  Serial.println(BeatTimeMS);
  //  Serial.println(BPMfraction);
  //  Serial.println(BeatTimeMS * BPMfraction);
  if (currentMillis >= previousMillis + BeatTimeMS * BPMfraction) {
    trigger = true;
    previousMillis = currentMillis;
  }
  else {
    trigger = false;
  }
}
