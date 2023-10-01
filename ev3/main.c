#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include "main.h"

int main()
{
    int file;
    char filename[] = "/dev/i2c-3"; 
    char buf[100];
    unsigned char *ubuf=(unsigned char *)buf;
    if ((file = open(filename, O_RDWR)) < 0)
    {
        strerror_r(errno, buf, 100);
        fprintf(stderr, "error: failed to open bus\n"
                "err: %s\n", buf);
        exit(1);
    }
    /*
    // initiate communication
    if (ioctl(file, I2C_SLAVE, TCS34725_ADDR) < 0)
    {
        strerror_r(errno, buf, 100);
        fprintf(stderr, "error: failed to acquire bus acces and/or talk to slave\n"
                "err: %s\n", buf);
        close(file);
        exit(1);
    }
    unsigned char dat = CMD_BIT | AUTO_INC_TRANSAC | 0x12; // 0x12 is id register
    if(write(file, &dat, 1) != 1)
    {
        fprintf(stderr, "didnt write 1 byte\n");
    }

    if (read(file, ubuf, 1) != 1)
    {
        fprintf(stderr, "didnt read 1 byte\n");
    }
    else
    {
        printf("read %x\n", *ubuf);
    }
    */
    unsigned long funcs = 0;
    ioctl(file, I2C_FUNCS, &funcs);
    if(funcs & I2C_FUNC_I2C)
    {
        printf("supports I2C!\n");
    }
    else
    {
        fprintf(stderr, "does not support i2c stuffy\n");
    }

    close(file);
    return 0;
}
