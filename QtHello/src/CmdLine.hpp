#pragma once

#ifdef APP_USES_WINDOWS

#include <vector>

void StartCmdLine(int* out_argc, std::vector<char*>* out_argv);

#endif
