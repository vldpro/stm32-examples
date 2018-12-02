# USART I/O example on stm32
> USART - PC interconnection

## How to build  and flash

### Requirements

- docker
- cmake 
- build-essential
- libusb-1.0.0-dev
- putty

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

## Usage

### With putty

- Connect device to your PC via UART to USB adapter (For example - CP2120)
- Type:
```
$ sudo putty
```

- Selet *Serial* device
- Type `/dev/ttyUSB0` with `115200` baudrate
- Go to *Connection* -> *Serial* and disable *Flow control*
- Be sure to select *8 data bits* and *1 stop bit* with no *Parity*
- Press *Open*

### Work Modes

Mode can be switched by ressing a button

#### Decimal to Binary parser
- Type a decimal number from 0 to 255
- As a result, you should se the number in binary form in your terminal and leds
- If you try to type something wrong, mcu should report about error via simple message on terminal

#### Simple ECHO with some transformation
- Type smth with *latin* script in upper/lower case. 
  On terminal you should see the same characters only in lower case
- If you try to type smth in *cyrillic* script, you will see it in lower case only


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

### UART

| Pin  | Descr   |
| ---- | ------- |
| PC10 | UART_TX |
| PC11 | UART_RX |

### Leds

| TIMER    | Pin  | LED  |
| -------- | ---- | ---- |
| TIM2_CH1 | PA0  | LED1 |
| TIM2_CH2 | PA1  | LED2 |
| TIM2_CH3 | PA2  | LED3 |
| TIM2_CH4 | PA3  | LED4 |
| TIM4_CH1 | PD12 | LED5 |
| TIM4_CH2 | PD13 | LED6 |
| TIM4_CH3 | PD14 | LED7 |
| TIM4_CH4 | PF6  | LED8 |

### Btns

| Btn name      | Pin  | Desription                      |
| ------------- | ---- | ------------------------------- |
| Switch button | PC13 | Switch from one view to another |


## Docs and specs for STM32F303ZE

- About USART. User [manual](https://www.st.com/content/ccc/resource/technical/document/reference_manual/4a/19/6e/18/9d/92/43/32/DM00043574.pdf/files/DM00043574.pdf/jcr:content/translations/en.DM00043574.pdf)
    - page 885 - USART Chapter

- Articles
    - HAL USART [here](https://visualgdb.com/tutorials/arm/stm32/uart/)
    - http://www.rotr.info/electronics/mcu/arm_usart.htm
