#ifndef CORE_H
#define CORE_H

#define DEBUG_MODE

#include "nhntxtcol.h"

#ifdef DEBUG_MODE
#define LOG_INFO(...) nhntxtcol::TextColor::WHT(__VA_ARGS__)
#define LOG_TRACE(...) nhntxtcol::TextColor::GRN(__VA_ARGS__)
#define LOG_WARN(...) nhntxtcol::TextColor::YLW(__VA_ARGS__)
#define LOG_ERROR(...) nhntxtcol::TextColor::RED(__VA_ARGS__)
#else
#define LOG_INFO(...) ;
#define LOG_TRACE(...) ;
#define LOG_WARN(...) ;
#define LOG_ERROR(...) ;
#endif

#endif