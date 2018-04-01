
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"
#include "RadioComm.hpp"

#define RADIO_LOOP_PERIOD_MILLI_SEC 1.0/500.0 * 1000.0
#define PRINT_LOOP_PERIOD_MILLI_SEC 1.0/1.0 * 1000.0

const uint64_t local_addr = {0xA0A0A0A0D2LL}; // skip the last two char for python
const uint64_t remote_addr = {0xA0A0A0A0E1LL};
const uint8_t channel = 1;

RadioComm radio(4, 5);

long sent_pacakge = 0;
long received_pacakge = 0;

/*****************************************************************/

void print_payload(const char* type, const uint8_t* data, const uint8_t len) {
  /*
  Serial.print(type);
  Serial.print(": ");
  for (uint8_t i=0; i<len; i++)
    Serial.print((char) data[i]);
  Serial.print(", ");
  Serial.println(len);*/
}

void generate_payload(const uint8_t len, uint8_t* data) {
  for(int i = 0; i<len; i++) {
    data[i] = (uint8_t) random(32, 127);
  }
}

/*****************************************************************/

unsigned long last_time_print_loop;
unsigned long last_time_radio_loop;
void setup() {
  Serial.begin(115200);

  radio.begin(local_addr, remote_addr, channel, RF24_PA_LOW);

  last_time_print_loop = millis();
  last_time_radio_loop = millis();
  randomSeed(analogRead(0));
}

uint8_t send_payload[MAX_NRF24_PAYLOAD_SIZE];
uint8_t send_len;
uint8_t received_payload[MAX_NRF24_PAYLOAD_SIZE];
uint8_t received_len;

void loop() {
  if ((millis() - last_time_print_loop) > PRINT_LOOP_PERIOD_MILLI_SEC) {
    Serial.print("R/S: ");
    Serial.print(received_pacakge);
    Serial.print("/");
    Serial.print(sent_pacakge);
    Serial.print(": ");
    Serial.println((double) received_pacakge / (double) sent_pacakge);
    last_time_print_loop = millis();
  }

  
  if ((millis() - last_time_radio_loop) > RADIO_LOOP_PERIOD_MILLI_SEC) {
    send_len = (uint8_t) random(1, MAX_NRF24_PAYLOAD_SIZE);
    generate_payload(send_len, send_payload);
    radio.send(send_payload, send_len);
    sent_pacakge += 1;
    
    print_payload("Sent ", send_payload, send_len);
    
    last_time_radio_loop = millis();
  }

  if (radio.read(received_payload, received_len)) {
    print_payload("Got ", received_payload, received_len);

    if (send_len == received_len) {
      int i = 0;
      while( (send_payload[i] == received_payload[i]) ) {
        i++;
        if (!(i < received_len)) {
           received_pacakge += 1;
           break;
        }
      }
    }
  }
}

