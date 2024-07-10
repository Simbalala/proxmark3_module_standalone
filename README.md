# Proxmark3 Standalone Modes

The most curious concept of Proxmark3 is standalone mode.

If you can power your device from battery, you can run small modules from the PM3 itself, without needing a computer running the PM3 client.

# Be warned!
> ⚠ It should be pointed out quite early that the Proxmark3 is not for beginners.
>
> If you are not already fairly familiar with electronics, embedded programming, some RF design and ISO standards, this device will probably bring you more frustration than anything else!
>
> Users that do not understand the basic principles behind RFID may have difficulty using the device.

# Rules of Proxmark3

_First rule of proxmarking_   **read the datasheets**

_Second rule of proxmarking_    **search the forum**

_Third rule of proxmarking_       **learning is hard and takes time**


## Update MAKEFILE.HAL

Add your mode to the `Makefile.hal` help and modes list (alphabetically):
```
+==========================================================+
| STANDALONE      | DESCRIPTION                            |
+==========================================================+
...
+----------------------------------------------------------+
| LF_FOO          | My foobar mode will make you coffee    |
+----------------------------------------------------------+

STANDALONE_MODES := LF_... LF_FOO
STANDALONE_MODES += HF_...
```

If your mode is using one of the unique features of the RDV4, add it to the proper list:

```
STANDALONE_MODES_REQ_SMARTCARD :=
STANDALONE_MODES_REQ_FLASH :=
STANDALONE_MODES_REQ_BT :=
```

Please respect alphabetic order!

## Update MAKEFILE.INC

Add your source code files like the following sample in the `Makefile.inc`

```
# WITH_STANDALONE_LF_SKELETON
ifneq (,$(findstring WITH_STANDALONE_LF_SKELETON,$(APP_CFLAGS)))
    SRC_STANDALONE = lf_skeleton.c
endif

# WITH_STANDALONE_LF_FOO
ifneq (,$(findstring WITH_STANDALONE_LF_FOO,$(APP_CFLAGS)))
    SRC_STANDALONE = lf_foo.c
endif
```

Please respect alphabetic order!

## Compiling your standalone mode
^[Top](#top)

Once all this is done, you and others can now easily compile different standalone modes by just selecting one of the standalone modes (list in `Makefile.hal` or ) , e.g.:

- rename  Makefile.platform.sample -> Makefile.platform
- edit the "STANDALONE" row inside Makefile.platform.  You need to uncomment it and add your standalone mode name

Makefile.platform.sample
```
# If you want to use it, copy this file as Makefile.platform and adjust it to your needs
PLATFORM=PM3RDV4
#PLATFORM_EXTRAS=BTADDON
#STANDALONE=LF_SAMYRUN
```
 becomes
 
 Makefile.platform
 ```
# If you want to use it, copy this file as Makefile.platform and adjust it to your needs
PLATFORM=PM3RDV4
#PLATFORM_EXTRAS=BTADDON
STANDALONE=LF_FOO
```

Remember only one can be selected at a time for now.

When compiling you will see a header showing what configurations your project compiled with.
Make sure it says your standalone mode name.

After changing your standalone mode, _don't forget to build and flash the code to the Proxmark3_:

1. `make clean`
2. `make -j`
3. `./pm3-flash-fullimage`

## Writing your own

See: https://github.com/RfidResearchGroup/proxmark3/blob/master/armsrc/Standalone/readme.md

# Source

_Now go and read the_ [Readme](https://github.com/RfidResearchGroup/proxmark3/blob/master/README.md)

_Other valuable source of information is the_   [Official Proxmark3 wiki](https://github.com/Proxmark/proxmark3/wiki) _but beware a lot of things have changed between the official Proxmark3 and this repo_

# Additional information

Some members of our community have produced text and video walk-throughs of these modules:

* [@Hacker warehouse](https://twitter.com/hackerwarehouse) did a nice [video on youtube](https://www.youtube.com/watch?v=W22juSqhJSA).
* Troy also has a nice [image of the leds for LF_SAMYRUN](https://twitter.com/waveguyd/status/1051546698500067328).
* [TinkerSec](https://twitter.com/TinkerSec) [blogged about HID Prox badge cloning](https://www.tinker.sh/badge-cloning-clone-hid-prox-with-proxmark3-rvd4/).
* [quentynblog](https://twitter.com/quentynblog) made [a video about the HF_TCPRST (IKEA Rothult) standalone module](https://www.youtube.com/watch?v=Q08qhJ3TOM8).


