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
    $(LOCAL_PATH)/fstab.ramdisk:$(TARGET_COPY_OUT_RAMDISK)/fstab.axolotl \
    $(LOCAL_PATH)/fstab.ramdisk:$(TARGET_COPY_OUT_VENDOR)/etc/fstab.axolotl \
    device/generic/sdm845/shared/init.sdm845.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/init.axolotl.rc \
    device/generic/sdm845/shared/init.sdm845.usb.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/init.axolotl.usb.rc \
    device/generic/sdm845/shared/key_layout.kl:$(TARGET_COPY_OUT_VENDOR)/usr/keylayout/axolotl.kl

# Audio
PRODUCT_PACKAGES += audio.primary.axolotl

# Lights
PRODUCT_PACKAGES += lights.axolotl

# Dynamic Partitions
PRODUCT_BUILD_SUPER_PARTITION := true
PRODUCT_USE_DYNAMIC_PARTITIONS := true
PRODUCT_USE_DYNAMIC_PARTITION_SIZE := true
