#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include "main.h"

int main()
{
    int file;
    char filename[] = "/dev/i2c-3"; 
    if ((file = open(filename, O_RDWR)) < 0)
    {
        fprintf(stderr, "error: failed to open bus\n%s\n", strerror(errno));
        exit(1);
    }


    /*
    unsigned long funcs = 0;
    ioctl(file, I2C_FUNCS, &funcs);
    if(funcs & I2C_FUNC_I2C)
    {
        printf("supports I2C!\n");
    }
    */
    
    if(ioctl(file, I2C_SLAVE, TCS34725_ADDR) < 0)
    {
        fprintf(stderr, "error: Could not ioctl I2C_SLAVE;\n%s\n", strerror(errno));
        close(file);
        exit(1);
    }

    __u8 reg = CMD_BIT | AUTO_INC_TRANSAC;
    __u8 dat = 0xA;
    __u8 datbuf[] = {reg, dat};
    if (write(file, datbuf, 1) != 1)
    {
        fprintf(stderr, "error writing (AT FIRST WRITE);\n%s\n", strerror(errno));
        close(file);
        exit(1);
    }
    if (write(file, datbuf+1, 1) != 1)
    {
        fprintf(stderr, "error writing (AT SECOND WRITE);\n%s\n", strerror(errno));
        close(file);
        exit(1);
    }
    else
    {
        printf("wrote some shit!\n");
    };


    close(file);
    return 0;
}
