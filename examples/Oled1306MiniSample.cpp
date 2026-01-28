//==============================================================================
// Работа с OLED дисплеем на I2C через самодельную библиотеку Wire и
// библиотеку Oled1306Mini.
//------------------------------------------------------------------------------

#define OLED_1306_MINI // Отключает часть функций (которые могут потребоваться) (экономия ~150 байт)
// #define OLED_NO_PRINT  // Отключить вывод текста на дисплей. Остается только графика
// #define OLED_NO_RUS    // Отключить русский язык для экономии памяти (освобождает около 400 байт)

#include <Oled1306Mini.hpp>
#include <Wire.h>

#define delay(x) Delay_Ms(x)

OledMini<SSD1306_128x64> oled(0x3C);
// GyvOLED<SSD1306_128x32> oled(0x3C);

int main(void) {
  SystemCoreClockUpdate();
  Delay_Init();

  // --------------------------
  // СЕРВИС
  oled.init(); // инициализация
  // настройка скорости I2C, если не сделано для другого устройства на шине
  Wire.setClock(400000L); // макс. 800'000. Ниже 250000 - не работает
  oled.flipV(true);       // отразить по вертикали
  oled.flipH(true);       // отразить по горизонтали
  oled.clear();           // очистить дисплей (или буфер)
  oled.setContrast(64);   // яркость 0..255
  oled.setPower(true);    // true/false - включить/выключить дисплей

  // --------------------------
#ifndef OLED_NO_PRINT
  oled.home(); // курсор в 0,0
  oled.print("Hello!");
  oled.setCursor(5, 1); // курсор в (пиксель X, строка Y)
  oled.setScale(2);
  oled.print("Hello!");
  oled.setCursorXY(15, 30); // курсор в (пиксель X, пиксель Y)
  oled.setScale(3);
  oled.invertText(true); // инвертируй текст!
  oled.print("Привет!");
  delay(1000);
#endif

  // --------------------------
  oled.home();
  oled.textMode(0); // 0 BUF_ADD - наложить текст
                    // 1 BUF_SUBTRACT - вычесть текст
                    // 2 BUF_REPLACE - заменить (весь прямоугольник буквы)
  oled.home();
  oled.setScale(3);
  oled.print("TEXT");
  delay(500);

  // --------------------------
  oled.clear();
  oled.dot(0, 0);          // точка на x,y
  oled.dot(0, 1, 1);       // третий аргумент: 0 выкл пиксель, 1 вкл пиксель (по умолч)
  oled.line(5, 5, 10, 10); // линия x0,y0,x1,y1
  // oled.line(5, 5, 10, 10, 0);   // пятый аргумент: 0 стереть, 1 нарисовать (по умолч)
  oled.fastLineH(0, 5, 10); // горизонтальная линия (y, x1, x2)
  // oled.fastLineH(0, 5, 10, 0);  // четвёртый аргумент: 0 стереть, 1 нарисовать (по умолч)
  oled.fastLineV(0, 5, 10);              // аналогично верт. линия (x, y1, y2)
  oled.rect(20, 20, 30, 25);             // прямоугольник (x0,y0,x1,y1)
  oled.rect(5, 35, 35, 60, OLED_STROKE); // прямоугольник (x0,y0,x1,y1)
                                         // параметры фигуры:
                                         // OLED_CLEAR - очистить
                                         // OLED_FILL - залить
                                         // OLED_STROKE - нарисовать рамку
  // oled.roundRect(50, 5, 80, 25, OLED_STROKE); // аналогично скруглённый прямоугольник
  // oled.circle(60, 45, 15, OLED_STROKE);       // окружность с центром в (x,y, с радиусом)
  // oled.circle(60, 45, 5, OLED_FILL);          // четвёртый аргумент: параметр фигуры
  delay(2000);

  // --- Рисуем антенны -----------------------
  const uint8_t ANT_BIG[] = {                                                 // 8x16
                             0x03, 0x0C, 0x30, 0xFF, 0x30, 0x0C, 0x03, 0x00,  // Первая строка битмапа слева направо. Младший бит сверху
                             0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00}; // Вторая строка битмапа
  const uint8_t ANT[] = {                                                     // Антеннка в одно знакоместо
                         0x01, 0x02, 0x04, 0xFF, 0x04, 0x02, 0x01};
  oled.setScale(1);
  oled.clear();
  oled.setCursor(1, 2);
  // oled.drawByte(0xff);
  // oled.drawBytes(ANT_BIG, sizeof(ANT_BIG));
  oled.drawBitmap(1, 1, ANT_BIG, 8, 16);
  oled.setCursor(10, 2);
  oled.drawBytes(ANT, sizeof(ANT));
  delay(1000);

  // ---- Рисуем прямоугольники ----------------------
  bool state = true;
  int i = 4;
  while (--i) {
    oled.clear();
    oled.home();
    // delay(1000);
    if (state) {
      oled.rect(0, 0, 127, 63, OLED_FILL); // прямоугольник (x0,y0,x1,y1)
    } else {
      oled.rect(5, 35, 35, 90, OLED_STROKE); // прямоугольник (x0,y0,x1,y1)
    }
    state = !state;
    delay(500);
  }

  // --- Рисуем сеточку линиями -----------------------
  i = 0;
  oled.clear();
  while (i < 8) {
    oled.fastLineH(i * 8, 0, 127, 1);
    i++;
    delay(50);
  }
  i = 0;
  while (i < 16) {
    oled.fastLineV(i * 8, 0, 63, 1);
    i++;
    delay(50);
  }
  delay(500);

  // --- Рисуем квадраты один в одном -----------------------
  i = 0;
  oled.clear();
  while (i < 4) {
    oled.rect(64 - i * 8, 32 - i * 8, 64 + i * 8, 32 + i * 8, OLED_STROKE);
    i++;
    delay(100);
  }
  delay(1000);

#ifndef OLED_1306_MINI
  // --- Рисуем окружности одна в одной -----------------------
  oled.clear();
  i = 0;
  while (i < 4) {
    oled.circle(64, 32, i * 8, OLED_STROKE);
    i++;
    delay(100);
  }
  delay(1000);
#endif

  // --- Выводим текст -----------------------
  oled.clear();
  oled.invertText(false); 
  // oled.autoPrintln(true);
  oled.setScale(1);
  uint8_t x = 0, y = 0;
  while (1) {
    if (y == 0) {
      oled.clear();
      oled.home();
    }
    oled.setCursor(x, y);
    oled.print("Hello, привет! 0123456789ABCDEF");
    delay(1000);
    y = (y + 1) % 8;
  }
}
