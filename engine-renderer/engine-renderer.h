#ifdef _IS_ENGINE_RENDERER
#define RENDERDLL __declspec(dllexport)
#else
#define RENDERDLL __declspec(dllimport)
#endif