#include <stdio.h>
#include <fcntl.h>        // For open()
#include <unistd.h>       // For read(), write(), close()
#include <errno.h>        // For errno
#include <string.h>       // For strerror()

int main() 
{
    int fd;
    char buffer[64];
    ssize_t bytesRead;

    // Try to open a file
    fd = open("sample.txt", O_RDONLY);

    if (fd == -1) {
        printf("open() failed!\n");
        printf("errno = %d\n", errno);
        perror("Error using perror()");
        printf("strerror(): %s\n", strerror(errno));
        return 1;
    }

    printf("File opened successfully!\n\nReading file contents:\n");

    // Read loop
    while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';   // Null terminate so we can print as a string
        write(1, buffer, bytesRead); // Write to stdout (fd = 1)
    }

    if (bytesRead == -1) {
        printf("\nread() failed!\n");
        printf("errno = %d\n", errno);
        perror("Error using perror()");
    }

    close(fd);
    return 0;
}
