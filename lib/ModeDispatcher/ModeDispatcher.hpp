#pragma once

#include "usfl.h"
#include "Animator.hpp"
#include "Animation.hpp"
#include "ByteData.hpp"
#include "BTServer.hpp"

#include "AllColor/AllColorAnimation.hpp"
#include "Rainbow/RainbowAnimation.hpp"
#include "RunningLights/RunningLightsAnimation.hpp"

class ModeDispatcher : protected BTServerCallbacks
{
public:
    enum
    {
        ALL_COLOR_ID,
        RAINBOW_ID,
        RUNNING_LIGHTS_ID,

        ANIMATION_COUNT
    };

public:
    ModeDispatcher(Animator *animator) : animator(animator)
    {
        _initAnimations();
        currentMode = ALL_COLOR_ID; //FIXME: change
        BTServer::getInstance().addListener(this);
        xMutex = xSemaphoreCreateMutex();
        animator->setAnimation(animations[currentMode]);
    }

    void setMode(unsigned int mode_id)
    {
        if (mode_id < ANIMATION_COUNT && mode_id != currentMode)
        {
            if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
            {
                if (taskCreated)
                {
                    LOG("RESETTING MODE");
                    currentMode = mode_id;
                    vTaskDelete(taskHandle);
                    animator->setAnimation(animations[currentMode]);
                    taskHandle = NULL;
                    xTaskCreate(taskFunction, "AnimatorTask", 2048, NULL, 0, &taskHandle);
                }
                xSemaphoreGive(xMutex);
            }
            LOGF("Set mode: %u", mode_id);
        }
    }

    void setData(const ByteData &data)
    {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
            animations[currentMode]->configure(data);
            xSemaphoreGive(xMutex);
        }
    }

    void startAnimatorTask(TaskFunction_t task)
    {
        if (!taskCreated)
        {
            taskFunction = task;
            xTaskCreate(task, "AnimatorTask", 2048, NULL, 0, &taskHandle);
            taskCreated = true;
            taskSuspended = false;
        }
    }

    void resumeAnimatorTask()
    {
        if (taskCreated && taskSuspended)
        {
            vTaskResume(taskHandle);
            taskSuspended = true;
        }
    }

    void suspendAnimatorTask()
    {
        if (taskCreated && !taskSuspended)
        {
            vTaskSuspend(taskHandle);
            taskSuspended = true;
        }
    }

    void deleteAnimatorTask()
    {
        if (taskCreated)
        {
            vTaskDelete(taskHandle);
            taskHandle = NULL;
            taskFunction = NULL;
            taskCreated = false;
            taskSuspended = false;
        }
    }

    ~ModeDispatcher()
    {
        BTServer::getInstance().removeListener(this);
    }

protected:
    void onWriteMode(unsigned int id) override
    {
        LOG("");
        setMode(id);
    }

    void onWriteData(const ByteData &data) override
    {
        setData(data);
    }

    void onWriteLedCount(unsigned int led_count) override {}

private:
    Animator *animator;
    Animation *animations[ANIMATION_COUNT];
    uint32_t currentMode;
    TaskHandle_t taskHandle;
    TaskFunction_t taskFunction;
    bool taskCreated = false;
    bool taskSuspended = false;
    xSemaphoreHandle xMutex;

private:
    void _initAnimations()
    {
        animations[ALL_COLOR_ID] = new AllColorAnimation();
        animations[RAINBOW_ID] = new RainbowAnimation();
        animations[RUNNING_LIGHTS_ID] = new RunningLightsAnimation();
    }
};