/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */
#pragma once
#include <corecrt.h>

#ifdef WIN32

#else

#endif

namespace cpkg::info
{
  time_t get_last_update_time();
  bool set_last_update_time();
  bool is_update_available();
}

