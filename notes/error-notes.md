# ERROR TYPES AND NOTES

## ERROR (Type 1)

- **Description:** The algorithm neglects the right half of the structure, even if it contains the results. The error occurs if the listed conditions are met. It is a direct consequence of the implemented logic, as if none of the conditions are met, it defaults to searching in the left half of the structure.

- **Conditions:** These are the conditions that cause the error. Individually, they do not affect the output, but a combination of them does. Since these conditions are dependent on one another, there is a cascade-like relationship between them:
    - **Size of N:** If N > 5 (the size of the array is larger than N), some of the following conditions may occur, thus interfering with the validity of the output.
    - **Position of min/max:** min/max ∉ [mid - 1, mid + 1]. If, after the first iteration, the value is not in this interval and condition 3 is met, the program is "blind", and it does not have criteria to select the proper half to search. By default, it searches the left side.
    - **Value of min/max:** mid_value = left_value = right_value. If, after the first iteration, the values are the same for all three, and the value of mid+1 and mid-1 is also the same, the program is "blind", and it does not have criteria to select the proper half to search. By default, it searches the left side.
    - **Location of results:** If the results are located on the right side of the structure and other conditions are met, the left side of the array will be searched, and therefore, the results will end up being incorrect.
