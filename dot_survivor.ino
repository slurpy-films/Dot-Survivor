// CONFIG:
// Music will be played with the pin you set here. Set it to 0 for no music.
int buzzerPin = 8;

// Display config:
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// Joystick config:
#define JOYSTICK_X A0
#define JOYSTICK_Y A1
#define JOYSTICK_SW 2

// Gamemodes:
const char* modes[] = {"Easy", "Medium", "Hard", "Super Hard"};

// Reset pin: (0 for no reset.)
#define RESET_PIN 4

// Default radius:
float radius = 5;
int enemyRadius = 7;

// Default player speed:
int spd = 2;

// END OF CONFIG




#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int modeIndex = 0;
bool selected = false;
int gamemode = 0;

int playerx = SCREEN_WIDTH / 2;
int playery = SCREEN_HEIGHT / 2;
int enemyx = SCREEN_WIDTH - 10;
int enemyy = SCREEN_HEIGHT / 2;
int enemySpdX = -2;
int enemySpdY = 1;
bool gameOver = false;
int ticks = 0;
int score = 0;

int noteC4 = 262;
int noteD4 = 294;
int noteE4 = 330;
int noteF4 = 349;
int noteG4 = 392;
int noteA4 = 440;
int noteB4 = 494;
int noteC5 = 523;

int wholeNote = 1000;
int halfNote = 500;
int quarterNote = 250;

void setup() {
    if (RESET_PIN != 0) {
      pinMode(RESET_PIN, OUTPUT);
    }
    pinMode(8, OUTPUT);
    if (RESET_PIN != 0) {
      digitalWrite(RESET_PIN, HIGH);
    }
    Serial.begin(9600);
    pinMode(JOYSTICK_SW, INPUT_PULLUP);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    ui();
}

void loop() {
      if (gameOver) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(20, 20);
        display.print("GAME OVER");
        display.display();
        playGameOverTune();
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Your score: ");
        display.print(score);
        display.display();
        delay(1000);

        while (digitalRead(JOYSTICK_SW) == HIGH) {
        }

        if (RESET_PIN != 0) {
          digitalWrite(RESET_PIN, LOW);
        }
        return;
    }
    
    if (!selected) {
        handleModeSelection();
        delay(100);
        return;
    }

    int x = analogRead(JOYSTICK_X);
    int y = analogRead(JOYSTICK_Y);
    bool sw = digitalRead(JOYSTICK_SW) == LOW;

    if (x > 600) playerx += spd;
    if (x < 400) playerx -= spd;
    if (y > 600) playery += spd;
    if (y < 400) playery -= spd;

    enemyx += enemySpdX;
    enemyy += enemySpdY;

    if (enemyx <= 0 || enemyx >= SCREEN_WIDTH) enemySpdX *= -1;
    if (enemyy <= 0 || enemyy >= SCREEN_HEIGHT) enemySpdY *= -1;

    playerx = constrain(playerx, 0, SCREEN_WIDTH - 1);
    playery = constrain(playery, 0, SCREEN_HEIGHT - 1);

    float distance = sqrt(pow(playerx - enemyx, 2) + pow(playery - enemyy, 2));
    if (distance < radius + enemyRadius) {
        gameOver = true;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Score: ");
    display.print(score);
    display.fillCircle(playerx, playery, radius, SSD1306_WHITE);
    display.fillCircle(enemyx, enemyy, enemyRadius, SSD1306_WHITE);
    display.display();
    ticks += 50;
    if (ticks > 1000) {
        ticks = 0;
        score += 1;
    }
    if (gamemode == 1) {
        enemySpdX *= 1.01;
        enemySpdY *= 1.01;
        radius += 0.025;
        if (radius > 25) radius = 5;
    } else if (gamemode == 2) {
        enemySpdX *= 1.02;
        enemySpdY *= 1.02;
        radius += 0.05;
        if (radius > 30) radius = 7;
    } else if (gamemode == 3) {
        enemySpdX *= 1.05;
        enemySpdY *= 1.05;
        radius += 0.075;
        if (radius > 70) radius = 10;
    } else if (gamemode == 4) {
        enemySpdX *= 1.1;
        enemySpdY *= 1.1;
        radius += 0.15;
        if (radius > 70) radius = 15;
    }

    delay(50);
}

void ui() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 10);
    display.print("Dot Survivor");
    display.display();
    startGame();
}

void handleModeSelection() {
    static bool joystickMoved = false;
    int x = analogRead(JOYSTICK_Y);
    bool sw = digitalRead(JOYSTICK_SW) == LOW;

    if (!joystickMoved) {
        if (x > 600 && modeIndex < 3) {
            modeIndex++;
            joystickMoved = true;
        } else if (x < 400 && modeIndex > 0) {
            modeIndex--;
            joystickMoved = true;
        }
    }

    if (x >= 400 && x <= 600) {  
        joystickMoved = false;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(20, 10);
    display.print("Select Mode:");
    
    for (int i = 0; i < 4; i++) {
        display.setCursor(30, 20 + i * 10);
        if (i == modeIndex) {
            display.print("> ");
        } else {
            display.print("  ");
        }
        display.print(modes[i]);
    }
    display.display();

    if (sw) {
        gamemode = modeIndex + 1;
        selected = true;
        delay(300);
        while (digitalRead(JOYSTICK_SW) == LOW);
    }
}

void playGameOverTune() {
  tone(buzzerPin, noteC4, quarterNote);
  delay(quarterNote);
  tone(buzzerPin, noteD4, quarterNote);
  delay(quarterNote);
  tone(buzzerPin, noteE4, quarterNote);
  delay(quarterNote);
  tone(buzzerPin, noteD4, quarterNote);
  delay(quarterNote);
  
  tone(buzzerPin, noteC4, halfNote);
  delay(halfNote);
  
  tone(buzzerPin, noteB4, quarterNote);
  delay(quarterNote);
  tone(buzzerPin, noteC5, quarterNote);
  delay(quarterNote);
  
  tone(buzzerPin, noteA4, wholeNote);
  delay(wholeNote);
  
  noTone(buzzerPin);
}

void startGame() {
  tone(buzzerPin, noteG4, quarterNote);
  delay(quarterNote);
  tone(buzzerPin, noteC5, quarterNote);
  delay(quarterNote);
  tone(buzzerPin, noteE4, quarterNote);
  delay(quarterNote);
  tone(buzzerPin, noteD4, quarterNote);
  delay(quarterNote);

  tone(buzzerPin, noteA4, halfNote);
  delay(halfNote);
  
  tone(buzzerPin, noteB4, quarterNote);
  delay(quarterNote);
  tone(buzzerPin, noteG4, quarterNote);
  delay(quarterNote);

  tone(buzzerPin, noteC4, wholeNote);
  delay(wholeNote);

  noTone(buzzerPin);
}
