#include "RadioComm.hpp"
#include "RF24.h"
#include "printf.h"

#include <SPI.h>
/*
RadioComm::RadioComm(const uint8_t _cepin, const uint8_t _cspin) {
  this->radio = &RF24(_cepin, _cspin);
}*/

void RadioComm::begin(const uint64_t _local_address, 
                      const uint64_t _remote_address, 
                      const uint8_t _channel,
                      const uint8_t _power_level) {
  this->local_address = _local_address;
  this->remote_address = _remote_address;
  this->channel = _channel;
  
  this->radio.begin();
  this->radio.setPALevel(_power_level);
  this->radio.enableDynamicPayloads();

  this->radio.openWritingPipe(_remote_address);
  this->radio.openReadingPipe(_channel, _local_address);

  this->radio.startListening();

  this->radio.printDetails();
}

bool RadioComm::read(uint8_t* payload, uint8_t& n) {
  if (this->radio.available()) {
    n = this->radio.getDynamicPayloadSize();
    
    if (n > 0) {
      this->radio.read(payload, n);
      return true;
    }
  }
  return false;
  
}
void RadioComm::send(const uint8_t* payload, const uint8_t n) {
  this->radio.stopListening();
  this->radio.write(payload, n);
  this->radio.startListening();
}

