#ifndef CORE_H
#define CORE_H

#include "nhntxtcol.h"

#define LOG_INFO(...) nhntxtcol::TextColor::WHT(__VA_ARGS__)
#define LOG_TRACE(...) nhntxtcol::TextColor::GRN(__VA_ARGS__)
#define LOG_WARN(...) nhntxtcol::TextColor::YLW(__VA_ARGS__)
#define LOG_ERROR(...) nhntxtcol::TextColor::RED(__VA_ARGS__)

#endif