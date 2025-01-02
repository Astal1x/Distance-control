#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Настройка модуля nRF24L01
RF24 radio(9, 10);  // CE, CSN
const byte address[6] = "R1337";  // Адрес
const int channel = 108;          // Уникальный канал

const int motorXPin = A0;  
const int motorYPin = A1;  
const int servoYPin = A3;  


struct DataPacket {
  int motorX;  
  int motorY;  
  int servoY;  
};
DataPacket dataToSend;

void setup() {
  Serial.begin(9600);


  if (!radio.begin()) {
    Serial.println("Ошибка: модуль nRF24L01 не инициализирован");
    while (1);  
  }
  radio.openWritingPipe(address);  
  radio.setPALevel(RF24_PA_HIGH);  
  radio.setDataRate(RF24_1MBPS);   
  radio.setChannel(channel);       

  Serial.println("Пульт готов!");
}

void loop() {
  dataToSend.motorX = analogRead(motorXPin);
  dataToSend.motorY = analogRead(motorYPin);
  dataToSend.servoY = analogRead(servoYPin);

  bool success = radio.write(&dataToSend, sizeof(dataToSend));
  if (!success) {
    Serial.println("Ошибка отправки данных");  
  } else {
    Serial.print("Отправлено: ");
    Serial.print("M_X=");
    Serial.print(dataToSend.motorX);
    Serial.print(" M_Y=");
    Serial.print(dataToSend.motorY);
    Serial.print(" Servo_Y=");
    Serial.println(dataToSend.servoY);
  }

  delay(100);  
}
