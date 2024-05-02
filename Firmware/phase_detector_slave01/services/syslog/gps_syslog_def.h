#ifndef __GPS_SYSLOG_DEF_H__
#define __GPS_SYSLOG_DEF_H__

/*******************************************************************************
* cause syslog init     0
*
* cause device start    1
*   - value: reset reason
* cause mmi test        2
*   - start: 0
*   - success: 1
*   - failed: 2 
* cause voltage: 3, value: voltage
*   - extra = 0: low voltage
*   - extra = 1: device start by voltage
*   - extra = 2: box off by voltage
*   - extra = 3: sim voltage
*   - extra = 4: power change
*   - extra = 5: device start by speed
*   - extra = 6: device start by power change
* cause power           4
*   - sleep: extra = 0, value: exti
*   - wakeup: extra = 1, value: exti
* cause sensor bma      5
*   - value: 0
* cause secure          6
*   - value: key_state
* cause ota             7
*   - start: 0, extra = 1 when magic valid
*   - stop: 1, extra = 1 when checksum valid
* cause simcom          8
*   - simcom:0, extra = ?
        + 0: init error
        + 1: no response
        + 2: network connect fail
        + 3: ntp update fail
        + 10: network failed by unknown
        + 11: network failed by gatt
        + 12: network failed by ip
        + 20: sms failed by unkown
        + 21: sms failed by sim pin
        + 22: sms failed by reg
*   - sms:1, extra = ?
        + 0: error
        + 1: turn on
        + 2: turn off
        + 3: restore
        + 4: restart
        + 5: search
        + 6: set ota mode
        + 7: set auto
        + 8: set voltage
        + 9: set server
        + 10: set password
        + 11: set phone admin
        + 12: factory reset device
        + 13: sms brand name
        + 14: sms sos
        + 254: syntax invalid
        + 255: wrong phone number
*   - socket:2, extra
        + 0: init error
        + 1: send error
*   - gps:3, extra
        + 0: error
        + 1: success
        + 2: error time
*   - rssi:4, extra = rssi + 255
    - simcard: 5, extra = error code
    - network: 6, extra = error code
    - mqtt: 7, extra = error code
    - gps: 8, extra = error code
    - sms: 9, extra = error code
* cause temperature     9
    - value: temp
* cause os task         10
    - value: cpu, extra: task id
*******************************************************************************/

void printSysLog(uint32_t address, uint32_t ts, uint32_t log);

#endif //__GPS_SYSLOG_DEF_H__