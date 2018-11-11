# Animation with brightness on LEDs
> Animation with 8 LEDs via brightness range

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

## Pins-LEDs mapping (STM32F303ZETx)

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


## About timer on STM32
### Params overview
- `Prescaler` - divisor for timer tick frequency.
  Timer tick frequency formula: `timer_tick_frequency = timer_default_freq / (Prescaler + 1)`.
  `timer_default_freq` is a default freq of the bus, to which timer is connected. See docs for concrete values.
- `CounterMode` - count up or count down.
- `Period` - period of your impulses (aka threshold for you counter). The default value depends on bitness of your counter (16 or 32)

```
PWM_Frequency = timer_tick_frequency / (Period - 1)
```

Also, see [Origin](https://stm32f4-discovery.net/2014/05/stm32f4-stm32f429-discovery-pwm-tutorial/)

## Docs and specs for STM32F303ZE

- Product [Specification](https://www.st.com/resource/en/datasheet/stm32f303ze.pdf):
  - page 54: Alternate functions mapping

- General purpose timer [cookbook](https://www.st.com/content/ccc/resource/technical/document/application_note/group0/91/01/84/3f/7c/67/41/3f/DM00236305/files/DM00236305.pdf/jcr:content/translations/en.DM00236305.pdf)
  - page 9: The timier-channels unit
  - page 51: Memory map

- User [manual](https://www.st.com/content/ccc/resource/technical/document/reference_manual/4a/19/6e/18/9d/92/43/32/DM00043574.pdf/files/DM00043574.pdf/jcr:content/translations/en.DM00043574.pdf)
  - page 601: General purpose timers

- User [manual](https://www.st.com/content/ccc/resource/technical/document/user_manual/1b/03/1b/b4/88/20/4e/cd/DM00105928.pdf/files/DM00105928.pdf/jcr:content/translations/en.DM00105928.pdf) for nucleo board
