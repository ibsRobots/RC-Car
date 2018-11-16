#include <SoftwareSerial.h>
#include <Servo.h>
#include <stdlib.h>

int servoPin = 7;
long servoPosition = 0;
Servo servo;
int angle;
int power;
SoftwareSerial softSerial = SoftwareSerial(2, 3);

String portData = "";     // Переменная приема команды
boolean endOfString = false;

void setup() {
  Serial.begin(9600);
  softSerial.begin(9600);
  servo.attach(servoPin);
  servo.write(90);
}

void loop() {
//  if (softSerial.available() > 0) {
//    portData = softSerial.readString();
//    Serial.println("Port data: " + portData);
//    angle = get_angle(portData);
//    Serial.println("After map: " + angle);
//    if (angle > -1) {
////      Serial.println(angle);
//      servo.write(angle);
//    } 
//  }
//  delay(5);

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

int get_angle(String portData) {
  int angle = -1;
  String command = portData.substring(0, 4);
  if (command == "dir ") {
    portData = portData.substring(4, portData.length());
    int index = find_text(" ", portData);
    angle = atoi(portData.substring(0, index + 1).c_str());
    Serial.println("Before map: " + angle);
    angle = map(angle, -10, 10, 60, 120);
  }
  return angle;
}

int get_power(String portData) {
  int power = -1;
  String command = portData.substring(0, 4);
  if (command == "dir ") {
    portData = portData.substring(4, portData.length());
    int index = find_text(" ", portData);
    power = atoi(portData.substring(index + 1, portData.length()).c_str());
    power = abs(power);
    power = map(power, 0, 10, 0, 255);
  }
  return power;
}

int find_text(String needle, String haystack) {
  int foundpos = -1;
  for (int i = 0; i <= haystack.length() - needle.length(); i++) {
    if (haystack.substring(i,needle.length()+i) == needle) {
      foundpos = i;
    }
  }
  return foundpos;
}
