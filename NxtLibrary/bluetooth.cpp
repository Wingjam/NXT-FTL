#include "stdafx.h"
#include "bluetooth.h"
#include "error.h"


Bluetooth::Bluetooth() {
    this->comport = 0;
}

Bluetooth::~Bluetooth() {
    disconnect();
};

void Bluetooth::send(unsigned char *buffer, unsigned int num_bytes) {
    unsigned int i = 0;
    if (handle == INVALID_HANDLE_VALUE) {
        throw Nxt_exception("send", "Bluetooth", BT_INVALID_COM_PORT);
    }
    byte = (BYTE *)malloc(num_bytes * sizeof(BYTE));
    while (i<num_bytes) {
        byte[i] = buffer[i];
        i++;
    }
    if (!WriteFile(handle, byte, num_bytes, &number_bytes, NULL) != 0) {
        free(byte);
        throw Nxt_exception("send", "Bluetooth", BT_ERROR_WRITING_COM_PORT);
    }
    if (number_bytes != num_bytes) {
        free(byte);
        throw Nxt_exception("send", "Bluetooth", BT_ERROR_WRITING_COM_PORT);
    }
    free(byte);
    return;
}

void Bluetooth::connect(unsigned int comport) {
    /***************************************************************************
    *** See http://www.robbayer.com/files//serial-win.pdf for dokumentation  ****
    ***************************************************************************/
    std::stringstream temp;
    temp << comport;
    string port = "\\\\.\\COM" + temp.str();
    handle = CreateFile((LPCWSTR)port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if (handle == INVALID_HANDLE_VALUE) {
        throw Nxt_exception("connect", "Bluetooth", BT_INVALID_COM_PORT);
    }
    this->comport = comport;
    dcb.ByteSize = 8;//sets bit-size
    dcb.BaudRate = CBR_9600;//sets baudrate
                            //Sets timeout for reading - 0 = deactivate

    timeout.ReadIntervalTimeout = 3000;
    timeout.ReadTotalTimeoutConstant = 0;
    timeout.ReadTotalTimeoutMultiplier = 200;

    timeout.WriteTotalTimeoutConstant = 3000;
    timeout.WriteTotalTimeoutMultiplier = 200;
    SetCommTimeouts(handle, &timeout);
    dcb.Parity = 0;
    SetCommState(handle, &dcb);
    flush();
    //Try to read firmware check if a NXT is at the other end
    unsigned char answer[NXT_BUFFER_SIZE];
    unsigned char command[4];
    command[0] = 0x02;  //command length
    command[1] = 0x00;

    command[2] = 0x01;
    command[3] = 0x88;
    try {
        this->send(&command[0], 4);
        this->receive(&answer[0], 9);
    }
    catch (Nxt_exception&) {
        this->disconnect();
        throw Nxt_exception("connect", "Bluetooth", BT_UNABLE_TO_CONNECT);
    }
    return;
}


void Bluetooth::disconnect() {
    if (handle != INVALID_HANDLE_VALUE) {
        CloseHandle(handle);
        handle = INVALID_HANDLE_VALUE;
    }
    this->comport = 0;
    return;
}

void Bluetooth::receive(unsigned char *buffer, unsigned int length) {
    unsigned int i = 0;
    byte = (BYTE *)malloc(length * sizeof(BYTE));
    number_bytes = 0;
    buffer[0] = '\0';
    if (ReadFile(handle, byte, length, &number_bytes, NULL) == 0) {
        free(byte);
        throw Nxt_exception("receive", "Bluetooth", BT_ERROR_READING_COM_PORT);
    }
    //printf("Reveive length %d No of bytes %d\n",length, nrBytes);
    if (number_bytes == 0 && length != 0) {
        free(byte);
        throw Nxt_exception("receive", "Bluetooth", BT_NO_REPLY);
    }
    i = (unsigned int)number_bytes;
    //printf("No bytes received:%d\n", i);
    if (number_bytes<length) {
        free(byte);
        throw Nxt_exception("receive", "Bluetooth", BT_NOT_ENOUGH_BYTES);
    }
    i = 0;
    while (i<number_bytes) {
        buffer[i] = byte[i];
        //printf("Receive: %d\n ",buffer[i]);
        i++;
    }
    buffer[i] = '\0';
    free(byte);
    return;
}

void Bluetooth::flush() {
    PurgeComm(handle, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

Connection_type Bluetooth::get_type() {
    return BT;
}
