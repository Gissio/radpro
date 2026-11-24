# H240 register provenance

The H240 factory image was read twice from the tested board. Both
128 KiB reads produced SHA-256
`604638ea4dbdd9553c7803e87cf8ce7bdbf1ad0a3afc52028293f7fca92ed46f`
and matched the public H240 MCU sample. Decode it as Thumb at `0x08000000`.

| Area | Verified reference |
| --- | --- |
| GPIO | Factory helpers at `0x08007438` and `0x08007484` cover GPIOA/B/C from `0x40022000` at 64-byte strides. |
| Panel | `0x08004040` configures the panel, with landscape MADCTL `0xe8`. `0x0800c53c` selects SPI1 on PC6 and PC8. |
| Buttons | `0x08005f18` reads active-low PB15, PB14, PB12, PB11 and PB13. Live GPIO reads establish their direction and center assignments. |
| Geiger input | IRQ 16 at `0x08005624` uses TIM3 and calls the PA10 detection path at `0x08008e70`. |
| Converter | `0x0800766c` selects PA15 AF6 and a nominal 2 kHz, 20% TIM14 drive. |
| Programming clock | `0x08008f64` configures the 64 MHz PLL, analog timing and flash wait state. |
| FCI | Factory `0x08002ff0` identifies the key and busy registers. The same-family sample confirms aligned-word program and page erase. |

[Sources](sources.md) · [Board architecture](status.md)
