#ifndef wifi_transmit_h
#define wifi_transmit_h

#include "WiFi.h"
#include <string>

class wifi_transmit {
public:
        bool init();
        bool send_message(String message);
		bool flush(String message);
};

#endif