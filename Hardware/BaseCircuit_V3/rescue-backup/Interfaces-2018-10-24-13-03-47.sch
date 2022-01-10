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
Sheet 3 4
Title "RhizoTech - Motherboard - Interfaces"
Date "30.01.2018"
Rev "V2"
Comp "TH Köln Institut für Nachrichtentechnik"
Comment1 "Erstellt von Daniel Lohmann"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Micro_SD_Card J1
U 1 1 5A6E3414
P 4200 2600
F 0 "J1" H 3550 3200 50  0000 C CNN
F 1 "Micro_SD_Card" H 4850 3200 50  0000 R CNN
F 2 "503398-1892:503398-1892" H 5350 2900 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2138495.pdf" H 4200 2600 50  0001 C CNN
F 4 "Farnell" H 4200 2600 60  0001 C CNN "Distributor"
F 5 "2358234" H 4200 2600 60  0001 C CNN "Ordercode"
F 6 "1,19" H 4200 2600 60  0001 C CNN "Cost"
	1    4200 2600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR071
U 1 1 5A6E342B
P 3200 3100
F 0 "#PWR071" H 3200 2850 50  0001 C CNN
F 1 "GND" H 3200 2950 50  0000 C CNN
F 2 "" H 3200 3100 50  0001 C CNN
F 3 "" H 3200 3100 50  0001 C CNN
	1    3200 3100
	1    0    0    -1  
$EndComp
$Comp
L R R13
U 1 1 5A6E3522
P 2250 1900
F 0 "R13" V 2330 1900 50  0000 C CNN
F 1 "47k" V 2250 1900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2180 1900 50  0001 C CNN
F 3 "" H 2250 1900 50  0001 C CNN
F 4 "Farnell" H 2250 1900 60  0001 C CNN "Distributor"
F 5 "2447376" H 2250 1900 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 2250 1900 60  0001 C CNN "Cost"
	1    2250 1900
	1    0    0    -1  
$EndComp
$Comp
L R R14
U 1 1 5A6E356C
P 2450 1900
F 0 "R14" V 2530 1900 50  0000 C CNN
F 1 "47k" V 2450 1900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2380 1900 50  0001 C CNN
F 3 "" H 2450 1900 50  0001 C CNN
F 4 "Farnell" H 2450 1900 60  0001 C CNN "Distributor"
F 5 "2447376" H 2450 1900 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 2450 1900 60  0001 C CNN "Cost"
	1    2450 1900
	1    0    0    -1  
$EndComp
$Comp
L R R15
U 1 1 5A6E3589
P 2650 1900
F 0 "R15" V 2730 1900 50  0000 C CNN
F 1 "47k" V 2650 1900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2580 1900 50  0001 C CNN
F 3 "" H 2650 1900 50  0001 C CNN
F 4 "Farnell" H 2650 1900 60  0001 C CNN "Distributor"
F 5 "2447376" H 2650 1900 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 2650 1900 60  0001 C CNN "Cost"
	1    2650 1900
	1    0    0    -1  
$EndComp
$Comp
L R R16
U 1 1 5A6E35A9
P 2850 1900
F 0 "R16" V 2930 1900 50  0000 C CNN
F 1 "47k" V 2850 1900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2780 1900 50  0001 C CNN
F 3 "" H 2850 1900 50  0001 C CNN
F 4 "Farnell" H 2850 1900 60  0001 C CNN "Distributor"
F 5 "2447376" H 2850 1900 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 2850 1900 60  0001 C CNN "Cost"
	1    2850 1900
	1    0    0    -1  
$EndComp
$Comp
L R R17
U 1 1 5A6E35CC
P 3050 1900
F 0 "R17" V 3130 1900 50  0000 C CNN
F 1 "47k" V 3050 1900 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2980 1900 50  0001 C CNN
F 3 "" H 3050 1900 50  0001 C CNN
F 4 "Farnell" H 3050 1900 60  0001 C CNN "Distributor"
F 5 "2447376" H 3050 1900 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 3050 1900 60  0001 C CNN "Cost"
	1    3050 1900
	1    0    0    -1  
$EndComp
Text HLabel 1850 1450 3    60   Input ~ 0
PWR
Text HLabel 1900 2300 0    60   BiDi ~ 0
D2
Text HLabel 1900 2400 0    60   BiDi ~ 0
D3
Text HLabel 1900 2500 0    60   BiDi ~ 0
CMD
Text HLabel 1900 2700 0    60   Input ~ 0
CLK
Text HLabel 1900 2900 0    60   BiDi ~ 0
D0
Text HLabel 1900 3000 0    60   BiDi ~ 0
D1
$Comp
L FT231XS U1
U 1 1 5A71143F
P 3650 6450
F 0 "U1" H 3100 7250 50  0000 L CNN
F 1 "FT231XS" H 3950 7250 50  0000 L CNN
F 2 "SMD_Packages:SSOP-20" H 3650 6450 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1913754.pdf" H 3650 6450 50  0001 C CNN
F 4 "Farnell" H 3650 6450 60  0001 C CNN "Distributor"
F 5 "2081328" H 3650 6450 60  0001 C CNN "Ordercode"
F 6 "1,80" H 3650 6450 60  0001 C CNN "Cost"
	1    3650 6450
	1    0    0    -1  
$EndComp
$Comp
L R R10
U 1 1 5A711454
P 5300 6050
F 0 "R10" V 5380 6050 50  0000 C CNN
F 1 "10k" V 5300 6050 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5230 6050 50  0001 C CNN
F 3 "" H 5300 6050 50  0001 C CNN
F 4 "Farnell" H 5300 6050 60  0001 C CNN "Distributor"
F 5 "2447230" H 5300 6050 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 5300 6050 60  0001 C CNN "Cost"
	1    5300 6050
	0    1    1    0   
$EndComp
$Comp
L R R11
U 1 1 5A71145B
P 5300 6750
F 0 "R11" V 5380 6750 50  0000 C CNN
F 1 "10k" V 5300 6750 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5230 6750 50  0001 C CNN
F 3 "" H 5300 6750 50  0001 C CNN
F 4 "Farnell" H 5300 6750 60  0001 C CNN "Distributor"
F 5 "2447230" H 5300 6750 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 5300 6750 60  0001 C CNN "Cost"
	1    5300 6750
	0    1    1    0   
$EndComp
Text Label 4400 6050 0    60   ~ 0
RTS
Text Label 4400 6250 0    60   ~ 0
DTR
Text Label 5900 7050 0    60   ~ 0
ESP_RESET
Text Label 5900 5750 0    60   ~ 0
ESP_GPIO0
Text Label 1700 6150 0    60   ~ 0
D-_ESP
Text Label 1700 6250 0    60   ~ 0
D+_ESP
$Comp
L GND #PWR072
U 1 1 5A711468
P 3650 7550
F 0 "#PWR072" H 3650 7300 50  0001 C CNN
F 1 "GND" H 3650 7400 50  0000 C CNN
F 2 "" H 3650 7550 50  0001 C CNN
F 3 "" H 3650 7550 50  0001 C CNN
	1    3650 7550
	1    0    0    -1  
$EndComp
NoConn ~ 4850 7050
NoConn ~ 4850 6750
NoConn ~ 4350 6350
NoConn ~ 4350 6450
NoConn ~ 4350 6550
NoConn ~ 4350 6150
Text Label 1150 5950 0    60   ~ 0
+5V_ESP
$Comp
L R R2
U 1 1 5A711477
P 2400 6150
F 0 "R2" V 2480 6150 50  0000 C CNN
F 1 "27R" V 2400 6150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2330 6150 50  0001 C CNN
F 3 "" H 2400 6150 50  0001 C CNN
F 4 "Farnell" H 2400 6150 60  0001 C CNN "Distributor"
F 5 "2447316" H 2400 6150 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 2400 6150 60  0001 C CNN "Cost"
	1    2400 6150
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 5A71147E
P 2600 6250
F 0 "R3" V 2680 6250 50  0000 C CNN
F 1 "27R" V 2600 6250 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2530 6250 50  0001 C CNN
F 3 "" H 2600 6250 50  0001 C CNN
F 4 "Farnell" H 2600 6250 60  0001 C CNN "Distributor"
F 5 "2447316" H 2600 6250 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 2600 6250 60  0001 C CNN "Cost"
	1    2600 6250
	0    1    1    0   
$EndComp
$Comp
L Ferrite_Bead L1
U 1 1 5A711485
P 1950 5700
F 0 "L1" V 1800 5725 50  0000 C CNN
F 1 "Ferrite_Bead" V 2100 5700 50  0000 C CNN
F 2 "Inductors_SMD:L_0603_HandSoldering" V 1880 5700 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1851895.pdf" H 1950 5700 50  0001 C CNN
F 4 "Farnell" H 1950 5700 60  0001 C CNN "Distributor"
F 5 "2443244" H 1950 5700 60  0001 C CNN "Ordercode"
F 6 "0,0372" H 1950 5700 60  0001 C CNN "Cost"
F 7 "60R" V 1900 5900 60  0000 C CNN "Z100"
F 8 "500mA" V 2200 5700 60  0000 C CNN "I"
F 9 "0R1" V 1900 5550 60  0000 C CNN "DC_R"
	1    1950 5700
	-1   0    0    1   
$EndComp
$Comp
L C C7
U 1 1 5A71148C
P 1950 6500
F 0 "C7" H 1975 6600 50  0000 L CNN
F 1 "10nF" H 1975 6400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1988 6350 50  0001 C CNN
F 3 "" H 1950 6500 50  0001 C CNN
F 4 "Farnell" H 1950 6500 60  0001 C CNN "Distributor"
F 5 "9406182" H 1950 6500 60  0001 C CNN "Ordercode"
F 6 "0,0107" H 1950 6500 60  0001 C CNN "Cost"
	1    1950 6500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR073
U 1 1 5A711493
P 1950 6850
F 0 "#PWR073" H 1950 6600 50  0001 C CNN
F 1 "GND" H 1950 6700 50  0000 C CNN
F 2 "" H 1950 6850 50  0001 C CNN
F 3 "" H 1950 6850 50  0001 C CNN
	1    1950 6850
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 5A711499
P 1650 6500
F 0 "C6" H 1675 6600 50  0000 L CNN
F 1 "47pF" H 1675 6400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 1688 6350 50  0001 C CNN
F 3 "" H 1650 6500 50  0001 C CNN
F 4 "Farnell" H 1650 6500 60  0001 C CNN "Distributor"
F 5 "1759062" H 1650 6500 60  0001 C CNN "Ordercode"
F 6 "0,0123" H 1650 6500 60  0001 C CNN "Cost"
	1    1650 6500
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 5A7114A0
P 1350 6500
F 0 "C5" H 1375 6600 50  0000 L CNN
F 1 "47pF" H 1375 6400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 1388 6350 50  0001 C CNN
F 3 "" H 1350 6500 50  0001 C CNN
F 4 "Farnell" H 1350 6500 60  0001 C CNN "Distributor"
F 5 "1759062" H 1350 6500 60  0001 C CNN "Ordercode"
F 6 "0,0123" H 1350 6500 60  0001 C CNN "Cost"
	1    1350 6500
	1    0    0    -1  
$EndComp
$Comp
L C C10
U 1 1 5A7114A7
P 2850 6700
F 0 "C10" H 2875 6800 50  0000 L CNN
F 1 "100nF" H 2875 6600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 2888 6550 50  0001 C CNN
F 3 "" H 2850 6700 50  0001 C CNN
F 4 "Farnell" H 2850 6700 60  0001 C CNN "Distributor"
F 5 "1759017" H 2850 6700 60  0001 C CNN "Ordercode"
F 6 "0,0199" H 2850 6700 60  0001 C CNN "Cost"
	1    2850 6700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR074
U 1 1 5A7114AE
P 2850 6950
F 0 "#PWR074" H 2850 6700 50  0001 C CNN
F 1 "GND" H 2850 6800 50  0000 C CNN
F 2 "" H 2850 6950 50  0001 C CNN
F 3 "" H 2850 6950 50  0001 C CNN
	1    2850 6950
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 5A7114B4
P 2250 5700
F 0 "C8" H 2275 5800 50  0000 L CNN
F 1 "100nF" H 2275 5600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2288 5550 50  0001 C CNN
F 3 "" H 2250 5700 50  0001 C CNN
F 4 "Farnell" H 2250 5700 60  0001 C CNN "Distributor"
F 5 "1759017" H 2250 5700 60  0001 C CNN "Ordercode"
F 6 "0,0199" H 2250 5700 60  0001 C CNN "Cost"
	1    2250 5700
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 5A7114BB
P 2550 5700
F 0 "C9" H 2575 5800 50  0000 L CNN
F 1 "4,7µF" H 2575 5600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2588 5550 50  0001 C CNN
F 3 "" H 2550 5700 50  0001 C CNN
F 4 "Farnell" H 2550 5700 60  0001 C CNN "Distributor"
F 5 "2320818" H 2550 5700 60  0001 C CNN "Ordercode"
F 6 "0,0571" H 2550 5700 60  0001 C CNN "Cost"
	1    2550 5700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR075
U 1 1 5A7114C2
P 2400 5900
F 0 "#PWR075" H 2400 5650 50  0001 C CNN
F 1 "GND" H 2400 5750 50  0000 C CNN
F 2 "" H 2400 5900 50  0001 C CNN
F 3 "" H 2400 5900 50  0001 C CNN
	1    2400 5900
	1    0    0    -1  
$EndComp
Text Label 4400 6750 0    60   ~ 0
TXDEN
Text Label 4400 6850 0    60   ~ 0
RXLED
Text Label 4400 6950 0    60   ~ 0
TXLED
Text Label 4400 7050 0    60   ~ 0
SLEEP
$Comp
L +5VP #PWR076
U 1 1 5A7114CE
P 1950 5350
F 0 "#PWR076" H 1950 5200 50  0001 C CNN
F 1 "+5VP" H 1950 5490 50  0000 C CNN
F 2 "" H 1950 5350 50  0001 C CNN
F 3 "" H 1950 5350 50  0001 C CNN
	1    1950 5350
	1    0    0    -1  
$EndComp
$Comp
L +3.3VP #PWR077
U 1 1 5A7114D4
P 3750 5250
F 0 "#PWR077" H 3900 5200 50  0001 C CNN
F 1 "+3.3VP" H 3750 5350 50  0000 C CNN
F 2 "" H 3750 5250 50  0001 C CNN
F 3 "" H 3750 5250 50  0001 C CNN
	1    3750 5250
	1    0    0    -1  
$EndComp
$Comp
L USB_OTG J2
U 1 1 5A7114DA
P 850 6150
F 0 "J2" H 650 6600 50  0000 L CNN
F 1 "USB B mini" H 650 6500 50  0000 L CNN
F 2 "Connectors:USB_Mini-B" H 1000 6100 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/2286408.pdf" H 1000 6100 50  0001 C CNN
F 4 "Farnell" H 850 6150 60  0001 C CNN "Distributor"
F 5 "1125348" H 850 6150 60  0001 C CNN "Ordercode"
F 6 "0,587" H 850 6150 60  0001 C CNN "Cost"
	1    850  6150
	1    0    0    -1  
$EndComp
Text HLabel 4550 5950 2    60   Input ~ 0
ESP_TxD0
Text HLabel 4550 5850 2    60   Output ~ 0
ESP_RxD0
Text HLabel 6400 5750 2    60   BiDi ~ 0
ESP_GPIO0
Text HLabel 6400 7050 2    60   BiDi ~ 0
ESP_RESET
$Comp
L CONN_01X06 J3
U 1 1 5A71411B
P 9700 5650
F 0 "J3" H 9700 6000 50  0000 C CNN
F 1 "CONN_01X06" V 9800 5650 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 9700 5650 50  0001 C CNN
F 3 "" H 9700 5650 50  0001 C CNN
	1    9700 5650
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR078
U 1 1 5A714122
P 9450 5350
F 0 "#PWR078" H 9450 5200 50  0001 C CNN
F 1 "+3.3V" H 9450 5490 50  0000 C CNN
F 2 "" H 9450 5350 50  0001 C CNN
F 3 "" H 9450 5350 50  0001 C CNN
	1    9450 5350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR079
U 1 1 5A714128
P 9450 6000
F 0 "#PWR079" H 9450 5750 50  0001 C CNN
F 1 "GND" H 9450 5850 50  0000 C CNN
F 2 "" H 9450 6000 50  0001 C CNN
F 3 "" H 9450 6000 50  0001 C CNN
	1    9450 6000
	1    0    0    -1  
$EndComp
Text Label 8950 5500 0    60   ~ 0
SWCLK
Text Label 8950 5700 0    60   ~ 0
SWDIO
Text Label 8950 5800 0    60   ~ 0
NRST
Text Label 8950 5900 0    60   ~ 0
TRACESWO
Text Notes 8450 5100 0    60   ~ 0
SWD Interface for STM32L4
Text Notes 2350 4900 0    60   ~ 0
Programming Interface ESP32
$Comp
L R R18
U 1 1 5A715CBE
P 5550 7300
F 0 "R18" V 5630 7300 50  0000 C CNN
F 1 "1k" V 5550 7300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5480 7300 50  0001 C CNN
F 3 "" H 5550 7300 50  0001 C CNN
F 4 "Farnell" H 5550 7300 60  0001 C CNN "Distributor"
F 5 "2447272" H 5550 7300 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 5550 7300 60  0001 C CNN "Cost"
	1    5550 7300
	0    1    1    0   
$EndComp
$Comp
L R R19
U 1 1 5A715D49
P 5550 7600
F 0 "R19" V 5630 7600 50  0000 C CNN
F 1 "1k" V 5550 7600 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 5480 7600 50  0001 C CNN
F 3 "" H 5550 7600 50  0001 C CNN
F 4 "Farnell" H 5550 7600 60  0001 C CNN "Distributor"
F 5 "2447272" H 5550 7600 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 5550 7600 60  0001 C CNN "Cost"
	1    5550 7600
	0    1    1    0   
$EndComp
$Comp
L +3.3VP #PWR080
U 1 1 5A716901
P 5800 7200
F 0 "#PWR080" H 5950 7150 50  0001 C CNN
F 1 "+3.3VP" H 5800 7300 50  0000 C CNN
F 2 "" H 5800 7200 50  0001 C CNN
F 3 "" H 5800 7200 50  0001 C CNN
	1    5800 7200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 2800 3200 2800
Wire Wire Line
	3200 2800 3200 3100
Wire Wire Line
	1900 3000 3300 3000
Wire Wire Line
	1900 2300 3300 2300
Wire Wire Line
	1900 2400 3300 2400
Wire Wire Line
	1900 2500 3300 2500
Wire Wire Line
	3300 2700 1900 2700
Wire Wire Line
	1900 2900 3300 2900
Wire Wire Line
	3050 2050 3050 3000
Connection ~ 3050 3000
Wire Wire Line
	2850 2050 2850 2900
Connection ~ 2850 2900
Wire Wire Line
	2650 2050 2650 2500
Connection ~ 2650 2500
Wire Wire Line
	2450 2400 2450 2050
Connection ~ 2450 2400
Wire Wire Line
	2250 2050 2250 2300
Connection ~ 2250 2300
Wire Wire Line
	2250 1550 2250 1750
Wire Wire Line
	2250 1650 3200 1650
Wire Wire Line
	3050 1650 3050 1750
Wire Wire Line
	2850 1750 2850 1650
Connection ~ 2850 1650
Wire Wire Line
	2650 1750 2650 1650
Connection ~ 2650 1650
Wire Wire Line
	2450 1750 2450 1650
Connection ~ 2450 1650
Wire Wire Line
	3200 1650 3200 2600
Wire Wire Line
	3200 2600 3300 2600
Connection ~ 3050 1650
Wire Wire Line
	5850 6250 5850 6350
Wire Wire Line
	5850 6350 5750 6450
Wire Wire Line
	5750 6450 4950 6450
Wire Wire Line
	5850 6550 5850 6450
Wire Wire Line
	5850 6450 5750 6350
Wire Wire Line
	5750 6350 5050 6350
Wire Wire Line
	5450 6750 5550 6750
Wire Wire Line
	4950 6750 5150 6750
Wire Wire Line
	4350 6050 5150 6050
Connection ~ 5050 6050
Wire Wire Line
	5450 6050 5550 6050
Wire Wire Line
	5850 6950 5850 7050
Wire Wire Line
	5850 7050 6400 7050
Wire Wire Line
	5850 5850 5850 5750
Wire Wire Line
	5850 5750 6400 5750
Wire Wire Line
	5050 6350 5050 6050
Wire Wire Line
	4350 6250 4950 6250
Wire Wire Line
	4950 6250 4950 6750
Connection ~ 4950 6450
Wire Wire Line
	3550 7350 3550 7450
Wire Wire Line
	3550 7450 3750 7450
Wire Wire Line
	3750 7450 3750 7350
Wire Wire Line
	3650 7450 3650 7550
Connection ~ 3650 7450
Wire Wire Line
	4350 6750 4850 6750
Wire Wire Line
	4350 6850 5000 6850
Wire Wire Line
	4350 6950 4950 6950
Wire Wire Line
	4850 7050 4350 7050
Wire Wire Line
	4350 5850 4550 5850
Wire Wire Line
	4550 5950 4350 5950
Wire Wire Line
	2850 5850 2950 5850
Wire Wire Line
	2850 5350 2850 6550
Wire Wire Line
	2850 5350 3750 5350
Wire Wire Line
	3750 5250 3750 5550
Wire Wire Line
	3550 5450 3550 5550
Wire Wire Line
	1950 5450 3550 5450
Wire Wire Line
	1150 6350 1250 6350
Wire Wire Line
	1250 6750 1250 6350
Wire Wire Line
	2850 6450 2950 6450
Connection ~ 2850 5850
Wire Wire Line
	2750 6250 2950 6250
Wire Wire Line
	2950 6150 2550 6150
Wire Wire Line
	1950 5850 1950 6350
Connection ~ 1950 5950
Wire Wire Line
	1950 6650 1950 6850
Wire Wire Line
	750  6750 1950 6750
Wire Wire Line
	1650 6750 1650 6650
Connection ~ 1950 6750
Wire Wire Line
	1350 6750 1350 6650
Connection ~ 1650 6750
Connection ~ 1350 6750
Wire Wire Line
	1350 6350 1350 6250
Connection ~ 1350 6250
Wire Wire Line
	1650 6350 1650 6150
Connection ~ 1650 6150
Connection ~ 2850 6450
Wire Wire Line
	2850 6950 2850 6850
Wire Wire Line
	1950 5950 1150 5950
Wire Wire Line
	1950 5350 1950 5550
Wire Wire Line
	2250 5550 2250 5450
Connection ~ 2250 5450
Wire Wire Line
	2550 5550 2550 5450
Connection ~ 2550 5450
Wire Wire Line
	2250 5850 2550 5850
Wire Wire Line
	2400 5900 2400 5850
Connection ~ 2400 5850
Connection ~ 1950 5450
Connection ~ 3750 5350
Wire Wire Line
	850  6750 850  6550
Connection ~ 1250 6750
Wire Wire Line
	750  6750 750  6550
Connection ~ 850  6750
Wire Wire Line
	8900 5500 9500 5500
Wire Wire Line
	8900 5700 9500 5700
Wire Wire Line
	8900 5800 9500 5800
Wire Wire Line
	8900 5900 9500 5900
Wire Wire Line
	9450 5350 9450 5400
Wire Wire Line
	9450 5400 9500 5400
Wire Wire Line
	9500 5600 9450 5600
Wire Wire Line
	9450 5600 9450 6000
$Comp
L LED D4
U 1 1 5A7178CF
P 5200 7300
F 0 "D4" H 5200 7400 50  0000 C CNN
F 1 "Rot" H 5200 7200 50  0000 C CNN
F 2 "LEDs:LED_0603_HandSoldering" H 5200 7300 50  0001 C CNN
F 3 "" H 5200 7300 50  0001 C CNN
F 4 "Farnell" H 5200 7300 60  0001 C CNN "Distributor"
F 5 "2469449" H 5200 7300 60  0001 C CNN "Ordercode"
F 6 "0,218" H 5200 7300 60  0001 C CNN "Cost"
	1    5200 7300
	1    0    0    -1  
$EndComp
$Comp
L LED D5
U 1 1 5A71792F
P 5200 7600
F 0 "D5" H 5200 7700 50  0000 C CNN
F 1 "Grün" H 5200 7500 50  0000 C CNN
F 2 "LEDs:LED_0603_HandSoldering" H 5200 7600 50  0001 C CNN
F 3 "" H 5200 7600 50  0001 C CNN
F 4 "Farnell" H 5200 7600 60  0001 C CNN "Distributor"
F 5 "2469448" H 5200 7600 60  0001 C CNN "Ordercode"
F 6 "0,222" H 5200 7600 60  0001 C CNN "Cost"
	1    5200 7600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 6850 5000 7300
Wire Wire Line
	5000 7300 5050 7300
Wire Wire Line
	4950 6950 4950 7600
Wire Wire Line
	4950 7600 5050 7600
Wire Wire Line
	5400 7300 5350 7300
Wire Wire Line
	5400 7600 5350 7600
Wire Wire Line
	5800 7200 5800 7600
Wire Wire Line
	5800 7300 5700 7300
Wire Wire Line
	5800 7600 5700 7600
Connection ~ 5800 7300
Text HLabel 8900 5500 0    60   Input ~ 0
SWCLK
Text HLabel 8900 5700 0    60   Input ~ 0
SWDIO
Text HLabel 8900 5800 0    60   Input ~ 0
NRST
Text HLabel 8900 5900 0    60   Input ~ 0
TRACESWO
Wire Notes Line
	500  4350 11200 4350
Wire Notes Line
	7000 4350 7000 6500
$Comp
L GND #PWR081
U 1 1 5A888F62
P 5100 3300
F 0 "#PWR081" H 5100 3050 50  0001 C CNN
F 1 "GND" H 5100 3150 50  0000 C CNN
F 2 "" H 5100 3300 50  0001 C CNN
F 3 "" H 5100 3300 50  0001 C CNN
	1    5100 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3300 5100 3200
Wire Wire Line
	5100 3200 5000 3200
$Comp
L Q_NPN_BEC Q2
U 1 1 5A799E7A
P 5750 6050
F 0 "Q2" H 5950 6100 50  0000 L CNN
F 1 "MMBT2222" H 5950 6000 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 5950 6150 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/454046.pdf" H 5750 6050 50  0001 C CNN
F 4 "Farnell" H 5750 6050 60  0001 C CNN "Distributor"
F 5 "1757934" H 5750 6050 60  0001 C CNN "Ordercode"
F 6 "0,106" H 5750 6050 60  0001 C CNN "Cost"
	1    5750 6050
	1    0    0    -1  
$EndComp
$Comp
L Q_NPN_BEC Q3
U 1 1 5A79A3CF
P 5750 6750
F 0 "Q3" H 5950 6800 50  0000 L CNN
F 1 "MMBT2222" H 5950 6700 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23_Handsoldering" H 5950 6850 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/454046.pdf" H 5750 6750 50  0001 C CNN
F 4 "Farnell" H 5750 6750 60  0001 C CNN "Distributor"
F 5 "1757934" H 5750 6750 60  0001 C CNN "Ordercode"
F 6 "0,106" H 5750 6750 60  0001 C CNN "Cost"
	1    5750 6750
	1    0    0    1   
$EndComp
$Comp
L +3.3V #PWR082
U 1 1 5A7FF619
P 2250 650
F 0 "#PWR082" H 2250 500 50  0001 C CNN
F 1 "+3.3V" H 2250 790 50  0000 C CNN
F 2 "" H 2250 650 50  0001 C CNN
F 3 "" H 2250 650 50  0001 C CNN
	1    2250 650 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 1100 2250 1150
Wire Wire Line
	1750 1350 1950 1350
$Comp
L R R34
U 1 1 5A7FF621
P 1600 1350
F 0 "R34" V 1680 1350 50  0000 C CNN
F 1 "100k" V 1600 1350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 1530 1350 50  0001 C CNN
F 3 "" H 1600 1350 50  0001 C CNN
F 4 "Farnell" H 1600 1350 60  0001 C CNN "Distributor"
F 5 "2447226" H 1600 1350 60  0001 C CNN "Ordercode"
F 6 "0,0033" H 1600 1350 60  0001 C CNN "Cost"
	1    1600 1350
	0    -1   -1   0   
$EndComp
$Comp
L +3.3V #PWR083
U 1 1 5A7FF629
P 1350 1200
F 0 "#PWR083" H 1350 1050 50  0001 C CNN
F 1 "+3.3V" H 1350 1340 50  0000 C CNN
F 2 "" H 1350 1200 50  0001 C CNN
F 3 "" H 1350 1200 50  0001 C CNN
	1    1350 1200
	1    0    0    -1  
$EndComp
$Comp
L Q_PMOS_GSD Q7
U 1 1 5A7FF633
P 2150 1350
F 0 "Q7" H 2350 1400 50  0000 L CNN
F 1 "DMP2170" H 2350 1300 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2350 1450 50  0001 C CNN
F 3 "http://www.farnell.com/datasheets/1842110.pdf" H 2150 1350 50  0001 C CNN
F 4 "Farnell" H 2150 1350 60  0001 C CNN "Distributor"
F 5 "2709531" H 2150 1350 60  0001 C CNN "Ordercode"
F 6 "0,156" H 2150 1350 60  0001 C CNN "Cost"
	1    2150 1350
	1    0    0    1   
$EndComp
Connection ~ 2250 1650
Wire Wire Line
	1350 1200 1350 1350
Wire Wire Line
	1350 1350 1450 1350
Wire Wire Line
	1850 1450 1850 1350
Connection ~ 1850 1350
Text Notes 3550 1800 0    60   ~ 0
SD-Karte z.B:\nWatterott: 20110999\nReichelt: INTENSO 3413450\nFarnell: 2309539
$Comp
L GS2 J21
U 1 1 5A8A697A
P 2250 900
F 0 "J21" H 2350 1050 50  0000 C CNN
F 1 "GS2" H 2350 751 50  0000 C CNN
F 2 "Connectors:GS2" V 2324 900 50  0001 C CNN
F 3 "" H 2250 900 50  0001 C CNN
	1    2250 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 650  2250 700 
Wire Wire Line
	1300 6150 2250 6150
Wire Wire Line
	1300 6150 1200 6250
Wire Wire Line
	1200 6250 1150 6250
Wire Wire Line
	1150 6150 1200 6150
Wire Wire Line
	1200 6150 1300 6250
Wire Wire Line
	1300 6250 2450 6250
$EndSCHEMATC
