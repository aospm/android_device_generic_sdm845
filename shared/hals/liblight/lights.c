/*
 * Copyright (C) 2014, 2017-2018 The  Linux Foundation. All rights reserved.
 * Not a contribution
 * Copyright (C) 2008 The Android Open Source Project
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

#include <log/log.h>
#include <cutils/properties.h>

#include <dirent.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>

#include <sys/ioctl.h>
#include <sys/types.h>

#include <hardware/lights.h>

#define BACKLIGHT_CLASS "/sys/class/backlight"
#define BACKLIGHT_CLASS_FSTRING "/sys/class/backlight/%s"
#define MAX_BRIGHTNESS "max_brightness"
#define BRIGHTNESS "brightness"

typedef struct {
    // Must be the first member so we can cast to/from it
    struct light_device_t lights;

    char brightness_path[PATH_MAX];
    int max_brightness;
} gen_lights_t;

/**
 * device methods
 */

static bool findFirstBacklightDevice(char *filename) {
    struct dirent *de;
    DIR *dir = opendir(BACKLIGHT_CLASS);
    if (dir == NULL)
        return NULL;
    while((de = readdir(dir))) {
        if(de->d_name[0] == '.' &&
           (de->d_name[1] == '\0' ||
            (de->d_name[1] == '.' && de->d_name[2] == '\0')))
            continue;
        strcpy(filename, de->d_name);
        closedir(dir);
        return true;
    }
    closedir(dir);
    return false;
}

/*
 * Read an integer from a file into val.
 * Returns 0 on success, negative errno on failure
 */
static int read_int_file(char *path, int *val) {
    int fd;
    char *end = NULL;
    // largest valid integer we can read is -2147483647 which is 11 digits
    char val_str[16];
    ssize_t amt;
    
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        ALOGE("%s: failed to open %s (%d: %s)\n", __func__, path, errno, strerror(errno));
        return -errno;
    }

    amt = read(fd, val_str, 16);
    if (amt < 0) {
        ALOGE("%s: failed to read from %s (%d: %s)\n", __func__, path, errno, strerror(errno));
        return -errno;
    }

    *val = strtol(val_str, &end, 10);
    if (val_str == end) {
        ALOGE("%s: Couldn't parse '%s' as a number (%d: %s)\n", __func__, val_str, errno, strerror(errno));
        return -errno;
    }
    close(fd);
    return 0;
}

static int write_brightness(gen_lights_t *gl, int value)
{
    int fd = open(gl->brightness_path, O_WRONLY);
    if (fd >= 0) {
        char buffer[20];
        ssize_t amt;
        size_t bytes = snprintf(buffer, sizeof(buffer), "%d\n", value);
        if(bytes < sizeof(buffer)) {
            amt = write(fd, buffer, bytes);
        } else {
            amt = -1;
            errno = -EINVAL;
        }
        close(fd);
        return amt == -1 ? -errno : 0;
    } else {
        ALOGE("write_brightness failed to open %s, fd = %d, errno = %d\n", gl->brightness_path, fd, errno);
        return -errno;
    }
}

static int rgb_to_brightness(struct light_state_t const* state)
{
    int color = state->color & 0x00ffffff;
    return ((77*((color>>16)&0x00ff))
            + (150*((color>>8)&0x00ff)) + (29*(color&0x00ff))) >> 8;
}

static int set_light_backlight(struct light_device_t* dev,
        struct light_state_t const* state)
{
    int brightness;
    gen_lights_t *gl = (gen_lights_t*)dev;
    if(!dev) {
        return -1;
    }

    brightness = rgb_to_brightness(state); // Scale up to 1020
    brightness = (brightness * gl->max_brightness + 127) / 255;
    write_brightness(gl, brightness);

    return 0;
}

/** Close the lights device */
static int close_lights(struct light_device_t *dev)
{
    if (dev) {
        free(dev);
    }
    return 0;
}

/**
 * module methods
 */

/** Open a new instance of a lights device using name */
static int open_lights(const struct hw_module_t* module, char const* name,
        struct hw_device_t** device)
{
    gen_lights_t *dev;
    struct light_device_t *lights;
    int fd;
    char backlight_name[NAME_MAX];
    char max_brightness_path[PATH_MAX];
    int backlight_dev_path_len;

    (void)name;

    dev = malloc(sizeof(gen_lights_t));

    if(!dev)
        return -ENOMEM;

    memset(dev, 0, sizeof(*dev));
    memset(max_brightness_path, 0, PATH_MAX);
    lights = (struct light_device_t*)dev;

    lights->common.tag = HARDWARE_DEVICE_TAG;
    lights->common.version = LIGHTS_DEVICE_API_VERSION_2_0;
    lights->common.module = (struct hw_module_t*)module;
    lights->common.close = (int (*)(struct hw_device_t*))close_lights;
    lights->set_light = set_light_backlight;

    if (!findFirstBacklightDevice(backlight_name)) {
        free(dev);
        ALOGE("Couldn't find a backlight device!");
        return -ENODEV;
    }

    ALOGI("Using backlight device: '" BACKLIGHT_CLASS "/%s'", backlight_name);

    /*
     * Cache the brightness path so we aren't re-allocing and memcpy'ing it
     * everytime the brightness is set. We want to read the max brightness first
     * and then change it to the actual brightness path
     */
    snprintf(dev->brightness_path, PATH_MAX, BACKLIGHT_CLASS_FSTRING "/",
             backlight_name);
    backlight_dev_path_len = strlen(dev->brightness_path);
    strlcpy(dev->brightness_path + backlight_dev_path_len,
            MAX_BRIGHTNESS, strlen(MAX_BRIGHTNESS) + 1);
    
    ALOGI("Max brightness path: %s\n", dev->brightness_path);
    read_int_file(dev->brightness_path, &dev->max_brightness);
    ALOGI("Got max brightness: %d\n", dev->max_brightness);
    // Otherwise we never actually set the max brightness
    if (dev->max_brightness == 1023)
        dev->max_brightness += 4;

    memset(dev->brightness_path + backlight_dev_path_len, 0, strlen(MAX_BRIGHTNESS));
    strlcpy(dev->brightness_path + backlight_dev_path_len, BRIGHTNESS, strlen(BRIGHTNESS) + 1);

    ALOGI("Brightness path: %s\n", dev->brightness_path);

    *device = (struct hw_device_t*)dev;
    return 0;
}

static struct hw_module_methods_t lights_module_methods = {
    .open =  open_lights,
};

/*
 * The lights Module
 */
struct hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = HARDWARE_MODULE_TAG,
    .version_major = 1,
    .version_minor = 0,
    .id = LIGHTS_HARDWARE_MODULE_ID,
    .name = "lights Module",
    .author = "Google, Inc.",
    .methods = &lights_module_methods,
};
