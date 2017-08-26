#ifdef DLL
// the dll exports
	#define FxDLL __declspec(dllexport)
#else
// the exe imports
	#define FxDLL __declspec(dllimport)
#endif
