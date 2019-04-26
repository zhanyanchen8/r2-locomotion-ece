#include <Servo.h>
#include <R2Protocol.h>

// FSM Protocol decoder
uint8_t fsm_buffer[256];
r2pf_t fsm;

// Left Motor
#define LEFT_SPEED_READ_PIN A0
#define LEFT_SPEED_SET_PIN 11
#define LEFT_CW_PIN 10
#define LEFT_CCW_PIN 9

// Right Motor
#define RIGHT_SPEED_READ_PIN A1
#define RIGHT_SPEED_SET_PIN 6
#define RIGHT_CW_PIN 5
#define RIGHT_CCW_PIN 4

// Head Motor
Servo head_servo;
#define HEAD_PWM_PIN 3
unsigned long head_end_time = 0;

void setup() {
  Serial.begin(115200);

  fsm = r2pf_init(fsm_buffer, 256);
  
  pinMode(LEFT_SPEED_READ_PIN, INPUT);
  pinMode(LEFT_SPEED_SET_PIN, OUTPUT);
  pinMode(LEFT_CW_PIN, OUTPUT);
  pinMode(LEFT_CCW_PIN, OUTPUT);
  digitalWrite(LEFT_CW_PIN, HIGH);
  digitalWrite(LEFT_CCW_PIN, LOW);

  pinMode(RIGHT_SPEED_READ_PIN, INPUT);
  pinMode(RIGHT_SPEED_SET_PIN, OUTPUT);
  pinMode(RIGHT_CW_PIN, OUTPUT);
  pinMode(RIGHT_CCW_PIN, OUTPUT);
  digitalWrite(RIGHT_CW_PIN, LOW);
  digitalWrite(RIGHT_CCW_PIN, HIGH);

  analogWrite(LEFT_SPEED_SET_PIN, 25); // x/255*100% PWM
  analogWrite(RIGHT_SPEED_SET_PIN, 25); // x/255*100% PWM

  head_servo.attach(HEAD_PWM_PIN);
}

uint8_t send_buffer[256];
void send(char type[5], const uint8_t* data, uint32_t data_len) {
  uint32_t written = r2p_encode(type, data, data_len, send_buffer, 256);
  Serial.write(send_buffer, written);
}

void set_left_motor(byte dir, byte speed) {
  if (dir == 0) {
    digitalWrite(LEFT_CW_PIN, LOW);
    digitalWrite(LEFT_CCW_PIN, HIGH);
  }
  else {
    digitalWrite(LEFT_CW_PIN, HIGH);
    digitalWrite(LEFT_CCW_PIN, LOW);
  }
  analogWrite(LEFT_SPEED_SET_PIN, speed);
}

void set_right_motor(byte dir, byte speed) {
  if (dir == 0) {
    digitalWrite(RIGHT_CW_PIN,HIGH);
    digitalWrite(RIGHT_CCW_PIN,LOW);
  }
  else {
    digitalWrite(RIGHT_CW_PIN, LOW);
    digitalWrite(RIGHT_CCW_PIN, HIGH);
  }
  analogWrite(RIGHT_SPEED_SET_PIN, speed);
}

void set_head_motor(byte dir) {
  head_servo.write(dir);
}

void loop() {
  unsigned long now = millis();

  while (Serial.available() > 0) {
    uint8_t b = Serial.read();
    r2pf_read(&fsm, b);
    if (fsm.done) {
      if (strncmp("WHO", fsm.type, 3) == 0) {
        send("WHO\0", reinterpret_cast<const uint8_t*>("HEADLOCO"), 8);
      }
      else if (strncmp("PING", fsm.type, 4) == 0) {
        send("PONG", 0, 0);
      }
      else if (strncmp("LM", fsm.type, 2) == 0 && fsm.data_len == 2) {
        set_left_motor(fsm.data[0], fsm.data[1]);
      }
      else if (strncmp("RM", fsm.type, 2) == 0 && fsm.data_len == 2) {
        set_right_motor(fsm.data[0], fsm.data[1]);
      }
      else if (strncmp("BM", fsm.type, 2) == 0 && fsm.data_len == 4) {
        set_left_motor(fsm.data[0], fsm.data[1]);
        set_right_motor(fsm.data[2], fsm.data[3]);
      }
      else if (strncmp("STOP", fsm.type, 2) == 0) {
        set_left_motor(0, 0);
        set_right_motor(0, 0);
      }
      else if (strncmp("HM", fsm.type, 2) == 0 && fsm.data_len == 3) {
        set_head_motor(fsm.data[0]);
        head_end_time = now + ((fsm.data[1] << 8) | fsm.data[2]);
      }
      fsm.done = 0;
    }
  }

  if (now > head_end_time || head_end_time - now > 0xffff) {
    set_head_motor(90);
  }
}
