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

/*
 * These have to be kept in line with HAL version
 */
static constexpr Effect MAX_EFFECT = Effect::TICK;
static constexpr EffectStrength MAX_EFFECT_STRENGTH = EffectStrength::STRONG;

/**
 * Helper to index effects in the mEffects map.
 * from their strength and effect type.
 */
#define EFFECT_INDEX(effect, strength) \
	((uint16_t)effect * (uint16_t)MAX_EFFECT_STRENGTH + (uint16_t)strength)

static constexpr int DEFAULT_EFFECT_ID = EFFECT_INDEX(Effect::CLICK, EffectStrength::MEDIUM);

Vibrator::Vibrator(std::string devpath) :
	mInputDevPath(devpath)
{
	// We just keep the input device open the whole time we're running.
	// Closing / reopening it seems to break things.
	if (mInputDevPath.length() < 2) {
		mIsStub = true;
		return;
	}
	mfd = openInputDev();
	if (mfd < 0) {
		ALOGE("%s() can't open FF input device %s", __func__, mInputDevPath.c_str());
		return;
	}

	// Build a table of effects for each strength of each effect.
	int baseStrength = 0;
	for (uint16_t i = 0; i <= (uint16_t)MAX_EFFECT; i++)
	{
		for (uint16_t j = 0; j <= (uint16_t)MAX_EFFECT_STRENGTH; j++)
		{
			switch((Effect)i) {
				default:
				case Effect::CLICK:
					baseStrength = 0x4000;
					break;
				case Effect::TICK:
					baseStrength = 0x1000;
					break;
				case Effect::DOUBLE_CLICK:
					baseStrength = 0x3000;
					break;
			}
			ALOGV("%s() uploading effect %d, strength %d, magnitude = 0x%x", __func__, i, j, baseStrength + j * 0x1000);
			// 0x8000 is about the max for qcom_spmi_haptics.
			mEffects[EFFECT_INDEX(i, j)] = FF_EFFECT((uint16_t)(baseStrength + j * 0x800));

			uploadEffectToKernel(&mEffects[EFFECT_INDEX(i, j)]);
		}
	}
}

int Vibrator::openInputDev() {
	if (mIsStub)
		return 0;
	return open(mInputDevPath.c_str(), O_RDWR|O_CLOEXEC);
}

void Vibrator::uploadEffectToKernel(struct ff_effect* effect) {
	int ret;
	if (mIsStub)
		return;
	
	ret = ioctl(mfd, EVIOCSFF, effect);
	if (ret < 0) {
		ALOGE("%s() Couldn't upload rumble effect to device %s, ret = %d", __func__, mInputDevPath.c_str(), ret);
	}
}

void Vibrator::deleteEffectFromKernel(struct ff_effect* effect) { // Unload rumble effect
	int ret;
	if (mIsStub)
		return;
	
	ret = ioctl(mfd, EVIOCRMFF, effect);
	if (ret < 0) {
		ALOGE("%s() Failed to remove rumble effect from device %s, ret = %d", __func__, mInputDevPath.c_str(), ret);
		return;
	}
	effect->id = -1;
}

// Methods from ::android::hardware::vibrator::V1_1::IVibrator follow.
Return<Status> Vibrator::on(uint32_t timeoutMs) {
	struct ff_effect* effect;
	if (mIsStub)
		return Status::OK;
	// If the active effect is set, use it instead of the default
	if (mActiveEffectId < 0) {
		effect = &mEffects[DEFAULT_EFFECT_ID];
	} else {
		effect = &mEffects[mActiveEffectId];
	}

	ALOGV("%s() mActiveEffectId = %d, timeoutMs = %d, effect.id = %d, magnitude = %d", __func__, mActiveEffectId, timeoutMs, effect->id, effect->u.rumble.strong_magnitude);
	struct input_event play;
	int ret;

	play.type = EV_FF;
	play.code = effect->id;
	play.value = 1;

	ret = write(mfd, (const void*) &play, sizeof(play));
	if (ret != sizeof(play)) {
		ALOGE("%s() failed to fully write play event to input device: %d", __func__, errno);
		return Status::UNKNOWN_ERROR;
	}

	usleep(timeoutMs * 1000);

	off();

	return Status::OK;
}

Return<Status> Vibrator::off()  {
	ALOGV("%s", __func__);
	struct input_event stop;
	struct ff_effect* effect;
	if (mIsStub)
		return Status::OK;
	// If the active effect is set, use it instead of the default
	if (mActiveEffectId < 0) {
		ALOGV("%s() no active effect, stopping default", __func__);
		effect = &mEffects[DEFAULT_EFFECT_ID];
	} else {
		effect = &mEffects[mActiveEffectId];
	}

	stop.type = EV_FF;
	stop.code = effect->id;
	stop.value = 0;
	if (write(mfd, (const void*) &stop, sizeof(stop)) != sizeof(stop)) {
		ALOGE("%s() failed to fully write stop event to input device", __func__);
		return Status::UNKNOWN_ERROR;
	}

	return Status::OK;
}

Return<bool> Vibrator::supportsAmplitudeControl()  {
	ALOGV("%s", __func__);
	return false;
}

Return<Status> Vibrator::setAmplitude(uint8_t amplitude) {
	ALOGV("%s", __func__);

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
	ALOGV("%s, ", __func__);
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
	uint32_t timeMs = 9;
	bool doubleClick = effect == Effect::DOUBLE_CLICK;

	ALOGV("%s() effect = %d, strength = %d", __func__, effect, (int)strength);

	if (effect > MAX_EFFECT || mIsStub){
		_hidl_cb(Status::UNSUPPORTED_OPERATION, 0);
		return Void();
	}

	mActiveEffectId = EFFECT_INDEX(effect, strength);

	// Play the effect
	on(timeMs);
	/*
	 * Android calls off() for us, so
	 * only call it if we're doing a double click
	 */
	if (doubleClick) {
		timeMs += 59;
		usleep(50 * 1000);
		on(timeMs);
	}
	mActiveEffectId = -1;

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