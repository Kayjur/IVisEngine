#include "TestFramework.h"

#include "Container/CellArray.h"

TEST_GROUP(TestCellArray)

namespace {
    FCellArray Array;
}


TEST(AddCell)
{
    FTimer Timer;

    Timer.Start();

    TArray<int32> VertexIndices;
    VertexIndices.Resize(3);
    for (int i = 0; i < 10000000; i++) {

        //VertexIndices.Add(i);
        VertexIndices[0] = (i * 3 + 0);
        VertexIndices[1] = (i * 3 + 1);
        VertexIndices[2] = (i * 3 + 2);

        Array.AddCell(ECellType::Triangle, VertexIndices);
    }
    // 一千万：160 ~ 200 ms

    Timer.Stop();
    //Timer.Print();
}

TEST(GetCell)
{
    FTimer Timer;
    Timer.Start();

    FCellInfo CellInfo;
    for (int i = 0; i < 10000000; i++) {
        Array.GetCell(i, CellInfo);

        for (int j = 0; j < CellInfo.Num(); j++) {
            int32 Index = CellInfo[j];
        }
    }

    Timer.Stop();
    Timer.Print();
}