#include "lcd.h"
#include "globals.h"

void lcdInit() {
    lcd.init();
    lcd.backlight();
}

void printLCD(int col, int row, char *str) {
    for (int i = 0; i < strlen(str); i++) {
        lcd.setCursor(col + i, row);
        lcd.print(str[i]);
    }
}
