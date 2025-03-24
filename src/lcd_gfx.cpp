#include "TFT_eSPI.h"
#include "TJpg_Decoder.h"
#include "FS.h"
#include "SPIFFS.h"

#include "lcd_gfx.h"
#include "data_handling.h"
#include "hardware_config.h"

TFT_eSPI tft = TFT_eSPI();         // Invoke custom library

// This next function will be called during decoding of the jpeg file to
// render each block to the TFT.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // Return 1 to decode next block
  return 1;
}

void prepare_lcd(){
    // Initialise SPIFFS
    if (!SPIFFS.begin()) {
        Serial.println("SPIFFS initialisation failed!");
        while (1) yield(); // Stay here twiddling thumbs waiting
    }
    Serial.println("\r\nInitialisation done.");

    // Initialise the TFT
    tft.begin();
    tft.setRotation(1);
    tft.setTextColor(0xFFFF, 0x0000);
    tft.setTextSize(2);
    tft.fillScreen(TFT_BLACK);
    tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

    // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
    TJpgDec.setJpgScale(1);

    // The decoder must be given the exact name of the rendering function above
    TJpgDec.setCallback(tft_output);
}

void display_notification(){
    if ( Battery_Voltage < 2.2 ) 
    {
        tft.fillScreen(TFT_RED);
        tft.setTextColor(TFT_WHITE, TFT_RED);
        tft.drawString("Low Power.", 100, 100);
        tft.drawString("Please replace batteries.", 10, 140);
    }
    else if ( Moisture_Level < 0.2 ) 
    {
        TJpgDec.drawFsJpg(0, 0, "/thirstyflower.jpg");
    }
    else if ( Light_Level > 0.8 ) 
    {
        TJpgDec.drawFsJpg(0, 0, "/hotflower.jpg");
    }
    else if ( Tank_Level <= 1 ) 
    {
        tft.fillScreen(TFT_BLUE);
        tft.setTextColor(TFT_WHITE, TFT_BLUE);
        tft.drawString("Water tank is empty.", 40, 100);
        tft.drawString("Please refill.", 70, 140);
    }
    else
    {
        TJpgDec.drawFsJpg(0, 0, "/happyflower.jpg");
    }
    digitalWrite(LCD_LED, HIGH);
}