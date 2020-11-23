#include "./Internal.h"

RuntimeState Runtime;

#ifdef _WIN32
#   pragma comment(lib, "OpenGL32.lib")
#   pragma comment(lib, "SetupAPI.lib")
#   pragma comment(lib, "Version.lib")
#   pragma comment(lib, "Imm32.lib")
#   pragma comment(lib, "Winmm.lib")
#endif
