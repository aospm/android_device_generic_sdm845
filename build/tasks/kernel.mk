ifneq ($(filter enchilada, $(TARGET_DEVICE)),)

IMAGE_GZ := device/oneplus/enchilada/prebuilt-kernel/android-$(TARGET_KERNEL_USE)/Image.gz
DTB := device/oneplus/enchilada/prebuilt-kernel/android-$(TARGET_KERNEL_USE)/sdm845-oneplus-enchilada.dtb

$(PRODUCT_OUT)/kernel: $(IMAGE_GZ) $(DTB)
	cat $(IMAGE_GZ) $(DTB) > $@

droidcore: $(PRODUCT_OUT)/kernel

endif
