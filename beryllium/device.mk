#
# Copyright (C) 2011 The Android Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

PRODUCT_COPY_FILES := \
    $(LOCAL_PATH)/fstab.ramdisk:$(TARGET_COPY_OUT_RAMDISK)/fstab.beryllium \
    $(LOCAL_PATH)/fstab.ramdisk:$(TARGET_COPY_OUT_VENDOR)/etc/fstab.beryllium \
    device/generic/sdm845/etc/audio.sdm845.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio.beryllium.xml \
    device/generic/sdm845/init.common.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/init.beryllium.rc \
    device/generic/sdm845/init.common.usb.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/init.beryllium.usb.rc \
    device/generic/sdm845/common.kl:$(TARGET_COPY_OUT_VENDOR)/usr/keylayout/beryllium.kl

# Build generic Audio HAL
PRODUCT_PACKAGES := audio.primary.beryllium

# Build generic lights HAL
PRODUCT_PACKAGES += lights.beryllium
