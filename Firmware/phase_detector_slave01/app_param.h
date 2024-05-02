#ifndef __APP_PARAM_H__
#define __APP_PARAM_H__

#ifndef FIRMWARE_VERSION
#define FIRMWARE_VERSION    102
#endif

#ifndef HARDWARE_VERSION
#define HARDWARE_VERSION    100
#endif


#define APP_INFO    "\x1B[33m\r\n"\
                    "===============================================================\r\n"\
                    " __  __    _    _   _ ____             \r\n"\               
                    "|  \\/  |  / \\  | \\ | |  _ \\  _____   _ _  ___ ___  ___ \r\n"\
                    "| |\\/| | / _ \\ |  \\| | | | |/ _ \\ \\ / / |/ __/ _ \\/ __|\r\n"\
                    "| |  | |/ ___ \\| |\\  | |_| |  __/\\ V /| | (_|  __/\\__ \r\n"\
                    "|_|  |_/_/   \\_\\_| \\_|____/ \\___| \\_/ |_|\\___\\___||___/\r\n"\

#define APP_INFO_FM "===============================================================\r\n"\
                    "Ver: %d, %d  Author: Kyuubi  Build: %s, %s  Mem: %d\r\n"\
                    "===============================================================\x1B[0m\r\n"


#endif