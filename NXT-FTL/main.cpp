#include <cstdlib>  
#include <iostream>
#include <string>
#include <conio.h>
#include "../NxtLibrary/nxt.h"

using namespace std;

//set up the NXT  
Connection *connection = new Bluetooth();
Sensor *sensor1 = new Touch(IN_1, connection);
Motor *motorB = new Motor(OUT_B, connection);
int main()
{
    try {
        cout << "Try to connect to the NXT" << endl;
        connection->connect(40);
        cout << "Connected" << endl;
        //connection->connect(1500, "192.168.0.1"); //Network connection  
        cout << "Press touch sensor - hit any key to end" << endl;
        while (!_kbhit()) {//hit a key to end  
            if (sensor1->read()) {
                motorB->on(75);
            }
            else {
                motorB->stop();
            }
        }
        connection->disconnect();
    }
    catch (Nxt_exception& e) {
        //some error occurred - print it out  
        cout << e.what() << endl;
        cout << "error code: " << e.error_code() << endl;
        cout << "error type: " << e.error_type() << endl;
        cout << e.who() << endl;
        connection->disconnect();
    }
    return 0;
}