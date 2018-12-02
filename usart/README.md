# USART I/O example on stm32
> USART - PC interconnection

## How to use 

### Prerequirements

- docker
- cmake 
- build-essential
- libusb-1.0.0-dev

### Build 

```
make -f Makefile.util build-proj
```

### Flash
Connect your device via USB and:
```
sudo make -f Makefile.util flash
```

### STM32CubeMX

Simply open the ioc file with STM32CubeMX

## Structure
- UART Polling driver
    - `Inc/uart_poll_driver.h`
    - `Src/uart_poll_driver.c`
- UART iterrupt driver
    - `Inc/uart_int_driver.h`
    - `Src/uart_int_driver.c`
- Hardware initialization
    - `Inc/hw_init.h`
    - `Src/hw_init.c`
- FIFO (Cyclic buffer with fixed size)
    - `Inc/fifo.h`
- Utils (read/write line, etc)
    - `Inc/utils.h`
- Main function
    - `Src/entrypoint.c`

## Pins Mapping

| Pin  | Descr   |
| ---- | ------- |
| PC10 | UART_TX |
| PC11 | UART_RX |

## Docs and specs for STM32F303ZE

- About USART. User [manual](https://www.st.com/content/ccc/resource/technical/document/reference_manual/4a/19/6e/18/9d/92/43/32/DM00043574.pdf/files/DM00043574.pdf/jcr:content/translations/en.DM00043574.pdf)
    - page 885 - USART Chapter

- Articles
    - HAL USART [here](https://visualgdb.com/tutorials/arm/stm32/uart/)
    - http://www.rotr.info/electronics/mcu/arm_usart.htm
