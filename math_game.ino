#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <stdlib.h>
#include <time.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

// define inputs
#define ip_1 7
#define ip_2 8
#define ip_3 9
#define ip_4 10
// define outputs
#define op_1 11
#define op_2 12
#define op_3 13

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte pin_rows[ROW_NUM] = {7, 8, 9, 10}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {11, 12, 13, 14}; //connect to the column pinouts of the keypad
bool start = false;

// Array of questions
String problems[20] = {
  "3 X 6 + 116 =", "19 - 34 X 61= ", "100 / 25 X 25= ", "99 - 56 X 225/5", "-3 + 2x = 11", "4x + 6 = -10" , "x + 9 = 18 - 2x", "2x + 6 = 4x - 2",
  "-x-1 = 221 + 2x" , "15 + 5x = 0", "17x-12 = 114+3x", "2x-10 = 10-3x",  "12x + 60 = 144", "-10x-19 = 19-8x", "6/2 + 7 X 4 =", "4 + 82X(30/5) =",
  "3(125-?) = 3", "x% of 5=6% of 40", "616+ ? +333=255", "[6-{4-(8-6+3)}]"
};
//Array of real solutions
int solutions[20] = {
  134, -2055, 100, -2421, 7, -4, 3, 4, -74, -3, 9, 4, 7, -19, 31, 496, 124, 48, -694, 7
};

unsigned long period = 60000;

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
#define BUZZER A1

void setup()
{
  Serial.begin(9600);

  time_t t;
  srand(time(0));
  
  lcd.init(); // initialize the lcd
  lcd.backlight();

  lcd.setCursor(4, 0);         // move cursor to   (0, 0)
  lcd.print("Math");        // print message at (0, 0)
  lcd.setCursor(9, 1);         // move cursor to   (2, 1)
  lcd.print("Game"); // print message at (2, 1)
  delay(5000);
  lcd.clear();
  randomSeed(analogRead(A2));
}

void(* resetFunc) (void) = 0;  // declare reset fuction at address 0

void game_over() {
  tone(BUZZER, 440, 200);
  delay(200);
  noTone(BUZZER);
  tone(BUZZER, 494, 500);
  delay(500);
  noTone(BUZZER);
  tone(BUZZER, 523, 300);
  delay(300);
}

int take_input() {
  char key = keypad.getKey();
  int j = 0;
  lcd.setCursor(0,1);
  char res[10];
  memset(res, 0, 10);
  while (key != '#') {
    if (key) {
      if (key == '*') {
        lcd.print("-");
        res[j] = '-';
      } else {
        lcd.print(key);
        res[j] = key;
      }
      j++;
    }
    key = keypad.getKey();
  }
  int r = atoi(res);
  return r;
}

int perform_op(int op, int a, int b) {
  int result;
  if (op == 1) {
    result = a + b;
    char operation[20];
    sprintf(operation, "%d + %d =", a, b);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(operation);
    int r = take_input();
    if (result == r)
      return 1;
  } 
  if (op == 2) {
    result = a - b;
    char operation[20];
    sprintf(operation, "%d - %d =", a, b);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(operation);
    int r = take_input();
   
    if (result == r) {
      return 1;
    }
  }
  if (op == 3) {
    result = a * b;
    char operation[20];
    sprintf(operation, "%d * %d =", a, b);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(operation);
    int r = take_input();
    if (result == r)
      return 1;
  } 
  if (op == 4) {
    if(a % b != 0)
    {
        int m = a % b;
        a -= m;
    }
    result = a / b;
    char operation[20];
    sprintf(operation, "%d / %d =", a, b);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(operation);
    int r = take_input();
    if (result == r)
      return 1;
  }
  return 0;
}

int l1() {
  int a, b, op, res, temp;
  op = random(1, 5);
  a = random(1, 11);
  b = random(1, 11);
  
  if (a < b) {
    temp = a;
    a = b;
    b = temp;  
  }
  int good = perform_op(op, a, b);
  return good;
}


int l2() {
  int a, b, op, res, temp;
  op = random(1, 5);
  a = random(1, 50);
  b = random(1, 50);
  if(a < b)
  {
    temp = a;
    a = b;
    b = temp;
  }
  int good = perform_op(op, a, b);
  return good;
}

int l3() {
  int a, b, op, res, temp;
  op = random(1, 5);
  a = random(1, 100);
  b = random(1, 100);
  if(a < b)
  {
    temp = a;
    a = b;
    b = temp;
  }
  int good = perform_op(op, a, b);
  return good;
}

int l4() {
  int a, b, op, res, temp;
  op = random(1, 5);
  a = random(1, 250);
  b = random(1, 250);
  if(a < b)
  {
    temp = a;
    a = b;
    b = temp;
  }
  int good = perform_op(op, a, b);
  return good;
}

void victory() {
  tone(BUZZER, 523, 300);
  delay(300);
  noTone(BUZZER);
  tone(BUZZER, 494, 500);
  delay(500);
  noTone(BUZZER);
  tone(BUZZER, 440, 200);
  delay(200);
  noTone(BUZZER);
  tone(BUZZER, 494, 500);
  delay(500);
  noTone(BUZZER);
  
}

void loop()
{
  long start_time;
  long end_time;
  if (start) {
    int i;
    for (i = 0; i < 12; i++) {
      if (i == 0)
        start_time = millis();
      int good;
      if (i >= 0 && i < 3) {
        good = l1();
      }
      if (i >= 3 && i < 6) {
        good = l2();  
      }
      if (i >= 6 && i < 9) {
        good = l3();
      }
      if (i >= 9 && i < 12) {
        good = l4();
      }
      end_time = millis();
      if (end_time - start_time > period) {
        game_over();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("YOUR SCORE IS");
        lcd.setCursor(5, 1);
        lcd.print(i);
        delay(5000);
        resetFunc(); //call reset
      }
      else {
        if (good == 1) {
          continue;
        } else {
          game_over();
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("YOUR SCORE IS");
          lcd.setCursor(5, 1);
          lcd.print(i);
          delay(5000);
          resetFunc(); //call reset
        }
      }
    }
    if (i == 12) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("YOU WON");
      victory();
      delay(3000);
      resetFunc();  
    }
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("Press *");
    lcd.setCursor(3, 1);
    lcd.print("to start");
    char key = keypad.getKey();

    if (key == '*') {
      start = true;
      lcd.clear();
      lcd.setCursor(0, 0);
    }
  }
  
}
