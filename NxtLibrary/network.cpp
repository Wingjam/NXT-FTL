#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include "network.h"
#include <stdlib.h>
#include "error.h"

#define ERROR_STR "Network communication error: "
#define ERROR_STR_END " - Consult www.msdn.microsoft.com"

#define DEFALT_PASSWORD "LEGOCLIENT"
#define MAX_PASSWORD_LENGTH 10

#define TIMEOUT 3000

//******************************************************************************************************
//if you get a compile error with "undefined reference to WSAStartup...." remember to link to libwsock32
//In code blocks: project->build option->linker setting->add - and then locate the libwsock32.a file
//In other compilers you might be able to use #pragma comment(lib, "wsock32.lib") or something simular
//*******************************************************************************************************

unsigned int network_send(SOCKET *this_socket, unsigned char *this_buffer, unsigned int length){
  return send(*this_socket, (char *) this_buffer, length, 0); //Send "Hello server"
}

unsigned int network_connect(SOCKET *this_socket, sockaddr_in *this_sockadd_in){
  return connect( *this_socket, (sockaddr*) this_sockadd_in, sizeof(*this_sockadd_in));
}


Nxt_network::Nxt_network(){
}

Nxt_network::~Nxt_network(){
  disconnect();
};

void Nxt_network::send(unsigned char *buffer, unsigned int num_bytes){
   /*unsigned int i;
   i=0;
   while(i<num_bytes){
     printf("\nSend Buffer[%d]=0x%x\n", i, buffer[i]);
     i++;
   }*/
   int send_bytes = network_send(&this->my_sock, buffer, num_bytes);
   //printf("Send bytes: %d\n", send_bytes);
   if(send_bytes == -1 || (unsigned int) send_bytes != num_bytes ){
      string s;
      s = this->error_to_string(WSAGetLastError());
      throw Nxt_exception("send", "Nxt_network", NETWORK_COM_ERROR, s);
   }
   return;
}

void Nxt_network::connect(unsigned int port, string ip_add){
  unsigned char buffer[MAX_PASSWORD_LENGTH+1];
  unsigned short int pass_len, i;
  unsigned char answer[5];
  struct timeval  timeout;

  timeout.tv_sec = TIMEOUT;
  timeout.tv_usec = 0;
  ws_ver=MAKEWORD(2, 0); // winsock 2.0
  ws_status=WSAStartup(ws_ver, &ws_data);
  if(ws_status != 0){
    string s;
    s = this->error_to_string(WSAGetLastError());
    throw Nxt_exception("connect", "Nxt_network", NETWORK_COM_ERROR, s);
  }
  this->my_sock=socket(AF_INET, SOCK_STREAM, 0);
  if(my_sock == INVALID_SOCKET){
    string s;
    s = this->error_to_string(WSAGetLastError());
    throw Nxt_exception("connect", "Nxt_network", NETWORK_COM_ERROR, s);
  }
  sockaddr_in sock_in;
  sock_in.sin_port=htons(port);
  sock_in.sin_addr.s_addr=inet_addr(ip_add.c_str());
  sock_in.sin_family=AF_INET;
  if(setsockopt(my_sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) == -1){
    string s;
    s = this->error_to_string(WSAGetLastError());
    throw Nxt_exception("connect", "Nxt_network", NETWORK_COM_ERROR, s);
  }
  if(network_connect(&this->my_sock,&sock_in)){
    string s;
    s = this->error_to_string(WSAGetLastError());
    throw Nxt_exception("send", "Nxt_network", NETWORK_COM_ERROR, s);
  }
  return;
}

void Nxt_network::disconnect(){
  Sleep(2000);
  WSACleanup();
  closesocket(my_sock);
  return;
}

void Nxt_network::receive(unsigned char *buffer, unsigned int length){
  int get_bytes = recv(my_sock, (char *) buffer, length, 0);
  if( get_bytes == -1){
    string s;
    s = this->error_to_string(WSAGetLastError());
    throw Nxt_exception("send", "Nxt_network", NETWORK_COM_ERROR, s);
  }
  if(get_bytes == 0 && length !=0){
     //hack to avoid that when the client is thrown off
     //a wrong error message might be thrown if the client pools the server for a message
     if(length >= 5){
       buffer[4] = 0x00;
     }
  }
  //the server will send these bytes if case of an error
  //if( buffer[4] <= (unsigned char) NETWORK_ERROR_HIGH && buffer[4] >= (unsigned char) NETWORK_ERROR_LOW ){
  //   throw Nxt_exception("receive", "Nxt_network", buffer[4] & 0xff);
  //}
  return;
}

//does nothing
void Nxt_network::flush(){
  return;
}

Connection_type Nxt_network::get_type(){
  return NXT_NETWORK;
}

std::string Nxt_network::error_to_string(int err){
        string s;
        char *error_s;
        if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                           NULL,
                           err,
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           (LPTSTR) &error_s,
                           0,
                           NULL)){
          std::stringstream error_string;
          error_string << ERROR_STR << WSAGetLastError() << ERROR_STR_END;
          s = error_string.str();
          return s;
        }
        s = error_s;
        //free(error_s);
        return s;
}
