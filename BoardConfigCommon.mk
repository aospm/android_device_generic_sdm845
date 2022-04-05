# Copyright (C) 2013 The Android Open Source Project
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

# Primary Arch
TARGET_ARCH := arm64
TARGET_ARCH_VARIANT := armv8-2a
TARGET_CPU_VARIANT := kryo385
TARGET_CPU_ABI := arm64-v8a

# Secondary Arch
TARGET_2ND_ARCH := arm
TARGET_2ND_ARCH_VARIANT := armv8-2a
TARGET_2ND_CPU_VARIANT := kryo385
TARGET_2ND_CPU_ABI := armeabi-v7a
TARGET_2ND_CPU_ABI2 := armeabi

TARGET_USES_64_BIT_BINDER := true

TARGET_NO_BOOTLOADER := true
TARGET_NO_KERNEL := false
TARGET_NO_RECOVERY := true

# Kernel/boot.img Configuration
BOARD_KERNEL_BASE        := 0x00000000
BOARD_KERNEL_PAGESIZE    := 4096
BOARD_KERNEL_TAGS_OFFSET := 0x00000100
BOARD_RAMDISK_OFFSET     := 0x01000000
BOARD_KERNEL_CMDLINE     := console=ttyMSM0,115200n8 earlycon earlyprintk console=tty0 androidboot.console=ttyMSM0
BOARD_KERNEL_CMDLINE     += androidboot.boot_devices=soc@0/1d84000.ufshc
BOARD_KERNEL_CMDLINE     += firmware_class.path=/vendor/firmware/ init=/init printk.devkmsg=on
BOARD_KERNEL_CMDLINE     += deferred_probe_timeout=30 androidboot.selinux=permissive

# Image Configuration
BOARD_FLASH_BLOCK_SIZE := 262144
BOARD_BOOTIMAGE_PARTITION_SIZE     := 67108864 #64M
BOARD_VENDORIMAGE_PARTITION_SIZE   := 536870912

BOARD_SYSTEMIMAGE_FILE_SYSTEM_TYPE := ext4
TARGET_USERIMAGES_USE_EXT4 := true
TARGET_USERIMAGES_USE_F2FS := true
TARGET_COPY_OUT_VENDOR := vendor
BOARD_VENDORIMAGE_FILE_SYSTEM_TYPE := ext4

# Enable Treble
PRODUCT_FULL_TREBLE := true
BOARD_VNDK_VERSION := current

# Mesa DRM hwcomposer
BOARD_USES_DRM_HWCOMPOSER := true
BOARD_GPU_DRIVERS := freedreno virgl
TARGET_USES_HWC2 := true

# WiFi
WPA_SUPPLICANT_VERSION := VER_0_8_X
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
BOARD_HOSTAPD_DRIVER := NL80211
BOARD_WLAN_DEVICE := qcwcn
BOARD_HOSTAPD_PRIVATE_LIB := lib_driver_cmd_$(BOARD_WLAN_DEVICE)

# BT
BOARD_HAVE_BLUETOOTH := true

# TinyHAL
BOARD_USES_TINYHAL_AUDIO := true

BOARD_SEPOLICY_DIRS += \
    device/generic/sdm845/sepolicy \
    system/bt/vendor_libs/linux/sepolicy

DEVICE_MANIFEST_FILE := device/generic/sdm845/manifest.xml
DEVICE_MATRIX_FILE := device/generic/sdm845/compatibility_matrix.xml

# Enable dex pre-opt to speed up initial boot
ifeq ($(HOST_OS),linux)
  ifeq ($(WITH_DEXPREOPT),)
    WITH_DEXPREOPT := true
    WITH_DEXPREOPT_PIC := true
  endif
endif
