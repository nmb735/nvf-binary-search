#include "binary_search.h"

#define MAX_VALUE 0xFFFFFFFFU
#define MIN_VALUE 0x00000000U

// Cuando buscamos el minimo, reemplazamos los 0x00000000 por 0xFFFFFFFF y buscamos el pivot point.
// Cuando buscamos el maximo, reemplazamos los 0xFFFFFFFF por 0x00000000 y buscamos el pivot point.

/**
 * Calculates the midpoint between two given values.
 * @author Albert Alvarez
 * @param left: The left-most index in an array.
 * @param right: The right-most index in an array.
 * @return The midpoint between the left and right values.
 */
static inline uint32_t calculate_mid(uint32_t left, uint32_t right)
{
    return left + (((right - left) >> 1));
}

/**
 * Check if the conditions for initializing FIFO indexes are met.
 * @author Nedal Martinez Benelmekki
 * @param arr: Pointer to an array of uint32_t values.
 * @param size: The size of the array.
 * @param init_fifo_indexes_res: Pointer to a Fifo_Indexes_t structure to store the result.
 * @return True if the conditions are met, False otherwise.
 */
bool check_conditions(uint32_t *arr, size_t size, Fifo_Indexes_t *init_fifo_indexes_res)
{
    if (arr == NULL)
    {
        return false;
    }

    if(init_fifo_indexes_res == NULL)
    {
        return false;
    }

    if (size == 0)
    {
        return false;
    }

    if (size == 1)
    {
        // For future use
        /*
        if (arr[0] > MIN_VALUE && arr[0] < MAX_VALUE) {
            init_fifo_indexes_res->tail = 0;
            init_fifo_indexes_res->head = 0;
        }*/
        return false;
    }

    return true;
}

/**
 * Replaces a value with a new value if it matches a specified old value.
 * @authors Albert Alvarez, Nedal Martinez Benelmekki
 * @param value: The value to be replaced.
 * @param old_value: The value to be checked against.
 * @param new_value: The value to replace with if the old value matches.
 * @return The replaced value if it matches the old value, otherwise the original value.
 */
static inline uint32_t replace_value(uint32_t value, uint32_t old_value, uint32_t new_value)
{
    return (value == old_value) ? new_value : value;
}

/**
 * Initializes the head index of the FIFO non volatile memory. It searches for the maximum value in the array and sets the head index accordingly.
 * @author Albert Alvarez
 * @param arr: Pointer to an array of uint32_t values.
 * @param size: The size of the array.
 * @param init_fifo_indexes_res: Pointer to a Fifo_Indexes_t structure to store the result.
 * @return None.
 */
void init_fifo_head(uint32_t *arr, size_t size, Fifo_Indexes_t *init_fifo_indexes_res)
{
    uint32_t left_idx = 0U;
    uint32_t right_idx = size - 1U;
    uint32_t mid_idx = calculate_mid(left_idx, right_idx);

    // Buscamos el maximo
    while (left_idx < right_idx)
    {
        if ((mid_idx == 0U) || ((mid_idx == left_idx) && (mid_idx == right_idx)))
        {
            init_fifo_indexes_res->head = mid_idx;
            break;
        }

        // Reemplazamos los 0xFFFFFFFFU por 0x00000000U
        uint32_t arr_mid_idx = replace_value(arr[mid_idx], MAX_VALUE, MIN_VALUE);
        uint32_t arr_mid_p1_idx = replace_value(arr[mid_idx + 1U], MAX_VALUE, MIN_VALUE);
        uint32_t arr_right_idx = replace_value(arr[right_idx], MAX_VALUE, MIN_VALUE);

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

        mid_idx = calculate_mid(left_idx, right_idx);
        init_fifo_indexes_res->head = mid_idx;
    }
}

/**
 * Initializes the tail index of the FIFO non volatile memory. It searches for the minimum value in the array and sets the tail index accordingly.
 * @author Albert Alvarez
 * @param arr: Pointer to an array of uint32_t values.
 * @param size: The size of the array.
 * @param init_fifo_indexes_res: Pointer to a Fifo_Indexes_t structure to store the result.
 * @return None.
 */
void init_fifo_tail(uint32_t *arr, size_t size, Fifo_Indexes_t *init_fifo_indexes_res)
{
    uint32_t left_idx = 0U;
    uint32_t right_idx = size - 1U;
    uint32_t mid_idx = calculate_mid(left_idx, right_idx);

    // Buscamos el minimimo
    while (left_idx < right_idx)
    {
        if ((mid_idx == 0U) || ((mid_idx == left_idx) && (mid_idx == right_idx)))
        {
            init_fifo_indexes_res->tail = mid_idx;
            break;
        }

        // Reemplazamos los 0x00000000U por 0xFFFFFFFFU
        uint32_t arr_mid_idx = replace_value(arr[mid_idx], MIN_VALUE, MAX_VALUE);
        uint32_t arr_mid_p1_idx = replace_value(arr[mid_idx + 1U], MIN_VALUE, MAX_VALUE);
        uint32_t arr_right_idx = replace_value(arr[right_idx], MIN_VALUE, MAX_VALUE);

        if (arr_mid_idx > arr_mid_p1_idx)
        {
            init_fifo_indexes_res->tail = mid_idx + 1U;
            break;
        }

        if (mid_idx > 0U)
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

        mid_idx = calculate_mid(left_idx, right_idx);
        init_fifo_indexes_res->tail = mid_idx;
    }
}

Fifo_Indexes_t init_fifo_indexes(uint32_t *arr, size_t size)
{
    Fifo_Indexes_t init_fifo_indexes_res = {0};

    if (!check_conditions(arr, size, &init_fifo_indexes_res))
    {
        return init_fifo_indexes_res;
    }

    init_fifo_head(arr, size, &init_fifo_indexes_res);
    init_fifo_tail(arr, size, &init_fifo_indexes_res);

    return init_fifo_indexes_res;
}

int NVF_init(Fifo_Indexes_t *self, uint32_t *arr, size_t size)
{
    if (self == NULL)
    {
        return -1;
    }
    self->head = 0x0U;
    self->tail = 0x0U;

    Fifo_Indexes_t x = init_fifo_indexes(arr, size);

    if (x.head == 0U && x.tail == 0U)
    {
        return -1;
    }

    return 0;
}