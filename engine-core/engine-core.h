#pragma once

#ifdef _IS_ENGINE_CORE
#define COREDLL __declspec(dllexport)
#else
#define COREDLL __declspec(dllimport)
#endif