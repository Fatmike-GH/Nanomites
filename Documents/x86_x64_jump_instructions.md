# x86 Conditional and Unconditional Jump Instructions

| Instruction | Description | Signedness | Flags | Opcode (short / near) | Length (short / near) |
|--------------|--------------|-------------|--------|-----------------------|------------------------|
| JO | Jump if overflow | – | OF = 1 | 70 / 0F 80 | 2 / 6 |
| JNO | Jump if not overflow | – | OF = 0 | 71 / 0F 81 | 2 / 6 |
| JS | Jump if sign | – | SF = 1 | 78 / 0F 88 | 2 / 6 |
| JNS | Jump if not sign | – | SF = 0 | 79 / 0F 89 | 2 / 6 |
| JE / JZ | Jump if equal / zero | – | ZF = 1 | 74 / 0F 84 | 2 / 6 |
| JNE / JNZ | Jump if not equal / not zero | – | ZF = 0 | 75 / 0F 85 | 2 / 6 |
| JB / JNAE / JC | Jump if below / not above or equal / carry | Unsigned | CF = 1 | 72 / 0F 82 | 2 / 6 |
| JNB / JAE / JNC | Jump if not below / above or equal / not carry | Unsigned | CF = 0 | 73 / 0F 83 | 2 / 6 |
| JBE / JNA | Jump if below or equal / not above | Unsigned | CF = 1 or ZF = 1 | 76 / 0F 86 | 2 / 6 |
| JA / JNBE | Jump if above / not below or equal | Unsigned | CF = 0 and ZF = 0 | 77 / 0F 87 | 2 / 6 |
| JL / JNGE | Jump if less / not greater or equal | Signed | SF ≠ OF | 7C / 0F 8C | 2 / 6 |
| JGE / JNL | Jump if greater or equal / not less | Signed | SF = OF | 7D / 0F 8D | 2 / 6 |
| JLE / JNG | Jump if less or equal / not greater | Signed | ZF = 1 or SF ≠ OF | 7E / 0F 8E | 2 / 6 |
| JG / JNLE | Jump if greater / not less or equal | Signed | ZF = 0 and SF = OF | 7F / 0F 8F | 2 / 6 |
| JP / JPE | Jump if parity even | – | PF = 1 | 7A / 0F 8A | 2 / 6 |
| JNP / JPO | Jump if parity odd | – | PF = 0 | 7B / 0F 8B | 2 / 6 |
| JCXZ | Jump if CX = 0 | – | – | E3 | 2 |
| JECXZ | Jump if ECX = 0 | – | – | E3 | 2 |
| **JMP_S** | **Unconditional short jump** | – | – | **EB** | **2** |
| **JMP_N** | **Unconditional near jump** | – | – | **E9** | **5** |

> **x86 notes:**  
> - Conditional jumps: short = 2 bytes, near = 6 bytes.  
> - Unconditional jumps: JMP_S (short) = 2 bytes, JMP_N (near) = 5 bytes.  
> - All jumps are **relative**, except for far-jump forms not listed here.

---

# x64 Conditional and Unconditional Jump Instructions

| Instruction | Description | Signedness | Flags | Opcode (short / near) | Length (short / near) |
|--------------|--------------|-------------|--------|-----------------------|------------------------|
| JO | Jump if overflow | – | OF = 1 | 70 / 0F 80 | 2 / 6 |
| JNO | Jump if not overflow | – | OF = 0 | 71 / 0F 81 | 2 / 6 |
| JS | Jump if sign | – | SF = 1 | 78 / 0F 88 | 2 / 6 |
| JNS | Jump if not sign | – | SF = 0 | 79 / 0F 89 | 2 / 6 |
| JE / JZ | Jump if equal / zero | – | ZF = 1 | 74 / 0F 84 | 2 / 6 |
| JNE / JNZ | Jump if not equal / not zero | – | ZF = 0 | 75 / 0F 85 | 2 / 6 |
| JB / JNAE / JC | Jump if below / not above or equal / carry | Unsigned | CF = 1 | 72 / 0F 82 | 2 / 6 |
| JNB / JAE / JNC | Jump if not below / above or equal / not carry | Unsigned | CF = 0 | 73 / 0F 83 | 2 / 6 |
| JBE / JNA | Jump if below or equal / not above | Unsigned | CF = 1 or ZF = 1 | 76 / 0F 86 | 2 / 6 |
| JA / JNBE | Jump if above / not below or equal | Unsigned | CF = 0 and ZF = 0 | 77 / 0F 87 | 2 / 6 |
| JL / JNGE | Jump if less / not greater or equal | Signed | SF ≠ OF | 7C / 0F 8C | 2 / 6 |
| JGE / JNL | Jump if greater or equal / not less | Signed | SF = OF | 7D / 0F 8D | 2 / 6 |
| JLE / JNG | Jump if less or equal / not greater | Signed | ZF = 1 or SF ≠ OF | 7E / 0F 8E | 2 / 6 |
| JG / JNLE | Jump if greater / not less or equal | Signed | ZF = 0 and SF = OF | 7F / 0F 8F | 2 / 6 |
| JP / JPE | Jump if parity even | – | PF = 1 | 7A / 0F 8A | 2 / 6 |
| JNP / JPO | Jump if parity odd | – | PF = 0 | 7B / 0F 8B | 2 / 6 |
| JCXZ | Jump if CX = 0 | – | – | E3 | 2 |
| JECXZ | Jump if ECX = 0 | – | – | E3 | 2 |
| **JRCXZ** | **Jump if RCX = 0 (x64 only)** | – | – | **E3** | **2** |
| **JMP_S** | **Unconditional short jump** | – | – | **EB** | **2** |
| **JMP_N** | **Unconditional near jump** | – | – | **E9** | **5** |

> **x64 notes:**  
> - Encodings identical to x86.  
> - No far jumps in long mode — all are **relative**.  
> - `JRCXZ` is unique to x64, checking 64-bit RCX.
