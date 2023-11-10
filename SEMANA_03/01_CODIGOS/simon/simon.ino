#include <vector>
using std::vector;

const int DEBOUNCE_DELAY = 50;

int buttons[4] = {19, 18, 17, 16};
int leds[4] = {27, 26, 33, 32};
int buttonState[4] = {0, 0, 0, 0};
int lastButtonState[4] = {0, 0, 0, 0};
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};

const int statusLed = 21;
const int switchButton = 15;
int switchState = 0;
int lastSwitchState = 0;
unsigned long lastSwitchDebounceTime = 0;

vector<int> sequence;

int currentStep = 0;
boolean gameActive = false;
unsigned long gameStartTime = 0;
const unsigned long gameInterval = 5000;  // Time given to player to press a button

int demoing = 0;

void setup() {
    for (unsigned int i = 0; i < 4; i++) {
        pinMode(leds[i], OUTPUT);
        pinMode(buttons[i], INPUT);
    }

    pinMode(switchButton, INPUT);
    pinMode(statusLed, OUTPUT);

    randomSeed(analogRead(0));

    Serial.begin(9600);
    Serial.println("Simon Game Started");
}

void loop() {
    int reading;
    // Check the switch button state for starting the game.
    reading = digitalRead(switchButton);
    if (reading != lastSwitchState) {
        lastSwitchDebounceTime = millis();
    }

    if ((millis() - lastSwitchDebounceTime) > DEBOUNCE_DELAY) {
        if (reading != switchState) {
            switchState = reading;
            if (switchState == HIGH) {
                startGame();
            }
        }
    }
    lastSwitchState = reading;

    // If the game is active, check for button inputs.
    if (gameActive) {
        for (int i = 0; i < 4; i++) {
            reading = digitalRead(buttons[i]);
            if (reading != lastButtonState[i]) {
                lastDebounceTime[i] = millis();
            }

            if ((millis() - lastDebounceTime[i]) > DEBOUNCE_DELAY) {
                if (reading != buttonState[i]) {
                    buttonState[i] = reading;
                    if (buttonState[i] == HIGH) {
                        checkInput(i);
                    }
                }
            }
            lastButtonState[i] = reading;
        }

        // If the player is taking too long to press a button.
        if ((millis() - gameStartTime) > gameInterval) {
            loseGame();
        }
    }
}

void startGame() {
    sequence.clear();
    currentStep = 0;
    addStep();
    gameActive = true;
    digitalWrite(statusLed, HIGH);
    Serial.println("Game has started.");
}

void addStep() {
    sequence.push_back(random(0, 4));
    demoing = true;
    for (int i = 0; i < sequence.size(); i++) {
        digitalWrite(leds[sequence[i]], HIGH);
        delay(500);
        digitalWrite(leds[sequence[i]], LOW);
        delay(250);
    }
    demoing = false;
    gameStartTime = millis(); // Reset the timer for the player's turn.
}

void checkInput(int button) {
    if (!demoing) {
        if (button == sequence[currentStep]) {
            digitalWrite(leds[button], HIGH);
            delay(250);
            digitalWrite(leds[button], LOW);
            currentStep++;
            if (currentStep == sequence.size()) {
                delay(500); // A short delay before starting the next round.
                currentStep = 0;
                addStep();
            }
            gameStartTime = millis(); // Reset the timer as the player pressed a button.
        } else {
            loseGame();
        }
    }
}

void loseGame() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(statusLed, HIGH);
        delay(250);
        digitalWrite(statusLed, LOW);
        delay(250);
    }
    gameActive = false;
    Serial.println("Game over. Press the switch button to restart.");
}