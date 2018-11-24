# USART I/O example on stm32
> USART - PC interconnection

## How to use 

### Prerequirements

- Docker
- st-link

### Build 

```
make -f Makefile.util build-proj
```

### Flash
Connect your device via USB and:
```
make -f Makefile.util flash
```

### STM32CubeMX

Simply open the ioc file with STM32CubeMX

## Docs and specs for STM32F303ZE

- About USART. User [manual](https://www.st.com/content/ccc/resource/technical/document/reference_manual/4a/19/6e/18/9d/92/43/32/DM00043574.pdf/files/DM00043574.pdf/jcr:content/translations/en.DM00043574.pdf)
    - page 885 - USART Chapter

- Articles
    - HAL USART [here](https://visualgdb.com/tutorials/arm/stm32/uart/)
