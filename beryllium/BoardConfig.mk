include device/generic/sdm845/BoardConfigCommon.mk

# Copied from https://github.com/LineageOS/android_device_xiaomi_sdm845-common/blob/lineage-16.0/BoardConfigCommon.mk
# Board Information
TARGET_BOOTLOADER_BOARD_NAME := beryllium
TARGET_BOARD_PLATFORM := beryllium

# Kernel/boot.img Configuration
BOARD_KERNEL_CMDLINE     += androidboot.hardware=beryllium

# Image Configuration
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 3221225472
BOARD_USERDATAIMAGE_PARTITION_SIZE := 57453555712
