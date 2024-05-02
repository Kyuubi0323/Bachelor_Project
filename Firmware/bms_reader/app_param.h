#ifndef __APP_PARAM_H__
#define __APP_PARAM_H__

#ifndef FIRMWARE_VERSION
#define FIRMWARE_VERSION    101
#endif

#ifndef HARDWARE_VERSION
#define HARDWARE_VERSION    101
#endif


#define BMS_INFO    "\x1B[33m\r\n"\
                    "=============================================================================\r\n"\
                    "        ____  __  __ ____    ____  _____    _    ____  _____ ____      \r\n"\
                    "       | __ )|  \\/  / ___|  |  _ \\| ____|  / \\  |  _ \\| ____|  _ \\     \r\n"\
                    "       |  _ \\| |\\/| \\___ \\  | |_) |  _|   / _ \\ | | | |  _| | |_) |    \r\n" \
                    "       | |_) | |  | |___) | |  _ <| |___ / ___ \\| |_| | |___|  _ <     \r\n"\
                    "       |____/|_|  |_|____/  |_| \\_\\_____/_/   \\_\\____/|_____|_| \\_\\    \r\n"

#define BMS_INFO_FM "=============================================================================\r\n"\
                    "Ver: %d, %d  Author: ManhTH  Build: %s, %s  Mem: %d\r\n"\
                    "=============================================================================\x1B[0m\r\n"


#endif