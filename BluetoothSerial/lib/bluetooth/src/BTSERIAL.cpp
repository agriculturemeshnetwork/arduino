
#include <BTSERIAL.h>

bool bluetoothSerial::init(String bluetooth_name) {
    SerialBT.begin("ESP32test"); //Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");
}

bool bluetoothSerial::send(String message) {
    
    size_t check = SerialBT.write(message.c_str);
    if ( check > 0)
        return true;
    
    return false;
}

String bluetoothSerial::receive() {
    String message;
    if (SerialBT.available()) {
        message = SerialBT.read();
      
    }
    return message;
}

String bluetoothSerial::send_recieve(String message) {
    SerialBT.write(message.c_str);
    if (SerialBT.available()) {
      message = SerialBT.read();
    }
    return message;
}

String bluetoothSerial::send_recieve_serial() {
    String message
    if (Serial.available()) {
        SerialBT.write(Serial.read());
    }
    if (SerialBT.available()) {
        message = SerialBT.read();
    }
    return message;
}
