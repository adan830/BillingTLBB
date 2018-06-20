# TLBB.Billing

## Installation

### Normal users

- You can download executable binary in Release
- Import Sql file in `db` to your database (Must choose only one)

### Developers

- Install CMake (>=2.8)
- Install C++ compiler that support C++11 (Clang>=3.3 / GCC=>4.8 / VS >= 2012 / Msys2 GCC||Clang)
- Clone this repository to your computer
- Use CMake to parse the `CMakeLists.txt` to your compiler
- Compile it!

## Usage

Open `Config.ini` to configure the info
You can use one `Billing(.exe)` to open/stop itself

### In Linux

- To Open:
```
chmod +x Billing
./Billing
```

- To Stop:
```
./Billing stop
```

## License

MIT License
Copyright (c) Dark.Hades (a.k.a HaiLe)

