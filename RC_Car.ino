#include <SoftwareSerial.h>
#include <Servo.h>
#include <stdlib.h>

int servoPin = 7;
long servoPosition = 0;
Servo servo;
int angle;
int power;
int servoD = 5; //полярность выхода на серву, порт должен быть с возможностью аналога
int servoV = 6; //напряжение на серву, порт должен быть с возможностью аналога
int motorD = 10; //полярность выхода на двиган, порт должен быть с возможностью аналога
int motorV = 11; //напряжение на двиган, порт должен быть с возможностью аналога

SoftwareSerial softSerial = SoftwareSerial(2, 3);

String portData = "";     // Переменная приема команды
boolean endOfString = false;

void setup() {
  Serial.begin(9600);
  softSerial.begin(9600);
  
  pinMode(servoD, OUTPUT);
  pinMode(servoV, OUTPUT);
  pinMode(servoD, OUTPUT);
  pinMode(motorV, OUTPUT);
  digitalWrite(servoD, HIGH);
  analogWrite(servoV, 85);   //5V from 7V, если servoD=HIGH, то 0-максимальное, а 255 - ноль, но НУЖНы живые акумы!!!!
  digitalWrite(motorD, LOW);
  analogWrite(motorV, 255);
  
  servo.attach(servoPin);
  servo.write(90);
}

void loop() {
  while (softSerial.available() > 0 && !endOfString) {
    char c = softSerial.read();    // Очередной символ в строке
    if (c != '\n') portData += c;     // Если это не символ конца строки, то добавляем его в строку
    else endOfString = true;
  }

  if (endOfString) {    // Если получен символ конца строки то разбор строки на ключ - значение и сравнение с готовыми
    Serial.println("portData: " + portData);
    String key = "", value = "";   // ключ, значение
    int i = 0;
    while (portData.length()>i) {
      if (portData[i]==' ') break;
      key += portData[i];
      i++;
    }
    if (portData[i] == ' ') {
      i++;
    }
    while (portData.length() > i) {
      value += portData[i];
      i++;
    }
//    Serial.println("key:" + key + "kakoy-to simvol");
//    Serial.println("value:" + value);
    portData = "";
    endOfString = false;

    if (key.equals("dir")) {  // Делать при получении значения с джойстика с идентификатором = 1
      boolean boolJoy = false; // 
      int i = 0;
      String valueX = "", valueY = "";
      while (value.length() > i){
        if (value[i] == ' ') boolJoy = true;
        if (value[i] != ' ' && !boolJoy) valueX += value[i];
        else if (value[i] != ' ' && boolJoy) valueY += value[i];
        i++;
      }
      int iValueX = valueX.toInt(); // Координата X
      int iValueY = valueY.toInt(); // Координата Y
//      Serial.println("valueX:" + valueX);
//      Serial.println("iValueX:" + String(iValueX));
      angle = map(-iValueX, -10, 10, 60, 120);
      Serial.println("Angle: " + String(angle));
      Serial.println("");
      servo.write(angle);
    }
    if(key.equals("L1on")){ //Делать при включении переключателя с идентификатором = 2
      
    }
    if(key.equals("L1off")){    //Делать при выключении переключателя с идентификатором = 2
      
    }
  }
  delay(50);
}
