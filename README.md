# Ventilator Prototype

# About the project

This project presents sample code for a ventilator prototype developed by a team of engineers at the Technical University of Kenya in 2020 during the Covid-19 pandemic. This is not the actual code for the ventilator but a demonstration of how a ventilator state machine controller can be developed.

The ventilator provides the following functionalities:
- set the breaths per minutes (BPM)
- monitor air pressure

The project was tested on an STM32 NUCLEO-F446RE development board.

## Built with:
 - C++
 - C
 - Arm Mbed

# Getting Started

## Prerequisites  

- Arm Mbed or Arm Keil Studio or similar environment
- Mbed compatible development board
- Other hardware such as motors, sensors and displays

## Installation
1. Clone the repo with to e.g., Arm Keil Studio with:
```
git clone https://github.com/denis-mutuma/ventilator-prototype.git
```

2. Make changes to match your board such as GPIO pin numbers etc.,
   
3. This code can be converted to run on an Arduino with relative ease.