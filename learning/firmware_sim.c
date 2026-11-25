
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {

    // Open the virtual serial port
    int fd = open("/tmp/virtual_mcu", O_RDWR | O_NOCTTY);

    if (fd < 0) {
        perror("Failed to open port");
        return 1;
    }

    printf("Firmware simulator started!\n");
    fflush(stdout);

    char command[64];
    int index = 0;
    char c;

    // Simulated LED state: 0 = OFF, 1 = ON
    int led_state = 0;

    // Continuously listen for commands
    while (1) {

        ssize_t n = read(fd, &c, 1);

        if (n <= 0) {
            if (n < 0) {
                perror("Serial read failed");
            }
            break;
        }

        if (c == '\n') {

            // Terminate the C string
            command[index] = '\0';

            printf("Received: %s\n", command);

            const char *response;

            if (strcmp(command, "STATUS") == 0) {
                response = "STATUS: OK\n";

            } else if (strcmp(command, "PING") == 0) {
                response = "PONG\n";

            } else if (strcmp(command, "LED_ON") == 0) {
                led_state = 1;
                response = "LED: ON\n";

            } else if (strcmp(command, "LED_OFF") == 0) {
                led_state = 0;
                response = "LED: OFF\n";

            } else if (strcmp(command, "GET_LED_STATE") == 0) {

                if (led_state == 1) {
                    response = "LED: ON\n";
                } else {
                    response = "LED: OFF\n";
                }

            } else {
                response = "UNKNOWN COMMAND\n";
            }

            // Send the response to Python
            write(fd, response, strlen(response));

            // Reset the command buffer
            index = 0;

        } else if (c != '\r') {

            if (index < (int)sizeof(command) - 1) {
                command[index++] = c;
            } else {
                // Discard the overlong command
                index = 0;
                while (read(fd, &c, 1) == 1 && c != '\n') {
                }
                const char *error = "ERROR: COMMAND TOO LONG\n";
                write(fd, error, strlen(error));
            }
        }
    }

    close(fd);
    return 0;
}
