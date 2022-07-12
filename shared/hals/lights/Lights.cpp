/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "Lights.h"

#include <android-base/logging.h>
#include <dirent.h>
#include <fstream>

static const std::string BACKLIGHT_DIR = "/sys/class/backlight";
static const std::string LEDS_DIR = "/sys/class/leds";

Light::Light(HwLight hwLight, std::string path)
    : hwLight(hwLight)
    , path(path)
{
}

Backlight::Backlight(HwLight hwLight, std::string path, uint32_t maxBrightness)
    : Light(hwLight, path)
    , maxBrightness(maxBrightness)
{
}

Backlight *Backlight::createBacklight(HwLight hwLight, std::string path)
{
    uint32_t maxBrightness;
    std::ifstream stream(path + "/max_brightness");
    if (auto stream = std::ifstream(path + "/max_brightness")) {
        stream >> maxBrightness;
    } else {
        LOG(ERROR) << "Failed to read `max_brightness` for " << path;
        return nullptr;
    }

    LOG(INFO) << "Creating backlight " << path << " with max brightness " << maxBrightness;

    return new Backlight(hwLight, path, maxBrightness);
}

static int32_t rgbToBrightness(int32_t color)
{
    auto r = (color >> 16) & 0xff;
    auto g = (color >> 8) & 0xff;
    auto b = color & 0xff;
    return (77 * r + 150 * g + 29 * b) >> 8;
}

ndk::ScopedAStatus Backlight::setLightState(const HwLightState &state) const
{
    auto brightness = rgbToBrightness(state.color);
    // Adding half of the max (255/2=127) provides proper rounding while staying in integer mode:
    brightness = (brightness * maxBrightness + 127) / 255;
    if (state.brightnessMode == BrightnessMode::LOW_PERSISTENCE)
        LOG(ERROR) << "TODO: Implement Low Persistence brightness mode";
    LOG(DEBUG) << "Changing backlight to level " << brightness << "/" << maxBrightness;
    if (auto stream = std::ofstream(path + "/brightness")) {
        stream << brightness;
        return ndk::ScopedAStatus::ok();
    } else {
        LOG(ERROR) << "Failed to write `brightness` to " << path;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
}

Led::Led(HwLight hwLight, std::string path, uint32_t maxBrightness)
    : Light(hwLight, path)
    , maxBrightness(maxBrightness)
{
}

Led *Led::createLed(HwLight hwLight, std::string path)
{
    uint32_t maxBrightness;
    std::ifstream stream(path + "/max_brightness");
    if (auto stream = std::ifstream(path + "/max_brightness")) {
        stream >> maxBrightness;
    } else {
        LOG(ERROR) << "Failed to read `max_brightness` for " << path;
        return nullptr;
    }
    std::string color0, color1, color2;
    if (auto stream = std::ifstream(path + "/multi_index")) {
        stream >> color0 >> color1 >> color2;
        if (color0 != "red" || color1 != "green" || color2 != "blue") {
            // We can easily implement support for this by storing the indices of the three channels and
            // shifting in setLightState.
            LOG(ERROR) << "Color indices `" << color0 << " " << color1 << " " << color2 << "` not supported, expected `red green blue`";
            return nullptr;
        }
    } else {
        LOG(ERROR) << "Failed to read `multi_index` for " << path;
        return nullptr;
    }

    LOG(INFO) << "Creating led " << path << " with max brightness " << maxBrightness;

    return new Led(hwLight, path, maxBrightness);
}

ndk::ScopedAStatus Led::setLightState(const HwLightState &state) const
{

    if (auto stream = std::ofstream(path + "/trigger")) {
        switch (state.flashMode) {
        case FlashMode::NONE:
            stream << "none";
            break;
        case FlashMode::HARDWARE:
            // This is probably only called for things like BATTERY/BLUETOOOTH/WIFI,
            // and expects us to set the appropriate `trigger` so that no HAL involvement
            // is necessary afterwards?
            LOG(ERROR) << "Hardware flash mode not yet supported - what trigger to set?";
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
        case FlashMode::TIMED:
            stream << "timer";
            break;
        }
    } else {
        LOG(ERROR) << "Failed to write `trigger` to " << path;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    if (auto stream = std::ofstream(path + "/multi_intensity")) {
        auto r = (state.color >> 16) & 0xff;
        auto g = (state.color >> 8) & 0xff;
        auto b = state.color & 0xff;
        stream << r << " " << g << " " << b;
    } else {
        LOG(ERROR) << "Failed to write `multi_intensity` to " << path;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    LOG(DEBUG) << "Setting global led to max brightness " << maxBrightness;
    if (auto stream = std::ofstream(path + "/brightness")) {
        stream << maxBrightness;
    } else {
        LOG(ERROR) << "Failed to write `brightness` to " << path;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    switch (state.flashMode) {
    case FlashMode::NONE:
    case FlashMode::HARDWARE:
        break;
    case FlashMode::TIMED:
        LOG(DEBUG) << "Setting global led to turn on " << state.flashOnMs << "ms and off " << state.flashOffMs << "ms";
        if (auto stream = std::ofstream(path + "/delay_on")) {
            stream << state.flashOnMs;
        } else {
            LOG(ERROR) << "Failed to write `delay_on` to " << path;
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
        }
        if (auto stream = std::ofstream(path + "/delay_off")) {
            stream << state.flashOffMs;
        } else {
            LOG(ERROR) << "Failed to write `delay_on` to " << path;
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
        }
        break;
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Lights::setLightState(int id, const HwLightState &state)
{
    LOG(DEBUG) << "Lights setting state for id=" << id << " to color " << std::hex << state.color;

    if (id >= lights.size())
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);

    const auto &light = lights[id];
    return light->setLightState(state);
}

ndk::ScopedAStatus Lights::getLights(std::vector<HwLight> *hwLights)
{
    for (const auto &light : lights)
        hwLights->emplace_back(light->hwLight);
    return ndk::ScopedAStatus::ok();
}

Lights::Lights()
{
    int id = 0;
    int ordinal = 0;
    // Cannot use std::filesystem from libc++fs which is not available for vendor modules.
    // Maybe with Android S?
    // for (const auto &backlight : std::filesystem::directory_iterator("/sys/class/backlight"))
    //     if (backlight.is_directory() || backlight.is_symlink())
    //         lights.emplace_back(..);

    if (auto backlights = opendir(BACKLIGHT_DIR.c_str())) {
        while (dirent *ent = readdir(backlights)) {
            if ((ent->d_type == DT_DIR && ent->d_name[0] != '.') || ent->d_type == DT_LNK) {
                std::string backlightPath = BACKLIGHT_DIR + "/" + ent->d_name;
                if (auto backlight = Backlight::createBacklight(
                        HwLight { .id = id++, .ordinal = ordinal++, .type = LightType::BACKLIGHT },
                        backlightPath))
                    lights.emplace_back(backlight);
            }
        }
        closedir(backlights);
    } else {
        LOG(ERROR) << "Failed to open " << BACKLIGHT_DIR;
    }

    LOG(INFO) << "Found " << ordinal << " backlights";

    // Ordinal must be unique per type
    ordinal = 0;

    if (auto leds = opendir(LEDS_DIR.c_str())) {
        while (dirent *ent = readdir(leds)) {
            if ((ent->d_type == DT_DIR && ent->d_name[0] != '.') || (ent->d_type == DT_LNK && strncmp("mmc", ent->d_name, 3))) {
                std::string ledPath = LEDS_DIR + "/" + ent->d_name;
                if (auto led = Led::createLed(
                        // TODO: Also used for ATTENTION and BATTERY - register multiple instances?
                        HwLight { .id = id++, .ordinal = ordinal++, .type = LightType::NOTIFICATIONS },
                        ledPath))
                    lights.emplace_back(led);
            }
        }
        closedir(leds);
    } else {
        LOG(ERROR) << "Failed to open " << LEDS_DIR;
    }

    LOG(INFO) << "Found " << ordinal << " leds";
}
