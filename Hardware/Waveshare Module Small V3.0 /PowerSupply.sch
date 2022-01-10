EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L suf:MCP16251 U201
U 1 1 5FBC07D1
P 4500 1900
F 0 "U201" H 4500 2503 60  0000 C CNN
F 1 "MCP16251" H 4500 2397 60  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6_Handsoldering" H 4500 1900 60  0001 C CNN
F 3 "" H 4500 1900 60  0000 C CNN
	1    4500 1900
	1    0    0    -1  
$EndComp
$Comp
L device:INDUCTOR L201
U 1 1 5FBC1359
P 3650 1250
F 0 "L201" V 3843 1250 40  0000 C CNN
F 1 "4,7µH" V 3767 1250 40  0000 C CNN
F 2 "Inductor_SMD:L_Bourns-SRN4018" H 3650 1250 50  0001 C CNN
F 3 "" H 3650 1250 50  0001 C CNN
	1    3650 1250
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R201
U 1 1 5FBC38FE
P 5400 2050
F 0 "R201" H 5470 2096 50  0000 L CNN
F 1 "1M69" H 5470 2005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5330 2050 50  0001 C CNN
F 3 "~" H 5400 2050 50  0001 C CNN
	1    5400 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R202
U 1 1 5FBC47F0
P 5400 2500
F 0 "R202" H 5470 2546 50  0000 L CNN
F 1 "1M" H 5470 2455 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5330 2500 50  0001 C CNN
F 3 "~" H 5400 2500 50  0001 C CNN
	1    5400 2500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0210
U 1 1 5FBC5696
P 5400 2800
F 0 "#PWR0210" H 5400 2550 50  0001 C CNN
F 1 "GND" H 5405 2627 50  0000 C CNN
F 2 "" H 5400 2800 50  0001 C CNN
F 3 "" H 5400 2800 50  0001 C CNN
	1    5400 2800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0208
U 1 1 5FBC59E5
P 4500 2800
F 0 "#PWR0208" H 4500 2550 50  0001 C CNN
F 1 "GND" H 4505 2627 50  0000 C CNN
F 2 "" H 4500 2800 50  0001 C CNN
F 3 "" H 4500 2800 50  0001 C CNN
	1    4500 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 1800 3850 1800
Wire Wire Line
	4000 2000 3850 2000
Wire Wire Line
	3850 2000 3850 1800
Wire Wire Line
	4500 2300 4500 2800
Wire Wire Line
	5400 2800 5400 2650
Wire Wire Line
	5400 2350 5400 2250
Wire Wire Line
	5000 2000 5150 2000
Wire Wire Line
	5150 2000 5150 2250
Wire Wire Line
	5150 2250 5400 2250
Connection ~ 5400 2250
Wire Wire Line
	5400 2250 5400 2200
Wire Wire Line
	5000 1800 5400 1800
Wire Wire Line
	5400 1800 5400 1900
Wire Wire Line
	5950 1800 5400 1800
Connection ~ 5400 1800
Connection ~ 3850 1800
$Comp
L Connector_Generic:Conn_01x02 J201
U 1 1 5FBCF60C
P 1950 5450
F 0 "J201" H 2030 5442 50  0000 L CNN
F 1 "Conn_01x02" H 2030 5351 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 1950 5450 50  0001 C CNN
F 3 "~" H 1950 5450 50  0001 C CNN
	1    1950 5450
	1    0    0    -1  
$EndComp
$Comp
L power:+3V0 #PWR0218
U 1 1 5FBD0A4F
P 1500 5050
F 0 "#PWR0218" H 1500 4900 50  0001 C CNN
F 1 "+3V0" H 1515 5223 50  0000 C CNN
F 2 "" H 1500 5050 50  0001 C CNN
F 3 "" H 1500 5050 50  0001 C CNN
	1    1500 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 5450 1750 5450
Wire Wire Line
	1500 5050 1500 5450
$Comp
L power:GND #PWR0219
U 1 1 5FBD1818
P 1500 5850
F 0 "#PWR0219" H 1500 5600 50  0001 C CNN
F 1 "GND" H 1505 5677 50  0000 C CNN
F 2 "" H 1500 5850 50  0001 C CNN
F 3 "" H 1500 5850 50  0001 C CNN
	1    1500 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 5850 1500 5550
Wire Wire Line
	1500 5550 1750 5550
Text Notes 1800 5150 0    50   ~ 0
Battery Connector 2xAAA Cells
$Comp
L Device:R R203
U 1 1 5FBD37F7
P 5500 4950
F 0 "R203" H 5570 4996 50  0000 L CNN
F 1 "1M" H 5570 4905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5430 4950 50  0001 C CNN
F 3 "~" H 5500 4950 50  0001 C CNN
	1    5500 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R204
U 1 1 5FBD37FD
P 5500 5400
F 0 "R204" H 5570 5446 50  0000 L CNN
F 1 "1M" H 5570 5355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5430 5400 50  0001 C CNN
F 3 "~" H 5500 5400 50  0001 C CNN
	1    5500 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0221
U 1 1 5FBD3803
P 5500 5700
F 0 "#PWR0221" H 5500 5450 50  0001 C CNN
F 1 "GND" H 5505 5527 50  0000 C CNN
F 2 "" H 5500 5700 50  0001 C CNN
F 3 "" H 5500 5700 50  0001 C CNN
	1    5500 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 5700 5500 5550
Wire Wire Line
	5500 5250 5500 5150
Wire Wire Line
	5100 4900 5250 4900
Wire Wire Line
	5250 4900 5250 5150
Wire Wire Line
	5250 5150 5500 5150
Connection ~ 5500 5150
Wire Wire Line
	5500 5150 5500 5100
Wire Wire Line
	5500 4700 5500 4800
$Comp
L power:+3V0 #PWR0220
U 1 1 5FBD4BDC
P 5500 4700
F 0 "#PWR0220" H 5500 4550 50  0001 C CNN
F 1 "+3V0" H 5515 4873 50  0000 C CNN
F 2 "" H 5500 4700 50  0001 C CNN
F 3 "" H 5500 4700 50  0001 C CNN
	1    5500 4700
	1    0    0    -1  
$EndComp
Text GLabel 5100 4900 0    50   Input ~ 0
AIN_U_BAT
$Comp
L power:GND #PWR0201
U 1 1 5FBD7700
P 2000 2800
F 0 "#PWR0201" H 2000 2550 50  0001 C CNN
F 1 "GND" H 2005 2627 50  0000 C CNN
F 2 "" H 2000 2800 50  0001 C CNN
F 3 "" H 2000 2800 50  0001 C CNN
	1    2000 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 2800 2000 2500
$Comp
L device:C C204
U 1 1 5FBDAC65
P 2500 2300
F 0 "C204" H 2630 2346 50  0000 L CNN
F 1 "100nF" H 2630 2255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 2500 2300 50  0001 C CNN
F 3 "" H 2500 2300 50  0001 C CNN
	1    2500 2300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0206
U 1 1 5FBDAC6B
P 2500 2800
F 0 "#PWR0206" H 2500 2550 50  0001 C CNN
F 1 "GND" H 2505 2627 50  0000 C CNN
F 2 "" H 2500 2800 50  0001 C CNN
F 3 "" H 2500 2800 50  0001 C CNN
	1    2500 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 2100 2500 1800
Wire Wire Line
	2500 2800 2500 2500
$Comp
L power:GND #PWR0209
U 1 1 5FBDF621
P 6450 2800
F 0 "#PWR0209" H 6450 2550 50  0001 C CNN
F 1 "GND" H 6455 2627 50  0000 C CNN
F 2 "" H 6450 2800 50  0001 C CNN
F 3 "" H 6450 2800 50  0001 C CNN
	1    6450 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 2100 6450 1800
Wire Wire Line
	6450 2800 6450 2500
$Comp
L device:C C210
U 1 1 5FBDF645
P 6950 2300
F 0 "C210" H 7080 2346 50  0000 L CNN
F 1 "100nF" H 7080 2255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 6950 2300 50  0001 C CNN
F 3 "" H 6950 2300 50  0001 C CNN
	1    6950 2300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0216
U 1 1 5FBDF64B
P 6950 2800
F 0 "#PWR0216" H 6950 2550 50  0001 C CNN
F 1 "GND" H 6955 2627 50  0000 C CNN
F 2 "" H 6950 2800 50  0001 C CNN
F 3 "" H 6950 2800 50  0001 C CNN
	1    6950 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 2800 6950 2500
$Comp
L Device:CP C205
U 1 1 5FBE09B5
P 3000 2300
F 0 "C205" H 3118 2346 50  0000 L CNN
F 1 "10µF" H 3118 2255 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" H 3038 2150 50  0001 C CNN
F 3 "~" H 3000 2300 50  0001 C CNN
	1    3000 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 1800 3000 2150
$Comp
L power:GND #PWR0207
U 1 1 5FBE65E1
P 3000 2800
F 0 "#PWR0207" H 3000 2550 50  0001 C CNN
F 1 "GND" H 3005 2627 50  0000 C CNN
F 2 "" H 3000 2800 50  0001 C CNN
F 3 "" H 3000 2800 50  0001 C CNN
	1    3000 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 2450 3000 2800
$Comp
L power:+3V3 #PWR0212
U 1 1 5FBEFD41
P 6950 1500
F 0 "#PWR0212" H 6950 1350 50  0001 C CNN
F 1 "+3V3" H 6965 1673 50  0000 C CNN
F 2 "" H 6950 1500 50  0001 C CNN
F 3 "" H 6950 1500 50  0001 C CNN
	1    6950 1500
	1    0    0    -1  
$EndComp
$Comp
L power:+3V0 #PWR0204
U 1 1 5FBF1AFB
P 2000 1500
F 0 "#PWR0204" H 2000 1350 50  0001 C CNN
F 1 "+3V0" H 2015 1673 50  0000 C CNN
F 2 "" H 2000 1500 50  0001 C CNN
F 3 "" H 2000 1500 50  0001 C CNN
	1    2000 1500
	1    0    0    -1  
$EndComp
$Comp
L device:C C206
U 1 1 5FBDF61B
P 6450 2300
F 0 "C206" H 6580 2346 50  0000 L CNN
F 1 "100pF" H 6580 2255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 6450 2300 50  0001 C CNN
F 3 "" H 6450 2300 50  0001 C CNN
	1    6450 2300
	1    0    0    -1  
$EndComp
$Comp
L device:C C201
U 1 1 5FBD76FA
P 2000 2300
F 0 "C201" H 2130 2346 50  0000 L CNN
F 1 "100pF" H 2130 2255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 2000 2300 50  0001 C CNN
F 3 "" H 2000 2300 50  0001 C CNN
	1    2000 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 1800 3850 1800
Connection ~ 3000 1800
Wire Wire Line
	4500 1500 4500 1250
Wire Wire Line
	4500 1250 3950 1250
Wire Wire Line
	3350 1250 3000 1250
Wire Wire Line
	3000 1250 3000 1800
Wire Wire Line
	5950 2800 5950 2500
Connection ~ 5950 1800
Wire Wire Line
	5950 2100 5950 1800
$Comp
L power:GND #PWR0215
U 1 1 5FBC4A7E
P 5950 2800
F 0 "#PWR0215" H 5950 2550 50  0001 C CNN
F 1 "GND" H 5955 2627 50  0000 C CNN
F 2 "" H 5950 2800 50  0001 C CNN
F 3 "" H 5950 2800 50  0001 C CNN
	1    5950 2800
	1    0    0    -1  
$EndComp
$Comp
L device:C C209
U 1 1 5FBC2775
P 5950 2300
F 0 "C209" H 6080 2346 50  0000 L CNN
F 1 "22µF" H 6080 2255 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.4" H 5950 2300 50  0001 C CNN
F 3 "" H 5950 2300 50  0001 C CNN
	1    5950 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 1800 6450 1800
Wire Wire Line
	2500 1800 3000 1800
Wire Wire Line
	2000 1500 2000 1800
Wire Wire Line
	2500 1800 2000 1800
Connection ~ 2500 1800
Connection ~ 2000 1800
Wire Wire Line
	2000 1800 2000 2100
Wire Wire Line
	6950 1500 6950 1800
Wire Wire Line
	6450 1800 6950 1800
Connection ~ 6450 1800
Connection ~ 6950 1800
Wire Wire Line
	6950 1800 6950 2100
$Comp
L device:DIODE D201
U 1 1 5FDDD3DA
P 7500 2350
F 0 "D201" V 7538 2272 40  0000 R CNN
F 1 "DESD5V0U1BA-7" V 7462 2272 40  0000 R CNN
F 2 "Diode_SMD:D_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 7500 2350 50  0001 C CNN
F 3 "" H 7500 2350 50  0001 C CNN
	1    7500 2350
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0202
U 1 1 5FDDE619
P 7500 2800
F 0 "#PWR0202" H 7500 2550 50  0001 C CNN
F 1 "GND" H 7505 2627 50  0000 C CNN
F 2 "" H 7500 2800 50  0001 C CNN
F 3 "" H 7500 2800 50  0001 C CNN
	1    7500 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 2800 7500 2550
Wire Wire Line
	7500 2150 7500 1800
Wire Wire Line
	7500 1800 6950 1800
$EndSCHEMATC
