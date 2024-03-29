#ifndef OLYMPUS_BUILD_TYPES_H
#define OLYMPUS_BUILD_TYPES_H

#ifndef WANTED_DEBUG_FLAGS
#define WANTED_DEBUG_FLAGS @WANTED_DEBUG_FLAGS@
#endif

#define STR(x) #x
#define Olympus_VERSION STR(@PROJECT_VERSION@)
#define Olympus_VERSION_MAJOR @PROJECT_VERSION_MAJOR@
#define Olympus_VERSION_MINOR @PROJECT_VERSION_MINOR@
#define Olympus_VERSION_PATCH @PROJECT_VERSION_PATCH@

#cmakedefine F_STRICT

#endif //OLYMPUS_BUILD_TYPES_H
