/*
  ===========================================================================
  SMART GREEN TUNNEL ECOSYSTEM
  Piezoelectric energy harvesting + automated tunnel lighting
  ===========================================================================
  Board   : Arduino UNO (ATmega328P)
  Author  : M Sabith Ismail (4DM25AI035)
  Course  : 1BPRJ258 - Interdisciplinary Project Work, Sem 2, AI&ML
            Yenepoya Institute of Technology, Moodbidri (VTU, 2025-26)

  IMPORTANT NOTE ON THIS FILE:
  The project report only contains a screenshot of the Arduino IDE showing
  lines 92-123 of the original sketch (Fig 5.3). Those lines - the eco-mode
  decay block and the updateLedgerDisplay() function - are reproduced here
  exactly as shown, comments included. The rest of this file (pin setup,
  constants, and the main loop's strike-detection/debounce logic) has been
  reconstructed to match every parameter explicitly stated elsewhere in the
  report: NOISE_THRESHOLD = 100, a 400ms debounce lockout, analog input on
  A0, PWM output on pin 9, I2C LCD on A4/A5, two axle hits = one vehicle
  pass, and P = V^2 / R with R = 1kohm (this value isn't stated directly,
  but it's what makes every row of Table 6.1 work out exactly).

  If you still have the original .ino on the machine you used for the
  Arduino IDE screenshot (look for a folder named "sketch_apr6a" in your
  Arduino sketchbook directory), use that as the source of truth and treat
  this file as a clean, documented reference copy instead.
  ===========================================================================
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------------------------------------------------------------------
// PIN DEFINITIONS
// ---------------------------------------------------------------------
const int PIEZO_PIN  = A0;  // Conditioned DC output from the rectifier/cap/Zener stage
const int TUNNEL_LED = 9;   // PWM output to the tunnel LED array
// LCD runs on the hardware I2C bus: SDA -> A4, SCL -> A5

// ---------------------------------------------------------------------
// TUNING CONSTANTS (values as documented in the report, Ch. 4-5)
// ---------------------------------------------------------------------
const int NOISE_THRESHOLD        = 100;    // ADC counts; ignores ambient road vibration
const unsigned long DEBOUNCE_TIME = 400;   // ms; lockout to avoid double-counting one tire hit
const int ECO_BRIGHTNESS         = 51;     // ~20% of 255
const int FULL_BRIGHTNESS        = 255;    // 100%
const float LOAD_RESISTANCE_OHMS = 1000.0; // R in P = V^2 / R (matches every row of Table 6.1)
const float ADC_TO_VOLTS         = 5.0 / 1023.0;

// ---------------------------------------------------------------------
// LCD (16x2, I2C backpack - default address 0x27, change if yours differs)
// ---------------------------------------------------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------------------------------------------------------------
// STATE VARIABLES
// ---------------------------------------------------------------------
unsigned long lastStrikeTime    = 0;
unsigned long currentTime       = 0;
unsigned int  totalAxles        = 0;
unsigned int  estimatedVehicles = 0;
float accumulatedEnergy         = 0.0; // running ledger total, in mW

void setup() {
  Serial.begin(9600);
  pinMode(TUNNEL_LED, OUTPUT);

  lcd.init();
  lcd.backlight();

  analogWrite(TUNNEL_LED, ECO_BRIGHTNESS); // Start in 20% Eco Mode
  updateLedgerDisplay();
}

void loop() {
  currentTime = millis();
  int rawReading = analogRead(PIEZO_PIN);

  // Only treat this as a real axle strike if it clears the noise floor
  // AND the debounce lockout window has elapsed since the last strike.
  if (rawReading > NOISE_THRESHOLD && (currentTime - lastStrikeTime) > DEBOUNCE_TIME) {

    totalAxles++;

    // Two axle hits = one completed vehicle pass
    if (totalAxles % 2 == 0) {
      estimatedVehicles++;
    }

    // Instantaneous power from the filtered DC reading: P = V^2 / R
    float voltage = rawReading * ADC_TO_VOLTS;
    float instantPowerMilliWatts = (voltage * voltage / LOAD_RESISTANCE_OHMS) * 1000.0;
    accumulatedEnergy += instantPowerMilliWatts;

    // Jump tunnel lighting straight to full brightness on detection
    analogWrite(TUNNEL_LED, FULL_BRIGHTNESS);

    updateLedgerDisplay();

    lastStrikeTime = currentTime;
  }

  // ---- Verbatim from Fig 5.3 (lines 92-99 in the report screenshot) ----
  // LOGIC BLOCK: Drop system lights smoothly back to 20% energy baseline when vehicle moves away
  // If no wheel compression hits are logged for longer than 3.5 seconds, clear illumination levels
  if (currentTime - lastStrikeTime > 3500) {
    analogWrite(TUNNEL_LED, 51); // Smooth transition back to Eco Mode
  }
  // ------------------------------------------------------------------
}

// ============================
// SUBSYSTEM SUPPORT FUNCTIONS
// ============================
// ---- Verbatim from Fig 5.3 (lines 107-123 in the report screenshot) ----

void updateLedgerDisplay() {
  // Clear layout segments efficiently
  lcd.clear();

  // Row 0 Layout: Print vehicle tracking counts
  lcd.setCursor(0, 0);
  lcd.print("Vehs: ");
  lcd.print(estimatedVehicles);
  lcd.print(" | Axle:");
  lcd.print(totalAxles);

  // Row 1 Layout: Print real-time energy values
  lcd.setCursor(0, 1);
  lcd.print("ENERGY: ");
  lcd.print(accumulatedEnergy, 2); // Print value to two decimal places
  lcd.print(" mW");
}
