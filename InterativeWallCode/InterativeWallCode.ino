// Feb 5,2025
// Newson's Electronics
// Code for interative hexagon display updated (background and annimations.)

#include <Adafruit_NeoPixel.h>
#include <RtcDS1302.h>  // Library Rtc by Makuna

// Maximum number of timers
#define MAX_TIMERS 8


// NeoPixel settings
#define LED_PIN A1
#define LED_COUNT 105
ThreeWire myWire(3, 2, 4);  // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);



// for display array (x,y dimension)
const int ROWS = 5, COLS = 19;
const int numButtons = 19;
const int numRows = 9;                                                                     // Total number of row pins
const int numBacklights = 19;        
int col = 0,row = 0; // for button return. converted into x,y location on display. 
int row2=0,col2=0; // for drawing on the ledArray eg. x,y back into row,col for 2d array. 


// Array to store the color state for each button
int colorState[numButtons] = { 0 };  // 0: Red, 1: Green, 2: Blue, 3: White, 4: Off
int expectedADCValues[numButtons] = { 1020, 925, 765, 612, 475, 361, 265, 190, 133, 92 };  // buttons 0,0 to
int threshold = 10;                                                                        // Adjusted threshold for better accuracy (test and adjust further)
int rowPins[numRows] = { 5, 6, 7, 8, 9, 10, 11, 12, 13 };
int backLights[numBacklights] = { 5, 10, 16, 21, 27, 32, 38, 43, 49, 54, 60, 65, 71, 76, 82, 87, 93, 98, 104 };

int background = 1;  // start with solid fill ba
int x,y=0;



int beeX = 4, beeY = 4;  // Starting position of the bee


int hue = 132;   // Hue for background
int hue2 = 42;   // Hue for digits.
int changeTime = 0;

int beeMoving = 0;
int drawingMode = 0;
int flashTime = 0;

// Bees' initial locations
int bx = 16;
int by = 2;
int dy = 1;
int dx = 1;  // dc 2
int litLeds[54] = {};

// Bee move to location
int bmx = 4, bmy = 1;

// Button x, y pressed location variables.
int buttonX = 4, buttonY = 1;


int fy = 1; // for flower movment
int fy2 = 1; // for bee movment

int h1, h2 = 0;  // h1= hour ones, h2= hour tens (digits)
int m1, m2 = 0;  // m1= minutes ones, m2= minutes tens
int s = 0;       // seconds
int dots = 0;
int year, month, day, hours, minutes, seconds = 0;
int shift = 0;
int mode = 0;

int brightness = 180;// intal screen brightness


// Define each digit as an array
int digit0[] = {9, 8, 7, 6, 11, 15, 20, 19, 18, 17};
int digit1[] = {20, 19, 18, 17};
int digit2[] = {9, 7, 6, 11, 13, 14, 20, 19, 17};
int digit3[] = {9, 18, 6, 11, 13, 15, 20, 19, 17};
int digit4[] = {9, 8, 13, 20, 19, 18, 17};
int digit5[] = {9, 8, 7, 11, 13, 15, 18, 17};
int digit6[] = {9, 8, 7, 6, 11, 13, 15, 20, 18, 17};
int digit7[] = {9, 11, 20, 19, 18, 17};
int digit8[] = {9, 8, 7, 6, 11, 13, 15, 20, 19, 18, 17};
int digit9[] = {9, 8, 11, 13, 20, 19, 18, 17};

// Array of pointers to each digit array
int* digits[] = {digit0, digit1, digit2, digit3, digit4, digit5, digit6, digit7, digit8, digit9};

// Array storing the size of each digit array
int sizes[] = {
  sizeof(digit0) / sizeof(digit0[0]),
  sizeof(digit1) / sizeof(digit1[0]),
  sizeof(digit2) / sizeof(digit2[0]),
  sizeof(digit3) / sizeof(digit3[0]),
  sizeof(digit4) / sizeof(digit4[0]),
  sizeof(digit5) / sizeof(digit5[0]),
  sizeof(digit6) / sizeof(digit6[0]),
  sizeof(digit7) / sizeof(digit7[0]),
  sizeof(digit8) / sizeof(digit8[0]),
  sizeof(digit9) / sizeof(digit9[0])
};


                                                      // Total number of row pins



// For x, y coordinates creating displays.
int displayLeds[ROWS][COLS] = {
  { 0, 10, 11, 21, 22, 32, 33, 43, 44, 54, 55, 65, 66, 76, 77, 87, 88, 98, 99 },
  { 1, 9, 12, 20, 23, 31, 34, 42, 45, 53, 56, 64, 67, 75, 78, 86, 89, 97, 100 },
  { 2, 8, 13, 19, 24, 30, 35, 41, 46, 52, 57, 63, 68, 74, 79, 85, 90, 96, 101 },
  { 3, 7, 14, 18, 25, 29, 36, 40, 47, 51, 58, 62, 69, 73, 80, 84, 91, 95, 102 },
  { 4, 6, 15, 17, 26, 28, 37, 39, 48, 50, 59, 61, 70, 72, 81, 83, 92, 94, 103 }
};

const char* bee[] = {
  "w0w",
  "0y0",
};

const char* bee2[] = {
  "wyw",
};

const char* flowerRed[] = {
  "0r0",
  "ryr",
  "rrr",
  "0g0",
  "0g0"
};

void convertRowColToXY(int row, int col) {
    // Calculate x,y based on our display. 
    if (row % 2 == 0) {
        // For even rows: x = 2 * col
        x = 2 * col;
    } else {
        // For odd rows: x = 2 * col + 1
        x = 2 * col + 1;
    }

    // Calculate y based on the row
    y = (8 - row) / 2;
     // used for drawing on the display array
     col2=x;
     row2 =  (4 - y) ;  // Flip y based on even/odd x

        
}












struct Timer {
  unsigned long previousMillis = 0;  // Stores last triggered time
};
// Create an array of timers
Timer timers[MAX_TIMERS];

// Function to check time for a specific timer ID without blocking
bool millisDelay(int timerID, unsigned long interval) {
  if (timerID < 0 || timerID >= MAX_TIMERS) return false;  // Invalid ID check

  unsigned long currentMillis = millis();  // Get the current time

  if (currentMillis - timers[timerID].previousMillis >= interval) {
    timers[timerID].previousMillis = currentMillis;  // Update last triggered time
    return true;                                     // Return true when the delay has passed
  }
  return false;  // Return false if the delay has not passed yet
}


void clearDisplay() {
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));  // Off
  }
  //strip.show();
}

// Function to map a character to an actual color
uint32_t getColorFromChar(char c) {
  switch (c) {
    case 'r': return strip.Color(255, 0, 0);      // Red
    case 'g': return strip.Color(0, 255, 0);      // Green
    case 'b': return strip.Color(0, 0, 255);      // Blue
    case 'y': return strip.Color(255, 255, 0);    // Yellow
    case 'c': return strip.Color(0, 255, 255);    // Cyan
    case 'm': return strip.Color(255, 0, 255);    // Magenta
    case 'w': return strip.Color(255, 255, 255);  // White
    case '0':
      return strip.Color(0, 0, 0);  // see through not drawn
                                    //default: return strip.Color(0, 0, 0);      // Default off
  }
}



const char* bee4[] = {
  "wyw",
};

void drawImage(int startCol, int startRow, const char* image[], int imgRows) {
  // Start drawing from the bottom-most row (ROWS - 1)
  for (int imgRow = 0; imgRow < imgRows; imgRow++) {
    int imgCols = strlen(image[imgRow]);  // Get the number of columns dynamically

    // Calculate the targetY position to start from the bottom, considering startRow
    int targetY = (ROWS - 1) - (imgRows - 1 - imgRow) - startRow;

    // Skip drawing if out of bounds for Y
    if (targetY < 0 || targetY >= ROWS) continue;

    for (int imgCol = 0; imgCol < imgCols; imgCol++) {
      if (image[imgRow][imgCol] == '0') continue;  // Skip transparent pixels

      // Calculate the target X position
      int targetX = startCol + imgCol;

      // Skip drawing if out of bounds for X
      if (targetX < 0 || targetX >= COLS) continue;

      // Get LED index from displayLeds array
      int ledIndex2 = displayLeds[targetY][targetX];
      
      if (ledIndex2 != -1) {  // Ensure valid LED index
        uint32_t color = getColorFromChar(image[imgRow][imgCol]);
        strip.setPixelColor(ledIndex2, color);
        //strip.show();
        //delay(300);
      }
    }
  }
}















void updateTime() {
  // Create a new RtcDateTime object with updated minutes
  RtcDateTime updatedTime(
    year, month, day,
    hours, minutes, seconds);

  // Set the updated time back to the RTC
  Rtc.SetIsWriteProtected(false);
  Rtc.SetDateTime(updatedTime);
  delay(200);
  Rtc.SetIsWriteProtected(true);
}




void gradientWavesBackground() {
  static uint16_t timeOffset = 0;  // Keeps track of animation over time

  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      int ledIndex = displayLeds[row][col];  // Get LED index

      // Create a smooth color wave using sine function
      int hue = (sin((col + row + timeOffset) * 0.2) * 32768) + 32768;  // Normalize sine wave to HSV range

      strip.setPixelColor(ledIndex, strip.gamma32(strip.ColorHSV(hue, 255, 150)));
    }
  }

 
  if (millisDelay(6, 100)) timeOffset -= 1;  // Slowly increment to animate the waves
 // delay(300);
}




void changingGradientBackground() {
  static uint16_t timeOffset = 0;  // Keeps track of animation over time
  static unsigned long lastColorChange = 0;  // Time of last color change
  static unsigned long colorChangeInterval = 5000;  // Interval to change color (in milliseconds)
  static int hue = 0;  // Initial hue (starting color)
  
  // Check if it's time to change the color
  if (millis() - lastColorChange > colorChangeInterval) {
    hue = random(0, 65535);  // Generate a new random color
    lastColorChange = millis();  // Reset the time for next color change
  }

  // Loop through all rows and columns
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      int ledIndex = displayLeds[row][col];  // Get LED index
      
      // Reverse the brightness so bottom is brightest and top is dimmer
      int brightness = map(row, 0, ROWS - 1, 70, 150);  // Soft gradient: Bottom (150) → Top (70)

      // Set pixel color based on gradient
      strip.setPixelColor(ledIndex, strip.gamma32(strip.ColorHSV(hue, 255, brightness)));
    }
  }


 
}

void gradientBackground(uint8_t hue) {
  uint16_t mappedHue = hue * 256;  // Convert 0-255 hue range to 0-65535

  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      int ledIndex = displayLeds[row][col];

      // gradient from bottom to top
      int brightness2 = map(row, 0, ROWS - 1, 70, 150);

      strip.setPixelColor(ledIndex, strip.gamma32(strip.ColorHSV(mappedHue, 255, brightness2)));
    }
  }

  
}


void fadingGradientBackground(uint8_t hue) {
  uint16_t mappedHue = hue * 256;  // Yellow color
  static uint32_t startTime = millis();  // Capture initial time
  uint32_t elapsedTime = millis() - startTime;  // Time since function started

  // Define fade-in and fade-out duration
  uint32_t fadeInTime = 5000;   // 5 seconds to reach max brightness
  uint32_t fadeOutTime = 5000;  // 5 seconds to fade out
  uint32_t totalCycleTime = fadeInTime + fadeOutTime;  // Full cycle (10 sec)

  // Use cosine function for smooth fade-in and fade-out
  float brightnessScale = 0.3 + (0.7 * (1 - cos((PI * (elapsedTime % totalCycleTime)) / fadeInTime)));
  
  // Ensure brightness scale stays withi9n range (prevent fading too much)
  brightnessScale = constrain(brightnessScale, 0.6, 1.0);

  // Apply gradient with brightness scaling
  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
      int ledIndex = displayLeds[row][col];

      // Gradient from bottom to top
      int brightness2 = map(row, 0, ROWS - 1, 70, 190);

      // Apply brightness scaling
      brightness2 = brightness2 * brightnessScale;

  
      strip.setPixelColor(ledIndex, strip.gamma32(strip.ColorHSV(mappedHue, 255, brightness2)));
    }
  }
}


















void fillColor(uint8_t hue) {
  uint16_t mappedHue = hue * 256;  // Convert 0-255 hue range to 0-65535
  uint32_t color = strip.gamma32(strip.ColorHSV(mappedHue, 255, 150));  // Fixed brightness at 150

  // Set every LED to the same solid color
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, color);
  }

 }


// Declare an array of function pointers to hold the functions

void displayDigit(int *digit, int size) {
  for (int i = 0; i < size; i++) {
    strip.setPixelColor(digit[i] + shift, strip.ColorHSV(hue2 * 256));

    if (shift == 0) litLeds[i] = digit[i] + shift;
    if (shift == 22) litLeds[i + 12] = digit[i] + shift;
    if (shift == 55) litLeds[i + 24] = digit[i] + shift;
    if (shift == 77) litLeds[i + 36] = digit[i] + shift;
  }
}











void displayTime() {

// clear the stored lit digits array
    for (int i = 0; i < 52; i++) {  // 13 * 4 = 52
    litLeds[i] = -1;
  }

  if (seconds % 2 == 0 && changeTime != 1) {
    strip.setPixelColor(52, strip.ColorHSV(hue2 * 256));  // Red
    strip.setPixelColor(50, strip.ColorHSV(hue2 * 256));  // Red
                                                          //redBee();
  }

  if (changeTime == 1) {
    strip.setPixelColor(displayLeds[2][9], strip.Color(255, 0, 0));  // Red // opposite for location row,col
    strip.setPixelColor(displayLeds[4][9], strip.Color(255, 0, 0));  // Red
  }
  //strip.show();

  // Directly call the function using the array of digits
shift = 0;
if (h2 >= 0 && h2 <= 9) displayDigit(digits[h2], sizes[h2]);

shift = 22;
if (h1 >= 0 && h1 <= 9) displayDigit(digits[h1], sizes[h1]);

shift = 55;
if (m2 >= 0 && m2 <= 9) displayDigit(digits[m2], sizes[m2]);

shift = 77;
if (m1 >= 0 && m1 <= 9) displayDigit(digits[m1], sizes[m1]);

strip.show();
}


void setup() {
  Serial.begin(9600);  // disabled serial print

 

  



  randomSeed(analogRead(2));

  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();

  if (!Rtc.IsDateTimeValid()) {
    // Common Causes:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing

    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected()) {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning()) {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  } else if (now > compiled) {
    Serial.println("RTC is newer than compile time. (this is expected)");
  } else if (now == compiled) {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }



  for (int i = 0; i < numRows; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], LOW);  // Ensure rows are initially LOW
  }

  strip.begin();  // Initialize NeoPixel strip object
  clearDisplay();

  strip.setBrightness(brightness);  // Set maximum brightness
}

void loop() {

  RtcDateTime now = Rtc.GetDateTime();

  m1 = minutes % 10;
  m2 = minutes / 10;
  h1 = hours % 10;
  h2 = hours / 10;
  s = seconds % 10;
  printDateTime(now);




  //Main display update
  if (millisDelay(0, 50))

  {

    if (drawingMode == 0) {
      strip.clear();

      if (mode == 0) {  /// flower mode with bee.

        if (millisDelay(1, 100))  // bee movment speed
        {
          if (beeMoving == 1) {
            // Move bx toward bmx
            if (bx + 1 < bmx) {
              bx += dx;  // Move bx to the right
            } else if (bx + 1 > bmx) {
              bx -= dx;  // Move bx to the left
            }

            // Move by toward bmy
            if (by < bmy) {
              by += dy;  // Move by down
            } else if (by > bmy) {
              by -= dy;  // Move by up
            }
          }
        }

        if (millisDelay(3, 5000)) fy = !fy;  // move the flowers
        if (millisDelay(4, 4000) && beeMoving == 0) {
          fy2 = !fy2;
          by += (fy2 ? 1 : -1);  // if fy2 is true, move up; if false, move down
        }


        // Bee drawing background 
        fadingGradientBackground(132);
      drawImage(13, 0 -fy, flowerRed, sizeof(flowerRed) / sizeof(flowerRed[0]));
        drawImage(9, -1 + fy, flowerRed, sizeof(flowerRed) / sizeof(flowerRed[0]));
        drawImage(5, 0 - fy, flowerRed, sizeof(flowerRed) / sizeof(flowerRed[0]));
        drawImage(1, -1 +fy,flowerRed, sizeof(flowerRed) / sizeof(flowerRed[0]));
        if (bx % 2 == 0) drawImage(bx, by, bee, sizeof(bee) / sizeof(bee[0]));
        if (bx % 2 != 0) drawImage(bx, by, bee2, sizeof(bee2) / sizeof(bee2[0]));

        if (flashTime == 1 && beeMoving == 0 || (millisDelay(5, 900000))) {  // flash time every 15 minutes
         gradientBackground(hue);
          //hue2=180; // change color to blue.
          displayTime();
          if (millisDelay(2, 10000)) { flashTime = 0; }  // Flash the time for 10 seconds
         
        }
      }


      if (mode == 1) {  //time mode

        // What type of background behind the clock digits?
        if (background == 0) clearDisplay();            // no brackground
        if (background == 1) fillColor(hue);  // hue, sat, brightness          // solid background color can change by pushing top right button
        if (background == 2) gradientBackground(hue);
         
         if (background == 3) fadingGradientBackground(hue);
         if (background == 4) changingGradientBackground();
       
     
       
   if (background == 5) gradientWavesBackground();            // top right button changes the cycle speed.
             
      

        displayTime();
      }
    }
    strip.show();
  }
  //}




  //Check the buttons pressed
  for (int i = 0; i < numRows; i++) {
    // Activate the current row
    row = i;
    digitalWrite(rowPins[row], HIGH);

    // Read the ADC value
    int sensorValue = analogRead(A0);

    // Check which button is pressed
    int buttonPressed = getButtonPressed(sensorValue);

    if (buttonPressed != -1) {
      col = buttonPressed;

      convertRowColToXY(row,col); // changed the pushed button into x,y coordinates, when updating 

      if (beeMoving == 1) { // move to the pushed button if bee is moving. 
        //buttonToXY(row, col, buttonX, buttonY);
        bmx = x;
        bmy = y;
      }


      // Cycle through colors for the button pressed
      cycleColor(buttonPressed);
      // if (mode == 1&&col!=0&&row!=i,&&row!=i) delay(200);  // if drawing mode


      // If top left hand button is press increase the brightness of display
      // The button under this one will decrease the brightness of the display.
      if (x==0 && y == 4 && brightness < 255) {
        brightness += 5;
        strip.setBrightness(brightness);
      }

      if (x==0 && y==3 && brightness > 5) {
        brightness -= 5;
        strip.setBrightness(brightness);
      }


     
      // press the bee function if you press the yellow body of the bee
      if ((displayLeds[x][y] == displayLeds[bx+1][by]) && mode == 0) {
       
        beeMoving = !beeMoving;  // make the bee move or not move.
                  
        delay(200);              // delay for button debounce
      }

      

      //change time if either the two center dots are pressed and in time mode.
      if (((x==9&& y == 0) || (x == 9 && y == 2)) && mode == 1 && drawingMode == 0) {

        changeTime = !changeTime;
        displayTime();


        delay(200);
      }

      //
      //change the time.
      if (changeTime == 1) {


        

        if (x>14&x<18) {
          m1 += 1;
          if (m1 == 10) {
            m1 = 0;
          }
          minutes = m2 * 10 + m1;
          updateTime();
        }

        if (x>10&&x<14) {
          m2 += 1;
          if (m2 == 6) {
            m2 = 0;
          }
          minutes = m2 * 10 + m1;
          updateTime();
        }


        if (x>4&&x<8) {
          h1 += 1;
          if (h1 == 10 || h1 > 3 && h2 == 2) {
            if (h2 != 0) h1 = 0;
            if (h2 == 0) h1 = 1;
          }
          hours = h2 * 10 + h1;
          updateTime();
        }
        if (x>0&&x<4) {
          h2 += 1;
          if (h2 == 3) {
            if (h1 != 0) h2 = 0;
            if (h1 == 0) h2 = 1;
          }
          hours = h2 * 10 + h1;
          updateTime();
        }

  
        fillColor(hue);  // hue, sat, brightness
        displayTime();
      }

      if (changeTime == 0) {

        // test to see if button coresponds to a lit led from the digits.

        if (mode == 1) {
          bool found = false;  // Track if a match is found

          for (int c = 0; c < 13 * 4; c++) {
            if (litLeds[c] == displayLeds[row2][col2]) {
              hue2 += 2;  // Increment hue2
              if (hue2 >= 256) hue2 = 0;
              found = true;
              break;  // Stop looping after finding a match
            }
          }

          if (!found && x != 0 && ((background >0)&&(background<4))) {  // If no match was found, increase hue instead for the
            hue += 2;
            if (hue >= 256) hue = 0;
          }
        }
        //drawing mode




        
        // change the background mode
        if (x== 0 && y == 1&&mode==1) {  // top right hand of display 2nd button down. used to change background.
          background++;
          if (background > 5) background = 0;  //loop around
          delay(200);
        }


        // change the mode button left middle
        if (x == 0 && y == 2) {
          mode++;
          if (mode > 2) mode = 0;
          if (mode != 0) beeMoving = 0;  //turn off the bee moving function
          if (mode != 2) drawingMode = 0;
          if (mode == 2) {

            drawingMode = !drawingMode;
            gradientBackground(hue);  // hue, sat, brightness
          }
          delay(200);
        }







        if (drawingMode == 1)  // coloring mode
        {

          if (x == 0 && y == 0) {

            clearDisplay();
            strip.show();
          }
          if (x == 0 && y == 1) {
            hue = hue + 2;
            if (hue >= 256) hue = 0;
            gradientBackground(hue);  // hue, sat, brightness
            //strip.show();
          }
        
        }
      }




      // Print which button is pressed
      Serial.print("Button [");
      Serial.print(row);
      Serial.print(",");
      Serial.print(col);
      Serial.print("]: ");



      Serial.print(sensorValue);
      
      //Serial.print("mode:");
     // Serial.print(mode);
     //   Serial.print(" back:");
      //Serial.print(background);
      //     Serial.print(" hue:");
     // Serial.print(hue);
      
      Serial.println("");
     // convertRowColToXY(row,col,x,y);
      delay(30);  // for stablility
    
    }

    // Deactivate the row
    digitalWrite(rowPins[row], LOW);
  }
}

int getButtonPressed(int adcValue) {
  for (int i = 0; i < numButtons; i++) {
    if ((adcValue >= (expectedADCValues[i] - threshold) && adcValue <= (expectedADCValues[i] + threshold)) && adcValue > threshold) {
      return i;
    }
  }
  return -1;
}








void randomHex() {
  //strip.setPixelColor(displayLeds[random(9)][random(10)], strip.Color(random(200, 240), random(200, 240), random(0, 10))); // yellow
  strip.setPixelColor(displayLeds[random(9)][random(10)], strip.Color(random(0, 255), random(0, 255), random(0, 255)));  // yellow
                                                                                                                         //strip.show();
                                                                                                                         //delay(200);
}

void cycleColor(int buttonIndex) {
  colorState[buttonIndex] = (colorState[buttonIndex] + 1) % 5;  // Cycle through 0 to 4

  // Set the color based on the current state
  switch (colorState[buttonIndex]) {
    case 0:
      strip.setPixelColor(displayLeds[row2][col2], strip.Color(255, 0, 0));  // Red
      break;
    case 1:
      strip.setPixelColor(displayLeds[row2][col2], strip.Color(0, 255, 0));  // Green
      break;
    case 2:
      strip.setPixelColor(displayLeds[row2][col2], strip.Color(238, 210, 2));  // yellow
      break;
    case 3:
      strip.setPixelColor(displayLeds[row2][col2], strip.Color(255, 255, 255));  // White
      break;
    case 4:
      strip.setPixelColor(displayLeds[row2][col2], strip.Color(0, 0, 0));  // Off
      break;
  }
  if (mode == 0) {  /// put 0 later
                    //displayTime();
    if (x != 0 && beeMoving == 0) {
      flashTime = !flashTime;  // don't flash if it's the first column pressed or last column
      delay(200);
    }
  }
  //if (mode == 1) delay(100);  //delay the press if in draw mode

  strip.show();
}


#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt) {
  char datestring[26];
  //    uint8_t hours = dt.Hour();

  month = dt.Month();
  year = dt.Year();
  day = dt.Day();
  hours = dt.Hour();
  minutes = dt.Minute();
  seconds = dt.Second();

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second());
  //Serial.print(datestring);
}