//
// Created by mecha on 25.04.2022.
//

#pragma once

#if defined(_WIN64)
typedef unsigned __int64 SOCKET;
#else
typedef _W64 unsigned int SOCKET;
#endif
