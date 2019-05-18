#pragma once

#include "BTServer.hpp"

class AppStarter : protected BTServerCallbacks
{
public:
    typedef void (* StartFunction)(unsigned int led_count);

    AppStarter()
    {
        BTServer::getInstance().addListener(this);
    }
    
    void setStartFunction(StartFunction startFunction)
    {
        this->startFunction = startFunction;
    }

    ~AppStarter()
    {
        BTServer::getInstance().removeListener(this);
    }
private:
    StartFunction startFunction;
    bool appStarted = false;
protected:
    void onWriteMode(unsigned int mode) {};
    void onWriteData(const ByteData &data) {};
    void onWriteLedCount(unsigned int led_count) 
    {
        if (startFunction != NULL && !appStarted)
        {
            startFunction(led_count);
            appStarted = true;
        }
    };
};
