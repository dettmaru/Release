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
Sheet 4 4
Title "RhizoTech - Motherboard - UserInterface"
Date "30.01.2018"
Rev "V2"
Comp "TH Köln Institut für Nachrichtentechnik"
Comment1 "Erstellt von Daniel Lohmann"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L SW_Push SW1
U 1 1 5A702DFC
P 1350 1550
F 0 "SW1" H 1400 1650 50  0000 L CNN
F 1 "SW_Push" H 1350 1490 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_EVQQ2" H 1350 1750 50  0001 C CNN
F 3 "http://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Customer+Drawing%7F1977223%7FA5%7Fpdf%7FEnglish%7FENG_CD_1977223_A5.pdf%7F1977223-1" H 1350 1750 50  0001 C CNN
F 4 "Farnell" H 1350 1550 60  0001 C CNN "Distributor"
F 5 "2610948" H 1350 1550 60  0001 C CNN "Ordercode"
F 6 "0,143" H 1350 1550 60  0001 C CNN "Cost"
	1    1350 1550
	0    1    1    0   
$EndComp
Text Notes 950  1100 0    60   ~ 0
Reset Button
$Comp
L GND #PWR084
U 1 1 5A702E8F
P 1350 1850
F 0 "#PWR084" H 1350 1600 50  0001 C CNN
F 1 "GND" H 1350 1700 50  0000 C CNN
F 2 "" H 1350 1850 50  0001 C CNN
F 3 "" H 1350 1850 50  0001 C CNN
	1    1350 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 1850 1350 1750
Wire Wire Line
	1350 1350 1350 1250
Text HLabel 1350 1250 0    60   BiDi ~ 0
RESET
$Comp
L LED D6
U 1 1 5A702EC0
P 1250 3550
F 0 "D6" H 1250 3650 50  0000 C CNN
F 1 "Gelb" H 1250 3450 50  0000 C CNN
F 2 "LEDs:LED_0603_HandSoldering" H 1250 3550 50  0001 C CNN
F 3 "" H 1250 3550 50  0001 C CNN
F 4 "Farnell" H 1250 3550 60  0001 C CNN "Distributor"
F 5 "2469452" H 1250 3550 60  0001 C CNN "Ordercode"
F 6 "0,218" H 1250 3550 60  0001 C CNN "Cost"
	1    1250 3550
	0    -1   -1   0   
$EndComp
$Comp
L LED D7
U 1 1 5A702F83
P 1550 3550
F 0 "D7" H 1550 3650 50  0000 C CNN
F 1 "Gelb" H 1550 3450 50  0000 C CNN
F 2 "LEDs:LED_0603_HandSoldering" H 1550 3550 50  0001 C CNN
F 3 "" H 1550 3550 50  0001 C CNN
F 4 "Farnell" H 1550 3550 60  0001 C CNN "Distributor"
F 5 "2469452" H 1550 3550 60  0001 C CNN "Ordercode"
F 6 "0,218" H 1550 3550 60  0001 C CNN "Cost"
	1    1550 3550
	0    -1   -1   0   
$EndComp
$Comp
L LED D8
U 1 1 5A702FA4
P 1850 3550
F 0 "D8" H 1850 3650 50  0000 C CNN
F 1 "Rot" H 1850 3450 50  0000 C CNN
F 2 "LEDs:LED_0603_HandSoldering" H 1850 3550 50  0001 C CNN
F 3 "" H 1850 3550 50  0001 C CNN
F 4 "Farnell" H 1850 3550 60  0001 C CNN "Distributor"
F 5 "2469449" H 1850 3550 60  0001 C CNN "Ordercode"
F 6 "0,218" H 1850 3550 60  0001 C CNN "Cost"
	1    1850 3550
	0    -1   -1   0   
$EndComp
$Comp
L LED D10
U 1 1 5A702FC8
P 2450 3550
F 0 "D10" H 2450 3650 50  0000 C CNN
F 1 "Rot" H 2450 3450 50  0000 C CNN
F 2 "LEDs:LED_0603_HandSoldering" H 2450 3550 50  0001 C CNN
F 3 "" H 2450 3550 50  0001 C CNN
F 4 "Farnell" H 2450 3550 60  0001 C CNN "Distributor"
F 5 "2469449" H 2450 3550 60  0001 C CNN "Ordercode"
F 6 "0,218" H 2450 3550 60  0001 C CNN "Cost"
	1    2450 3550
	0    -1   -1   0   
$EndComp
$Comp
L R R25
U 1 1 5A702FF1
P 2450 3150
F 0 "R25" V 2530 3150 50  0000 C CNN
F 1 "1k" V 2450 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2380 3150 50  0001 C CNN
F 3 "" H 2450 3150 50  0001 C CNN
F 4 "Farnell" H 2450 3150 60  0001 C CNN "Distributor"
F 5 "2447272" H 2450 3150 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 2450 3150 60  0001 C CNN "Cost"
	1    2450 3150
	1    0    0    -1  
$EndComp
$Comp
L R R22
U 1 1 5A703018
P 1850 3150
F 0 "R22" V 1930 3150 50  0000 C CNN
F 1 "1k" V 1850 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 1780 3150 50  0001 C CNN
F 3 "" H 1850 3150 50  0001 C CNN
F 4 "Farnell" H 1850 3150 60  0001 C CNN "Distributor"
F 5 "2447272" H 1850 3150 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 1850 3150 60  0001 C CNN "Cost"
	1    1850 3150
	1    0    0    -1  
$EndComp
$Comp
L R R21
U 1 1 5A70304C
P 1550 3150
F 0 "R21" V 1630 3150 50  0000 C CNN
F 1 "1k" V 1550 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 1480 3150 50  0001 C CNN
F 3 "" H 1550 3150 50  0001 C CNN
F 4 "Farnell" H 1550 3150 60  0001 C CNN "Distributor"
F 5 "2447272" H 1550 3150 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 1550 3150 60  0001 C CNN "Cost"
	1    1550 3150
	1    0    0    -1  
$EndComp
$Comp
L R R20
U 1 1 5A70307C
P 1250 3150
F 0 "R20" V 1330 3150 50  0000 C CNN
F 1 "1k" V 1250 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 1180 3150 50  0001 C CNN
F 3 "" H 1250 3150 50  0001 C CNN
F 4 "Farnell" H 1250 3150 60  0001 C CNN "Distributor"
F 5 "2447272" H 1250 3150 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 1250 3150 60  0001 C CNN "Cost"
	1    1250 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 3300 1250 3400
Wire Wire Line
	1550 3400 1550 3300
Wire Wire Line
	1850 3300 1850 3400
Wire Wire Line
	2450 3400 2450 3300
Wire Wire Line
	2450 2900 2450 3000
Wire Wire Line
	1250 2900 3050 2900
Wire Wire Line
	1850 2800 1850 3000
Connection ~ 1850 2900
Wire Wire Line
	1250 3000 1250 2900
Connection ~ 1550 2900
$Comp
L +3.3V #PWR085
U 1 1 5A703186
P 1850 2800
F 0 "#PWR085" H 1850 2650 50  0001 C CNN
F 1 "+3.3V" H 1850 2940 50  0000 C CNN
F 2 "" H 1850 2800 50  0001 C CNN
F 3 "" H 1850 2800 50  0001 C CNN
	1    1850 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 3700 1250 3800
Wire Wire Line
	1550 3800 1550 3700
Wire Wire Line
	1850 3700 1850 3800
Wire Wire Line
	2450 3800 2450 3700
Text HLabel 1250 3800 3    60   Input ~ 0
STM32_LED1
Text HLabel 1550 3800 3    60   Input ~ 0
STM32_LED2
Text HLabel 1850 3800 3    60   Input ~ 0
STM32_LED3
Text HLabel 2450 3800 3    60   Input ~ 0
ESP32_LED5
Text HLabel 2750 3800 3    60   Input ~ 0
ESP32_LED6
$Comp
L LED D11
U 1 1 5A70329C
P 2750 3550
F 0 "D11" H 2750 3650 50  0000 C CNN
F 1 "Grün" H 2750 3450 50  0000 C CNN
F 2 "LEDs:LED_0603_HandSoldering" H 2750 3550 50  0001 C CNN
F 3 "" H 2750 3550 50  0001 C CNN
F 4 "Farnell" H 2750 3550 60  0001 C CNN "Distributor"
F 5 "2469448" H 2750 3550 60  0001 C CNN "Ordercode"
F 6 "0,222" H 2750 3550 60  0001 C CNN "Cost"
	1    2750 3550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2750 3700 2750 3800
$Comp
L R R26
U 1 1 5A70330D
P 2750 3150
F 0 "R26" V 2830 3150 50  0000 C CNN
F 1 "1k" V 2750 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2680 3150 50  0001 C CNN
F 3 "" H 2750 3150 50  0001 C CNN
F 4 "Farnell" H 2750 3150 60  0001 C CNN "Distributor"
F 5 "2447272" H 2750 3150 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 2750 3150 60  0001 C CNN "Cost"
	1    2750 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 3300 2750 3400
Wire Wire Line
	2750 2900 2750 3000
Connection ~ 2450 2900
Wire Wire Line
	1550 3000 1550 2900
$Comp
L SW_Push SW2
U 1 1 5A70461F
P 2100 1550
F 0 "SW2" H 2150 1650 50  0000 L CNN
F 1 "SW_Push" H 2100 1490 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_EVQQ2" H 2100 1750 50  0001 C CNN
F 3 "http://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Customer+Drawing%7F1977223%7FA5%7Fpdf%7FEnglish%7FENG_CD_1977223_A5.pdf%7F1977223-1" H 2100 1750 50  0001 C CNN
F 4 "Farnell" H 2100 1550 60  0001 C CNN "Distributor"
F 5 "2610948" H 2100 1550 60  0001 C CNN "Ordercode"
F 6 "0,143" H 2100 1550 60  0001 C CNN "Cost"
	1    2100 1550
	0    1    1    0   
$EndComp
$Comp
L SW_Push SW3
U 1 1 5A704664
P 2850 1550
F 0 "SW3" H 2900 1650 50  0000 L CNN
F 1 "SW_Push" H 2850 1490 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_EVQQ2" H 2850 1750 50  0001 C CNN
F 3 "http://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Customer+Drawing%7F1977223%7FA5%7Fpdf%7FEnglish%7FENG_CD_1977223_A5.pdf%7F1977223-1" H 2850 1750 50  0001 C CNN
F 4 "Farnell" H 2850 1550 60  0001 C CNN "Distributor"
F 5 "2610948" H 2850 1550 60  0001 C CNN "Ordercode"
F 6 "0,143" H 2850 1550 60  0001 C CNN "Cost"
	1    2850 1550
	0    1    1    0   
$EndComp
Wire Wire Line
	2850 1750 2850 1850
Wire Wire Line
	2100 1750 2100 1850
Wire Wire Line
	2100 1200 2100 1350
Wire Wire Line
	2850 1200 2850 1350
Text HLabel 2100 1850 0    60   Output ~ 0
BT1
Text HLabel 2850 1850 0    60   Output ~ 0
BT2
$Comp
L SW_Push SW4
U 1 1 5A70DA21
P 3600 1550
F 0 "SW4" H 3650 1650 50  0000 L CNN
F 1 "SW_Push" H 3600 1490 50  0000 C CNN
F 2 "Buttons_Switches_SMD:SW_SPST_EVQQ2" H 3600 1750 50  0001 C CNN
F 3 "http://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Customer+Drawing%7F1977223%7FA5%7Fpdf%7FEnglish%7FENG_CD_1977223_A5.pdf%7F1977223-1" H 3600 1750 50  0001 C CNN
F 4 "Farnell" H 3600 1550 60  0001 C CNN "Distributor"
F 5 "2610948" H 3600 1550 60  0001 C CNN "Ordercode"
F 6 "0,143" H 3600 1550 60  0001 C CNN "Cost"
	1    3600 1550
	0    1    1    0   
$EndComp
Wire Wire Line
	3600 1750 3600 1850
Wire Wire Line
	3600 1200 3600 1350
Text HLabel 3600 1850 0    60   Output ~ 0
BT3
$Comp
L C C22
U 1 1 5A70E971
P 2350 1550
F 0 "C22" H 2375 1650 50  0000 L CNN
F 1 "100nF" H 2375 1450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 2388 1400 50  0001 C CNN
F 3 "" H 2350 1550 50  0001 C CNN
F 4 "Farnell" H 2350 1550 60  0001 C CNN "Distributor"
F 5 "1759017" H 2350 1550 60  0001 C CNN "Ordercode"
F 6 "0,0199" H 2350 1550 60  0001 C CNN "Cost"
	1    2350 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 1700 2350 1800
Wire Wire Line
	2350 1800 2100 1800
Connection ~ 2100 1800
Wire Wire Line
	2350 1400 2350 1300
Wire Wire Line
	2350 1300 2100 1300
Connection ~ 2100 1300
$Comp
L C C23
U 1 1 5A70EB23
P 3100 1550
F 0 "C23" H 3125 1650 50  0000 L CNN
F 1 "100nF" H 3125 1450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 3138 1400 50  0001 C CNN
F 3 "" H 3100 1550 50  0001 C CNN
F 4 "Farnell" H 3100 1550 60  0001 C CNN "Distributor"
F 5 "1759017" H 3100 1550 60  0001 C CNN "Ordercode"
F 6 "0,0199" H 3100 1550 60  0001 C CNN "Cost"
	1    3100 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 1700 3100 1800
Wire Wire Line
	3100 1800 2850 1800
Connection ~ 2850 1800
Wire Wire Line
	2850 1300 3100 1300
Wire Wire Line
	3100 1300 3100 1400
Connection ~ 2850 1300
$Comp
L C C24
U 1 1 5A70EBEA
P 3900 1550
F 0 "C24" H 3925 1650 50  0000 L CNN
F 1 "100nF" H 3925 1450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 3938 1400 50  0001 C CNN
F 3 "" H 3900 1550 50  0001 C CNN
F 4 "Farnell" H 3900 1550 60  0001 C CNN "Distributor"
F 5 "1759017" H 3900 1550 60  0001 C CNN "Ordercode"
F 6 "0,0199" H 3900 1550 60  0001 C CNN "Cost"
	1    3900 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 1400 3900 1300
Wire Wire Line
	3900 1300 3600 1300
Connection ~ 3600 1300
Wire Wire Line
	3900 1700 3900 1800
Wire Wire Line
	3900 1800 3600 1800
Connection ~ 3600 1800
$Comp
L R R23
U 1 1 5A711132
P 2100 1050
F 0 "R23" V 2180 1050 50  0000 C CNN
F 1 "10k" V 2100 1050 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2030 1050 50  0001 C CNN
F 3 "" H 2100 1050 50  0001 C CNN
F 4 "Farnell" H 2100 1050 60  0001 C CNN "Distributor"
F 5 "2447230" H 2100 1050 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 2100 1050 60  0001 C CNN "Cost"
	1    2100 1050
	1    0    0    -1  
$EndComp
$Comp
L R R27
U 1 1 5A7111D6
P 2850 1050
F 0 "R27" V 2930 1050 50  0000 C CNN
F 1 "10k" V 2850 1050 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2780 1050 50  0001 C CNN
F 3 "" H 2850 1050 50  0001 C CNN
F 4 "Farnell" H 2850 1050 60  0001 C CNN "Distributor"
F 5 "2447230" H 2850 1050 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 2850 1050 60  0001 C CNN "Cost"
	1    2850 1050
	1    0    0    -1  
$EndComp
$Comp
L R R29
U 1 1 5A71123D
P 3600 1050
F 0 "R29" V 3680 1050 50  0000 C CNN
F 1 "10k" V 3600 1050 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 3530 1050 50  0001 C CNN
F 3 "" H 3600 1050 50  0001 C CNN
F 4 "Farnell" H 3600 1050 60  0001 C CNN "Distributor"
F 5 "2447230" H 3600 1050 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 3600 1050 60  0001 C CNN "Cost"
	1    3600 1050
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR086
U 1 1 5A711DB7
P 2100 800
F 0 "#PWR086" H 2100 650 50  0001 C CNN
F 1 "+3V3" H 2100 940 50  0000 C CNN
F 2 "" H 2100 800 50  0001 C CNN
F 3 "" H 2100 800 50  0001 C CNN
	1    2100 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 800  2100 900 
$Comp
L +3V3 #PWR087
U 1 1 5A711E98
P 2850 800
F 0 "#PWR087" H 2850 650 50  0001 C CNN
F 1 "+3V3" H 2850 940 50  0000 C CNN
F 2 "" H 2850 800 50  0001 C CNN
F 3 "" H 2850 800 50  0001 C CNN
	1    2850 800 
	1    0    0    -1  
$EndComp
$Comp
L +3V3 #PWR088
U 1 1 5A712097
P 3600 800
F 0 "#PWR088" H 3600 650 50  0001 C CNN
F 1 "+3V3" H 3600 940 50  0000 C CNN
F 2 "" H 3600 800 50  0001 C CNN
F 3 "" H 3600 800 50  0001 C CNN
	1    3600 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 800  3600 900 
Wire Wire Line
	2850 800  2850 900 
Text HLabel 3050 3800 3    60   Input ~ 0
ESP32_LED7
$Comp
L LED D12
U 1 1 5A7137F5
P 3050 3550
F 0 "D12" H 3050 3650 50  0000 C CNN
F 1 "Gelb" H 3050 3450 50  0000 C CNN
F 2 "LEDs:LED_0603_HandSoldering" H 3050 3550 50  0001 C CNN
F 3 "" H 3050 3550 50  0001 C CNN
F 4 "Farnell" H 3050 3550 60  0001 C CNN "Distributor"
F 5 "2469452" H 3050 3550 60  0001 C CNN "Ordercode"
F 6 "0,218" H 3050 3550 60  0001 C CNN "Cost"
	1    3050 3550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3050 3700 3050 3800
$Comp
L R R28
U 1 1 5A7137FC
P 3050 3150
F 0 "R28" V 3130 3150 50  0000 C CNN
F 1 "1k" V 3050 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2980 3150 50  0001 C CNN
F 3 "" H 3050 3150 50  0001 C CNN
F 4 "Farnell" H 3050 3150 60  0001 C CNN "Distributor"
F 5 "2447272" H 3050 3150 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 3050 3150 60  0001 C CNN "Cost"
	1    3050 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 3300 3050 3400
Wire Wire Line
	3050 2900 3050 3000
Connection ~ 2750 2900
$Comp
L LED D9
U 1 1 5A713946
P 2150 3550
F 0 "D9" H 2150 3650 50  0000 C CNN
F 1 "Grün" H 2150 3450 50  0000 C CNN
F 2 "LEDs:LED_0603_HandSoldering" H 2150 3550 50  0001 C CNN
F 3 "" H 2150 3550 50  0001 C CNN
F 4 "Farnell" H 2150 3550 60  0001 C CNN "Distributor"
F 5 "2469448" H 2150 3550 60  0001 C CNN "Ordercode"
F 6 "0,222" H 2150 3550 60  0001 C CNN "Cost"
	1    2150 3550
	0    -1   -1   0   
$EndComp
$Comp
L R R24
U 1 1 5A71394C
P 2150 3150
F 0 "R24" V 2230 3150 50  0000 C CNN
F 1 "1k" V 2150 3150 50  0000 C CNN
F 2 "Resistors_SMD:R_0603_HandSoldering" V 2080 3150 50  0001 C CNN
F 3 "" H 2150 3150 50  0001 C CNN
F 4 "Farnell" H 2150 3150 60  0001 C CNN "Distributor"
F 5 "2447272" H 2150 3150 60  0001 C CNN "Ordercode"
F 6 "0,0035" H 2150 3150 60  0001 C CNN "Cost"
	1    2150 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 3300 2150 3400
Wire Wire Line
	2150 2900 2150 3000
Wire Wire Line
	2150 3700 2150 3800
Text HLabel 2150 3800 3    60   Input ~ 0
STM32_LED4
Connection ~ 2150 2900
$EndSCHEMATC
