#ifndef _RADIOCOMM_
#define _RADIOCOMM_

#include "RF24.h"

#define MAX_NRF24_PAYLOAD_SIZE 30

class RadioComm {
private:
  uint64_t local_address;
  uint64_t remote_address;
  uint8_t channel;
  RF24 radio;
  
public:
  RadioComm(const uint8_t _cepin, const uint8_t _cspin) : radio(_cepin, _cspin) { }
  void begin(const uint64_t _local_address, 
             const uint64_t _remote_address, 
             const uint8_t _channel = 1,
             const uint8_t _power_level = RF24_PA_LOW);
  
  bool read(uint8_t* _payload, uint8_t& _n);
  void send(const uint8_t* _payload, const uint8_t _n);
};

#endif
