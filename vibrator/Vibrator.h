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
#ifndef ANDROID_HARDWARE_VIBRATOR_V1_1_VIBRATOR_H
#define ANDROID_HARDWARE_VIBRATOR_V1_1_VIBRATOR_H

#include <android/hardware/vibrator/1.1/IVibrator.h>
#include <hidl/Status.h>

#include <linux/input.h>

#include <fstream>
#include <map>

// Borrowed from linuxconsole/utils/bitmaskros.h
/* Number of bits for 1 unsigned char */
#define nBitsPerUchar          (sizeof(unsigned char) * 8)
/* Index=Offset of given bit in 1 unsigned char */
#define bitOffsetInUchar(bit)  ((bit)%nBitsPerUchar)
/* Index=Offset of the unsigned char associated to the bit
 * at the given index=offset
 */
#define ucharIndexForBit(bit)  ((bit)/nBitsPerUchar)
/* Test the bit with given index=offset in an unsigned char array */
#define testBit(bit, array)    ((array[ucharIndexForBit(bit)] >> bitOffsetInUchar(bit)) & 1)

/*
 * Builder for ff_effect struct.
 * We don't bother using weak_magnitude here as most hardware
 * doesn't have any way to differentiate strong / weak haptics
  */
#define FF_EFFECT(rumbleStrenth) 						\
	{													\
		.type = FF_RUMBLE, 								\
		.id = -1, 										\
		.direction = 0, 								\
		.trigger = { .button = 0, .interval = 0, },		\
		.replay = { .length = 0, .delay = 0, },			\
		.u = {											\
			.rumble = {									\
				.strong_magnitude = rumbleStrenth,		\
				.weak_magnitude = 0,					\
			},											\
		},												\
	}

namespace android {
namespace hardware {
namespace vibrator {
namespace V1_1 {
namespace implementation {

class Vibrator : public IVibrator {
public:
	Vibrator(std::string mInputDevPath);

	// Methods from ::android::hardware::vibrator::V1_0::IVibrator follow.
	using Status = ::android::hardware::vibrator::V1_0::Status;
	Return<Status> on(uint32_t timeoutMs)  override;
	Return<Status> off()  override;
	Return<bool> supportsAmplitudeControl() override;
	Return<Status> setAmplitude(uint8_t amplitude) override;

	using EffectStrength = ::android::hardware::vibrator::V1_0::EffectStrength;
	Return<void> perform(V1_0::Effect effect, EffectStrength strength, perform_cb _hidl_cb)
			override;
	Return<void> perform_1_1(Effect_1_1 effect, EffectStrength strength, perform_cb _hidl_cb)
			override;

private:
	~Vibrator();
	void uploadEffectToKernel(struct ff_effect*);
	void deleteEffectFromKernel(struct ff_effect*);
	int openInputDev();
	template <typename T>
	Return<void> performWrapper(T effect, EffectStrength strength, perform_cb _hidl_cb);
	Return<void> performEffect(Effect_1_1 effect, EffectStrength strength, perform_cb _hidl_cb);
	std::string mInputDevPath;
	int mfd;
	int mActiveEffectId;
	// Look up table of effects by type and strength
	std::map<int, struct ff_effect> mEffects;

};

}  // namespace implementation
}  // namespace V1_1
}  // namespace vibrator
}  // namespace hardware
}  // namespace android

class FileDescGuard {
	public:
		FileDescGuard(int fd) : m_fd(fd) {}

		~FileDescGuard() {
			if (close(m_fd) != 0)
			{
				ALOGE("CA:: Failed to close fd %d, errno = %d", m_fd, errno);
			}
		}
	private:
		int m_fd = -1;
};

#endif  // ANDROID_HARDWARE_VIBRATOR_V1_1_VIBRATOR_H
