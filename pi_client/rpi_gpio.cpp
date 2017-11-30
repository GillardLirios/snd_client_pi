#include "rpi_gpio.h"
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>

int gpio_export(int pin)  
{  
    char buffer[4096];  
    int len;  
    int fd;  
  
    fd = open("/sys/class/gpio/export", O_WRONLY);  
    if (fd < 0) {  
        fprintf(stderr, "Failed to open export for writing!\n");  
        return(-1);  
    }  
  
    len = snprintf(buffer, 4096, "%d", pin);  
    if (write(fd, buffer, len) < 0) {  
        fprintf(stderr, "Fail to export gpio!");  
        return -1;  
    }  
     
    close(fd);  
    return 0;  
}  
int gpio_unexport(int pin)
{
    char buffer[4096];
    int len;
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "Failed to open unexport for writing!\n");
        return(-1);
    }

    len = snprintf(buffer, 4096, "%d", pin);
    write(fd, buffer, len);
    
    close(fd);
    return(0);
}
int gpio_direction(int pin, int dir)  
{  
    static const char dir_str[] = "in\0out";  
    char path[4096];  
    int fd;  

    snprintf(path, 4096, "/sys/class/gpio/gpio%d/direction", pin);  
    fd = open(path, O_WRONLY);  
    if (fd < 0) {  
        fprintf(stderr, "failed to open gpio direction for writing!\n");  
        return -1;  
    }  
  
    if (write(fd, &dir_str[dir == IN ? 0 : 3], dir == IN ? 2 : 3) < 0) {  
        fprintf(stderr, "failed to set direction!\n");  
        return -1;  
    }  
  
    close(fd);  
    return 0;  
}  

int gpio_write(int pin, int value)  
{  
    static const char values_str[] = "01";  
    char path[4096];  
    int fd;  

    snprintf(path, 4096, "/sys/class/gpio/gpio%d/value", pin);  
    fd = open(path, O_WRONLY);  
    if (fd < 0) {  
        fprintf(stderr, "failed to open gpio value for writing!\n");  
        return -1;  
    }  
  
    if (write(fd, &values_str[value == LOW ? 0 : 1], 1) < 0) {  
        fprintf(stderr, "failed to write value!\n");  
        return -1;  
    }  
  
    close(fd);  
    return 0;  
}  


int gpio_read(int pin)  
{  
    char path[4096];  
    char value_str[3];  
    int fd;  
    
    snprintf(path, 4096, "/sys/class/gpio/gpio%d/value", pin);  
    fd = open(path, O_RDONLY);  
    if (fd < 0) {  
        fprintf(stderr, "failed to open gpio value for reading!\n");  
        return -1;  
    }  
  
    if (read(fd, value_str, 3) < 0) {  
        fprintf(stderr, "failed to read value!\n");  
        return -1;  
    }  
  
    close(fd);  
    return (atoi(value_str));  
}  