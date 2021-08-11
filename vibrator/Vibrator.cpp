/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Adapted from Crosshatch downstream:
 * https://github.com/GrapheneOS/device_google_crosshatch/blob/c7f62a539b6cb4685348fbd747605cbfabf1b94d/vibrator/Vibrator.cpp
 */

#define LOG_TAG "VibratorService"

#include <log/log.h>

#include <hardware/hardware.h>
#include <hardware/vibrator.h>
#include <cutils/properties.h>
#include <linux/input.h>

#include "Vibrator.h"

#include <cinttypes>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstring>


namespace android {
namespace hardware {
namespace vibrator {
namespace V1_1 {
namespace implementation {

using Status = ::android::hardware::vibrator::V1_0::Status;
using EffectStrength = ::android::hardware::vibrator::V1_0::EffectStrength;
using Effect = ::android::hardware::vibrator::V1_1::Effect_1_1;

static constexpr int8_t MAX_TRIGGER_LATENCY_MS = 6;

Vibrator::Vibrator(std::string devpath) :
    mInputDevPath(devpath)
{
    std::memset(&mEffect, 0, sizeof(mEffect));
    mEffect.type = FF_RUMBLE;
    mEffect.id = -1;

    mfd = openInputDev();
    if (mfd < 0) {
		ALOGE("CA:: %s() can't open FF input device %s", __func__, mInputDevPath.c_str());
        return;
	}
}

int Vibrator::openInputDev() {
    return open(mInputDevPath.c_str(), O_RDWR|O_CLOEXEC);
}

Return<Status> Vibrator::on(uint32_t timeoutMs, uint32_t playCount) {
    ALOGI("CA:: %s, timeoutMs = %d, playCount = %d, effect.id = %d, strong magnitude = %d, weak magnitude = %d", __func__, timeoutMs, playCount, mEffect.id, mEffect.u.rumble.strong_magnitude, mEffect.u.rumble.weak_magnitude);
    struct input_event play;
    int ret;

    // Upload Rumble effect
    ret = ioctl(mfd, EVIOCSFF, &mEffect);
    if (ret < 0) {
        ALOGE("CA:: %s() Couldn't upload rumble effect to device %s, ret = %d", __func__, mInputDevPath.c_str(), ret);
        return Status::UNKNOWN_ERROR;
	}

    play.type = EV_FF;
    play.code = mEffect.id;
    play.value = playCount;
    ret = write(mfd, (const void*) &play, sizeof(play));
    if (ret != sizeof(play)) {
        ALOGE("CA:: %s() failed to fully write play event to input device: %d", __func__, errno);
        return Status::UNKNOWN_ERROR;
    }

    usleep(timeoutMs * 1000);
    
    return Status::OK;
}

// Methods from ::android::hardware::vibrator::V1_1::IVibrator follow.
Return<Status> Vibrator::on(uint32_t timeoutMs) {
    return(on(timeoutMs, 1));
}

Return<Status> Vibrator::off()  {
    ALOGI("CA:: %s", __func__);
    struct input_event stop;
    int ret;

    stop.type = EV_FF;
    stop.code = mEffect.id;
    stop.value = 0;
    if (write(mfd, (const void*) &stop, sizeof(stop)) != sizeof(stop)) {
        ALOGE("CA:: %s() failed to fully write stop event to input device", __func__);
        return Status::UNKNOWN_ERROR;
    }

    // Unload rumble effect
    ret = ioctl(mfd, EVIOCRMFF, &mEffect);
    if (ret < 0) {
		ALOGE("CA:: %s() Failed to remove rumble effect from device %s, ret = %d", __func__, mInputDevPath.c_str(), ret);
        return Status::UNKNOWN_ERROR;
	}
    mEffect.id = -1;

    return Status::OK;
}

Return<bool> Vibrator::supportsAmplitudeControl()  {
    ALOGI("CA:: %s", __func__);
    return false;
}

Return<Status> Vibrator::setAmplitude(uint8_t amplitude) {
    ALOGI("CA:: %s", __func__);

    if (!amplitude) {
        return Status::BAD_VALUE;
    }

    return Status::OK;
}

Return<void> Vibrator::perform(V1_0::Effect effect, EffectStrength strength,
        perform_cb _hidl_cb) {
    return performWrapper(effect, strength, _hidl_cb);
}

Return<void> Vibrator::perform_1_1(V1_1::Effect_1_1 effect, EffectStrength strength,
        perform_cb _hidl_cb) {
    return performWrapper(effect, strength, _hidl_cb);
}

template <typename T>
Return<void> Vibrator::performWrapper(T effect, EffectStrength strength, perform_cb _hidl_cb) {
    ALOGI("CA:: %s, ", __func__);
    auto validRange = hidl_enum_range<T>();
    if (effect < *validRange.begin() || effect > *std::prev(validRange.end())) {
        _hidl_cb(Status::UNSUPPORTED_OPERATION, 0);
        return Void();
    }
    return performEffect(static_cast<Effect>(effect), strength, _hidl_cb);
}

Return<void> Vibrator::performEffect(Effect effect, EffectStrength strength,
        perform_cb _hidl_cb) {
    Status status = Status::OK;
    uint32_t timeMs;
    int playCount = 1;
    mEffect.u.rumble.weak_magnitude = mEffect.u.rumble.strong_magnitude = 0;

    ALOGI("CA:: %s() effect = %d", __func__, effect);

    switch (effect) {
    case Effect::TICK:
        mEffect.u.rumble.weak_magnitude = 0x5000 + (int)strength * 0x1000;
        timeMs = 9;
        break;
    case Effect::CLICK:
        mEffect.u.rumble.strong_magnitude = 0x500 + (int)strength * 0x1000;
        timeMs = 9;
        break;
    case Effect::DOUBLE_CLICK:
        mEffect.u.rumble.strong_magnitude = 0x4000 + (int)strength * 0x1000;
        playCount = 2;
        timeMs = 130;
        break;
    default:
        _hidl_cb(Status::UNSUPPORTED_OPERATION, 0);
        return Void();
    }

    timeMs += MAX_TRIGGER_LATENCY_MS; // Add expected cold-start latency

    on(timeMs, playCount);
    // Android calls off() for us.
    _hidl_cb(status, timeMs);

    return Void();
}

Vibrator::~Vibrator() {
    close(mfd);
}


} // namespace implementation
}  // namespace V1_1
}  // namespace vibrator
}  // namespace hardware
}  // namespace android