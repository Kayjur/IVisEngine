#include "TestFramework.h"
#include "TestTimeUtil.h"
#include "Cell/CellTriangle.h"

TEST_GROUP(TestCell)

TEST(CellTriangle)
{
    FScopedTimer Timer;
    float arr;
    double X = 1;
    for (int i = 0; i < 100000000; i++)
    {
        arr = FMath::Sin(X);
    }
    ASSERT(arr > 0);
}

