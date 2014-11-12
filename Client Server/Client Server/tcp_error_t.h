#pragma once
#include <exception>
typedef enum tcp_error_t{ WSASTARTFAIL, GETADDRFAIL, SOCKETINVALID, CONNECTIONFAIL, SHUTDOWNFAIL, SUCCESS };
typedef unsigned char byte;
std::exception WRONG_FILE_TYPE;