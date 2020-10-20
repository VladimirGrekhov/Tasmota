/*
  xdsp_13_max7219.ino - Display MAX72xx support for Tasmota

  Copyright (C) 2020  Gerhard Mutz and Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//#ifdef USE_SPI
#ifdef USE_SPI
#ifdef USE_DISPLAY
#ifdef USE_DISPLAY_max72xxp
#warning **** MAX 72xxp ****
#define XDSP_14 14

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include "font_5x7.h" // подключаем шрифт

int matrix72xxp_speed = 40;
char matrix72xxp_buf[256];
char matrix72xxp_t_buf[10];
int matrix72xxp_rnd;
bool matrix72xxp_animate;

String matrix72xxp_screen[20];
bool matrix72xxp_screen_scroll[20];
int matrix72xxp_current_screen;

#define MAX_DEVICES 4 // количество матриц
#define CS_PIN 15     // D8
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
//MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
MD_Parola *max72xxp;
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

//max72xxpPanel matrix72xx = max72xxpPanel(15, numberOfHorizontalDisplays, numberOfVerticalDisplays);

void max72xxpInitDriver()
{
  if (!Settings.display_model)
  {
    Settings.display_model = XDSP_14;
    Serial.println("!!!DR");
  }
  if (PinUsed(GPIO_SSPI_CS) && PinUsed(GPIO_SSPI_MOSI) && PinUsed(GPIO_SSPI_SCLK))
  { //SSPI_CS D8G15 SSPI_MOSI D7 G13 SSPI_SCLK D5 G14
    Serial.println("!DEBAG PIN OK");
    max72xxp = new MD_Parola(HARDWARE_TYPE, Pin(GPIO_SSPI_CS), MAX_DEVICES);
  }
  else
  {
    if (PinUsed(GPIO_SPI_CS) && PinUsed(GPIO_SPI_MOSI) && PinUsed(GPIO_SPI_CLK))
    {
      // max72xxpPanel matrix72xx = max72xxpPanel(GPIO_SPI_CS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
    }
    else
    {
      return;
    }
  }

  matrix72xxpInitMode();
  //  matrix72xx.setIntensity(13); // Яркость матрицы от 0 до 15
}
void matrix72xxpInitMode(void)
{
  Serial.println("!DEBAG INIT");
  max72xxp->begin();
  max72xxp->setInvert(false);
  max72xxp->setFont(fontBG);
  max72xxp->setIntensity(5); // яркость дисплея
#define SHOW_SPLASH
#ifdef SHOW_SPLASH
  max72xxp->displayText("max7219", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  max72xxp->displayAnimate();

#endif
}
void max72xxpOnOff(void)
{
  if (!disp_power)
  {
  }
}
#ifndef UTF8RUS
#define UTF8RUS
String utf8rus(String source)
{
  int i, k;
  String target;
  unsigned char n;
  char m[2] = {'0', '\0'};

  k = source.length();
  i = 0;

  while (i < k)
  {
    n = source[i];
    i++;

    if (n >= 0xC0)
    {
      switch (n)
      {
      case 0xD0:
      {
        n = source[i];
        i++;
        if (n == 0x81)
        {
          n = 0xA8;
          break;
        }
        if (n >= 0x90 && n <= 0xBF)
          n = n + 0x30 - 1;
        break;
      }
      case 0xD1:
      {
        n = source[i];
        i++;
        if (n == 0x91)
        {
          n = 0xB8;
          break;
        }
        if (n >= 0x80 && n <= 0x8F)
          n = n + 0x70 - 1;
        break;
      }
      }
    }
    m[0] = n;
    target = target + String(m);
  }
  return target;
}
#endif
void max72xxpTime()
{
}
/*********************************************************************************************\
   Interface
  \*********************************************************************************************/

bool Xdsp14(byte function)
{
  if (function != 2)
  {
    //    Serial.print("function ");
    //    Serial.print(function);
  }
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function)
  {
    max72xxpInitDriver();
  }
  else if (XDSP_14 == Settings.display_model)
  {
    switch (function)
    {

    case FUNC_DISPLAY_MODEL:
      Serial.println("!!!M O D E L");
      result = true;
      break;
    case FUNC_DISPLAY_POWER:
      max72xxpOnOff();
      break;
    case FUNC_DISPLAY_DRAW_STRING:

      max72xxp->displayText(dsp_str, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
      max72xxp->displayAnimate();
      break;
#ifdef USE_DISPLAY_MODES1TO5 //3
    case FUNC_DISPLAY_EVERY_SECOND:
      char line[7];
      if (RtcTime.second & 1) {
      snprintf_P(line, sizeof(line), PSTR(" %02d" D_HOUR_MINUTE_SEPARATOR "%02d" ), RtcTime.hour, RtcTime.minute);  // [ 12:34:56 ]
      }else
      {
              snprintf_P(line, sizeof(line), PSTR(" %02d" D_HOUR_MINUTE_SEPARATOR "%02d" ), RtcTime.hour, RtcTime.minute);  // [ 12:34:56 ]

      }
      
      max72xxp->displayText(line, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
      max72xxp->displayAnimate();
      if (disp_power)
      {
      }
      break;
#endif // USE_DISPLAY_MODES1TO5
    }
  }
  return result;
}

#endif // USE_DISPLAY_MAX7279
//
#endif // Use_display
#endif // USE_SPI
