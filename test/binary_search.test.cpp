#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "binary_search.h"
}

TEST_GROUP(BinarySearch){};

TEST(BinarySearch, InitFifoFull)
{
    uint32_t arr[] = {12, 13, 45, 51, 0xFFFFFFFFU, 1, 3};

    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(5U, fifo_indexes.tail);
    CHECK_EQUAL(4U, fifo_indexes.head);
}

TEST(BinarySearch, InitFifoNotFullWithoutZeros)
{
    uint32_t arr[] = {12, 13, 45, 51, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 1, 3};

    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(7U, fifo_indexes.tail);
    CHECK_EQUAL(4U, fifo_indexes.head);
}

TEST(BinarySearch, InitFifoNotFullWithMultipleZeros)
{
    uint32_t arr[] = {12, 13, 45, 51, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0, 0, 0, 0, 1, 3};

    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(11U, fifo_indexes.tail);
    CHECK_EQUAL(4U, fifo_indexes.head);
}

TEST(BinarySearch, InitFifoNeverFullyWritten)
{
    uint32_t arr[] = {0, 0, 0, 12, 13, 45, 51, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};

    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(3U, fifo_indexes.tail);
    CHECK_EQUAL(7U, fifo_indexes.head);
}

TEST(BinarySearch, InitFifoEmptyNeverWritten)
{
    uint32_t arr[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};

    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(0U, fifo_indexes.tail);
    CHECK_EQUAL(0U, fifo_indexes.head);
}

TEST(BinarySearch, InitFifoEmptyAllWritten)
{
    uint32_t arr[] = {0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U};

    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(0U, fifo_indexes.tail);
    CHECK_EQUAL(0U, fifo_indexes.head);
}

TEST(BinarySearch, CycleTest)
{
    uint32_t arr[] = {1, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0};

    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(1U, fifo_indexes.head);
    CHECK_EQUAL(0U, fifo_indexes.tail);
}

TEST(BinarySearch, CycleTest2)
{
    uint32_t arr[] = {1, 2, 3, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(3U, fifo_indexes.head);
    CHECK_EQUAL(0U, fifo_indexes.tail);
}

TEST(BinarySearch, CycleTest3)
{
    uint32_t arr[] = {12, 13, 45, 51, 0xFFFFFFFFU, 0xFFFFFFFFU};

    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(0U, fifo_indexes.tail);
    CHECK_EQUAL(4U, fifo_indexes.head);
}

TEST(BinarySearch, CycleTest4)
{
    uint32_t arr[] = {2, 3, 4, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0, 0, 0, 1};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(3U, fifo_indexes.head);
    CHECK_EQUAL(9U, fifo_indexes.tail);
}

TEST(BinarySearch, CycleTest5)
{
    uint32_t arr[] = {11, 12, 13, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 10};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(3U, fifo_indexes.head);
    CHECK_EQUAL(8U, fifo_indexes.tail);
}

TEST(BinarySearch, CycleTest6)
{
    uint32_t arr[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0, 1, 2, 3};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(0U, fifo_indexes.head);
    CHECK_EQUAL(7U, fifo_indexes.tail);
}

TEST(BinarySearch, NullTest)
{
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(nullptr, 1);

    CHECK_EQUAL(0U, fifo_indexes.head);
    CHECK_EQUAL(0U, fifo_indexes.tail);
}

TEST(BinarySearch, sizeZero)
{
    uint32_t arr[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0, 1, 2, 3};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, 0);

    CHECK_EQUAL(0U, fifo_indexes.head);
    CHECK_EQUAL(0U, fifo_indexes.tail);
}

TEST(BinarySearch, sizeOne)
{
    uint32_t arr[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0, 1, 2, 3};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, 1);

    CHECK_EQUAL(0U, fifo_indexes.head);
    CHECK_EQUAL(0U, fifo_indexes.tail);
}

TEST(BinarySearch, ConerCase1)
{
    uint32_t arr[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 1, 2, 3, 4};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(0U, fifo_indexes.head);
    CHECK_EQUAL(2U, fifo_indexes.tail);
}

TEST(BinarySearch, ConerCase2)
{
    uint32_t arr[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 1, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(3U, fifo_indexes.head);
    CHECK_EQUAL(2U, fifo_indexes.tail);
}

TEST(BinarySearch, ConerCase3)
{
    uint32_t arr[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 1, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(7U, fifo_indexes.head);
    CHECK_EQUAL(6U, fifo_indexes.tail);
} // Logics --> In Notepad (Type 1)

TEST(BinarySearch, ConerCase4)
{
    uint32_t arr[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 1, 2, 3, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(9U, fifo_indexes.head);
    CHECK_EQUAL(6U, fifo_indexes.tail);
} // Logics (Error Type 1)

TEST(BinarySearch, ConerCase5)
{
    uint32_t arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(9U, fifo_indexes.head);
    CHECK_EQUAL(0U, fifo_indexes.tail);
}

TEST(BinarySearch, ConerCase6)
{
    uint32_t arr[] = {1, 2, 3, 4, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(4U, fifo_indexes.head);
    CHECK_EQUAL(0U, fifo_indexes.tail);
}

TEST(BinarySearch, ConerCase7)
{
    uint32_t arr[] = {1, 2, 3, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(3U, fifo_indexes.head);
    CHECK_EQUAL(0U, fifo_indexes.tail);
}

TEST(BinarySearch, ConerCase8)
{
    uint32_t arr[] = {1, 2, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(2U, fifo_indexes.head);
    CHECK_EQUAL(0U, fifo_indexes.tail);
}

TEST(BinarySearch, ConerCase9)
{
    uint32_t arr[] = {0xFFFFFFFFU, 1, 2, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(3U, fifo_indexes.head);
    CHECK_EQUAL(1U, fifo_indexes.tail);
}

TEST(BinarySearch, ConerCase10)
{
    uint32_t arr[] = {0xFFFFFFFFU, 1, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(2U, fifo_indexes.head);
    CHECK_EQUAL(1U, fifo_indexes.tail);
} 

TEST(BinarySearch, ConerCase11)
{
    uint32_t arr[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 1, 2, 0xFFFFFFFFU, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));

    CHECK_EQUAL(8U, fifo_indexes.head);
    CHECK_EQUAL(6U, fifo_indexes.tail);
} //ERROR. CONCLUSION: PROBLEMA QUAN EL SECTOR ESTÀ A LA MEITAT DRETA DE LA FIFO A UNA DISTANCIA > 1 DEL MIG (Type 1)

TEST(BinarySearch, ScaleHypothesis1)
{
    uint32_t arr[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));
    CHECK_EQUAL(0U, fifo_indexes.head);
    CHECK_EQUAL(0U, fifo_indexes.tail);
}

TEST(BinarySearch, ScaleHypothesis2)
{
    uint32_t arr[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 1, 2, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));
    CHECK_EQUAL(5U, fifo_indexes.head);
    CHECK_EQUAL(3U, fifo_indexes.tail);
} //ERROR (Type 1)

TEST(BinarySearch, ScaleHypothesis3)
{
    uint32_t arr[] = {1, 2, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));
    CHECK_EQUAL(2U, fifo_indexes.head);
    CHECK_EQUAL(0U, fifo_indexes.tail);
}

TEST(BinarySearch, ScaleHypothesis4)
{
    uint32_t arr[] = {0xFFFFFFFFU, 1, 2, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));
    CHECK_EQUAL(3U, fifo_indexes.head);
    CHECK_EQUAL(1U, fifo_indexes.tail);
}

TEST(BinarySearch, ScaleHypothesis5)
{
    uint32_t arr[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 1, 2, 0xFFFFFFFFU, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));
    CHECK_EQUAL(4U, fifo_indexes.head);
    CHECK_EQUAL(2U, fifo_indexes.tail);
}

TEST(BinarySearch, ScaleHypothesis6)
{
    uint32_t arr[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 1, 2};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));
    CHECK_EQUAL(0U, fifo_indexes.head);
    CHECK_EQUAL(4U, fifo_indexes.tail);
}

TEST(BinarySearch, ScaleHypothesis7)
{
    uint32_t arr[] = {3, 4, 5, 6, 1, 2};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));
    CHECK_EQUAL(4U, fifo_indexes.head);
    CHECK_EQUAL(4U, fifo_indexes.tail);
}

TEST(BinarySearch, ScaleHypothesis8)
{
    uint32_t arr[] = {3, 4, 0xFFFFFFFFU, 0xFFFFFFFFU, 1, 2};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));
    CHECK_EQUAL(2U, fifo_indexes.head);
    CHECK_EQUAL(4U, fifo_indexes.tail);
}

TEST(BinarySearch, CycleExtemeWithScale)
{
    uint32_t arr[] = {3, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 1};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));
    CHECK_EQUAL(1U, fifo_indexes.head);
    CHECK_EQUAL(5U, fifo_indexes.tail);
}

TEST(BinarySearch, NVFMock_NeverWriten)
{
    uint32_t arr[] = {0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));
    CHECK_EQUAL(0U, fifo_indexes.head);
    CHECK_EQUAL(0U, fifo_indexes.tail);
}

TEST(BinarySearch, NVFMock_AllWritten)
{
    uint32_t arr[] = {0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));
    CHECK_EQUAL(0U, fifo_indexes.head);
    CHECK_EQUAL(0U, fifo_indexes.tail);
}

TEST(BinarySearch, NVFMock_FifoFull)
{
    uint32_t arr[] = {4, 5, 6, 7, 8, 9, 10, 11, 0xFFFFFFFFU, 1, 2, 3};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));
    CHECK_EQUAL(9U, fifo_indexes.tail);
    CHECK_EQUAL(8U, fifo_indexes.head);
}

TEST(BinarySearch, NVFMock_InitFifoNotFullWithoutZeros)
{
    uint32_t arr[] = {5, 7, 9, 12, 13, 45, 51, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 1, 3};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));
    CHECK_EQUAL(10U, fifo_indexes.tail);
    CHECK_EQUAL(7U, fifo_indexes.head);
}

TEST(BinarySearch, NVFMock_InitFifoNeverFullyWritten)
{
    uint32_t arr[] = {0, 0, 0, 0, 12, 13, 45, 51, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};
    Fifo_Indexes_t fifo_indexes = init_fifo_indexes(arr, sizeof(arr) / sizeof(arr[0]));
    CHECK_EQUAL(4U, fifo_indexes.tail);
    CHECK_EQUAL(8U, fifo_indexes.head);
}
