#include "tester.h"

#include <gtest/gtest.h>

#ifdef _WIN32
/* Memory leak checker */
#ifdef _DEBUG
# include <crtdbg.h>
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#endif /* _DEBUG */
#endif

int main(int argc, char *argv[])
{
#ifdef _WIN32
#ifdef _DEBUG
  SetDbgMemHooks();
#endif
#endif

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
