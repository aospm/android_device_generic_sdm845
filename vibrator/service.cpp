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
#define LOG_TAG "android.hardware.vibrator@1.2-service.sdm845"

#include <log/log.h>

#include <android/hardware/vibrator/1.1/IVibrator.h>
#include <hidl/HidlSupport.h>
#include <hidl/HidlTransportSupport.h>
#include <utils/Errors.h>
#include <utils/StrongPointer.h>

#include <linux/input.h>

#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>

#include "Vibrator.h"

#define DEV_INPUT_DIR "/dev/input"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::hardware::vibrator::V1_1::IVibrator;
using android::hardware::vibrator::V1_1::implementation::Vibrator;
using namespace android;

// From : https://github.com/ubports/hfd-service/blob/xenial/src/vibrator-ff.cpp#L46
static bool inputDevSupportsFF(std::string devname) {
    int ret;
	unsigned char features[1 + FF_MAX/8/sizeof(unsigned char)];
	int tempFd = open(devname.c_str(), O_RDWR);
	int request = EVIOCGBIT(EV_FF, sizeof(features)*sizeof(unsigned char));
	bool supported = false;

    ALOGE("CA:: %s: Testing device: %s", __func__, devname.c_str());

	if ((ret = ioctl(tempFd, request, &features)) < 0) {
        ALOGE("CA:: %s: ioctl() failed with errno = %d", __func__, ret);
		supported = false;
    }
	if (testBit(FF_RUMBLE, features)) {
		supported = true;
	}

	close(tempFd);
	return supported;
}

// Returns path event path to to the first input device
// which supports forcefeedback.
// Adapted from getevent.c (I know this is bad)
static std::string findFirstFFDevice() {
    char devname[PATH_MAX];
    char *filename;
    struct dirent *de;
    DIR *dir = opendir(DEV_INPUT_DIR);
    if (dir == NULL)
        return NULL;
    strcpy(devname, DEV_INPUT_DIR);
    filename = devname + strlen(devname);
    *filename++ = '/';
    while((de = readdir(dir))) {
        if (std::string(de->d_name).find("event") == std::string::npos)
            continue;
        strcpy(filename, de->d_name);
        // At this point devname is the full path to the dirent
        std::string devpath(devname);
        if (inputDevSupportsFF(devpath)) {
            closedir(dir);
            ALOGI("CA:: %s(): Found haptics device: %s", __func__, devpath.c_str());
            return devpath;
        }
    }
    closedir(dir);
    return "";
}


status_t registerVibratorService() {
    std::string hapticsDev = findFirstFFDevice();
    if (hapticsDev.length() < 2)
        return UNKNOWN_ERROR;
    sp<IVibrator> vibrator = new Vibrator(hapticsDev);
    ALOGI("CA:: %s", __func__);

    return vibrator->registerAsService();
}

int main() {
    configureRpcThreadpool(1, true);
    status_t status = registerVibratorService();

    if (status != OK) {
        return status;
    }

    joinRpcThreadpool();
}
