#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define GAS_PIN A0
#define buzzer 7

MFRC522 rfid(SS_PIN, RST_PIN);
Servo myservo;

int scanCount = 0;

String card1 = "52FD8251";
String card2 = "01601810";
String card3 = "C89ABC12";
String card4 = "D456EF78";
void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Scan your RFID tag...");
  
  myservo.attach(8);
  pinMode(GAS_PIN, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
}

void loop() {
  int gaslevel = analogRead(GAS_PIN);
  Serial.print("Gas level: ");
  Serial.println(gaslevel);
  delay(1000);

  if (gaslevel >= 850) {
    Serial.println("Gas leaking detected! Check immediately.");
    myservo.write(0);
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
  }

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {  
    return;
  }

  String cardnumber = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 16) {
      cardnumber += "0";
    }
    cardnumber += String(rfid.uid.uidByte[i], HEX);
  }

  cardnumber.toUpperCase();
  Serial.println("Card UID: " + cardnumber);

  if (cardnumber == card1 || cardnumber == card2 || cardnumber == card3 || cardnumber == card4) {
    scanCount++;

    if (scanCount == 1) {
      myservo.write(90);
      Serial.println("Servo at 90");
    } else if (scanCount == 2) {
      myservo.write(0);
      Serial.println("Servo at 0");
      scanCount = 0;
    }
  }

  rfid.PICC_HaltA();
  delay(1000);
}
