#include <M5StickCPlus.h>

// Define the GPIO pins for continuity testing
const int outputPin = 26; // GPIO26 as output
const int inputPin = 36;  // GPIO36 (VP) as input

bool previousContinuity = false;

void setup() {
    // Initialize M5StickC Plus
    M5.begin();
    M5.Lcd.setRotation(3); // Set the display rotation if needed
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setTextSize(2);

    // Set GPIO modes
    pinMode(outputPin, OUTPUT);
    pinMode(inputPin, INPUT_PULLDOWN); // Use internal pull-down resistor

    // Initial state
    digitalWrite(outputPin, LOW);
}

void loop() {
    // Send a signal through the output pin
    digitalWrite(outputPin, HIGH);
    delay(10); // Short delay to ensure the signal is stable

    // Read the input pin
    int continuity = digitalRead(inputPin);

    // Display the result, beep, and flash the screen if continuity is detected
    M5.Lcd.setCursor(0, 0);
    if (continuity == HIGH) {
        if (!previousContinuity) {
            M5.Lcd.fillScreen(TFT_GREEN); // Flash screen green
            M5.Lcd.setTextColor(TFT_BLACK, TFT_GREEN); // Set text color to black with green background
            M5.Lcd.setCursor(20, 30);
            M5.Lcd.print("Continuity: YES");
            ledcAttachPin(SPEAKER_PIN, TONE_PIN_CHANNEL);
            ledcWriteTone(TONE_PIN_CHANNEL, 1000); // Beep at 1000 Hz
            delay(200); // Keep the screen green and beep for 200 ms
            ledcWriteTone(TONE_PIN_CHANNEL, 0); // Stop the beep
            M5.Lcd.fillScreen(TFT_BLACK); // Revert screen to black
            M5.Lcd.setTextColor(TFT_WHITE); // Set text color back to white
            previousContinuity = true;
        }
    } else {
        if (previousContinuity) {
            M5.Lcd.fillScreen(TFT_BLACK);
            M5.Lcd.setCursor(20, 30);
            M5.Lcd.print("Continuity: NO");
            previousContinuity = false;
        }
    }

    // Turn off the output signal
    digitalWrite(outputPin, LOW);
    delay(500); // Delay between checks to debounce
}
