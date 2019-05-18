#pragma once

#include <HardwareSerial.h>
#include <Arduino.h>
#include <BLEServer.h>
#include <BLEService.h>
#include <BLEUtils.h>
#include <forward_list>

#include "usfl.h"
#include "configuration.h"
#include "ByteData.hpp"

struct BTServerCallbacks
{
    virtual void onWriteMode(unsigned int mode);
    virtual void onWriteData(const ByteData &data);
    virtual void onWriteLedCount(unsigned int led_count);
};

class BTServer : protected BLECharacteristicCallbacks, protected BLEServerCallbacks
{
public:
public:
    static BTServer &getInstance()
    {
        static BTServer instance;
        return instance;
    }

public:
    void addListener(BTServerCallbacks *callback)
    {
        _writeDataCallbacks.push_front(callback);
    }

    void removeListener(BTServerCallbacks *callback)
    {
        _writeDataCallbacks.remove(callback);
    }

private:
    void _writeModeNotify(unsigned int mode)
    {
        for (auto &callback : _writeDataCallbacks)
            callback->onWriteMode(mode);
    }

    void _writeDataNotify(const ByteData &data)
    {
        for (auto &callback : _writeDataCallbacks)
            callback->onWriteData(data);
    }

    void _writeLedCountNotify(unsigned int led_count)
    {
        for (auto &callback : _writeDataCallbacks)
            callback->onWriteLedCount(led_count);
    }

private:
    std::forward_list<BTServerCallbacks *> _writeDataCallbacks;

private:
    BTServer()
    {
        BLEDevice::init(DEVICE_NAME);
        bleserver = BLEDevice::createServer();
        bleserver->setCallbacks(this);

        BLEService *service = bleserver->createService(SERVICE_UUID);

        service->createCharacteristic(VERSION_CHAR_UUID,
                                      BLECharacteristic::PROPERTY_READ)
            ->setValue(PROG_VERSION);

        led_count_char = service->createCharacteristic(LED_COUNT_CHAR_UUID,
                                                       BLECharacteristic::PROPERTY_READ |
                                                           BLECharacteristic::PROPERTY_WRITE);
        led_count_char->setCallbacks(this);

        mode_char = service->createCharacteristic(MODE_CHAR_UUID,
                                                  BLECharacteristic::PROPERTY_READ |
                                                      BLECharacteristic::PROPERTY_WRITE);
        mode_char->setCallbacks(this);

        data_char = service->createCharacteristic(DATA_CHAR_UUID,
                                                  BLECharacteristic::PROPERTY_READ |
                                                      BLECharacteristic::PROPERTY_WRITE);
        data_char->setCallbacks(this);

        service->start();
        _startAdvertising();
    }

    ~BTServer() = default;
    BTServer(const BTServer &) = delete;
    BTServer &operator=(const BTServer &) = delete;

    void _startAdvertising()
    {
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->setScanResponse(true);
        pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
        pAdvertising->setMinPreferred(0x12);
        BLEDevice::startAdvertising();
    }

private:
    BLEServer *bleserver;
    BLECharacteristic *led_count_char;
    BLECharacteristic *mode_char;
    BLECharacteristic *data_char;

protected:
    void onConnect(BLEServer *pServer) override
    {
        Serial.printf("Connected: id = %u\n", pServer->getPeerDevices(true).begin()->first);
        // connectEvent.notify(nullptr);
    }

    void onDisconnect(BLEServer *pServer) override
    {
        Serial.printf("Disconnected: id = %u\n", pServer->getPeerDevices(true).begin()->first);
        // disconnectEvent.notify(nullptr);
    }

    void onRead(BLECharacteristic *pCharacteristic) override
    {
    }

    void onWrite(BLECharacteristic *pCharacteristic) override
    {
        if (pCharacteristic == mode_char)
        {
            _writeModeNotify(*(unsigned int *)pCharacteristic->getData());
            LOG("mode_char");
        }
        else if (pCharacteristic == data_char)
        {
            ByteData data{
                data_char->getData(),
                data_char->getValue().size()};
            _writeDataNotify(data);
            LOG("data_char");
        }
        else if (pCharacteristic == led_count_char)
        {
            _writeLedCountNotify(*(unsigned int *)pCharacteristic->getData());
            LOG("led_count_char");
        }
    }

private:
};
