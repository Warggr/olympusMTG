#ifndef OLYMPUS_BUILD_TYPES_H
#define OLYMPUS_BUILD_TYPES_H

using flag_t = unsigned char;

#define WANTED_DEBUG_FLAGS DBG_READFILE

#define gdebug(x) if((x) & WANTED_DEBUG_FLAGS) std::cerr

#define ImplementIO @IOLibType@IO
#define ImplementUI DefaultUI

#define Olympus_VERSION_MAJOR @PROJECT_VERSION_MAJOR@
#define Olympus_VERSION_MINOR @PROJECT_VERSION_MINOR@
#define Olympus_VERSION_PATCH @PROJECT_VERSION_PATCH@
#cmakedefine IOLibType
#cmakedefine F_TESTS
#cmakedefine F_LOGGING

#define DBG_IMPORTANT 0x1
#define DBG_READFILE 0x2
#define DBG_X_READFILE 0x4
#define DBG_INTERNAL 0x8
#define DBG_TARGETING 0x10
#define DBG_RAGNAROK 0x20
#define DBG_IOUI 0x40

#endif //OLYMPUS_BUILD_TYPES_H