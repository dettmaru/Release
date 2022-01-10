EESchema Schematic File Version 2
LIBS:BaseCircuit_V1-rescue
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:stm32
LIBS:ftdi
LIBS:switches
LIBS:503398-1892
LIBS:AddedComponents
LIBS:LTC3440
LIBS:BaseCircuit_V2-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
Title "RhizoTech - Motherboard - MCUs"
Date "1.1.2018"
Rev "V2"
Comp "TH-Köln Institut für Nachrichtentechnik"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ESP32-WROOM U3
U 1 1 5A38EF9D
P 6250 4600
F 0 "U3" H 5550 5850 60  0000 C CNN
F 1 "ESP32-WROOM" H 6750 5850 60  0000 C CNN
F 2 "ESP32-footprints-Lib:ESP32-WROOM" H 6600 5950 60  0001 C CNN
F 3 "" H 5800 5050 60  0001 C CNN
F 4 "Watterott" H 6250 4600 60  0001 C CNN "Distributor"
F 5 "20160141" H 6250 4600 60  0001 C CNN "Ordercode"
F 6 "6" H 6250 4600 60  0001 C CNN "Cost"
	1    6250 4600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 5A38F9D7
P 5250 5350
F 0 "#PWR01" H 5250 5100 50  0001 C CNN
F 1 "GND" H 5250 5200 50  0000 C CNN
F 2 "" H 5250 5350 50  0001 C CNN
F 3 "" H 5250 5350 50  0001 C CNN
	1    5250 5350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 5A38FA9F
P 5750 5700
F 0 "#PWR02" H 5750 5450 50  0001 C CNN
F 1 "GND" H 5750 5550 50  0000 C CNN
F 2 "" H 5750 5700 50  0001 C CNN
F 3 "" H 5750 5700 50  0001 C CNN
	1    5750 5700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 5A38FAFC
P 7200 5300
F 0 "#PWR03" H 7200 5050 50  0001 C CNN
F 1 "GND" H 7200 5150 50  0000 C CNN
F 2 "" H 7200 5300 50  0001 C CNN
F 3 "" H 7200 5300 50  0001 C CNN
	1    7200 5300
	1    0    0    -1  
$EndComp
NoConn ~ 7150 4350
$Comp
L STM32L496Zx U2
U 1 1 5A4BE64D
P 2700 4200
F 0 "U2" H 2000 7850 60  0000 C CNN
F 1 "STM32L496Zx" H 2700 4250 60  0000 C CNN
F 2 "Housings_QFP:TQFP-144_20x20mm_Pitch0.5mm" H 2700 4200 60  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2240088.pdf" H 2700 4200 60  0001 C CNN
F 4 "Farnell" H 2700 4200 60  0001 C CNN "Distributor"
F 5 "2725141" H 2700 4200 60  0001 C CNN "Ordercode"
F 6 "10,30" H 2700 4200 60  0001 C CNN "Cost"
	1    2700 4200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 5A4BE8BE
P 1700 7650
F 0 "#PWR04" H 1700 7400 50  0001 C CNN
F 1 "GND" H 1700 7500 50  0000 C CNN
F 2 "" H 1700 7650 50  0001 C CNN
F 3 "" H 1700 7650 50  0001 C CNN
	1    1700 7650
	0    1    1    0   
$EndComp
$Comp
L GND #PWR05
U 1 1 5A4BEB39
P 3800 7650
F 0 "#PWR05" H 3800 7400 50  0001 C CNN
F 1 "GND" H 3800 7500 50  0000 C CNN
F 2 "" H 3800 7650 50  0001 C CNN
F 3 "" H 3800 7650 50  0001 C CNN
	1    3800 7650
	0    -1   -1   0   
$EndComp
$Sheet
S 8550 4850 900  1550
U 5A5DB2CB
F0 "Powersupply" 60
F1 "Powersupply.sch" 60
F2 "D+_LOAD" B L 8550 4950 60 
F3 "D-_LOAD" B L 8550 5050 60 
F4 "VBATT_SENSE" O L 8550 6100 60 
F5 "LOAD_STAT1" O L 8550 5300 60 
F6 "LOAD_STAT2" O L 8550 5400 60 
F7 "LOAD_PG" O L 8550 5500 60 
F8 "ESP_ON" I L 8550 5600 60 
F9 "NRF_ON" I L 8550 5700 60 
F10 "ARDUINO_ON" I L 8550 5800 60 
F11 "ARDUINOIO_ON" I L 8550 5900 60 
F12 "SENSE_ON" I L 8550 6000 60 
F13 "VBAT_OK" O L 8550 6200 60 
F14 "MODE_SYNC" I L 8550 6300 60 
$EndSheet
$Comp
L Crystal Y1
U 1 1 5A64E4CF
P 750 1550
F 0 "Y1" H 750 1700 50  0000 C CNN
F 1 "32.768kHz" H 750 1400 50  0000 C CNN
F 2 "Crystals:Crystal_C26-LF_d2.1mm_l6.5mm_Horizontal" H 750 1550 50  0001 C CNN
F 3 "http://www.ramitechnology.com/products/spec/crystal/crystal-r38_26_145.pdf" H 750 1550 50  0001 C CNN
F 4 "Farnell" H 750 1550 60  0001 C CNN "Distributor"
F 5 "1652573" H 750 1550 60  0001 C CNN "Ordercode"
F 6 "0,187" H 750 1550 60  0001 C CNN "Cost"
	1    750  1550
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5A64E6A0
P 600 2150
F 0 "C3" H 625 2250 50  0000 L CNN
F 1 "4.7pF" H 625 2050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 638 2000 50  0001 C CNN
F 3 "" H 600 2150 50  0000 C CNN
F 4 "Farnell" H 600 2150 60  0001 C CNN "Distributor"
F 5 "2627469" H 600 2150 60  0001 C CNN "Ordercode"
F 6 "0,0199" H 600 2150 60  0001 C CNN "Cost"
	1    600  2150
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 5A64E6E0
P 950 2000
F 0 "C4" H 975 2100 50  0000 L CNN
F 1 "4.7pF" H 975 1900 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 988 1850 50  0001 C CNN
F 3 "" H 950 2000 50  0000 C CNN
F 4 "Farnell" H 950 2000 60  0001 C CNN "Distributor"
F 5 "2627469" H 950 2000 60  0001 C CNN "Ordercode"
F 6 "0,0199" H 950 2000 60  0001 C CNN "Cost"
	1    950  2000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 5A64E960
P 600 2400
F 0 "#PWR06" H 600 2150 50  0001 C CNN
F 1 "GND" H 600 2250 50  0000 C CNN
F 2 "" H 600 2400 50  0000 C CNN
F 3 "" H 600 2400 50  0000 C CNN
	1    600  2400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 5A64E990
P 950 2400
F 0 "#PWR07" H 950 2150 50  0001 C CNN
F 1 "GND" H 950 2250 50  0000 C CNN
F 2 "" H 950 2400 50  0000 C CNN
F 3 "" H 950 2400 50  0000 C CNN
	1    950  2400
	1    0    0    -1  
$EndComp
Text Label 5300 4100 2    60   ~ 0
ESP_RESET
Text Label 7150 5050 0    60   ~ 0
ESP_GPIO0
Text Label 7150 4050 0    60   ~ 0
ESP_TXD0
Text Label 7150 4150 0    60   ~ 0
ESP_RXD0
Text Label 3750 3850 0    60   ~ 0
SWDIO
Text Label 3750 4250 0    60   ~ 0
SWCLK
Text Label 3750 6750 0    60   ~ 0
TRACESWO
Text Label 1500 3050 0    60   ~ 0
NRST
$Comp
L CONN_01X08 J9
U 1 1 5A68C3A5
P 7650 2150
F 0 "J9" H 7650 2600 50  0000 C CNN
F 1 "CONN_01X08" V 7750 2150 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x08_Pitch2.54mm" H 7650 2150 50  0001 C CNN
F 3 "" H 7650 2150 50  0001 C CNN
	1    7650 2150
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X10 J10
U 1 1 5A68C417
P 7650 1150
F 0 "J10" H 7650 1700 50  0000 C CNN
F 1 "CONN_01X10" V 7750 1150 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x10_Pitch2.54mm" H 7650 1150 50  0001 C CNN
F 3 "" H 7650 1150 50  0001 C CNN
	1    7650 1150
	1    0    0    -1  
$EndComp
Text Label 7100 900  0    60   ~ 0
VREF
$Comp
L GND #PWR08
U 1 1 5A68DFE9
P 6700 1000
F 0 "#PWR08" H 6700 750 50  0001 C CNN
F 1 "GND" H 6700 850 50  0000 C CNN
F 2 "" H 6700 1000 50  0001 C CNN
F 3 "" H 6700 1000 50  0001 C CNN
	1    6700 1000
	0    1    1    0   
$EndComp
Text Label 7100 700  0    60   ~ 0
ADC5
Text Label 7100 800  0    60   ~ 0
ADC4
$Comp
L CONN_02X40 J11
U 1 1 5A693A20
P 10500 2650
F 0 "J11" H 10500 4700 50  0000 C CNN
F 1 "CONN_02X40" V 10500 2650 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_2x40_Pitch2.54mm" H 10500 2650 50  0001 C CNN
F 3 "http://katalog.we-online.de/em/datasheet/6130xx21821.pdf" H 10500 2650 50  0001 C CNN
F 4 "Farnell" H 10500 2650 60  0001 C CNN "Distributor"
F 5 "2827916" H 10500 2650 60  0001 C CNN "Ordercode"
F 6 "1,68" H 10500 2650 60  0001 C CNN "Cost"
	1    10500 2650
	-1   0    0    -1  
$EndComp
$Comp
L CONN_02X40 J12
U 1 1 5A693E13
P 8700 2650
F 0 "J12" H 8700 4700 50  0000 C CNN
F 1 "CONN_02X40" V 8700 2650 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_2x40_Pitch2.54mm" H 8700 2650 50  0001 C CNN
F 3 "http://katalog.we-online.de/em/datasheet/6130xx21821.pdf" H 8700 2650 50  0001 C CNN
F 4 "Farnell" H 8700 2650 60  0001 C CNN "Distributor"
F 5 "2827916" H 8700 2650 60  0001 C CNN "Ordercode"
F 6 "1,68" H 8700 2650 60  0001 C CNN "Cost"
	1    8700 2650
	-1   0    0    -1  
$EndComp
Text Notes 8750 550  0    60   ~ 0
Connectors to other extension boards
Text Label 1500 650  0    60   ~ 0
PE2
Text Label 1500 750  0    60   ~ 0
PE3
Text Label 1500 850  0    60   ~ 0
PE4
Text Label 1500 950  0    60   ~ 0
PE5
Text Label 1500 1050 0    60   ~ 0
PE6
Text Label 1500 1250 0    60   ~ 0
PC13
Text Label 1500 1550 0    60   ~ 0
PF0
Text Label 1500 1650 0    60   ~ 0
PF1
Text Label 1500 1750 0    60   ~ 0
PF2
Text Label 1500 1850 0    60   ~ 0
PF3
Text Label 1500 1950 0    60   ~ 0
PF4
Text Label 1500 2050 0    60   ~ 0
PF5
Text Label 1500 2350 0    60   ~ 0
PF6
Text Label 1500 2450 0    60   ~ 0
PF7
Text Label 1500 2550 0    60   ~ 0
PF8
Text Label 1500 2650 0    60   ~ 0
PF9
Text Label 1500 2750 0    60   ~ 0
PF10
Text Label 1500 2850 0    60   ~ 0
PH0
Text Label 1500 2950 0    60   ~ 0
PH1
Text Label 1500 3150 0    60   ~ 0
PC0
Text Label 1500 3250 0    60   ~ 0
PC1
Text Label 1500 3350 0    60   ~ 0
PC2
Text Label 1500 3450 0    60   ~ 0
PC3
Text Label 1500 3950 0    60   ~ 0
PA0
Text Label 1500 4050 0    60   ~ 0
PA1
Text Label 1500 4150 0    60   ~ 0
PA2
Text Label 1500 4250 0    60   ~ 0
PA3
Text Label 1500 4550 0    60   ~ 0
PA4
Text Label 1500 4650 0    60   ~ 0
PA5
Text Label 1500 4750 0    60   ~ 0
PA6
Text Label 1500 4850 0    60   ~ 0
PA7
Text Label 1500 4950 0    60   ~ 0
PC4
Text Label 1500 5050 0    60   ~ 0
PC5
Text Label 1500 5150 0    60   ~ 0
PB0
Text Label 1500 5250 0    60   ~ 0
PB1
Text Label 1500 5350 0    60   ~ 0
PB2
Text Label 1500 5450 0    60   ~ 0
PF11
Text Label 1500 5550 0    60   ~ 0
PF12
Text Label 1500 5850 0    60   ~ 0
PF13
Text Label 1500 5950 0    60   ~ 0
PF14
Text Label 1500 6050 0    60   ~ 0
PF15
Text Label 1500 6150 0    60   ~ 0
PG0
Text Label 1500 6250 0    60   ~ 0
PG1
Text Label 1500 6350 0    60   ~ 0
PE7
Text Label 1500 6450 0    60   ~ 0
PE8
Text Label 1500 6550 0    60   ~ 0
PE9
Text Label 1500 6850 0    60   ~ 0
PE10
Text Label 1500 6950 0    60   ~ 0
PE11
Text Label 1500 7050 0    60   ~ 0
PE12
Text Label 1500 7150 0    60   ~ 0
PE13
Text Label 1500 7250 0    60   ~ 0
PE14
Text Label 1500 7350 0    60   ~ 0
PE15
Text Label 1500 7450 0    60   ~ 0
PB10
Text Label 1500 7550 0    60   ~ 0
PB11
Text Label 3750 650  0    60   ~ 0
PB12
Text Label 3750 750  0    60   ~ 0
PB13
Text Label 3750 850  0    60   ~ 0
PB14
Text Label 3750 950  0    60   ~ 0
PB15
Text Label 3750 1050 0    60   ~ 0
PD8
Text Label 3750 1150 0    60   ~ 0
PD9
Text Label 3750 1250 0    60   ~ 0
PD10
Text Label 3750 1350 0    60   ~ 0
PD11
Text Label 3750 1450 0    60   ~ 0
PD12
Text Label 3750 1550 0    60   ~ 0
PD13
Text Label 3750 1850 0    60   ~ 0
PD14
Text Label 3750 1950 0    60   ~ 0
PD15
Text Label 3750 2050 0    60   ~ 0
PG2
Text Label 3750 2150 0    60   ~ 0
PG3
Text Label 3750 2250 0    60   ~ 0
PG4
Text Label 3750 2350 0    60   ~ 0
PG5
Text Label 3750 2450 0    60   ~ 0
PG6
Text Label 3750 2550 0    60   ~ 0
PG7
Text Label 3750 2650 0    60   ~ 0
PG8
Text Label 3750 2950 0    60   ~ 0
PC6
Text Label 3750 3050 0    60   ~ 0
PC7
Text Label 3750 3150 0    60   ~ 0
PC8
Text Label 3750 3250 0    60   ~ 0
PC9
Text Label 3750 3350 0    60   ~ 0
PA8
Text Label 3750 3450 0    60   ~ 0
PA9
Text Label 3750 3550 0    60   ~ 0
PA10
Text Label 3750 4350 0    60   ~ 0
PA15
Text Label 3750 4450 0    60   ~ 0
PC10
Text Label 3750 4550 0    60   ~ 0
PC11
Text Label 3750 4650 0    60   ~ 0
PC12
Text Label 3750 4750 0    60   ~ 0
PD0
Text Label 3750 4850 0    60   ~ 0
PD1
Text Label 3750 4950 0    60   ~ 0
PD2
Text Label 3750 5050 0    60   ~ 0
PD3
Text Label 3750 5150 0    60   ~ 0
PD4
Text Label 3750 5250 0    60   ~ 0
PD5
Text Label 3750 5550 0    60   ~ 0
PD6
Text Label 3750 5650 0    60   ~ 0
PD7
Text Label 3750 5750 0    60   ~ 0
PG9
Text Label 3750 5850 0    60   ~ 0
PG10
Text Label 3750 5950 0    60   ~ 0
PG11
Text Label 3750 6050 0    60   ~ 0
PG12
Text Label 3750 6150 0    60   ~ 0
PG13
Text Label 3750 6250 0    60   ~ 0
PG14
Text Label 3750 6550 0    60   ~ 0
PG15
Text Label 3750 6650 0    60   ~ 0
PB3
Text Label 3750 6850 0    60   ~ 0
PB5
Text Label 3750 6950 0    60   ~ 0
PB6
Text Label 3750 7050 0    60   ~ 0
PB7
Text Label 3750 7250 0    60   ~ 0
PB8
Text Label 3750 7350 0    60   ~ 0
PB9
Text Label 3750 7450 0    60   ~ 0
PE0
Text Label 3750 7550 0    60   ~ 0
PE1
$Comp
L +3.3V #PWR09
U 1 1 5A6A749B
P 8000 900
F 0 "#PWR09" H 8000 750 50  0001 C CNN
F 1 "+3.3V" H 8000 1040 50  0000 C CNN
F 2 "" H 8000 900 50  0001 C CNN
F 3 "" H 8000 900 50  0001 C CNN
	1    8000 900 
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR010
U 1 1 5A6A7708
P 9200 900
F 0 "#PWR010" H 9200 650 50  0001 C CNN
F 1 "GND" H 9200 750 50  0000 C CNN
F 2 "" H 9200 900 50  0001 C CNN
F 3 "" H 9200 900 50  0001 C CNN
	1    9200 900 
	0    -1   -1   0   
$EndComp
Text Label 8450 1000 2    60   ~ 0
PE1
Text Label 8950 1000 0    60   ~ 0
PE0
Text Label 8450 1100 2    60   ~ 0
PB9
Text Label 8950 1100 0    60   ~ 0
PB8
Text Label 8950 1200 0    60   ~ 0
PB6
Text Label 8450 1300 2    60   ~ 0
PB5
Text Label 8950 1300 0    60   ~ 0
PB3
Text Label 8450 1400 2    60   ~ 0
PG15
Text Label 8950 1400 0    60   ~ 0
PG14
Text Label 8950 1500 0    60   ~ 0
PG12
Text Label 8950 1600 0    60   ~ 0
PG10
Text Label 8450 1500 2    60   ~ 0
PG13
Text Label 8450 1600 2    60   ~ 0
PG11
Text Label 8450 1700 2    60   ~ 0
PG9
Text Label 8950 1700 0    60   ~ 0
PD7
Text Label 8450 1800 2    60   ~ 0
PD6
Text Label 8950 1800 0    60   ~ 0
PD5
Text Label 8950 1900 0    60   ~ 0
PD3
Text Label 8950 2000 0    60   ~ 0
PD1
Text Label 8450 2100 2    60   ~ 0
PD0
Text Label 8450 2000 2    60   ~ 0
PD2
Text Label 8450 1900 2    60   ~ 0
PD4
Text Label 8950 2100 0    60   ~ 0
PC12
Text Label 8450 2200 2    60   ~ 0
PC11
Text Label 8950 2200 0    60   ~ 0
PC10
Text Label 8450 2300 2    60   ~ 0
PA15
Text Label 8950 2300 0    60   ~ 0
PA10
Text Label 8450 2400 2    60   ~ 0
PA9
Text Label 8950 2400 0    60   ~ 0
PA8
Text Label 8450 2500 2    60   ~ 0
PC9
Text Label 8950 2500 0    60   ~ 0
PC8
Text Label 8450 2600 2    60   ~ 0
PC7
Text Label 8950 2600 0    60   ~ 0
PC6
Text Label 8450 2700 2    60   ~ 0
PG8
Text Label 8950 2700 0    60   ~ 0
PG7
Text Label 8450 2800 2    60   ~ 0
PG6
Text Label 8450 2900 2    60   ~ 0
PG4
Text Label 8450 3000 2    60   ~ 0
PG2
Text Label 8950 2900 0    60   ~ 0
PG3
Text Label 8950 2800 0    60   ~ 0
PG5
Text Label 8950 3000 0    60   ~ 0
PD15
Text Label 8450 3100 2    60   ~ 0
PD14
Text Label 8950 3100 0    60   ~ 0
PD13
Text Label 8950 3200 0    60   ~ 0
PD11
Text Label 8950 3300 0    60   ~ 0
PD9
Text Label 8450 3400 2    60   ~ 0
PD8
Text Label 8450 3300 2    60   ~ 0
PD10
Text Label 8450 3200 2    60   ~ 0
PD12
Text Label 8950 3400 0    60   ~ 0
PB15
Text Label 8950 3500 0    60   ~ 0
PB13
Text Label 8450 3600 2    60   ~ 0
PB12
Text Label 8450 3500 2    60   ~ 0
PB14
Text Label 8450 1200 2    60   ~ 0
PB7
NoConn ~ 10100 700 
Text Notes 9950 950  0    60   ~ 0
RESET
Text Notes 9500 1150 0    60   ~ 0
5V not available
NoConn ~ 10250 1100
$Comp
L GND #PWR011
U 1 1 5A68F55E
P 10100 1200
F 0 "#PWR011" H 10100 950 50  0001 C CNN
F 1 "GND" H 10100 1050 50  0000 C CNN
F 2 "" H 10100 1200 50  0001 C CNN
F 3 "" H 10100 1200 50  0001 C CNN
	1    10100 1200
	0    1    1    0   
$EndComp
$Comp
L +5VL #PWR012
U 1 1 5A6903E6
P 10000 1400
F 0 "#PWR012" H 10000 1250 50  0001 C CNN
F 1 "+5VL" H 10000 1540 50  0000 C CNN
F 2 "" H 10000 1400 50  0001 C CNN
F 3 "" H 10000 1400 50  0001 C CNN
	1    10000 1400
	0    -1   -1   0   
$EndComp
Text Label 10250 2100 2    60   ~ 0
PC0
Text Label 10250 2000 2    60   ~ 0
PC1
Text Label 10750 700  0    60   ~ 0
PE2
Text Label 10750 800  0    60   ~ 0
PE3
Text Label 10750 900  0    60   ~ 0
PE4
Text Label 10750 1000 0    60   ~ 0
PE5
Text Label 10750 1100 0    60   ~ 0
PE6
Text Label 10750 1200 0    60   ~ 0
PC13
Text Label 10750 1300 0    60   ~ 0
PF0
Text Label 10750 1400 0    60   ~ 0
PF1
Text Label 10750 1500 0    60   ~ 0
PF2
Text Label 10250 1500 2    60   ~ 0
PF3
Text Label 10750 1600 0    60   ~ 0
PF4
Text Label 10250 1600 2    60   ~ 0
PF5
Text Label 10750 1700 0    60   ~ 0
PF6
Text Label 10250 1700 2    60   ~ 0
PF7
Text Label 10750 1800 0    60   ~ 0
PF8
Text Label 10250 1800 2    60   ~ 0
PF9
Text Label 10250 1900 2    60   ~ 0
PF10
Text Label 10750 1900 0    60   ~ 0
PH0
Text Label 10750 2000 0    60   ~ 0
PH1
Text Label 10750 2100 0    60   ~ 0
PC2
Text Label 10250 2200 2    60   ~ 0
PC3
Text Label 10750 2200 0    60   ~ 0
PA0
Text Label 10250 2300 2    60   ~ 0
PA1
Text Label 10750 2300 0    60   ~ 0
PA2
Text Label 10250 2400 2    60   ~ 0
PA3
Text Label 10750 2400 0    60   ~ 0
PA4
Text Label 10250 2500 2    60   ~ 0
PA5
Text Label 10750 2500 0    60   ~ 0
PA6
Text Label 10250 2600 2    60   ~ 0
PA7
Text Label 10750 2600 0    60   ~ 0
PC4
Text Label 10250 2700 2    60   ~ 0
PC5
Text Label 10750 2700 0    60   ~ 0
PB0
Text Label 10250 2800 2    60   ~ 0
PB1
Text Label 10750 2800 0    60   ~ 0
PB2
Text Label 10250 2900 2    60   ~ 0
PF11
Text Label 10750 2900 0    60   ~ 0
PF12
Text Label 10250 3000 2    60   ~ 0
PF13
Text Label 10750 3000 0    60   ~ 0
PF14
Text Label 10250 3100 2    60   ~ 0
PF15
Text Label 10750 3100 0    60   ~ 0
PG0
Text Label 10250 3200 2    60   ~ 0
PG1
Text Label 10750 3200 0    60   ~ 0
PE7
Text Label 10250 3300 2    60   ~ 0
PE8
Text Label 10750 3300 0    60   ~ 0
PE9
Text Label 10250 3400 2    60   ~ 0
PE10
Text Label 10750 3400 0    60   ~ 0
PE11
Text Label 10250 3500 2    60   ~ 0
PE12
Text Label 10750 3500 0    60   ~ 0
PE13
Text Label 10250 3600 2    60   ~ 0
PE14
Text Label 10750 3600 0    60   ~ 0
PE15
Text Label 10250 3700 2    60   ~ 0
PB10
Text Label 10750 3700 0    60   ~ 0
PB11
Text Label 5300 4200 2    60   ~ 0
SENSOR_VP
Text Label 5300 4300 2    60   ~ 0
SENSOR_VN
Text Label 5300 4400 2    60   ~ 0
IO34
Text Label 5300 4500 2    60   ~ 0
IO35
Text Label 5300 4600 2    60   ~ 0
IO32
Text Label 5300 4700 2    60   ~ 0
IO33
Text Label 5300 4800 2    60   ~ 0
IO25
Text Label 5300 4900 2    60   ~ 0
IO26
Text Label 5300 5000 2    60   ~ 0
IO27
Text Label 5300 5100 2    60   ~ 0
IO14
Text Label 5300 5200 2    60   ~ 0
IO12
Text Label 7150 3850 0    60   ~ 0
IO23
Text Label 7150 3950 0    60   ~ 0
IO22
Text Label 7150 4250 0    60   ~ 0
IO21
Text Label 7150 4450 0    60   ~ 0
IO19
Text Label 7150 4550 0    60   ~ 0
IO18
Text Label 7150 4650 0    60   ~ 0
IO5
Text Label 7150 4750 0    60   ~ 0
IO17
Text Label 7150 4850 0    60   ~ 0
IO16
Text Label 7150 4950 0    60   ~ 0
IO4
Text Label 5900 5650 3    60   ~ 0
IO13
Text Label 6000 5650 3    60   ~ 0
SD2
Text Label 6100 5650 3    60   ~ 0
SD3
Text Label 6200 5650 3    60   ~ 0
CMD
Text Label 6300 5650 3    60   ~ 0
CLK
Text Label 6400 5650 3    60   ~ 0
SDO
Text Label 6500 5650 3    60   ~ 0
SD1
Text Label 6600 5650 3    60   ~ 0
IO15
Text Label 6700 5650 3    60   ~ 0
IO2
$Comp
L GND #PWR013
U 1 1 5A6A4BC2
P 8400 4600
F 0 "#PWR013" H 8400 4350 50  0001 C CNN
F 1 "GND" H 8400 4450 50  0000 C CNN
F 2 "" H 8400 4600 50  0001 C CNN
F 3 "" H 8400 4600 50  0001 C CNN
	1    8400 4600
	0    1    1    0   
$EndComp
Text Label 8450 4500 2    60   ~ 0
ESP_RESET
Text Label 8450 4400 2    60   ~ 0
SENSOR_VN
Text Label 8450 4300 2    60   ~ 0
IO35
Text Label 8450 4200 2    60   ~ 0
IO33
Text Label 8950 4500 0    60   ~ 0
SENSOR_VP
Text Label 8950 4400 0    60   ~ 0
IO34
Text Label 8950 4300 0    60   ~ 0
IO32
Text Label 8950 4200 0    60   ~ 0
IO25
Text Label 8950 4100 0    60   ~ 0
IO27
Text Label 8950 4000 0    60   ~ 0
IO12
Text Label 8450 4100 2    60   ~ 0
IO26
Text Label 8450 4000 2    60   ~ 0
IO14
Text Label 8450 3900 2    60   ~ 0
IO13
Text Label 8950 3900 0    60   ~ 0
SD2
Text Label 8450 3800 2    60   ~ 0
SD3
Text Label 8950 3800 0    60   ~ 0
CMD
Text Label 8450 3700 2    60   ~ 0
CLK
Text Label 8950 3700 0    60   ~ 0
SDO
Text Label 8950 3600 0    60   ~ 0
SD1
Text Label 7100 1100 0    60   ~ 0
PG9
Text Label 7100 1200 0    60   ~ 0
PG10
Text Label 7100 1300 0    60   ~ 0
PG11
Text Label 7100 1400 0    60   ~ 0
PG12
Text Notes 6800 1100 0    60   ~ 0
SCK
Text Notes 6800 1200 0    60   ~ 0
MISO
Text Notes 6800 1300 0    60   ~ 0
MOSI
Text Notes 6800 1400 0    60   ~ 0
NSS
Text Label 7100 1600 0    60   ~ 0
PG14
Text Label 7100 1500 0    60   ~ 0
PG15
Text Label 7100 2400 0    60   ~ 0
PD5
Text Label 7100 2500 0    60   ~ 0
PD6
Text Notes 6800 2400 0    60   ~ 0
TxD
Text Notes 6800 2500 0    60   ~ 0
RxD
Text Label 7100 1800 0    60   ~ 0
PD7
Text Label 7100 1900 0    60   ~ 0
PD4
Text Label 7100 2000 0    60   ~ 0
PD3
Text Label 7100 2100 0    60   ~ 0
PD2
Text Label 7100 2200 0    60   ~ 0
PD1
Text Label 7100 2300 0    60   ~ 0
PD0
$Sheet
S 5450 550  850  2550
U 5A6E31D8
F0 "Interfaces" 60
F1 "Interfaces.sch" 60
F2 "PWR" I L 5450 650 60 
F3 "CLK" I L 5450 1050 60 
F4 "ESP_GPIO0" B L 5450 1650 60 
F5 "ESP_RESET" B L 5450 1750 60 
F6 "SWCLK" I L 5450 1900 60 
F7 "SWDIO" I L 5450 2000 60 
F8 "NRST" I L 5450 2100 60 
F9 "TRACESWO" I L 5450 2200 60 
F10 "NRF_CSN" I L 5450 2350 60 
F11 "NRF_MOSI" I L 5450 2450 60 
F12 "NRF_IRQ" O L 5450 2550 60 
F13 "NRF_MISO" O L 5450 2650 60 
F14 "NRF_SCK" I L 5450 2750 60 
F15 "NRF_CE" I L 5450 2850 60 
F16 "ESP_TxD0" I L 5450 1450 60 
F17 "ESP_RxD0" O L 5450 1550 60 
F18 "D2" B L 5450 750 60 
F19 "D3" B L 5450 850 60 
F20 "CMD" B L 5450 950 60 
F21 "D0" B L 5450 1150 60 
F22 "D1" B L 5450 1250 60 
$EndSheet
Text Label 4100 1550 0    60   ~ 0
SD_PWR
Text Label 4200 3150 0    60   ~ 0
SD_D0
Text Label 4200 3250 0    60   ~ 0
SD_D1
Text Label 4200 4450 0    60   ~ 0
SD_D2
Text Label 4200 4550 0    60   ~ 0
SD_D3
Text Label 4200 4650 0    60   ~ 0
SD_CLK
Text Label 4200 4950 0    60   ~ 0
SD_CMD
Text Label 4950 650  0    60   ~ 0
SD_PWR
Text Label 4950 750  0    60   ~ 0
SD_D2
Text Label 4950 850  0    60   ~ 0
SD_D3
Text Label 4950 950  0    60   ~ 0
SD_CMD
Text Label 4950 1050 0    60   ~ 0
SD_CLK
Text Label 4950 1150 0    60   ~ 0
SD_D0
Text Label 4950 1250 0    60   ~ 0
SD_D1
Text Label 7950 4950 0    60   ~ 0
D+_LOAD
Text Label 7950 5050 0    60   ~ 0
D-_LOAD
Text Label 3750 3650 0    60   ~ 0
D-_LOAD
Text Label 3750 3750 0    60   ~ 0
D+_LOAD
Text Label 4900 1450 0    60   ~ 0
ESP_TXD0
Text Label 4900 1550 0    60   ~ 0
ESP_RXD0
Text Label 4900 1650 0    60   ~ 0
ESP_GPIO0
Text Label 4900 1750 0    60   ~ 0
ESP_RESET
Text Label 1200 1050 0    60   ~ 0
BT1
Text Label 1200 1250 0    60   ~ 0
BT2
Text Label 4100 7450 0    60   ~ 0
LED1
Text Label 4100 7550 0    60   ~ 0
LED2
Text Label 4100 1850 0    60   ~ 0
LED3
Text Label 4100 1950 0    60   ~ 0
LED4
Text Label 7450 3850 0    60   ~ 0
LED7
Text Label 7450 3950 0    60   ~ 0
LED6
Text Label 7450 4250 0    60   ~ 0
LED5
Text Label 7400 4450 0    60   ~ 0
SE_SCK
Text Label 7400 4550 0    60   ~ 0
SE_MOSI
Text Label 7400 4650 0    60   ~ 0
SE_MISO
Text Label 7400 4750 0    60   ~ 0
SE_REA
Text Label 9800 4900 0    60   ~ 0
NRST
Text Label 9800 5000 0    60   ~ 0
LED1
Text Label 9800 5100 0    60   ~ 0
LED2
Text Label 9800 5200 0    60   ~ 0
LED3
Text Label 9800 5300 0    60   ~ 0
LED4
Text Label 9800 5400 0    60   ~ 0
LED5
Text Label 9800 5500 0    60   ~ 0
LED6
Text Label 9800 5600 0    60   ~ 0
LED7
Text Label 9800 5700 0    60   ~ 0
BT1
Text Label 9800 5800 0    60   ~ 0
BT2
Text Label 9800 5900 0    60   ~ 0
BT3
Text GLabel 4050 1700 2    60   Input ~ 0
STM3V3
Text GLabel 4050 2850 2    60   Input ~ 0
STM3V3
Text GLabel 4050 3950 2    60   Input ~ 0
STM3V3
Text GLabel 4050 4100 2    60   Input ~ 0
STM3V3
Text GLabel 4050 5450 2    60   Input ~ 0
STM3V3
Text GLabel 4050 6400 2    60   Input ~ 0
STM3V3
Text GLabel 4050 7700 2    60   Input ~ 0
STM3V3
$Comp
L GND #PWR014
U 1 1 5A7C6FF0
P 3800 6350
F 0 "#PWR014" H 3800 6100 50  0001 C CNN
F 1 "GND" H 3800 6200 50  0000 C CNN
F 2 "" H 3800 6350 50  0001 C CNN
F 3 "" H 3800 6350 50  0001 C CNN
	1    3800 6350
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR015
U 1 1 5A7C70D6
P 3800 5350
F 0 "#PWR015" H 3800 5100 50  0001 C CNN
F 1 "GND" H 3800 5200 50  0000 C CNN
F 2 "" H 3800 5350 50  0001 C CNN
F 3 "" H 3800 5350 50  0001 C CNN
	1    3800 5350
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR016
U 1 1 5A7C73A1
P 3800 4050
F 0 "#PWR016" H 3800 3800 50  0001 C CNN
F 1 "GND" H 3800 3900 50  0000 C CNN
F 2 "" H 3800 4050 50  0001 C CNN
F 3 "" H 3800 4050 50  0001 C CNN
	1    3800 4050
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR017
U 1 1 5A7C75AA
P 3800 2750
F 0 "#PWR017" H 3800 2500 50  0001 C CNN
F 1 "GND" H 3800 2600 50  0000 C CNN
F 2 "" H 3800 2750 50  0001 C CNN
F 3 "" H 3800 2750 50  0001 C CNN
	1    3800 2750
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR018
U 1 1 5A7C7814
P 3800 1650
F 0 "#PWR018" H 3800 1400 50  0001 C CNN
F 1 "GND" H 3800 1500 50  0000 C CNN
F 2 "" H 3800 1650 50  0001 C CNN
F 3 "" H 3800 1650 50  0001 C CNN
	1    3800 1650
	0    -1   -1   0   
$EndComp
Text GLabel 1450 7700 0    60   Input ~ 0
STM3V3
Text GLabel 1450 6700 0    60   Input ~ 0
STM3V3
Text GLabel 1450 5700 0    60   Input ~ 0
STM3V3
Text GLabel 1450 4400 0    60   Input ~ 0
STM3V3
Text GLabel 1450 2200 0    60   Input ~ 0
STM3V3
$Comp
L GND #PWR019
U 1 1 5A7D9F13
P 1700 2150
F 0 "#PWR019" H 1700 1900 50  0001 C CNN
F 1 "GND" H 1700 2000 50  0000 C CNN
F 2 "" H 1700 2150 50  0001 C CNN
F 3 "" H 1700 2150 50  0001 C CNN
	1    1700 2150
	0    1    1    0   
$EndComp
$Comp
L GND #PWR020
U 1 1 5A7DB017
P 1700 4350
F 0 "#PWR020" H 1700 4100 50  0001 C CNN
F 1 "GND" H 1700 4200 50  0000 C CNN
F 2 "" H 1700 4350 50  0001 C CNN
F 3 "" H 1700 4350 50  0001 C CNN
	1    1700 4350
	0    1    1    0   
$EndComp
$Comp
L GND #PWR021
U 1 1 5A7DB466
P 1700 5650
F 0 "#PWR021" H 1700 5400 50  0001 C CNN
F 1 "GND" H 1700 5500 50  0000 C CNN
F 2 "" H 1700 5650 50  0001 C CNN
F 3 "" H 1700 5650 50  0001 C CNN
	1    1700 5650
	0    1    1    0   
$EndComp
$Comp
L GND #PWR022
U 1 1 5A7DBC1E
P 1700 6650
F 0 "#PWR022" H 1700 6400 50  0001 C CNN
F 1 "GND" H 1700 6500 50  0000 C CNN
F 2 "" H 1700 6650 50  0001 C CNN
F 3 "" H 1700 6650 50  0001 C CNN
	1    1700 6650
	0    1    1    0   
$EndComp
Text GLabel 5200 3950 0    60   Input ~ 0
ESP3V3
Text Label 4900 2200 0    60   ~ 0
TRACESWO
Text Label 4900 2100 0    60   ~ 0
NRST
Text Label 4900 2000 0    60   ~ 0
SWDIO
Text Label 4900 1900 0    60   ~ 0
SWCLK
$Sheet
S 10100 4850 1050 1100
U 5A702D1F
F0 "UserInterface" 60
F1 "UserInterface.sch" 60
F2 "RESET" B L 10100 4900 60 
F3 "STM32_LED1" I L 10100 5000 60 
F4 "STM32_LED2" I L 10100 5100 60 
F5 "STM32_LED3" I L 10100 5200 60 
F6 "ESP32_LED5" I L 10100 5400 60 
F7 "ESP32_LED6" I L 10100 5500 60 
F8 "BT1" O L 10100 5700 60 
F9 "BT2" O L 10100 5800 60 
F10 "BT3" O L 10100 5900 60 
F11 "ESP32_LED7" I L 10100 5600 60 
F12 "STM32_LED4" I L 10100 5300 60 
$EndSheet
Text GLabel 10250 800  0    60   Input ~ 0
ARDUINO3V3
Text GLabel 10250 1000 0    60   Input ~ 0
ARDUINOIO3V3
Text Label 4100 650  0    60   ~ 0
SE_REA
Text Label 4100 750  0    60   ~ 0
SE_SCK
Text Label 4100 850  0    60   ~ 0
SE_MISO
Text Label 4100 950  0    60   ~ 0
SE_MOSI
Text Label 7950 5300 0    60   ~ 0
LOAD_STAT1
Text Label 7950 5400 0    60   ~ 0
LOAD_STAT2
Text Label 7950 5500 0    60   ~ 0
LOAD_PG
Text Label 7950 5600 0    60   ~ 0
ESP_ON
Text Label 7950 5700 0    60   ~ 0
NRF_ON
Text Label 7950 5800 0    60   ~ 0
ARD_ON
Text Label 7950 5900 0    60   ~ 0
ARDIO_ON
Text Label 1450 5450 2    60   ~ 0
ESP_ON
Text Label 1450 5550 2    60   ~ 0
NRF_ON
Text Label 1450 6150 2    60   ~ 0
ARD_ON
Text Label 1450 6250 2    60   ~ 0
ARDIO_ON
Text Label 1450 650  2    60   ~ 0
LOAD_STAT1
Text Label 1450 750  2    60   ~ 0
LOAD_STAT2
Text Label 1450 850  2    60   ~ 0
LOAD_PG
Text Label 4900 2350 0    60   ~ 0
NRF_CSN
Text Label 4900 2550 0    60   ~ 0
NRF_IRQ
Text Label 4900 2850 0    60   ~ 0
NRF_CE
Text Label 4100 2250 0    60   ~ 0
NRF_CSN
Text Label 4100 2150 0    60   ~ 0
NRF_IRQ
Text Label 4100 2050 0    60   ~ 0
NRF_CE
Text Label 4900 2450 0    60   ~ 0
SE_MOSI
Text Label 4900 2650 0    60   ~ 0
SE_MISO
Text Label 4900 2750 0    60   ~ 0
SE_SCK
Text Label 10250 3800 2    60   ~ 0
IO15
Text Label 10750 3800 0    60   ~ 0
IO2
Text Label 10250 3900 2    60   ~ 0
ESP_GPIO0
Text Label 10750 3900 0    60   ~ 0
IO4
Text Label 10250 4000 2    60   ~ 0
IO16
Text GLabel 10750 4600 2    60   Input ~ 0
NRF3V3
Text Label 10750 4500 0    60   ~ 0
NRF_CSN
Text Label 10750 4400 0    60   ~ 0
SE_MOSI
Text Label 10750 4300 0    60   ~ 0
NRF_IRQ
Text Label 10250 4300 2    60   ~ 0
SE_MISO
Text Label 10250 4400 2    60   ~ 0
SE_SCK
Text Label 10250 4500 2    60   ~ 0
NRF_CE
$Comp
L GND #PWR023
U 1 1 5A812325
P 9900 4600
F 0 "#PWR023" H 9900 4350 50  0001 C CNN
F 1 "GND" H 9900 4450 50  0000 C CNN
F 2 "" H 9900 4600 50  0001 C CNN
F 3 "" H 9900 4600 50  0001 C CNN
	1    9900 4600
	0    1    1    0   
$EndComp
Text Label 7950 6000 0    60   ~ 0
SENSE_ON
Text Label 4100 2350 0    60   ~ 0
SENSE_ON
Text Label 7950 6100 0    60   ~ 0
VBATT_SENSE
Wire Wire Line
	5300 5300 5250 5300
Wire Wire Line
	5250 5300 5250 5350
Wire Wire Line
	5750 5700 5800 5650
Wire Wire Line
	7150 5250 7200 5250
Wire Wire Line
	7200 5150 7200 5250
Wire Wire Line
	7200 5250 7200 5300
Wire Wire Line
	7150 5150 7200 5150
Connection ~ 7200 5250
Wire Wire Line
	5300 4000 5250 4000
Wire Wire Line
	1700 7650 1750 7650
Wire Wire Line
	1750 6650 1700 6650
Wire Wire Line
	1750 5650 1700 5650
Wire Wire Line
	1750 4350 1700 4350
Wire Wire Line
	550  3550 850  3550
Wire Wire Line
	850  3550 1100 3550
Wire Wire Line
	1100 3550 1400 3550
Wire Wire Line
	1400 3550 1700 3550
Wire Wire Line
	1700 3550 1750 3550
Wire Wire Line
	800  3850 850  3850
Wire Wire Line
	850  3850 1100 3850
Wire Wire Line
	1100 3850 1400 3850
Wire Wire Line
	1400 3850 1700 3850
Wire Wire Line
	1700 3850 1750 3850
Wire Wire Line
	1750 2150 1700 2150
Wire Wire Line
	3750 7650 3800 7650
Wire Wire Line
	3750 2850 4050 2850
Wire Wire Line
	3750 3950 4050 3950
Wire Wire Line
	3750 5450 4050 5450
Wire Wire Line
	600  1550 500  1550
Wire Wire Line
	500  1350 500  1550
Wire Wire Line
	500  1550 500  1900
Wire Wire Line
	900  1550 950  1550
Wire Wire Line
	950  1450 950  1550
Wire Wire Line
	950  1550 950  1850
Wire Wire Line
	950  2400 950  2150
Wire Wire Line
	600  2300 600  2400
Wire Wire Line
	1750 1450 950  1450
Connection ~ 950  1550
Wire Wire Line
	500  1350 1750 1350
Connection ~ 500  1550
Wire Wire Line
	7700 4050 7150 4050
Wire Wire Line
	7150 4150 7700 4150
Wire Wire Line
	5300 4100 4800 4100
Wire Wire Line
	7150 5050 7700 5050
Wire Wire Line
	3750 3650 4600 3650
Wire Wire Line
	3750 3750 4600 3750
Wire Wire Line
	3750 3850 4600 3850
Wire Wire Line
	3750 4250 4600 4250
Wire Wire Line
	3750 6750 4600 6750
Wire Wire Line
	1750 3050 1150 3050
Wire Wire Line
	7450 900  6750 900 
Wire Wire Line
	6700 1000 7450 1000
Wire Wire Line
	7450 1100 6750 1100
Wire Wire Line
	6750 1200 7450 1200
Wire Wire Line
	6750 1300 7450 1300
Wire Wire Line
	7450 1400 6750 1400
Wire Wire Line
	6750 1500 7450 1500
Wire Wire Line
	7450 1600 6750 1600
Wire Wire Line
	7450 1800 6750 1800
Wire Wire Line
	7450 1900 6750 1900
Wire Wire Line
	7450 2000 6750 2000
Wire Wire Line
	7450 2100 6750 2100
Wire Wire Line
	7450 2200 6750 2200
Wire Wire Line
	7450 2300 6750 2300
Wire Wire Line
	7450 2400 6750 2400
Wire Wire Line
	7450 2500 6750 2500
Wire Wire Line
	6750 800  7450 800 
Wire Wire Line
	7450 700  6750 700 
Wire Wire Line
	1150 650  1750 650 
Wire Wire Line
	1150 750  1750 750 
Wire Wire Line
	1150 850  1750 850 
Wire Wire Line
	1150 950  1750 950 
Wire Wire Line
	1150 1050 1750 1050
Wire Wire Line
	1150 1150 1750 1150
Wire Wire Line
	1150 1250 1750 1250
Wire Wire Line
	1150 1550 1750 1550
Wire Wire Line
	1150 1650 1750 1650
Wire Wire Line
	1150 1750 1750 1750
Wire Wire Line
	1150 1850 1750 1850
Wire Wire Line
	1150 1950 1750 1950
Wire Wire Line
	1150 2050 1750 2050
Wire Wire Line
	1750 3150 1150 3150
Wire Wire Line
	1750 2350 1150 2350
Wire Wire Line
	1750 2450 1150 2450
Wire Wire Line
	1750 2550 1150 2550
Wire Wire Line
	1750 2650 1150 2650
Wire Wire Line
	1750 2750 1150 2750
Wire Wire Line
	1750 2850 1150 2850
Wire Wire Line
	1750 2950 1150 2950
Wire Wire Line
	1750 4550 1150 4550
Wire Wire Line
	1750 3250 1150 3250
Wire Wire Line
	1750 3350 1150 3350
Wire Wire Line
	1750 3450 1150 3450
Wire Wire Line
	1750 3950 1150 3950
Wire Wire Line
	1750 4050 1150 4050
Wire Wire Line
	1750 4150 1150 4150
Wire Wire Line
	1750 4250 1150 4250
Wire Wire Line
	1750 5350 1150 5350
Wire Wire Line
	1750 4650 1150 4650
Wire Wire Line
	1750 4750 1150 4750
Wire Wire Line
	1750 4850 1150 4850
Wire Wire Line
	1750 4950 1150 4950
Wire Wire Line
	1750 5050 1150 5050
Wire Wire Line
	1750 5150 1150 5150
Wire Wire Line
	1750 5250 1150 5250
Wire Wire Line
	1750 6350 1150 6350
Wire Wire Line
	1750 5450 1150 5450
Wire Wire Line
	1750 5550 1150 5550
Wire Wire Line
	1750 5850 1150 5850
Wire Wire Line
	1750 5950 1150 5950
Wire Wire Line
	1750 6050 1150 6050
Wire Wire Line
	1750 6150 1150 6150
Wire Wire Line
	1750 6250 1150 6250
Wire Wire Line
	1750 7050 1150 7050
Wire Wire Line
	1750 6450 1150 6450
Wire Wire Line
	1750 6550 1150 6550
Wire Wire Line
	1750 6850 1150 6850
Wire Wire Line
	1750 6950 1150 6950
Wire Wire Line
	3750 6650 4600 6650
Wire Wire Line
	1750 7150 1150 7150
Wire Wire Line
	1750 7250 1150 7250
Wire Wire Line
	1750 7350 1150 7350
Wire Wire Line
	1750 7450 1150 7450
Wire Wire Line
	1750 7550 1150 7550
Wire Wire Line
	3750 6250 4600 6250
Wire Wire Line
	3750 6550 4600 6550
Wire Wire Line
	3750 7550 4600 7550
Wire Wire Line
	3750 6850 4600 6850
Wire Wire Line
	3750 6950 4600 6950
Wire Wire Line
	3750 7050 4600 7050
Wire Wire Line
	3750 7250 4600 7250
Wire Wire Line
	3750 7350 4600 7350
Wire Wire Line
	3750 7450 4600 7450
Wire Wire Line
	3750 850  4600 850 
Wire Wire Line
	3750 650  4600 650 
Wire Wire Line
	3750 750  4600 750 
Wire Wire Line
	3750 1850 4600 1850
Wire Wire Line
	3750 950  4600 950 
Wire Wire Line
	3750 1050 4600 1050
Wire Wire Line
	3750 1150 4600 1150
Wire Wire Line
	3750 1250 4600 1250
Wire Wire Line
	3750 1350 4600 1350
Wire Wire Line
	3750 1450 4600 1450
Wire Wire Line
	3750 1550 4600 1550
Wire Wire Line
	3750 2150 4600 2150
Wire Wire Line
	3750 1950 4600 1950
Wire Wire Line
	3750 2050 4600 2050
Wire Wire Line
	3750 3050 4600 3050
Wire Wire Line
	3750 2250 4600 2250
Wire Wire Line
	3750 2350 4600 2350
Wire Wire Line
	3750 2450 4600 2450
Wire Wire Line
	3750 2550 4600 2550
Wire Wire Line
	3750 2650 4600 2650
Wire Wire Line
	3750 2950 4600 2950
Wire Wire Line
	3750 3350 4600 3350
Wire Wire Line
	3750 3150 4600 3150
Wire Wire Line
	3750 3250 4600 3250
Wire Wire Line
	3750 4850 4600 4850
Wire Wire Line
	3750 3450 4600 3450
Wire Wire Line
	3750 3550 4600 3550
Wire Wire Line
	3750 4350 4600 4350
Wire Wire Line
	3750 4450 4600 4450
Wire Wire Line
	3750 4550 4600 4550
Wire Wire Line
	3750 4650 4600 4650
Wire Wire Line
	3750 4750 4600 4750
Wire Wire Line
	3750 5550 4600 5550
Wire Wire Line
	3750 4950 4600 4950
Wire Wire Line
	3750 5050 4600 5050
Wire Wire Line
	3750 6150 4600 6150
Wire Wire Line
	3750 5650 4600 5650
Wire Wire Line
	3750 5750 4600 5750
Wire Wire Line
	3750 5150 4600 5150
Wire Wire Line
	3750 5250 4600 5250
Wire Wire Line
	3750 5850 4600 5850
Wire Wire Line
	3750 5950 4600 5950
Wire Wire Line
	3750 6050 4600 6050
Wire Wire Line
	9450 2400 8950 2400
Wire Wire Line
	9450 2300 8950 2300
Wire Wire Line
	9450 2600 8950 2600
Wire Wire Line
	9450 2500 8950 2500
Wire Wire Line
	9450 2800 8950 2800
Wire Wire Line
	9450 2700 8950 2700
Wire Wire Line
	9450 3000 8950 3000
Wire Wire Line
	9450 2900 8950 2900
Wire Wire Line
	9450 3200 8950 3200
Wire Wire Line
	9450 3100 8950 3100
Wire Wire Line
	9450 3400 8950 3400
Wire Wire Line
	9450 3300 8950 3300
Wire Wire Line
	9450 3600 8950 3600
Wire Wire Line
	9450 3500 8950 3500
Wire Wire Line
	9450 3800 8950 3800
Wire Wire Line
	9450 3700 8950 3700
Wire Wire Line
	7950 800  8450 800 
Wire Wire Line
	8450 700  7950 700 
Wire Wire Line
	8450 1000 7950 1000
Wire Wire Line
	8000 900  8450 900 
Wire Wire Line
	9450 1200 8950 1200
Wire Wire Line
	8450 1100 7950 1100
Wire Wire Line
	9450 1400 8950 1400
Wire Wire Line
	9450 1300 8950 1300
Wire Wire Line
	9450 1600 8950 1600
Wire Wire Line
	9450 1500 8950 1500
Wire Wire Line
	9450 1800 8950 1800
Wire Wire Line
	9450 1700 8950 1700
Wire Wire Line
	9450 2000 8950 2000
Wire Wire Line
	9450 1900 8950 1900
Wire Wire Line
	9450 2200 8950 2200
Wire Wire Line
	9450 2100 8950 2100
Wire Wire Line
	9450 3900 8950 3900
Wire Wire Line
	9450 4100 8950 4100
Wire Wire Line
	9450 4000 8950 4000
Wire Wire Line
	9450 4300 8950 4300
Wire Wire Line
	9450 4200 8950 4200
Wire Wire Line
	9450 4500 8950 4500
Wire Wire Line
	9450 4400 8950 4400
Wire Wire Line
	9450 4600 8950 4600
Wire Wire Line
	8450 2500 7950 2500
Wire Wire Line
	8450 2400 7950 2400
Wire Wire Line
	8450 2700 7950 2700
Wire Wire Line
	8450 2600 7950 2600
Wire Wire Line
	8450 2900 7950 2900
Wire Wire Line
	8450 2800 7950 2800
Wire Wire Line
	8450 3100 7950 3100
Wire Wire Line
	8450 3000 7950 3000
Wire Wire Line
	8450 3300 7950 3300
Wire Wire Line
	8450 3200 7950 3200
Wire Wire Line
	8450 3500 7950 3500
Wire Wire Line
	8450 3400 7950 3400
Wire Wire Line
	8450 3700 7950 3700
Wire Wire Line
	8450 3600 7950 3600
Wire Wire Line
	8450 3900 7950 3900
Wire Wire Line
	8450 3800 7950 3800
Wire Wire Line
	9450 800  8950 800 
Wire Wire Line
	9450 700  8950 700 
Wire Wire Line
	9450 1000 8950 1000
Wire Wire Line
	8950 900  9200 900 
Wire Wire Line
	8450 1300 7950 1300
Wire Wire Line
	9450 1100 8950 1100
Wire Wire Line
	8450 1500 7950 1500
Wire Wire Line
	8450 1400 7950 1400
Wire Wire Line
	8450 1700 7950 1700
Wire Wire Line
	8450 1600 7950 1600
Wire Wire Line
	8450 1900 7950 1900
Wire Wire Line
	8450 1800 7950 1800
Wire Wire Line
	8450 2100 7950 2100
Wire Wire Line
	8450 2000 7950 2000
Wire Wire Line
	8450 2300 7950 2300
Wire Wire Line
	8450 2200 7950 2200
Wire Wire Line
	8450 4000 7950 4000
Wire Wire Line
	8450 4200 7950 4200
Wire Wire Line
	8450 4100 7950 4100
Wire Wire Line
	8450 4400 7950 4400
Wire Wire Line
	8450 4300 7950 4300
Wire Wire Line
	8450 4500 7950 4500
Wire Wire Line
	8450 1200 7950 1200
Wire Wire Line
	10250 2000 9900 2000
Wire Wire Line
	10250 1900 9900 1900
Wire Wire Line
	10250 2200 9900 2200
Wire Wire Line
	10250 2100 9900 2100
Wire Wire Line
	10250 2400 9900 2400
Wire Wire Line
	10250 2300 9900 2300
Wire Wire Line
	10250 2600 9900 2600
Wire Wire Line
	10250 2500 9900 2500
Wire Wire Line
	10250 2800 9900 2800
Wire Wire Line
	10250 2700 9900 2700
Wire Wire Line
	10250 3000 9900 3000
Wire Wire Line
	10250 2900 9900 2900
Wire Wire Line
	10250 3200 9900 3200
Wire Wire Line
	10250 3100 9900 3100
Wire Wire Line
	10250 3400 9900 3400
Wire Wire Line
	10250 3300 9900 3300
Wire Wire Line
	10250 4400 9900 4400
Wire Wire Line
	10250 4300 9900 4300
Wire Wire Line
	10250 4600 9900 4600
Wire Wire Line
	10250 4500 9900 4500
Wire Wire Line
	10250 700  10100 700 
Wire Wire Line
	10250 900  10100 900 
Wire Wire Line
	10100 1200 10150 1200
Wire Wire Line
	10150 1200 10250 1200
Wire Wire Line
	10250 1100 10250 1100
Wire Wire Line
	10250 1400 10000 1400
Wire Wire Line
	10250 1600 9900 1600
Wire Wire Line
	10250 1500 9900 1500
Wire Wire Line
	10250 1800 9900 1800
Wire Wire Line
	10250 1700 9900 1700
Wire Wire Line
	10250 3500 9900 3500
Wire Wire Line
	10250 3700 9900 3700
Wire Wire Line
	10250 3600 9900 3600
Wire Wire Line
	10250 3900 9900 3900
Wire Wire Line
	10250 3800 9900 3800
Wire Wire Line
	10250 4100 10100 4100
Wire Wire Line
	10250 4000 9900 4000
Wire Wire Line
	10250 4200 9900 4200
Wire Wire Line
	11100 2000 10750 2000
Wire Wire Line
	11100 1900 10750 1900
Wire Wire Line
	11100 2200 10750 2200
Wire Wire Line
	11100 2100 10750 2100
Wire Wire Line
	11100 2400 10750 2400
Wire Wire Line
	11100 2300 10750 2300
Wire Wire Line
	11100 2600 10750 2600
Wire Wire Line
	11100 2500 10750 2500
Wire Wire Line
	11100 2800 10750 2800
Wire Wire Line
	11100 2700 10750 2700
Wire Wire Line
	11100 3000 10750 3000
Wire Wire Line
	11100 2900 10750 2900
Wire Wire Line
	11100 3200 10750 3200
Wire Wire Line
	11100 3100 10750 3100
Wire Wire Line
	11100 3400 10750 3400
Wire Wire Line
	11100 3300 10750 3300
Wire Wire Line
	11100 4400 10750 4400
Wire Wire Line
	11100 4300 10750 4300
Wire Wire Line
	11100 4500 10750 4500
Wire Wire Line
	11100 800  10750 800 
Wire Wire Line
	11100 700  10750 700 
Wire Wire Line
	11100 1000 10750 1000
Wire Wire Line
	11100 900  10750 900 
Wire Wire Line
	11100 1200 10750 1200
Wire Wire Line
	11100 1100 10750 1100
Wire Wire Line
	11100 1400 10750 1400
Wire Wire Line
	11100 1300 10750 1300
Wire Wire Line
	11100 1600 10750 1600
Wire Wire Line
	11100 1500 10750 1500
Wire Wire Line
	11100 1800 10750 1800
Wire Wire Line
	11100 1700 10750 1700
Wire Wire Line
	11100 3500 10750 3500
Wire Wire Line
	11100 3700 10750 3700
Wire Wire Line
	11100 3600 10750 3600
Wire Wire Line
	11100 3900 10750 3900
Wire Wire Line
	11100 3800 10750 3800
Wire Wire Line
	11100 4100 10750 4100
Wire Wire Line
	11100 4000 10750 4000
Wire Wire Line
	11100 4200 10750 4200
Wire Wire Line
	10150 1200 10150 1200
Connection ~ 10150 1200
Wire Wire Line
	10250 1300 10150 1300
Wire Wire Line
	10150 1300 10150 1200
Wire Wire Line
	5300 4200 4800 4200
Wire Wire Line
	5300 4300 4800 4300
Wire Wire Line
	5300 4400 4800 4400
Wire Wire Line
	5300 4500 4800 4500
Wire Wire Line
	5300 4600 4800 4600
Wire Wire Line
	5300 4700 4800 4700
Wire Wire Line
	5300 4800 4800 4800
Wire Wire Line
	5300 4900 4800 4900
Wire Wire Line
	5300 5000 4800 5000
Wire Wire Line
	7700 3850 7150 3850
Wire Wire Line
	7700 3950 7150 3950
Wire Wire Line
	7700 4250 7150 4250
Wire Wire Line
	7700 4450 7150 4450
Wire Wire Line
	7700 4550 7150 4550
Wire Wire Line
	7700 4650 7150 4650
Wire Wire Line
	7700 4750 7150 4750
Wire Wire Line
	7700 4850 7150 4850
Wire Wire Line
	7700 4950 7150 4950
Wire Wire Line
	5300 5200 4800 5200
Wire Wire Line
	5300 5100 4800 5100
Wire Wire Line
	5900 5650 5900 5850
Wire Wire Line
	6000 5650 6000 5850
Wire Wire Line
	6100 5650 6100 5850
Wire Wire Line
	6200 5650 6200 5850
Wire Wire Line
	6300 5650 6300 5850
Wire Wire Line
	6400 5650 6400 5850
Wire Wire Line
	6500 5650 6500 5850
Wire Wire Line
	6600 5650 6600 5850
Wire Wire Line
	6700 5650 6700 5850
Wire Wire Line
	5450 650  4900 650 
Wire Wire Line
	5450 750  4900 750 
Wire Wire Line
	5450 850  4900 850 
Wire Wire Line
	5450 950  4900 950 
Wire Wire Line
	5450 1050 4900 1050
Wire Wire Line
	5450 1150 4900 1150
Wire Wire Line
	5450 1250 4900 1250
Wire Wire Line
	7950 4950 8550 4950
Wire Wire Line
	7950 5050 8550 5050
Wire Wire Line
	5450 1450 4900 1450
Wire Wire Line
	5450 1550 4900 1550
Wire Wire Line
	5450 1650 4900 1650
Wire Wire Line
	5450 1750 4900 1750
Wire Wire Line
	10100 4900 9750 4900
Wire Wire Line
	9750 5000 10100 5000
Wire Wire Line
	9750 5100 10100 5100
Wire Wire Line
	10100 5200 9750 5200
Wire Wire Line
	9750 5300 10100 5300
Wire Wire Line
	10100 5400 9750 5400
Wire Wire Line
	9750 5500 10100 5500
Wire Wire Line
	10100 5600 9750 5600
Wire Wire Line
	9750 5700 10100 5700
Wire Wire Line
	10100 5800 9750 5800
Wire Wire Line
	9750 5900 10100 5900
Wire Wire Line
	3800 1650 3750 1650
Wire Wire Line
	3800 2750 3750 2750
Wire Wire Line
	3800 4050 3750 4050
Wire Wire Line
	3800 5350 3750 5350
Wire Wire Line
	3800 6350 3750 6350
Wire Wire Line
	3750 1750 4000 1750
Wire Wire Line
	4000 1750 4050 1700
Wire Wire Line
	3750 6450 4000 6450
Wire Wire Line
	4000 6450 4050 6400
Wire Wire Line
	4050 6400 4050 6400
Wire Wire Line
	3750 7750 4000 7750
Wire Wire Line
	4000 7750 4050 7700
Wire Wire Line
	4050 7700 4050 7700
Wire Wire Line
	3750 4150 4000 4150
Wire Wire Line
	4000 4150 4050 4100
Wire Wire Line
	4050 4100 4050 4100
Wire Wire Line
	1450 7700 1450 7700
Wire Wire Line
	1450 7700 1500 7750
Wire Wire Line
	1500 7750 1750 7750
Wire Wire Line
	1450 6700 1450 6700
Wire Wire Line
	1450 6700 1500 6750
Wire Wire Line
	1500 6750 1750 6750
Wire Wire Line
	1750 5750 1500 5750
Wire Wire Line
	1500 5750 1450 5700
Wire Wire Line
	1450 5700 1450 5700
Wire Wire Line
	1450 4400 1450 4400
Wire Wire Line
	1450 4400 1500 4450
Wire Wire Line
	1500 4450 1750 4450
Wire Wire Line
	1450 2200 1450 2200
Wire Wire Line
	1450 2200 1500 2250
Wire Wire Line
	1500 2250 1750 2250
Wire Wire Line
	8400 4600 8450 4600
Wire Wire Line
	5200 3950 5200 3950
Wire Wire Line
	5250 4000 5200 3950
Wire Notes Line
	4650 500  4650 6100
Wire Notes Line
	11150 4700 7850 4700
Wire Notes Line
	7850 3200 7850 6500
Wire Notes Line
	7850 3200 4650 3200
Wire Notes Line
	6350 3200 6350 600 
Wire Wire Line
	5450 1900 4900 1900
Wire Wire Line
	4900 2000 5450 2000
Wire Wire Line
	5450 2100 4900 2100
Wire Wire Line
	4900 2200 5450 2200
Wire Wire Line
	5450 2350 4900 2350
Wire Wire Line
	4900 2450 5450 2450
Wire Wire Line
	5450 2550 4900 2550
Wire Wire Line
	4900 2650 5450 2650
Wire Wire Line
	5450 2750 4900 2750
Wire Wire Line
	4900 2850 5450 2850
Wire Wire Line
	7950 6100 8550 6100
Wire Wire Line
	7950 5300 8550 5300
Wire Wire Line
	7950 5400 8550 5400
Wire Wire Line
	7950 5500 8550 5500
Wire Wire Line
	7950 5600 8550 5600
Wire Wire Line
	7950 5700 8550 5700
Wire Wire Line
	7950 5800 8550 5800
Wire Wire Line
	7950 5900 8550 5900
Wire Wire Line
	500  1900 600  1900
Wire Wire Line
	600  1900 600  2000
Wire Wire Line
	7950 6000 8550 6000
Text Label 1450 1850 2    60   ~ 0
VBATT_SENSE
Wire Wire Line
	1700 3650 1700 3550
Connection ~ 1700 3550
Connection ~ 1700 3650
Wire Wire Line
	1700 3850 1700 3750
Connection ~ 1700 3750
Connection ~ 1700 3850
Wire Wire Line
	1750 3650 1700 3650
Wire Wire Line
	1700 3750 1750 3750
$Comp
L C C25
U 1 1 5A7BAFB3
P 1100 3700
F 0 "C25" H 1125 3800 50  0000 L CNN
F 1 "1µF" H 1125 3600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 1138 3550 50  0001 C CNN
F 3 "" H 1100 3700 50  0001 C CNN
F 4 "Farnell" H 1100 3700 60  0001 C CNN "Distributor"
F 5 "1759398" H 1100 3700 60  0001 C CNN "Ordercode"
F 6 "0,0229" H 1100 3700 60  0001 C CNN "Cost"
	1    1100 3700
	1    0    0    -1  
$EndComp
$Comp
L C C26
U 1 1 5A7BB0EC
P 1400 3700
F 0 "C26" H 1425 3800 50  0000 L CNN
F 1 "100nF" H 1425 3600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 1438 3550 50  0001 C CNN
F 3 "" H 1400 3700 50  0001 C CNN
F 4 "Farnell" H 1400 3700 60  0001 C CNN "Distributor"
F 5 "1759017" H 1400 3700 60  0001 C CNN "Ordercode"
F 6 "0,0199" H 1400 3700 60  0001 C CNN "Cost"
	1    1400 3700
	1    0    0    -1  
$EndComp
Connection ~ 1400 3550
Connection ~ 1100 3550
Connection ~ 1100 3850
Connection ~ 1400 3850
$Comp
L Ferrite_Bead L2
U 1 1 5A7C004E
P 800 4100
F 0 "L2" V 650 4125 50  0000 C CNN
F 1 "Ferrite_Bead" V 950 4100 50  0000 C CNN
F 2 "Inductors_SMD:L_0603_HandSoldering" V 730 4100 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1851895.pdf" H 800 4100 50  0001 C CNN
F 4 "Farnell" H 800 4100 60  0001 C CNN "Distributor"
F 5 "2443244" H 800 4100 60  0001 C CNN "Ordercode"
F 6 "0,0372" H 800 4100 60  0001 C CNN "Cost"
F 7 "60R" V 750 4300 60  0000 C CNN "Z100"
F 8 "500mA" V 1050 4100 60  0000 C CNN "I"
F 9 "0R1" V 750 3950 60  0000 C CNN "DC_R"
	1    800  4100
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR024
U 1 1 5A7C0B17
P 550 3550
F 0 "#PWR024" H 550 3300 50  0001 C CNN
F 1 "GND" H 550 3400 50  0000 C CNN
F 2 "" H 550 3550 50  0000 C CNN
F 3 "" H 550 3550 50  0000 C CNN
	1    550  3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	800  3850 800  3950
Wire Wire Line
	800  4250 800  4550
Text GLabel 800  4550 3    60   Input ~ 0
STM3V3
Wire Wire Line
	3750 7150 4850 7150
Wire Wire Line
	4850 7050 4850 7150
Wire Wire Line
	4850 7150 4850 7250
$Comp
L R R38
U 1 1 5A7D45EE
P 4850 7400
F 0 "R38" V 4930 7400 50  0000 C CNN
F 1 "100k" V 4850 7400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4780 7400 50  0001 C CNN
F 3 "" H 4850 7400 50  0001 C CNN
F 4 "Farnell" H 4850 7400 60  0001 C CNN "Distributor"
F 5 "2447226" H 4850 7400 60  0001 C CNN "Ordercode"
F 6 "0,0033" H 4850 7400 60  0001 C CNN "Cost"
	1    4850 7400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR025
U 1 1 5A7D472F
P 4850 7600
F 0 "#PWR025" H 4850 7350 50  0001 C CNN
F 1 "GND" H 4850 7450 50  0000 C CNN
F 2 "" H 4850 7600 50  0001 C CNN
F 3 "" H 4850 7600 50  0001 C CNN
	1    4850 7600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 7600 4850 7550
$Comp
L R R37
U 1 1 5A7D4F68
P 4850 6900
F 0 "R37" V 4930 6900 50  0000 C CNN
F 1 "optional" V 4850 6900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 4780 6900 50  0001 C CNN
F 3 "" H 4850 6900 50  0001 C CNN
	1    4850 6900
	1    0    0    -1  
$EndComp
Connection ~ 4850 7150
Text GLabel 4850 6650 1    60   Input ~ 0
STM3V3
Wire Wire Line
	4850 6650 4850 6750
Wire Notes Line
	4650 6100 5100 6100
Wire Notes Line
	5100 6100 5100 7750
Wire Wire Line
	8550 6200 7950 6200
Text Label 7950 6200 0    60   ~ 0
VBATT_OK
Text Label 1150 3950 0    60   ~ 0
BT3
Text Label 4100 3050 0    60   ~ 0
VBATT_OK
$Comp
L +3V3 #PWR026
U 1 1 5A8BDF53
P 10100 4100
F 0 "#PWR026" H 10100 3950 50  0001 C CNN
F 1 "+3V3" H 10100 4240 50  0000 C CNN
F 2 "" H 10100 4100 50  0001 C CNN
F 3 "" H 10100 4100 50  0001 C CNN
	1    10100 4100
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR027
U 1 1 5A8BE08B
P 9900 4200
F 0 "#PWR027" H 9900 3950 50  0001 C CNN
F 1 "GND" H 9900 4050 50  0000 C CNN
F 2 "" H 9900 4200 50  0001 C CNN
F 3 "" H 9900 4200 50  0001 C CNN
	1    9900 4200
	0    1    1    0   
$EndComp
Text Label 800  3850 2    60   ~ 0
VREF
Text Notes 6900 700  0    60   ~ 0
SCL
Text Notes 6900 800  0    60   ~ 0
SDA
Text Notes 6400 650  0    60   ~ 0
Arduino Compatible Connector\n(long one)
Wire Wire Line
	8550 6300 7950 6300
Text Label 7950 6300 0    60   ~ 0
MODE_SYNC
Text Label 1450 950  2    60   ~ 0
MODE_SYNC
$Comp
L R R?
U 1 1 5AE2A24B
P 5300 3750
F 0 "R?" V 5380 3750 50  0000 C CNN
F 1 "10k" V 5300 3750 50  0000 C CNN
F 2 "" V 5230 3750 50  0001 C CNN
F 3 "" H 5300 3750 50  0001 C CNN
	1    5300 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 3900 5300 4000
Connection ~ 5300 4000
Wire Wire Line
	5300 3600 5300 3500
Wire Wire Line
	5300 3500 4800 3500
Text Label 5300 3500 2    60   ~ 0
ESP_RESET
Connection ~ 850  3550
Connection ~ 850  3850
$Comp
L C C?
U 1 1 5AE2DC94
P 850 3700
F 0 "C?" H 875 3800 50  0000 L CNN
F 1 "10nF" H 875 3600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 888 3550 50  0001 C CNN
F 3 "" H 850 3700 50  0001 C CNN
F 4 "Farnell" H 850 3700 60  0001 C CNN "Distributor"
F 5 "9406182" H 850 3700 60  0001 C CNN "Ordercode"
F 6 "€0,0107" H 850 3700 60  0001 C CNN "Cost"
	1    850  3700
	1    0    0    -1  
$EndComp
$EndSCHEMATC
