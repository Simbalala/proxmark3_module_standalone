# Proxmark3 Standalone Modes

The most curious concept of Proxmark3 is standalone mode.

If you can power your device from battery, you can run small modules from the PM3 itself, without needing a computer running the PM3 client.

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

The final steps is to 
- force recompilation of all code.  ```make clean```
- compile ```make -j```
- flash your device
- connect to your device
- press button long time to trigger ledshow and enter your new standalone mode
- if connected with usb / fpc ,  you can also see debug statements from your device in standalone mode. Useful for debugging :)

When compiling you will see a header showing what configurations your project compiled with.
Make sure it says your standalone mode name.

# Writing your own

See: https://github.com/RfidResearchGroup/proxmark3/blob/master/armsrc/Standalone/readme.md

#Additional information

Some members of our community have produced text and video walk-throughs of these modules:

@Hacker warehouse did a nice video on youtube.
Troy also has a nice image of the leds for LF_SAMYRUN.
TinkerSec blogged about HID Prox badge cloning.
quentynblog made a video about the HF_TCPRST (IKEA Rothult) standalone module.


