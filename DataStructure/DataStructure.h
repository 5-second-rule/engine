#pragma once

#ifdef _IS_ENGINE_CORE
#define DATADLL __declspec(dllexport)
#else
#define DATADLL __declspec(dllimport)
#endif