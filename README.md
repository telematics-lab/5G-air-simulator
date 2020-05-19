
5G-air-simulator
=====================
## _an open source framework to simulate the 5G air interface_
 
---
### Table of Contents:
1. Getting 5G-air-simulator
2. Compiling 5G-air-simulator
3. Running 5G-air-simulator
4. Use Advanced Channels
5. How-to Debugging and Logging
6. Additional Support
---
### 1. Getting 5G-air-simulator
5G-air-simulator is available via Git at [this link][2]. 
To obtain 5G-air-simulator enter into the your prefered folder and write the following syntax:

    $ git clone https://github.com/telematics-lab/5G-air-simulator.git
To synchronize the project repository with the local copy, you can run the pull sub-command. The syntax is as follows:

    $ git pull
###  2. Compiling 5G-air-simulator
First, you need to install make utility and the armadillo library.
On recent Linux systems, you can run:

    $  sudo apt install make libarmadillo-dev
Then you can build 5G-air-simulator with the following command:

	$ cd 5G-air-simulator; make
To clear the project, you can use the following command:

	$ make clean
### 3. Running 5G-air-simulator
Several scenarios have been developed. To run a simple simulation, you can use the following command:

	$ ./5G-air-simulator Simple
For more details about the available scenarios, use

	$ ./5G-air-simulator -h

### 4. Use Advanced Channels
In order to exploit MIMO and mMIMO capabilities of 5G-air-simulator you need to perform additional steps.

- Download [this file][3]
- Extract it into src/channel/propagation-model/FastFadingRealization/
- Open the file src/load-parameters.h
- Comment the directive `#define _channel_simple_` 
- Uncomment `//#define _channel_simple_`
- Re-build 5G-air-simulator

### 5. How-to Debugging and Logging
5G-air-simulator leverages macros and environment variables to allow print debugging and logging without compiling the code each time.
The code must be placed between these macros:  

    DEBUG_LOG_START_1(SIM_ENV_FOO_VAR)
    ...
    //debug code
    ...
    DEBUG_LOG_END
 and each time you want to run that piece of code you just have to create the related environment variable, using the following command:

	$ export SIM_ENV_FOO_VAR=1

This operation can be undone by deleting the same environment variable:

	$ unset SIM_ENV_FOO_VAR

 5G-air-simulator works also with multiple environment variables at the same time (up to 4). 
 For example:
 
    DEBUG_LOG_START_2(SIM_ENV_FOO_VAR, SIM_ENV_FOO_VAR_2)
    ...
    //debug code
    ...
    DEBUG_LOG_END
    
tells the compiler to run the code only if either `SIM_ENV_FOO_VAR` or `SIM_ENV_FOO_VAR_2` have been defined.

Several instances are already present in the source files.

### 6. Additional Support
Please refer to this [web page][1] for additional support.

[1]: https://telematics.poliba.it/5G-air-simulator "5G-air-simulator Official Web Page"
[2]: https://github.com/telematics-lab/5G-air-simulator "5G-air-simulator Official Repository"
[3]: https://telematics.poliba.it/5G-air-simulator-ff "Additional Fast Fading Realizations"

---
© 2020 - TELEMATICS LAB - Politecnico di Bari
