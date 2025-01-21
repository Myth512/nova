# YAPI: Yet Another Python Interpreter

YAPI is an open-source implementation of the Python 3 programming language. Originally conceived as a custom language project, it has since evolved into a Python interpreter with the goal of replicating Python 3's syntax and behavior while providing a platform for learning and experimentation in interpreter design.

## Features
* Python 3 Compatibility: YAPI strives to fully support Python 3 syntax and core features, with the long-term goal of achieving compatibility with the official CPython implementation.
* Open Source: Released under the GPL-3.0 license for transparency and collaboration.
* Work in Progress: Many features are still under development. This project is actively evolving, and contributions or issue reports are welcome!

## Differences from CPython

YAPI is not a drop-in replacement for CPython, and there are a few notable differences:

* No C API Support: YAPI does not currently implement CPython's C API, and there are no immediate plans to support it. This means libraries that rely on C extensions, such as numpy or pandas, will not work out of the box.
* Custom Backend: The interpreter's backend is built from scratch, which could result in performance and behavior differences compared to CPython.
* Limited Standard Library: Many parts of Python’s standard library are yet to be implemented.

These differences mean that YAPI is primarily a tool for experimentation and learning, rather than a replacement for CPython.

## Getting Started

To build and run YAPI:

1. Clone the repository:
```
git clone https://github.com/myth512/yapi.git  
cd yapi
```
2. Build
``` 
make build
```

## Current State

* The interpreter supports dynamic typing, basic expressions, and function calls.
* Features like iterators, exception handling, and Python’s full standard library are under development.
* Python 3 syntax support is evolving, with many core features already implemented.

## Contribution Policy

YAPI is a personal project, and external contributions are not currently being accepted. However, issue reports are welcome! If you encounter any bugs or have suggestions, feel free to open an issue.

## License

This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.