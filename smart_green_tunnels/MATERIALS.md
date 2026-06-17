# Materials List / Bill of Materials

> Compiled from the components and quantities described in the project
> report (Sections 5.2-5.4 and the test photos). No standalone parts list
> was included in the original report, so this was reconstructed from
> context. Double-check quantities/specs against your physical build and
> edit freely.

## Mechanical / Structural
| Item | Qty | Notes |
|---|---|---|
| 3mm transparent acrylic sheet | 2 (base + floating upper track) | Laser-cut |
| Guide pins / alignment shafts | 2-4 | Prevents horizontal twisting under off-center hits |
| High-density rubber force-concentration buttons | 4 | One centered over each piezo disc |
| High-strength adhesive | 1 tube/bottle | Bonds discs to the acrylic base plate |

## Electrical - Harvesting & Conditioning
| Item | Qty | Notes |
|---|---|---|
| 35mm piezoelectric ceramic (PZT) discs | 4 | Wired in parallel |
| 1N4007 diodes | 4 | Full-wave bridge rectifier |
| Electrolytic capacitor | 1 x 1000µF | Smoothing/filtration |
| Zener diode | 1 x 5.1V | Over-voltage clamp |
| Load resistor | 1 x 1kΩ | Implied by the P = V²/R figures in Table 6.1 |
| Prototype/perf board | 1 | Houses the conditioning circuit |

## Electrical - Compute & Display
| Item | Qty | Notes |
|---|---|---|
| Arduino UNO (ATmega328P) | 1 | Edge controller |
| 16x2 LCD with I2C backpack module | 1 | I2C address typically 0x27 or 0x3F |
| Breadboard | 1 | |
| Jumper wires (M-M) | ~20-30 | |
| USB cable (Type-B) | 1 | Power + programming |

## Software
| Item | Notes |
|---|---|
| Arduino IDE | Sketch was built/tested on v2.3.x |
| `LiquidCrystal_I2C` library | Install via Library Manager for the I2C LCD |
| `Wire` library | Bundled with Arduino IDE |
