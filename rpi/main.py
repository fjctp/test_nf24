#/usr/bin/env python

from RF24 import *
import RPi.GPIO as GPIO

ADDR_LOCAL = 0xA0A0A0A0E1
ADDR_REMOTE = 0xA0A0A0A0D2
RADIO_CHANNEL = 1

def setup(local_addr, remote_addr, channel = 0):
    radio = RF24(25, 0)

    radio.begin()
    radio.setPALevel(RF24_PA_LOW)
    radio.enableDynamicPayloads()
    radio.printDetails()

    radio.openWritingPipe(remote_addr)
    radio.openReadingPipe(channel,local_addr)

    radio.startListening()
    return radio

def read_payload(radio):
    if (radio.available()):
        n = radio.getDynamicPayloadSize()
        if n > 0:
            payload = radio.read(n)
            return payload
    return None

def send_payload(radio, payload):
    radio.stopListening()
    radio.write(payload)
    radio.startListening()

if __name__ == '__main__':
    local_addr = ADDR_LOCAL
    remote_addr = ADDR_REMOTE
    channel = RADIO_CHANNEL

    radio = setup(local_addr, remote_addr, channel)
    while(True):
        data = read_payload(radio)
        if not data is None:
            print('(II) Got %s with %d characters'%(data.decode("utf-8"), len(data)))
            send_payload(radio, data)
            print('(II) Sent %s'%(data.decode("utf-8")))

