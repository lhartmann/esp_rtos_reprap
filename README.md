ESP_RTOS_ZERO
=============

Starting point project for those wishing to use FreeRTOS on an ESP8266.

Setup Option 1: From Sources
----------------------------

1. Linux OS required. I used a couple symlinks, so windows users are likely out of luck.
2. Get [esp-open-sdk](https://github.com/pfalcon/esp-open-sdk), and compile it as instructed.
3. Make the toolchain path match what is in the setenv file by either:
 * Copying esp-open-sdk's `xtensa-lx106-elf` and `esptool` folders to `/opt/espressif/`.
 * Changing the path inside setenv.
4. Clone [ESP_RTOS_SDK](https://github.com/espressif/ESP8266_RTOS_SDK) to `/opt/espressif/ESP_RTOS_SDK`.
5. If you plan on using C++ then:
 1. Clone [ESP_RTOS_SDK](https://github.com/espressif/ESP8266_RTOS_SDK) to `/opt/espressif/ESP_RTOS_SDK_CPP`.
 2. Enter the folder `ESP_RTOS_SDK_CPP` and run `cppchk.sh` (from the `other_tools` folder) to see what files need patching.
 3. If it seems right (only files under `/opt/espressif/ESP_RTOS_SDK`) then run `cppchk.sh --fix` to patch.
6. Fix `setenv` to point to the SDK path you just created.

Setup Option 2: Preconfigured Package
-------------------------------------

1. Ubuntu 16.04 xenial required. The toolchain was compiled on it, and may fail dependencies elsewhere.
2. Download the tarball [here](http://app.cear.ufpb.br/~lucas.hartmann/2016/08/24/ready-to-use-esp8266-toolchain-for-linux/) or [here](https://app.cear.ufpb.br/owncloud/index.php/s/IkVb6IRLLJCMqSv) (ignore the bad SSL certificate).
3. cd to /opt and untar.

Developing, Compiling and Burning
---------------------------------

1. Enter the project folder esp_rtos_zero.
2. Source the setenv scritpt with: `. setenv`
3. Edit the files at will.
4. Run `make clean && make && ./flash` when you are ready to try.
