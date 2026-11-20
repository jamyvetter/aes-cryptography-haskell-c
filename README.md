# AES Cryptosystem: From Abstract Algebra to C

> **High-assurance implementation of the Advanced Encryption Standard (AES / FIPS 197) developed from first algebraic principles in Haskell, paired with a high-performance C engine and cryptographic diffusion analysis.**

---

## 📌 Context & Authors

- **Institution**: Grenoble INP–Esisar
- **Course**: PX222 Maths-Info (Algebra & Cryptography)
- **Authors**: [Jamyson Vetter](https://github.com/jamyvetter) & [Siméon Studer](https://github.com/hiiiiiiiiiboux)
- **Technical Report**: Available on [jamyvetter.net](https://jamyvetter.net/projects/aes-cryptography/)

---

## 🏛️ Architecture Overview

The system bridges abstract mathematical theory with concrete cryptographic software across two complementary implementations:

### 1. Haskell Engine (Abstract Algebra & Provable Correctness)

Built in four decoupled abstraction layers:

1. **Foundational Algebra (`Anneau.hs`, `Z2.hs`, `Z3.hs`)**: Typeclasses defining rings (`Anneau`) and fields (`Corps`) with strict mathematical properties.
2. **Polynomial Arithmetic (`Polynome.hs`)**: Generic Euclidean division (`de_pol`), addition, and multiplication over arbitrary coefficient rings.
3. **Galois Field $\text{GF}(2^8)$ & Euclid (`GF28.hs`, `Euclide.hs`)**: Modular arithmetic modulo the irreducible polynomial $m(x) = x^8 + x^4 + x^3 + x + 1$ and Extended Euclidean algorithm for modular inversion.
4. **AES Algorithm (`AES.hs`, `SBox.hs`, `Main.hs`)**: Round transformations (`SubBytes`, `ShiftRows`, `MixColumns`, `AddRoundKey`) and inverse operations across AES-128, AES-192, and AES-256.

### 2. C Engine (Performance, Block Modes & Diffusion Metrics)

Located in `AES_en_c/`:

- **Block Modes**: Electronic Codebook (ECB) vs. Cipher Block Chaining (CBC).
- **Bitmap Image Encryption**: Visual demonstration of ECB pattern leakage versus CBC pseudorandom diffusion on image headers and pixel arrays (e.g., Tux bitmap benchmark).
- **Shannon Entropy Analysis (`entropie.c`)**: Quantitative measurement of ciphertext randomness approaching theoretical maximum entropy ($> 7.999$ bits/byte).

---

## 🧪 Verification & NIST FIPS 197 Compliance

Automated GHCi test benches validate intermediate state arrays and full cipher cycles against official NIST test vectors:

- `tests_anneaux.ghci`: Ring and field axioms.
- `tests_fonctions_AES.ghci`: Unit testing of individual round transformations against NIST Appendix B.
- `tests_chiffrement.ghci`: Round-trip encryption and decryption verification across all key lengths (128, 192, 256 bits).
- `tests_complets.ghci`: Full end-to-end regression test suite.

---

## 🚀 Quick Start

### Haskell (Interactive Test Suite)

```bash
ghci -ghci-script tests_complets.ghci
```

### C Engine (Compilation & Benchmarks)

```bash
cd AES_en_c
make
./test_aes
./test_ecb
./test_cbc
```

---

## 📄 Documentation

For complete mathematical derivations, architectural diagrams, and benchmarking methodology, consult the full technical specification report in the [jamyvetter.net Project Portfolio](https://jamyvetter.net/projects/aes-cryptography/).
