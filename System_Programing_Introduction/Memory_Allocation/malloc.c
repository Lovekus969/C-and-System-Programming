#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr;

    arr = (int *)malloc(50 * sizeof(int));

    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Store values
    for(int i = 0; i < 50; i++) {
        arr[i] = i + 1;
    }

    // Print first & last
    printf("First element: %d\n", arr[0]);
    printf("Last element: %d\n", arr[49]);

    free(arr);  // free the entire block

    return 0;
}
