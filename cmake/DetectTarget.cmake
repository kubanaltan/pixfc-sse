if(CMAKE_SYSTEM_PROCESSOR MATCHES "[Aa][Rr][Mm]")
	set(PIXFC_TARGET_ARCH "ARM" CACHE STRING "Target arch is ARM")
	message("Target arch is ARM")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^i.86")
	set(PIXFC_TARGET_ARCH "Intel x86")
	message("Target arch is Intel 32-bit")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^x86_64")
	set(PIXFC_TARGET_ARCH "Intel x86_64")
	message("Target arch is Intel 64-bit")
else(CMAKE_SYSTEM_PROCESSOR MATCHES "[Aa][Rr][Mm]")
	message(FATAL_ERROR "Unknown arch " ${CMAKE_SYSTEM_PROCESSOR})
endif(CMAKE_SYSTEM_PROCESSOR MATCHES "[Aa][Rr][Mm]")
