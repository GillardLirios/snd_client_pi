#ifndef __RPI_GPIO_H__
#define __RPI_GPIO_H__

#define IN	1
#define OUT	0
#define LOW 0
#define HIGH 1
int gpio_export(int pin);
int gpio_unexport(int pin);
int gpio_direction(int pin, int dir) ;
int gpio_write(int pin, int value) ;
int gpio_read(int pin) ;
#endif

