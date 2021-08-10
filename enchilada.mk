ifndef TARGET_KERNEL_USE
TARGET_KERNEL_USE := mainline
endif

KERNEL_MODS := $(wildcard device/generic/sdm845/prebuilt-kernel/android-$(TARGET_KERNEL_USE)/*.ko)

# Following modules go to vendor partition
# msm.ko is too big (31M) for ramdisk
VENDOR_KERN_MODS := %/qcom_q6v5_adsp.ko %/qcom_q6v5_mss.ko %/qcom_q6v5_pas.ko
BOARD_VENDOR_KERNEL_MODULES := $(filter $(VENDOR_KERN_MODS),$(KERNEL_MODS))

# All other modules go to ramdisk
BOARD_GENERIC_RAMDISK_KERNEL_MODULES := $(filter-out $(VENDOR_KERN_MODS),$(KERNEL_MODS))

# Inherit the full_base and device configurations
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, device/generic/sdm845/enchilada/device.mk)
$(call inherit-product, device/generic/sdm845/device-common.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)

# Product overrides
PRODUCT_NAME := enchilada
PRODUCT_DEVICE := enchilada
PRODUCT_BRAND := AOSP
