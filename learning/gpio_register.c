
#include <stdio.h>
#include <stdint.h>

int main(void) {
    uint8_t gpio_register = 0;

    // TODO 1: Set bit 3 to 1.
    gpio_register |= (1U << 3);

    // TODO 2: Set bit 5 to 1.
    gpio_register |= (1U << 6);

    // TODO 3: Check if bit 3 is set.
    // Print "LED ON" if it is.
if (gpio_register & (1U << 3)) {
        printf("LED ON\n");
    } else {
        printf("LED OFF\n");
    }
    // TODO 4: Clear bit 3.
    gpio_register &= ~(1U << 3);

    printf("Final register value: %u\n",
           (unsigned int)gpio_register);

    return 0;
}
