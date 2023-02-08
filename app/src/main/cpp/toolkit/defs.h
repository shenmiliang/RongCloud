#ifndef DEEP_TOOLKIT_DEFS_H
#define DEEP_TOOLKIT_DEFS_H

#ifndef DEEP_TOOLKIT_EXPORTS
# if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#   define DEEP_TOOLKIT_EXPORTS __declspec(dllexport)
# elif defined __GNUC__ && __GNUC__ >= 4 && (defined(__APPLE__))
#   define DEEP_TOOLKIT_EXPORTS __attribute__ ((visibility ("default")))
# endif
#endif

#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
# define DEEP_TOOLKIT_WIN32
#elif defined __GNUC__ && __GNUC__ >= 4 && (defined(__APPLE__))
# define DEEP_TOOLKIT_UNIX
#endif

#ifdef DEEP_TOOLKIT_WIN32
# define NOMINMAX
#endif

#ifndef DEEP_TOOLKIT_EXPORTS
# define DEEP_TOOLKIT_EXPORTS
#endif

#ifndef __unused
# define __unused
#endif

#endif 