#include <stdio.h>
#include <fcntl.h>      // For open()
#include <unistd.h>     // For close()
#include <errno.h>      // For errno
#include <string.h>     // For strerror()

int main() 
{
    int fd;

    // Try to open a file that does NOT exist
    fd = open("myfile.txt", O_RDONLY);

    if (fd == -1) {
        printf("open() failed!\n");

        // Print the error number
        printf("errno = %d\n", errno);

        // Print a human-readable message
        perror("Error using perror()");

        // Print message using strerror()
        printf("strerror: %s\n", strerror(errno));

        return 1;
    }

    // If open succeeds (rare in this case)
    printf("File opened successfully! fd = %d\n", fd);

    close(fd);
    return 0;
}
