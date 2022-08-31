#include <Wire.h> // Wire객체는 전역으로 이미 선언된 객체이므로 따로 선언해 줄 필요 없다.
#include "HX711.h"

#define SLAVE_ADDRESS 0x08  // I2C 주소 지정
#define water_level_sensor_PIN 7 // 수위 센서 핀 지정

// HX711 circuit wiring
#define LOADCELL_DOUT_PIN         3
#define LOADCELL_SCK_PIN          2

// I2C통신 메시지 저장
int number = 0;
char retMSG[1024] = ""; 

// scale - 10Kg loadcell : 226 / 5kg loadcell : 372
// ADC 모듈에서 측정된 결과값을 (loadcellValue)값 당 1g으로 변환해 줌
float loadcellValue = 372.0;
float rice_load = 0.0;
float rice_limit = 10000.0;

HX711 scale;

void setup() {
  // 슬레이브로써 I2C를 초기화한다.
  Wire.begin(SLAVE_ADDRESS);  // i2c통신을 초기화하고, 활성화하는 함수. 슬레이브 모드일 경우 주소 값을 지정한다. 지정하지 않을 경우 자동으로 마스터 모드로 설정됨. 통신을 시작하기 전 한 번만 호출하면 된다.
  Serial.begin(9600);

  // 로드셀 HX711 보드 핀 설정
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  // 수위센서 핀 설정
  pinMode(water_level_sensor_PIN, INPUT);  
  
  // 부팅 후 잠시 대기 (2초)
  delay(2000);

  // 측정값 1회 읽어오기
  Serial.print("read: \t\t\t");
  Serial.println(scale.read());

  delay(1000);

  // 스케일 설정
  scale.set_scale(loadcellValue);
  
  // 오프셋 설정(10회 측정 후 평균값 적용) - 저울 위에 아무것도 없는 상태를 0g으로 정하는 기준점 설정(저울 위에 아무것도 올려두지 않은 상태여야 합니다.)   
  scale.tare(10);    

  // 설정된 오프셋 및 스케일 값 확인
  Serial.print("Offset value :\t\t");
  Serial.println(scale.get_offset());
  Serial.print("Scale value :\t\t");
  Serial.println(scale.get_scale());

  // (read - offset) 값 확인 (scale 미적용)
  Serial.print("(read - offset) value: \t");  
  Serial.println(scale.get_value());
  delay(2000);

  // I2C 통신을 위한 콜백함수를 지정한다.
  Wire.onReceive(receiveData);  // 슬레이브 모드에서 사용되며, 마스터로부터 데이터가 수신됐을 때 호출될 이벤트 처리 함수를 등록한다. 
                                //이벤트 처리 함수에는 다른 이벤트 처리 함수들과 달리 int형 매개변수가 사용돼야 한다. 매개변수로 수신된 데이터의 길이가 전달된다.
  Wire.onRequest(sendData); // 슬레이브 모드에서 사용되며, 마스터로부터 데이터를 요청 받았을 때 호출될 이벤트 처리 함수를 등록한다.
                            // 이벤트 처리 함수의 매개변수는 없다.

}

void loop() {
  delay(100);

  int sensorVal = digitalRead(water_level_sensor_PIN); 
  
  check_rice_quantity();
  
  // 1초 대기
  delay(1000);
}

void receiveData(int byteCount)
{
  while (Wire.available()) {  // 수신된 데이터의 바이트 수를 반환. Serial.available()과 기능이 같다. 슬레이브 모드에서는 Wire.onReceive()에 지정한 수신 이벤트 처리 함수 내에서 사용될 수 있다.
    number = Wire.read(); // 수신된 데이터를 읽어온다. Serial.read()와 기능이 같다.
    if (number == 1) {
      Serial.println("1\n");
      rice_limit = 150.0;
    } else if (number == 2) {
      Serial.println("2\n");
      rice_limit = 300.0;
    } else if (number == 3) {
      Serial.println("3\n");     
      rice_limit = 450.0;
    } else if (number == 4) {
      Serial.println("4\n");     
      rice_limit = 600.0;
    } else if (number == 5) {
      Serial.println("5\n");     
      rice_limit = 650.0;
    }
  }
}

void sendData() {
  Wire.write(retMSG, strlen(retMSG)); // 전송될 데이터를 버퍼에 저장하는 함수. 슬레이브 모드에서는 마스터로부터 데이터 요청이 있을 때
                                      // 요청에 대한 데이터를 전송하기 위해 사용된다.
}

void check_rice_quantity()
{
  while (true) {
    // 오프셋 및 스케일이 적용된 측정값 출력 (5회 측정 평균값) 
    rice_load = scale.get_units(5);
    Serial.print("rice_load :\t");
    Serial.print(rice_load, 2);    // 5회 측정 평균값, 소수점 아래 2자리 출력
    Serial.println(" g");
  
    if (rice_load >= rice_limit) {
      strcpy(retMSG, "done");
      break;
    } 

    // 1초 대기
    delay(1000);
  }
  

}
