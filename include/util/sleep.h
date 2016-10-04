/**
* LICENSE PLACEHOLDER
*
* @file sleep.h
* @package OpenPST
* @brief multi platform sleep functions
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void sleep(int milliseconds);
