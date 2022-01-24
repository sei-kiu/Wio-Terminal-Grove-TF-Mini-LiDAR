#include <Arduino.h>

#include "../include/Free_Fonts.h" //include free fonts library from https://github.com/Seeed-Studio/Seeed_Arduino_LCD/blob/master/examples/320x240/Free_Font_Demo/Free_Fonts.h
#include "TFT_eSPI.h"

#include "TFLidar.h"

TFT_eSPI tft;

#define USETFMINI
// #define USETFLUNA

#define SERIAL Serial

#if defined(SEEED_XIAO_M0)
#define uart Serial1
#elif defined(SEEED_WIO_TERMINAL)
#define uart Serial1
#else
SoftwareSerial uart(2, 3);
#endif

#ifdef USETFLUNA
TFLuna SeeedTFLuna;
TFLidar SeeedTFLidar(&SeeedTFLuna);
#endif

#ifdef USETFMINI
TFMini SeeedTFMini;
TFLidar SeeedTFLidar(&SeeedTFMini);
#endif

void setup()
{
  // put your setup code here, to run once:
  SERIAL.begin(9600);
  while (!Serial)
    ;
  SeeedTFLidar.begin(&uart, 115200);

  // set up screen
  tft.begin();
  tft.setRotation(2);
}

void loop()
{
  while (!SeeedTFLidar.get_frame_data())
  {
    delay(1);
  }
  // put your main code here, to run repeatedly:
  SERIAL.print("dist = ");
  SERIAL.print(SeeedTFLidar.get_distance()); //output measure distance value of LiDAR
  SERIAL.print('\t');
  SERIAL.print("strength = ");
  SERIAL.print(SeeedTFLidar.get_strength()); //output signal strength value
#ifdef USETFLUNA
  SERIAL.print("\t Chip Temprature = ");
  SERIAL.print(SeeedTFLidar.get_chip_temperature());
  SERIAL.print(" celcius degree"); //output chip temperature of Lidar
#endif
  SERIAL.println(" ");
  // delay(1000);

  //Initializing buffer
  TFT_eSprite spr = TFT_eSprite(&tft);

  // Create buffer (portrait)
  spr.createSprite(TFT_WIDTH, TFT_HEIGHT);

  // Fill background
  spr.fillSprite(TFT_YELLOW);

  // Header section
  spr.fillRect(0, 0, 240, 30, TFT_WHITE);
  spr.setFreeFont(FMB12);
  spr.setTextColor(TFT_BLACK);
  spr.drawString("Grove Ultrasonic", 5, 6);

  // Body section
  spr.setFreeFont(FMB18);
  spr.setTextDatum(MC_DATUM);
  spr.drawString("Distance", 120, 60);
  spr.drawString("to obstacle", 120, 90);
  spr.drawString("in front", 120, 120);
  spr.setFreeFont(FMB24);
  spr.setTextDatum(MC_DATUM);
  spr.drawNumber(SeeedTFLidar.get_distance(), 120, 180);
  spr.setTextDatum(MC_DATUM);
  spr.drawString("cm", 120, 210);

  //Push to LCD
  spr.pushSprite(0, 0);

  // Delete buffer
  spr.deleteSprite();
}
 