/**
 * @file user_space_test.c
 * @Synopsis  User space program to test char_ddriver LKM
 * @author Shreyas Atre
 * @version 1.0
 * @date 2021-03-20
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define WR_VALUE _IOW('a', 1, char *)
#define RD_VALUE _IOR('a', 2, char *)
#define BUFFER_LENGTH 256

static char _rec_buf[BUFFER_LENGTH];

int main()
{
    int ret, fd;
    char _send_buf[BUFFER_LENGTH];

    printf("========== User Space Testing ==========\n");

    fd = open("/dev/char_ddriver", O_RDWR);

    if (fd < 0)
    {
        perror("Error opening");
        return errno;
    }

    printf("Send Buffer\t\t\t\t\t : ");

    scanf("%[^\n]%*c", _send_buf);
    printf("Writing\t\t\t\t\t : [%s]\n", _send_buf);

    ret = write(fd, _send_buf, strlen(_send_buf));
    if (ret < 0)
    {
        perror("Error Writing");
        return errno;
    }

    printf("ENTER to read");
    getchar();
    printf("Reading\n");

    ret = read(fd, _rec_buf, BUFFER_LENGTH);
    if (ret < 0)
    {
        perror("Failed to read");
        return errno;
    }

    printf("LKM\t\t\t\t\t\t : [%s]\n", _rec_buf);

    memset(_rec_buf, '\0', sizeof(_rec_buf));
    memset(_send_buf, '\0', sizeof(_send_buf));

    printf("\n=============== IOCTL ==================\n");

    printf("Enter Value\t\t\t\t\t : ");
    scanf("%[^\n]%*c", _send_buf);
    printf("IOCTL Write\t\t\t\t\t : [%s]\n", _send_buf);
    ioctl(fd, WR_VALUE, (char *)_send_buf);

    printf("ENTER to read");
    getchar();
    printf("Reading\n");
    ioctl(fd, RD_VALUE, (char *)_rec_buf);
    printf("IOCTL Read\t\t\t\t\t : [%s]\n", _rec_buf);

    return 0;
}