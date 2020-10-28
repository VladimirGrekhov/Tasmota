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
#ifdef USE_DISPLAY_MAX72xx

#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

#warning **** MAX 72xx ****
#define XDSP_13 13

int numberOfHorizontalDisplays = 4; // Количество светодиодных матриц по Горизонтали
int numberOfVerticalDisplays = 1;   // Количество светодиодных матриц по Вертикали
int Max72xx_spacer = 2;
int Max72xx_width = 5 + Max72xx_spacer; // Регулируем расстояние между символами

//Max72xxPanel matrix72xx =  Max72xxPanel(PinUsed(GPIO_SSPI_CS), numberOfHorizontalDisplays, numberOfVerticalDisplays);
//Max72xxPanel *matrix72xx;
uint8_t Max72xx_matrices = 4;
uint8_t Max72xx_state = 0;
uint8_t Max72xx_counter = 0;
int16_t Max72xx_x = 0;
int16_t Max72xx_y = 0;

//char Max72xx_buffer[Max72xx_MAX_SCREEN_BUFFER];
char *Max72xx_buffer = nullptr;

uint8_t Max72xx_mode = 0;
uint8_t Max72xx_loop = 0;
uint8_t Max72xx_done = 0;
Max72xxPanel *matrix72xx;
//Max72xxPanel matrix72xx = Max72xxPanel(15, numberOfHorizontalDisplays, numberOfVerticalDisplays);

void max72xxInitDriver()
{
  if (!Settings.display_model)
  {
    Settings.display_model = XDSP_13;
    Serial.println("!!!DR");
  }
  if (PinUsed(GPIO_SSPI_CS) && PinUsed(GPIO_SSPI_MOSI) && PinUsed(GPIO_SSPI_SCLK))
  { //SSPI_CS D8G15 SSPI_MOSI D7 G13 SSPI_SCLK D5 G14
    Serial.println("!DEBAG PIN OK");
    matrix72xx = new Max72xxPanel(Pin(GPIO_SSPI_CS), numberOfHorizontalDisplays, numberOfVerticalDisplays);
  }
  else
  {
    if (PinUsed(GPIO_SPI_CS) && PinUsed(GPIO_SPI_MOSI) && PinUsed(GPIO_SPI_CLK))
    {
      // Max72xxPanel matrix72xx = Max72xxPanel(GPIO_SPI_CS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
    }
    else
    {
      return;
    }
  }
  Settings.display_width = Max72xx_matrices * 8;
  Settings.display_height = 8;

  matrix72xxInitMode();
  //  matrix72xx.setIntensity(13); // Яркость матрицы от 0 до 15
}
void matrix72xxInitMode(void)
{
  if(Settings.display_rotate == 1 || Settings.display_rotate == 1 ){
  matrix72xx->setPosition(0, 0, 0); // The first display is at <0, 0>
  matrix72xx->setPosition(1, 1, 0); // The second display is at <1, 0>
  matrix72xx->setPosition(2, 2, 0); // The third display is at <2, 0>
  matrix72xx->setPosition(3, 3, 0); // And the last display is at <3, 0>
  } else{
  matrix72xx->setPosition(0, 3, 0); // The first display is at <0, 0>
  matrix72xx->setPosition(1, 2, 0); // The second display is at <1, 0>
  matrix72xx->setPosition(2, 1, 0); // The third display is at <2, 0>
  matrix72xx->setPosition(3, 0, 0); // And the last display is at <3, 0>
  }
  //  for (uint32_t i = 0; i < Max72xx_matrices; i++) {
  //    matrix72xx->setRotation(Settings.display_rotate);  // 1
  matrix72xx->setRotation(0, Settings.display_rotate); // 1 матрица
  matrix72xx->setRotation(1, Settings.display_rotate); // 2 матрица
  matrix72xx->setRotation(2, Settings.display_rotate); // 3 матрица
  matrix72xx->setRotation(3, Settings.display_rotate); // 4 матрица
  matrix72xx->setIntensity(Settings.display_dimmer);
  //    matrix72xx->blinkRate(0);               // 0 - 3
  //    matrix72xx[i]->setTextWrap(false);         // Allow text to run off edges

  //    matrix72xx->cp437(true);
  //  }
  matrix72xx->fillScreen(LOW);
  matrix72xx->write(); // Вывод на дисплей
#ifdef SHOW_SPLASH
  DisplayText("m72xx");
#endif
}
void Max72xxOnOff(void)
{
  if (!disp_power)
  {
    matrix72xx->fillScreen(LOW);
    matrix72xx->write(); // Вывод на дисплей
  }
}
void DisplayText(String text)
{
  matrix72xx->fillScreen(LOW);
  for (int i = 0; i < text.length(); i++)
  {

    int letter = (matrix72xx->width()) - i * (Max72xx_width - 1);
    int x = (matrix72xx->width() + 1) - letter;
    int y = (matrix72xx->height() - 8) / 2; // Центрируем текст по Вертикали
    matrix72xx->drawChar(x, y, text[i], HIGH, LOW, 1);
    matrix72xx->write(); // Вывод на дисплей
  }
}
#define UTF8RUS
#ifdef UTF8RUS
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
void Max72xxDrawStringAt(void)
{

  DisplayText(utf8rus(dsp_str));
}
void Max72xxTime()
{
  matrix72xx->fillScreen(LOW);
  int y = (matrix72xx->height() - 8) / 2; // Центрируем текст по Вертикали

  if (RtcTime.second & 1)
  {
    matrix72xx->drawChar(14, y, (String(":"))[0], HIGH, LOW, 1); //каждую четную секунду печатаем двоеточие по центру (чтобы мигало)
  }
  else
  {
    matrix72xx->drawChar(14, y, (String(" "))[0], HIGH, LOW, 1);
  }

  String hour1 = String(RtcTime.hour / 10);
  String hour2 = String(RtcTime.hour % 10);
  String min1 = String(RtcTime.minute / 10);
  String min2 = String(RtcTime.minute % 10);
  String sec1 = String(RtcTime.second / 10);
  String sec2 = String(RtcTime.second % 10);
  int xh = 2;
  int xm = 19;
  //    int xs = 28;

  matrix72xx->drawChar(xh, y, hour1[0], HIGH, LOW, 1);
  matrix72xx->drawChar(xh + 6, y, hour2[0], HIGH, LOW, 1);
  matrix72xx->drawChar(xm, y, min1[0], HIGH, LOW, 1);
  matrix72xx->drawChar(xm + 6, y, min2[0], HIGH, LOW, 1);
  //    matrix72xx->drawChar(xs, y, sec1[0], HIGH, LOW, 1);
  //    matrix72xx->drawChar(xs+6, y, sec2[0], HIGH, LOW, 1);

  matrix72xx->write(); // Вывод на дисплей
}
/*********************************************************************************************\
   Interface
  \*********************************************************************************************/

bool Xdsp13(byte function)
{
  if (function != 2)
  {
    //    Serial.print("function ");
    //    Serial.print(function);
  }
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function)
  {
    max72xxInitDriver();
  }
  else if (XDSP_13 == Settings.display_model)
  {
    switch (function)
    {

    case FUNC_DISPLAY_MODEL:
      Serial.println("!!!M O D E L");
      result = true;
      break;
    case FUNC_DISPLAY_POWER:
      Max72xxOnOff();
      break;
    case FUNC_DISPLAY_DRAW_STRING:
      matrix72xxInitMode();
      Max72xxDrawStringAt();
      break;
#ifdef USE_DISPLAY_MODES1TO5 //3
    case FUNC_DISPLAY_EVERY_SECOND:
      if (disp_power)
      {
        matrix72xxInitMode();
        Max72xxTime();
      }
      break;
#endif // USE_DISPLAY_MODES1TO5
    }
  }
  return result;
}

#endif // USE_DISPLAY_MAX7279
//
#endif // USE_DISPLAY
#endif // USE_SPI
