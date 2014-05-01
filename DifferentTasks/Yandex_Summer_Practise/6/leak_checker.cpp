// Memory leak checker
// Works only with the msvc compiler
// See http://msdn.microsoft.com/en-us/library/e5ewb1h3%28v=vs.80%29.aspx
#ifdef _MSC_VER

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

class LeakChecker
{
public:
	~LeakChecker()
	{
		_CrtDumpMemoryLeaks();
	}
};

static LeakChecker theLeakChecker;

#endif	// _MSC_VER
