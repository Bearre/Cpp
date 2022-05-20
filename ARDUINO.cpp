
#include <Servo.h>
#include <Adafruit_LiquidCrystal.h>


/* Светофор */
const int LED_RED = 12;             // Порт 13, красный светодиод
const int LED_YELLOW = 11;             // Порт 12, желтый светодиод
const int LED_GREEN = 10;             // Порт 11, зеленый светодиод

const int TIMEOUT_RED = 3000;          // Время горения красного сетодиода
const int TIMEOUT_YEL = 1690;          // Время горения желтого светодиода
const int TIMEOUT_GREEN = 2000;        // Время горения зеленого светодиода

const int TIMEOUT_FLASH_GREEN = 500;      // Время мигания зеленого светодиода

void setup()
{
  // Все порты светодиодов будут у нас установлены в режим "внешняя нагрузка", OUTPUT
  pinMode(LED_RED, OUTPUT);   
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  
  // Устанавливаем начальное значение светодиодов
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
}  

void loop()
{
  // Включаем зеленый цвет светофора
  digitalWrite(LED_GREEN, HIGH);    // Включаем светодиод       
  delay(TIMEOUT_GREEN);             // Ждем

  // Мигаем зеленым светодиодом 3 раза
  for (int i=0; i<3; i++)
    {
      digitalWrite(LED_GREEN, LOW);         
      delay(TIMEOUT_FLASH_GREEN);                
      digitalWrite(LED_GREEN, HIGH);        
      delay(TIMEOUT_FLASH_GREEN);                
    }  
  
  // Теперь отключаем зеленый и включаем желтый светодиод
  digitalWrite(LED_GREEN, LOW); 
  digitalWrite(LED_YELLOW, HIGH);           
  delay(TIMEOUT_YEL);            
  
  // Отключаем желтый светодиод.
  digitalWrite(LED_YELLOW, LOW); 
  // Теперь включаем красный цвет
  digitalWrite(LED_RED, HIGH);            
  delay(TIMEOUT_RED);                         
          
  // Включаем желтый светодиод,не выключая красный
  digitalWrite(LED_YELLOW, HIGH);           
  delay(TIMEOUT_YEL);                       
  
  // Отключаем желтый и красный светодиоды.
  digitalWrite(LED_YELLOW, LOW);    
  digitalWrite(LED_RED, LOW);   
  
}


/* Hand */

bool button_state_right = 0;
bool button_state_left = 0;
int pos = 0;

Adafruit_LiquidCrystal lcd_1(0);
Servo servo_9;


void setup()
{
  pinMode(10, INPUT);
  pinMode(8, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  servo_9.attach(9, 500, 2500);
  servo_9.write(0);
  lcd_1.begin(16, 2);
}


void turn_left()
{
  	lcd_1.print("Turning Left");
	for (pos; pos < 180; ++pos) 
    {
    	servo_9.write(pos);
    	delay(15); 
    }
  	lcd_1.setCursor(0, 1);
	lcd_1.print(pos);  
}


void turn_left(int new_pos)
{
  	lcd_1.print("Turning Left");
	for (pos; pos < new_pos; ++pos) 
    {
    	servo_9.write(pos);
    	delay(15); 
    }
  	lcd_1.setCursor(0, 1);
	lcd_1.print(pos);  
}
  

void turn_right()
{
  	lcd_1.print("Turning Right");
	for (pos; pos > 0; --pos) 
    {
    	servo_9.write(pos);
    	delay(15); 
    }
  	lcd_1.setCursor(5, 1);
	lcd_1.print(pos);
}


void turn_right(int new_pos)
{
  	lcd_1.print("Turning Right");
	for (pos; pos > new_pos; --pos) 
    {
    	servo_9.write(pos);
    	delay(15); 
    }
  	lcd_1.setCursor(5, 5);
	lcd_1.print(pos);
    
}

void loop()
{
  
  // 			Buttons
  //----------------------------------
  button_state_left = digitalRead(10);
  button_state_right = digitalRead(8);
  if (button_state_left == HIGH) 
  {
    digitalWrite(LED_BUILTIN, HIGH);
    turn_left(45);
    lcd_1.setCursor(0, 0);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  delay(15); 

  if (button_state_right == HIGH) 
  {
  	digitalWrite(LED_BUILTIN, HIGH);
    turn_right();
    lcd_1.setCursor(0, 0);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(15);
  //----------------------------------
}
