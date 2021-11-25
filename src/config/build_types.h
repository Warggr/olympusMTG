#ifndef OLYMPUS_BUILD_TYPES_H
#define OLYMPUS_BUILD_TYPES_H

#define WANTED_DEBUG_FLAGS @WANTED_DEBUG_FLAGS@

#define gdebug(x) if((x) & WANTED_DEBUG_FLAGS) std::cerr

#define STR(x) #x
#define Olympus_VERSION STR(@PROJECT_VERSION@)
#define Olympus_VERSION_MAJOR @PROJECT_VERSION_MAJOR@
#define Olympus_VERSION_MINOR @PROJECT_VERSION_MINOR@
#define Olympus_VERSION_PATCH @PROJECT_VERSION_PATCH@
#cmakedefine F_TESTS
#cmakedefine F_LOGGING

#define DBG_NONE 0x0
#define DBG_IMPORTANT 0x1
#define DBG_READFILE 0x2
#define DBG_INTERNAL 0x4
#define DBG_TARGETING 0x8
#define DBG_RAGNAROK 0x10
#define DBG_IOUI 0x20

#endif //OLYMPUS_BUILD_TYPES_H
