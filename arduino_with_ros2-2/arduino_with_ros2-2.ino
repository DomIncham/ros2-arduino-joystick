const int motorAPWM = 6;
const int motorADIR = 14;

const int motorBPWM = 4;
const int motorBDIR = 5;

const int motorCPWM = 12;
const int motorCDIR = 30;

const int motorDPWM = 13;
const int motorDDIR = 32;
int vel = 50;

struct JoyData {
    int Rotate;
    int x;
    int y;
    int checksum;
} joyData;

const int DATA_SIZE = sizeof(JoyData);
const int threshold = 10; // ค่าความไวในการตอบสนอง
const int range = 150;

unsigned long lastReceivedTime = 0; // เวลาที่รับข้อมูลล่าสุด
const unsigned long timeout = 1000; // เวลาหมดอายุในมิลลิวินาที

void setup() {
    Serial.begin(115200);
    pinMode(motorAPWM, OUTPUT);
    pinMode(motorADIR, OUTPUT);
    pinMode(motorBPWM, OUTPUT);
    pinMode(motorBDIR, OUTPUT);
    pinMode(motorCPWM, OUTPUT);
    pinMode(motorCDIR, OUTPUT);
    pinMode(motorDPWM, OUTPUT);
    pinMode(motorDDIR, OUTPUT);
}

void loop() {
    if (Serial.available() >= sizeof(JoyData)) {
        Serial.readBytes((char*)&joyData, sizeof(JoyData));
        
        // Calculate checksum
        int calculated_checksum = (joyData.Rotate + joyData.x + joyData.y);


          // Update lastReceivedTime เมื่อรับข้อมูลใหม่
        lastReceivedTime = millis(); // อัพเดทเวลาทุกครั้งที่รับข้อมูล
     
        
        // Verify checksum
        if (calculated_checksum == joyData.checksum) {
            // Proceed with mecanumDrive
            mecanumDrive(joyData.x, joyData.y, joyData.Rotate);
        } else {
            Serial.println("Checksum mismatch!");
            Serial.print("cal_checksum = ");
            Serial.print(calculated_checksum);
            Serial.print(" joyData.checksum = ");
            Serial.println(joyData.checksum);
            stopMotors();
        }
    }
    
    // Timeout to stop motors if no data received
    if (millis() - lastReceivedTime > timeout) {
        stopMotors();
    }
}

void mecanumDrive(int x, int y, int rotation) {
    // ตรวจสอบว่าค่าทั้งหมดอยู่ภายใต้ threshold หรือไม่
    if (abs(x) <= threshold && abs(y) <= threshold && abs(rotation) <= threshold) {
        stopMotors();
        return;
    }

    int motorASpeed, motorBSpeed, motorCSpeed, motorDSpeed;

    // คำนวณความเร็วของมอเตอร์แต่ละตัว สมการแมคคานัมครับหนุ่ม
    motorASpeed = y - x - rotation;  // Front-right
    motorBSpeed = y + x + rotation;  // Front-left
    motorCSpeed = y + x - rotation;  // Rear-left
    motorDSpeed = y - x + rotation;  // Rear-right


    // Normalize speeds to be within range
    motorASpeed = constrain(motorASpeed, -range, range);
    motorBSpeed = constrain(motorBSpeed, -range, range);
    motorCSpeed = constrain(motorCSpeed, -range, range);
    motorDSpeed = constrain(motorDSpeed, -range, range);

     Serial.print(motorASpeed);
    Serial.print("  ");
    Serial.print(motorBSpeed);
    Serial.print("  ");
    Serial.print(motorCSpeed);
    Serial.print("  ");
    Serial.print(motorDSpeed);
    Serial.println("  ");

    // ตั้งค่าความเร็วและทิศทางของมอเตอร์
    setMotorSpeed(motorAPWM, motorADIR, motorASpeed);
    setMotorSpeed(motorBPWM, motorBDIR, motorBSpeed);
    setMotorSpeed(motorCPWM, motorCDIR, motorCSpeed);
    setMotorSpeed(motorDPWM, motorDDIR, motorDSpeed);
}

void setMotorSpeed(int pwmPin, int dirPin, int speed) {
    if (speed >= 0) {
        digitalWrite(dirPin, HIGH);
    } else {
        digitalWrite(dirPin, LOW);
        speed = -speed;  // เปลี่ยนค่าเป็นบวกสำหรับ PWM
    }
    analogWrite(pwmPin, speed);
}

void stopMotors() {
    analogWrite(motorAPWM, 0);
    analogWrite(motorBPWM, 0);
    analogWrite(motorCPWM, 0);
    analogWrite(motorDPWM, 0);
}








/*// ฟังก์ชันการเคลื่อนที่เฉพาะทางสามารถเก็บไว้สำหรับการทดสอบหรือตัวเลือกเพิ่มเติม
void moveForward(int speed) {
    digitalWrite(motorADIR, HIGH);
    analogWrite(motorAPWM, speed);
    
    digitalWrite(motorBDIR, HIGH);
    analogWrite(motorBPWM, speed);
    
    digitalWrite(motorCDIR, HIGH);
    analogWrite(motorCPWM, speed);
    
    digitalWrite(motorDDIR, HIGH);
    analogWrite(motorDPWM, speed);
}

void moveBackward(int speed) {
    digitalWrite(motorADIR, LOW);
    analogWrite(motorAPWM, speed);
    
    digitalWrite(motorBDIR, LOW);
    analogWrite(motorBPWM, speed);
    
    digitalWrite(motorCDIR, LOW);
    analogWrite(motorCPWM, speed);
    
    digitalWrite(motorDDIR, LOW);
    analogWrite(motorDPWM, speed);
}

void rotateRight(int speed) {
    digitalWrite(motorADIR, HIGH);
    analogWrite(motorAPWM, speed);
    
    digitalWrite(motorBDIR, LOW);
    analogWrite(motorBPWM, speed);
    
    digitalWrite(motorCDIR, HIGH);
    analogWrite(motorCPWM, speed);
    
    digitalWrite(motorDDIR, LOW);
    analogWrite(motorDPWM, speed);
}

void rotateLeft(int speed) {
    digitalWrite(motorADIR, LOW);
    analogWrite(motorAPWM, speed);
    
    digitalWrite(motorBDIR, HIGH);
    analogWrite(motorBPWM, speed);
    
    digitalWrite(motorCDIR, LOW);
    analogWrite(motorCPWM, speed);
    
    digitalWrite(motorDDIR, HIGH);
    analogWrite(motorDPWM, speed);
}

void Right(int speed) {
    digitalWrite(motorADIR, HIGH);
    analogWrite(motorAPWM, speed);
    
    digitalWrite(motorBDIR, LOW);
    analogWrite(motorBPWM, speed);
    
    digitalWrite(motorCDIR, LOW);
    analogWrite(motorCPWM, speed);
    
    digitalWrite(motorDDIR, HIGH);
    analogWrite(motorDPWM, speed);
}

void Left(int speed) {
    digitalWrite(motorADIR, LOW);
    analogWrite(motorAPWM, speed);
    
    digitalWrite(motorBDIR, HIGH);
    analogWrite(motorBPWM, speed);
    
    digitalWrite(motorCDIR, HIGH);
    analogWrite(motorCPWM, speed);
    
    digitalWrite(motorDDIR, LOW);
    analogWrite(motorDPWM, speed);
}

void ForwardRight(int speed) {
    digitalWrite(motorADIR, HIGH);
    analogWrite(motorAPWM, speed);
    
    analogWrite(motorBPWM, 0);
    
    analogWrite(motorCPWM, 0);
    
    digitalWrite(motorDDIR, HIGH);
    analogWrite(motorDPWM, speed);
}

void ForwardLeft(int speed) {
    analogWrite(motorAPWM, 0);
    
    digitalWrite(motorBDIR, HIGH);
    analogWrite(motorBPWM, speed);
    
    digitalWrite(motorCDIR, HIGH);
    analogWrite(motorCPWM, speed);
    
    analogWrite(motorDPWM, 0);
}

void BackwardRight(int speed) {
    digitalWrite(motorADIR, LOW);
    analogWrite(motorAPWM, speed);
    
    analogWrite(motorBPWM, 0);
    
    analogWrite(motorCPWM, 0);
    
    digitalWrite(motorDDIR, LOW);
    analogWrite(motorDPWM, speed);
}

void BackwardLeft(int speed) {
    analogWrite(motorAPWM, 0);
    
    digitalWrite(motorBDIR, LOW);
    analogWrite(motorBPWM, speed);
    
    digitalWrite(motorCDIR, LOW);
    analogWrite(motorCPWM, speed);
    
    analogWrite(motorDPWM, 0);
}

void stopMotors() {
    analogWrite(motorAPWM, 0);
    analogWrite(motorBPWM, 0);
    analogWrite(motorCPWM, 0);
    analogWrite(motorDPWM, 0);
}*/
