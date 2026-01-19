# Paper154 e-paper display module (EN)

> **Minimum valid version: 0.1.1**

This module adds support for a 1.54" 200x200 e-paper display (GDEH0154D67 compatible) using the GxEPD2 library.

## Features
- Initialization and clearing of the e-paper display
- Centered and positioned text display
- Partial refresh support

## Pinout (default)
- CS: 15
- DC: 17
- RST: 16
- BUSY: 4

## Usage
```
#include "paper154.h"

void setup() {
  Paper154::begin();
  Paper154::printCenter("Hello e-paper!");
}
```

## Documentation
- All functions are documented in the header file.
- See ModularDocs/paper154.md for detailed usage and advanced features.

---

> **This document is valid from version 0.1.1.**
