#define VERSION 20200707

#ifdef __WIN32__
#	define ARCHITECTURE "w32u"
#elif defined(__WIN64__)
#	define ARCHITECTURE "w64u"
#elif defined(__APPLE__)
#	define ARCHITECTURE "macu"
#elif defined(__linux__) && defined(__x86_64__)
#	define ARCHITECTURE "l64u"
#elif defined(__linux__) && defined(__i386__)
#	define ARCHITECTURE "l32u"
#elif defined(__x86_64__)
#	define ARCHITECTURE "?64u"
#elif defined(__i386__)
#	define ARCHITECTURE "?32u"
#else
#	define ARCHITECTURE "???u"
#endif

