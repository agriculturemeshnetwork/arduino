#ifndef BTSERIAL_h
#define BTSERIAL_h

#include <BluetoothSerial.h>


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;




class bluetoothSerial {

    private: 
            


    public:
            bool init(String bluetooth_name);
             send(String message);
            bool receive();
            String send_recieve(String );
};

#endif

