#include "binary_search.h"

#define MAX_VALUE 0xFFFFFFFFU
#define MIN_VALUE 0x00000000U

static inline uint32_t calc_mid(uint32_t left, uint32_t right)
{
    return (left + (((right - left) >> 1)));
}

bool check_conditions(uint32_t *arr, size_t size, Fifo_Indexes_t *init_fifo_indexes_res)
{
    if (size == 0)
    {
        return false;
    }

    if (size == 1)
    {
        // For future use: Error handling/NVF_Status return value in function
        /*
        if (arr[0] > MIN_VALUE && arr[0] < MAX_VALUE) {
            init_fifo_indexes_res->tail = 0;
            init_fifo_indexes_res->head = 0;
        }*/
        return false;
    }

    return true;
}

void find_min_index(uint32_t *arr, size_t size, Fifo_Indexes_t *init_fifo_indexes_res)
{
    uint32_t left_idx = 0U;
    uint32_t right_idx = size - 1U;
    uint32_t mid_idx;

    // Buscamos el minimimo
    while (left_idx < right_idx)
    {
        // Reemplazamos los 0x00000000U por 0xFFFFFFFFU
        uint32_t arr_mid_idx = (arr[mid_idx] == MIN_VALUE) ? MAX_VALUE : arr[mid_idx];
        uint32_t arr_mid_p1_idx = (arr[mid_idx + 1U] == MIN_VALUE) ? MAX_VALUE : arr[mid_idx + 1U];
        uint32_t arr_right_idx = (arr[right_idx] == MIN_VALUE) ? MAX_VALUE : arr[right_idx];

        if (arr_mid_idx > arr_mid_p1_idx)
        {
            init_fifo_indexes_res->tail = mid_idx + 1U;
            break;
        }

        if (mid_idx > MIN_VALUE)
        {
            uint32_t arr_mid_m1_idx = (arr[mid_idx - 1U] == MIN_VALUE) ? MAX_VALUE : arr[mid_idx - 1U];
            if (arr_mid_m1_idx > arr_mid_idx)
            {
                init_fifo_indexes_res->tail = mid_idx;
                break;
            }
        }

        if (arr_mid_idx > arr_right_idx)
        {
            left_idx = mid_idx + 1U;
        }
        else
        {
            right_idx = mid_idx - 1U;
        }

        mid_idx = calc_mid(left_idx, right_idx);
        init_fifo_indexes_res->tail = mid_idx;
    }
}

void find_max_index(uint32_t *arr, size_t size, Fifo_Indexes_t *init_fifo_indexes_res)
{
    uint32_t left_idx = 0U;
    uint32_t right_idx = size - 1U;
    uint32_t mid_idx;

    // Buscamos el maximo
    while (left_idx < right_idx)
    {
        // Reemplazamos los 0xFFFFFFFFU por 0x00000000U
        uint32_t arr_mid_idx = (arr[mid_idx] == MAX_VALUE) ? MIN_VALUE : arr[mid_idx];
        uint32_t arr_mid_p1_idx = (arr[mid_idx + 1U] == MAX_VALUE) ? MIN_VALUE : arr[mid_idx + 1U];
        uint32_t arr_right_idx = (arr[right_idx] == MAX_VALUE) ? MIN_VALUE : arr[right_idx];

        if (arr_mid_idx > arr_mid_p1_idx)
        {
            init_fifo_indexes_res->head = mid_idx + 1U;
            break;
        }

        if (mid_idx > 0U)
        {
            uint32_t arr_mid_m1_idx = (arr[mid_idx - 1U] == MAX_VALUE) ? MIN_VALUE : arr[mid_idx - 1U];
            if (arr_mid_m1_idx > arr_mid_idx)
            {
                init_fifo_indexes_res->head = mid_idx;
                break;
            }
        }

        if (arr_mid_idx > arr_right_idx)
        {
            left_idx = mid_idx + 1U;
        }
        else
        {
            right_idx = mid_idx - 1U;
        }

        mid_idx = calc_mid(left_idx, right_idx);
        init_fifo_indexes_res->head = mid_idx;
    }
}

Fifo_Indexes_t init_fifo_indexes(uint32_t *arr, size_t size)
{
    Fifo_Indexes_t init_fifo_indexes_res = {0};

    if (!check_conditions(arr, size, &init_fifo_indexes_res))
    {
        return init_fifo_indexes_res;
    }

    find_min_index(arr, size, &init_fifo_indexes_res);

    find_max_index(arr, size, &init_fifo_indexes_res);

    return init_fifo_indexes_res;
}

