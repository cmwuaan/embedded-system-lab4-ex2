#include <Arduino.h>

#define btnPin A1

byte ledPins[] = {A0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
byte pinCount;

int currentIndex = 0;
bool directionRight = true;
int delayTime = 300;
int score = 0;
int level = 1;

bool btnStatus = false;

void setup()
{
  Serial.begin(9600);
  pinCount = sizeof(ledPins);
  for (int i = 0; i < pinCount; i++)
  {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  pinMode(btnPin, INPUT_PULLUP);
}

void loop()
{
  digitalWrite(ledPins[currentIndex], HIGH);

  for (int i = 0; i < pinCount; i++)
  {
    if (i != currentIndex)
    {
      digitalWrite(ledPins[i], LOW);
    }
  }

  if (digitalRead(btnPin) == LOW && !btnStatus)
  {
    btnStatus = true; // Đã nhấn nút

    if (currentIndex == 5)
    {
      score += level; // Tăng điểm tùy thuộc vào mức độ
      Serial.print("+");
      Serial.println(level);
      if (level == 1)
      {
        delayTime = 300; // Thời gian delay cho level 1
      }
      else if (level == 2)
      {
        delayTime = 150; // Thời gian delay cho level 2
      }
      else if (level == 3)
      {
        delayTime = 50; // Thời gian delay cho level 3
      }
      if (level < 3)
      {
        level++; // Tăng cấp độ lên khi đúng
      }
    }
    else
    {
      score -= level; // Trừ điểm theo mức độ
      Serial.print("-");
      Serial.println(level);
      level = 1;       // Đặt lại level về 1 khi nhấn sai
      delayTime = 300; // Đặt lại thời gian delay mặc định khi nhấn sai
    }

    score = max(0, score); // Đảm bảo điểm không bao giờ nhỏ hơn 0
    Serial.print("Score: ");
    Serial.println(score); // Hiển thị điểm hiện tại trên Serial Monitor
    delay(500);            // Dừng 1 giây

    while (digitalRead(btnPin) == LOW)
    {
      delay(50); // Đợi nút được thả ra trước khi tiếp tục
    }
  }
  else if (digitalRead(btnPin) == HIGH)
  {
    btnStatus = false; // Reset trạng thái của nút
  }

  // Kiểm tra nút nhấn để reset trò chơi
  if (digitalRead(btnPin) == LOW)
  {
    currentIndex = 0;      // Đặt lại chỉ số đèn về 0
    directionRight = true; // Đặt lại hướng di chuyển về phải
    delayTime = 500;       // Đặt lại thời gian delay mặc định
    score = 0;             // Đặt lại điểm về 0
    level = 1;             // Đặt lại mức độ về 1
    delay(500);            // Chờ 1 giây trước khi bắt đầu lại trò chơi
  }

  // Di chuyển đèn theo hướng
  if (directionRight)
  {
    currentIndex++;
    if (currentIndex >= pinCount)
    {
      currentIndex = pinCount - 2;
      directionRight = false;
    }
  }
  else
  {
    currentIndex--;
    if (currentIndex < 0)
    {
      currentIndex = 1;
      directionRight = true;
    }
  }

  delay(delayTime); // Chờ thời gian delay mới
}