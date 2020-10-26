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
#include "5bite_rus.h" // подключаем внешний шрифт
#include "font_5x7.h" // подключаем шрифт

char max7219[6] = "m7219";
char max_buf[256];

char matrix72xxp_buf[256];


#define MAX_DEVICES 4 // количество матриц
#define CS_PIN 15     // D8
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
//MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
MD_Parola *max72xxp;


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
  
  max72xxp->displayText(max7219, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  max72xxp->displayAnimate();

#endif
}
void max72xxpOnOff(void)
{
  if (!disp_power)
  {
  }
}
#ifndef UTF8RUSm
#define UTF8RUSm
String utf8rus_m(String source)
{
  bool up = false;
  if (up == true) source.toUpperCase();
  int i, k;
  String target;
  unsigned char n;
  k = source.length(); i = 0;
  while (i < k) {
    n = source[i]; i++;
    switch (n) {
      case 208: {
          n = source[i]; i++;
          if (n == 134) { n = 143; break; }  // І
          if (n == 129) { n = 168; break; }
          if (n >= 144 && n <= 191) n = n + 48;
          if (n >= 224 && up == true) n = n - 32;
          break;
        }
      case 209: {
          n = source[i]; i++;
          if (n == 150) { n = 144; break; }  // і
          if (n == 145) { n = 184; break; }
          if (n >= 128 && n <= 143) n = n + 112;
          if (n >= 224 && up == true) n = n - 32;
          break;
        }
      case 210: {
          n = source[i]; i++;
          switch (n) {
            case 146: n = 129; break; // Ғ
            case 147: n = 130; break; // ғ
            case 154: n = 131; break; // Қ
            case 155: n = 132; break; // қ
            case 162: n = 133; break; // Ң
            case 163: n = 134; break; // ң
            case 176: n = 137; break; // Ұ
            case 177: n = 138; break; // ұ
            case 174: n = 139; break; // Ү
            case 175: n = 140; break; // ү
            case 186: n = 141; break; // Һ
            case 187: n = 142; break; // h
            break;
          }
          break;
        }
      case 211: {
          n = source[i]; i++;
          switch (n) {
            case 152: n = 127; break; // Ә
            case 153: n = 128; break; // ә
            case 168: n = 135; break; // Ө
            case 169: n = 136; break; // ө
            break;
          }
          break;
        }
        break;
      case 226: {
        n = source[i]; i++;
          switch (n) {
            case 132:{
              n = source[i]; i++;
                switch (n) {
                  case 150: {
                    n = 35; break; // №
                  }
                }
            }
          }
      }
    }
    target += char(n);
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
      
      
      utf8rus_m(dsp_str).toCharArray(max_buf, 256);
             
      max72xxp->displayText(max_buf, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
      max72xxp->displayAnimate();
      break;
#ifdef USE_DISPLAY_MODES1TO5 //3
    case FUNC_DISPLAY_EVERY_SECOND:
      char line[7];
      if (RtcTime.second & 1)
      {
        snprintf_P(line, sizeof(line), PSTR(" %02d" ":" "%02d"), RtcTime.hour, RtcTime.minute); // [ 12:34:56 ]
      }
      else
      {
        snprintf_P(line, sizeof(line), PSTR(" %02d" ";" "%02d"), RtcTime.hour, RtcTime.minute); // [ 12:34:56 ]
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
