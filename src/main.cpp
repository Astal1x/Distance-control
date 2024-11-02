#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/* Распиновка:
  --------------------------------
  Антенна кабельное тв:
  CE - D10, 
  CSN - D9, 
  VCC - 3.3, 
  GND - GND, 
  MOSI - D11, 
  MISO - D12, 
  SCK - D13
  --------------------------------
  Сосочек джойстик для управления:
  X: A0;
  Y: A1;
  --------------------------------
  Питание: 9V крона
  --------------------------------
  upd for 02/11
  - задержка 50 мс
  - высокий уровень сигнала
*/

RF24 radio(10, 9); 

const int joystickXPin = A0; 
const int joystickYPin = A1;  

void setup() {
  Serial.begin(9600); 

  // Инициализация тв антенны бабушки
  if (!radio.begin()) {
    Serial.println("Ошибка: модуль не инициализирован");
    while (1); 
  }
  Serial.println("Модуль успешно инициализирован");

  // Настройки чудоантенны
  radio.setChannel(76); // Канал
  radio.setPayloadSize(32); // Размер нагрузки
  radio.setDataRate(RF24_1MBPS); // Скорость передачи
  radio.openWritingPipe(0xF0F0F0F0E1LL); // Адрес передачи
  radio.startListening(); // Начать прослушивание
  radio.setPALevel(RF24_PA_HIGH ); // Высокий уровень приема
}

void loop() {
  int xValue = analogRead(joystickXPin);  
  int yValue = analogRead(joystickYPin);  
  
  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print(" Y: ");
  Serial.println(yValue);

  // Подготовка данных для передачи
  int joystickData[2] = {xValue, yValue};  // Массив для передачи
  radio.stopListening(); // Остановить прослушивание перед передачей
  radio.write(&joystickData, sizeof(joystickData)); // Передача данных
  radio.startListening(); // Возобновить прослушивание

  delay(50); // Задержка месячных
}
