# 🅿️ Parking System

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Threading](https://img.shields.io/badge/Threading-Enabled-green?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge)

> **Real-time parking lot management system with employee discounts**

![Demo](https://img.shields.io/badge/Status-Working-brightgreen?style=flat-square)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey?style=flat-square)

## ✨ Features

 **Real-time clock** - Live time display with threading  
 **Employee discounts** - Automatic rate reduction for staff  
 **File-based config** - Easy discount management via text file  
 **Entry/Exit tracking** - Same code for check-in and check-out  
 **Fast simulation** - 1 real minute = 1 parking hour  

## 🚀 Quick Start

```bash
# Clone & navigate
git clone https://github.com/HectorPeralta12/Parking_system_cpp.git
cd parking-system-cpp

# Compile
g++ -std=c++11 -pthread main.cpp -o estacionamiento

# Run
./estacionamiento
```

## 🎮 Usage
1. **Press Enter** at startup to begin
2. **Enter vehicle code** → Records entry time
3. **Same code again** → Calculates cost & processes exit
4. **Employee codes** → Automatic discount applied
5. **Press Enter** after each operation to continue

**Try these codes:**
- `ABC123` - Regular customer
- `EMP001` - Employee (50% discount)

## 📊 Demo Output

```
🕐 Current time: 14:23:45 | Vehicles in parking: 1

=== PROCESSING CODE: EMP001 ===
🚗 EXIT registered
Time parked: 2.60 hours
Base cost: $65.00
Applied discount: 50%
Savings: $32.50
💰 TOTAL TO PAY: $32.50
```

## 📁 Project Structure

```
parking_system_cpp/
├── main.cpp          # Main application
├── discounts.txt     # Employee discount rates
├── README.md         # This file
└── .gitignore        # Git ignore rules
```

## 🛠️ Technical Details

![C++11](https://img.shields.io/badge/C%2B%2B-11-blue?style=flat-square)
![Threading](https://img.shields.io/badge/std%3A%3Athread-Yes-green?style=flat-square)
![Mutex](https://img.shields.io/badge/std%3A%3Amutex-Yes-green?style=flat-square)

- **Language:** C++11
- **Threading:** `std::thread`, `std::mutex`
- **Classes:** OOP with friend functions
- **File I/O:** Discount configuration management

## 📋 Requirements

- C++11 compatible compiler (g++, clang++)
- pthread support
- Terminal/Console environment

---

<div align="center">

**⭐ Star this repo if you found it helpful!**


</div>
