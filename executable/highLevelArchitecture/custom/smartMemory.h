#pragma once

#include "consoleLogger.h"
#include "consoleLogger.cpp"
#include "highLevelMemory.cpp"
#include "pointerTool.h"
#include "constDefinitions.h"
#include "eepromBasicConfig.h"
#include "networkUtils.h"
#include "platform.h"
#include "http.h"
#include "webService.h"
#include "fetch.h"

#include "jsonParser.h"
#include "localStorage.h"
#include "cyclicBinary.cpp"
#include "operaotors.h"


#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>

class memory{
	public:

		// std::vector<char[20]>memoryStringAddress;

		struct memoryElement{
			uint32_t address=-1;
			uint16_t stringAddress=-1;      //^ to be used with vector lookup table
			
		};
}