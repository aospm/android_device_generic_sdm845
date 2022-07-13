Android Lights AIDL HAL
=

Provides controls for backlight and leds. Now through [AIDL](https://source.android.com/devices/architecture/aidl/aidl-hals).

## Usage

Add the following to your Android device tree:

```Makefile
$(call inherit-product, the/path/to/lights.mk)
```

And set the appropriate user/group in your `ueventd.rc` file for all relevant lights, ie.:

```
/sys/class/backlight/backlight max_brightness    0644 root system
/sys/class/backlight/backlight brightness        0664 system system
```

## Planned improvements

- Extended led `trigger`s
