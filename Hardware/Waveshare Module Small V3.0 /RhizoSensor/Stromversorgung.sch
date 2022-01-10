EESchema Schematic File Version 5
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
Comment5 ""
Comment6 ""
Comment7 ""
Comment8 ""
Comment9 ""
$EndDescr
$Comp
L RhizoSensor-rescue:MCP16251-suf U402
U 1 1 5D987942
P 4800 2150
F 0 "U402" H 5150 1800 60  0000 C CNN
F 1 "MCP16251" H 5150 1700 60  0000 C CNN
F 2 "Package_TO_SOT_SMD:TSOT-23-6_HandSoldering" H 4800 2150 60  0001 C CNN
F 3 "" H 4800 2150 60  0000 C CNN
	1    4800 2150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R406
U 1 1 5D989592
P 5700 2350
F 0 "R406" H 5770 2396 50  0000 L CNN
F 1 "1M69" H 5770 2305 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5630 2350 50  0001 C CNN
F 3 "~" H 5700 2350 50  0001 C CNN
	1    5700 2350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R407
U 1 1 5DAF23CA
P 5700 2850
F 0 "R407" H 5770 2896 50  0000 L CNN
F 1 "1M" H 5770 2805 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5630 2850 50  0001 C CNN
F 3 "~" H 5700 2850 50  0001 C CNN
	1    5700 2850
	1    0    0    -1  
$EndComp
$Comp
L pspice:INDUCTOR L401
U 1 1 5D98A6B0
P 4350 1550
F 0 "L401" H 4350 1765 50  0000 C CNN
F 1 "INDUCTOR" H 4350 1674 50  0000 C CNN
F 2 "Inductor_SMD:L_Coilcraft_LPS4018" H 4350 1550 50  0001 C CNN
F 3 "~" H 4350 1550 50  0001 C CNN
	1    4350 1550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R401
U 1 1 5D98B8C4
P 1950 1850
F 0 "R401" H 2020 1896 50  0000 L CNN
F 1 "2M4" H 2020 1805 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1880 1850 50  0001 C CNN
F 3 "~" H 1950 1850 50  0001 C CNN
	1    1950 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R402
U 1 1 5DAF23CB
P 1950 2300
F 0 "R402" H 2020 2346 50  0000 L CNN
F 1 "2M" H 2020 2255 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1880 2300 50  0001 C CNN
F 3 "~" H 1950 2300 50  0001 C CNN
	1    1950 2300
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC557 Q401
U 1 1 5D98D28F
P 2600 2050
F 0 "Q401" H 2791 2004 50  0000 L CNN
F 1 "BC557" H 2791 2095 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2800 1975 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC557.pdf" H 2600 2050 50  0001 L CNN
	1    2600 2050
	1    0    0    1   
$EndComp
$Comp
L Transistor_BJT:BC547 Q402
U 1 1 5D98FB9F
P 2700 2800
F 0 "Q402" H 2891 2846 50  0000 L CNN
F 1 "BC547" H 2891 2755 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2900 2725 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC547.pdf" H 2700 2800 50  0001 L CNN
	1    2700 2800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1950 1310 1950 1550
Wire Wire Line
	1950 2000 1950 2050
$Comp
L power:GND #PWR0402
U 1 1 5D995719
P 1950 3100
F 0 "#PWR0402" H 1950 2850 50  0001 C CNN
F 1 "GND" H 1955 2927 50  0000 C CNN
F 2 "" H 1950 3100 50  0001 C CNN
F 3 "" H 1950 3100 50  0001 C CNN
	1    1950 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 3100 1950 2900
Wire Wire Line
	2700 1850 2700 1550
Wire Wire Line
	2700 1550 1950 1550
Connection ~ 1950 1550
Wire Wire Line
	1950 1550 1950 1700
Wire Wire Line
	2400 2050 1950 2050
Connection ~ 1950 2050
Wire Wire Line
	1950 2050 1950 2150
$Comp
L Device:R R404
U 1 1 5DAF23CC
P 3100 2550
F 0 "R404" H 3170 2596 50  0000 L CNN
F 1 "3M" H 3170 2505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3030 2550 50  0001 C CNN
F 3 "~" H 3100 2550 50  0001 C CNN
	1    3100 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R405
U 1 1 5D995EF3
P 3500 2550
F 0 "R405" H 3570 2596 50  0000 L CNN
F 1 "1M" H 3570 2505 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3430 2550 50  0001 C CNN
F 3 "~" H 3500 2550 50  0001 C CNN
	1    3500 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 2400 3500 2250
Wire Wire Line
	3500 2250 3100 2250
Wire Wire Line
	3100 2400 3100 2250
Connection ~ 3100 2250
Wire Wire Line
	3100 2250 2700 2250
$Comp
L power:GND #PWR0405
U 1 1 5D998A67
P 3500 3100
F 0 "#PWR0405" H 3500 2850 50  0001 C CNN
F 1 "GND" H 3505 2927 50  0000 C CNN
F 2 "" H 3500 3100 50  0001 C CNN
F 3 "" H 3500 3100 50  0001 C CNN
	1    3500 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 3100 3500 2700
$Comp
L power:GND #PWR0403
U 1 1 5D99B816
P 2600 3100
F 0 "#PWR0403" H 2600 2850 50  0001 C CNN
F 1 "GND" H 2605 2927 50  0000 C CNN
F 2 "" H 2600 3100 50  0001 C CNN
F 3 "" H 2600 3100 50  0001 C CNN
	1    2600 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 3100 2600 3000
Wire Wire Line
	3100 2700 3100 2800
Wire Wire Line
	3100 2800 2900 2800
$Comp
L Device:R R403
U 1 1 5D98CAF1
P 1950 2750
F 0 "R403" H 2020 2796 50  0000 L CNN
F 1 "2M2" H 2020 2705 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1880 2750 50  0001 C CNN
F 3 "~" H 1950 2750 50  0001 C CNN
	1    1950 2750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0406
U 1 1 5D99E575
P 3950 3100
F 0 "#PWR0406" H 3950 2850 50  0001 C CNN
F 1 "GND" H 3955 2927 50  0000 C CNN
F 2 "" H 3950 3100 50  0001 C CNN
F 3 "" H 3950 3100 50  0001 C CNN
	1    3950 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0411
U 1 1 5D99EAF0
P 4800 3100
F 0 "#PWR0411" H 4800 2850 50  0001 C CNN
F 1 "GND" H 4805 2927 50  0000 C CNN
F 2 "" H 4800 3100 50  0001 C CNN
F 3 "" H 4800 3100 50  0001 C CNN
	1    4800 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0412
U 1 1 5DAF23CD
P 5700 3100
F 0 "#PWR0412" H 5700 2850 50  0001 C CNN
F 1 "GND" H 5705 2927 50  0000 C CNN
F 2 "" H 5700 3100 50  0001 C CNN
F 3 "" H 5700 3100 50  0001 C CNN
	1    5700 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0414
U 1 1 5D99F324
P 6350 3100
F 0 "#PWR0414" H 6350 2850 50  0001 C CNN
F 1 "GND" H 6355 2927 50  0000 C CNN
F 2 "" H 6350 3100 50  0001 C CNN
F 3 "" H 6350 3100 50  0001 C CNN
	1    6350 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 3100 3950 2700
Wire Wire Line
	4800 3100 4800 2550
Wire Wire Line
	4300 2250 3950 2250
Wire Wire Line
	3950 2250 3950 2400
Wire Wire Line
	3950 2250 3500 2250
Connection ~ 3950 2250
Connection ~ 3500 2250
Wire Wire Line
	4800 1750 4800 1550
Wire Wire Line
	4800 1550 4600 1550
Wire Wire Line
	4100 1550 3900 1550
Wire Wire Line
	3900 2050 4300 2050
Wire Wire Line
	2700 1550 3900 1550
Connection ~ 2700 1550
Connection ~ 3900 1550
Wire Wire Line
	3900 1550 3900 2050
Wire Wire Line
	5700 3100 5700 3000
Wire Wire Line
	5700 2700 5700 2600
Wire Wire Line
	5700 2200 5700 2050
Wire Wire Line
	5700 2050 5300 2050
Wire Wire Line
	6350 2450 6350 2050
Wire Wire Line
	6350 2050 5700 2050
Connection ~ 5700 2050
Wire Wire Line
	6350 3100 6350 2750
Wire Wire Line
	5300 2250 5500 2250
Wire Wire Line
	5500 2250 5500 2600
Wire Wire Line
	5500 2600 5700 2600
Connection ~ 5700 2600
Wire Wire Line
	5700 2600 5700 2500
$Comp
L power:+3.3V #PWR0413
U 1 1 5D9B2F3B
P 6350 1650
F 0 "#PWR0413" H 6350 1500 50  0001 C CNN
F 1 "+3.3V" H 6365 1823 50  0000 C CNN
F 2 "" H 6350 1650 50  0001 C CNN
F 3 "" H 6350 1650 50  0001 C CNN
	1    6350 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 1650 6350 2050
Connection ~ 6350 2050
Wire Wire Line
	1950 2450 1950 2500
Wire Wire Line
	2600 2600 2600 2500
Wire Wire Line
	2600 2500 1950 2500
Connection ~ 1950 2500
Wire Wire Line
	1950 2500 1950 2600
$Comp
L RhizoSensor-rescue:ICL7660S-suf U?
U 1 1 5D9B5ABB
P 3450 5600
AR Path="/5D9B5ABB" Ref="U?"  Part="1" 
AR Path="/5D992CA4/5D9B5ABB" Ref="U401"  Part="1" 
F 0 "U401" H 3425 5999 60  0000 C CNN
F 1 "ICL7660S" H 3425 5892 60  0000 C CNN
F 2 "Package_SO:Diodes_SO-8EP" H 3450 5600 60  0001 C CNN
F 3 "" H 3450 5600 60  0000 C CNN
	1    3450 5600
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0408
U 1 1 5D9C0D2E
P 4100 5000
F 0 "#PWR0408" H 4100 4850 50  0001 C CNN
F 1 "+3.3V" H 4115 5173 50  0000 C CNN
F 2 "" H 4100 5000 50  0001 C CNN
F 3 "" H 4100 5000 50  0001 C CNN
	1    4100 5000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0404
U 1 1 5DAF23CE
P 2850 6300
F 0 "#PWR0404" H 2850 6050 50  0001 C CNN
F 1 "GND" H 2855 6127 50  0000 C CNN
F 2 "" H 2850 6300 50  0001 C CNN
F 3 "" H 2850 6300 50  0001 C CNN
	1    2850 6300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0410
U 1 1 5D9C147D
P 4450 6300
F 0 "#PWR0410" H 4450 6050 50  0001 C CNN
F 1 "GND" H 4455 6127 50  0000 C CNN
F 2 "" H 4450 6300 50  0001 C CNN
F 3 "" H 4450 6300 50  0001 C CNN
	1    4450 6300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0407
U 1 1 5D9C2988
P 4050 6300
F 0 "#PWR0407" H 4050 6050 50  0001 C CNN
F 1 "GND" H 4055 6127 50  0000 C CNN
F 2 "" H 4050 6300 50  0001 C CNN
F 3 "" H 4050 6300 50  0001 C CNN
	1    4050 6300
	1    0    0    -1  
$EndComp
$Comp
L power:-3V3 #PWR0409
U 1 1 5D9C2C55
P 4450 5000
F 0 "#PWR0409" H 4450 5100 50  0001 C CNN
F 1 "-3V3" H 4465 5173 50  0000 C CNN
F 2 "" H 4450 5000 50  0001 C CNN
F 3 "" H 4450 5000 50  0001 C CNN
	1    4450 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 5650 2850 5650
Wire Wire Line
	2850 5650 2850 6300
Wire Wire Line
	2950 5550 2400 5550
Wire Wire Line
	2400 5550 2400 5600
Wire Wire Line
	2400 5950 2750 5950
Wire Wire Line
	2750 5950 2750 5750
Wire Wire Line
	2750 5750 2950 5750
Wire Wire Line
	3900 5450 4100 5450
Wire Wire Line
	4100 5450 4100 5000
Wire Wire Line
	2400 5900 2400 5950
$Comp
L Device:CP C401
U 1 1 5D9C7549
P 2400 5750
F 0 "C401" H 2518 5796 50  0000 L CNN
F 1 "10µF" H 2500 5700 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2438 5600 50  0001 C CNN
F 3 "~" H 2400 5750 50  0001 C CNN
	1    2400 5750
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C403
U 1 1 5D9C7F2F
P 4450 6000
F 0 "C403" H 4332 5954 50  0000 R CNN
F 1 "10µF" H 4332 6045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4488 5850 50  0001 C CNN
F 3 "~" H 4450 6000 50  0001 C CNN
	1    4450 6000
	-1   0    0    1   
$EndComp
$Comp
L Device:C C402
U 1 1 5D988997
P 3950 2550
F 0 "C402" H 4065 2596 50  0000 L CNN
F 1 "4,7µF" H 4065 2505 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3988 2400 50  0001 C CNN
F 3 "~" H 3950 2550 50  0001 C CNN
	1    3950 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C404
U 1 1 5D9CA3AD
P 6350 2600
F 0 "C404" H 6232 2554 50  0000 R CNN
F 1 "10µF" H 6232 2645 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6388 2450 50  0001 C CNN
F 3 "~" H 6350 2600 50  0001 C CNN
	1    6350 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 6150 4450 6300
Wire Wire Line
	4450 5850 4450 5750
Wire Wire Line
	4050 6300 4050 5650
Wire Wire Line
	4050 5650 3900 5650
Wire Wire Line
	4450 5750 4450 5000
Connection ~ 4450 5750
Wire Wire Line
	4450 5750 3950 5750
$Comp
L Diode:B140-E3 D401
U 1 1 5D9D4CA1
P 3450 5950
F 0 "D401" H 3450 5800 50  0000 C CNN
F 1 "B140-E3" H 3450 5700 50  0000 C CNN
F 2 "Diode_SMD:D_SMA" H 3450 5775 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88946/b120.pdf" H 3450 5950 50  0001 C CNN
	1    3450 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 5950 3300 5950
Connection ~ 2750 5950
Wire Wire Line
	3950 5950 3950 5750
Connection ~ 3950 5750
Wire Wire Line
	3600 5950 3950 5950
Wire Wire Line
	3900 5750 3950 5750
$Comp
L power:GND #PWR0416
U 1 1 5D9FBDFB
P 7820 2050
F 0 "#PWR0416" H 7820 1800 50  0001 C CNN
F 1 "GND" H 7825 1877 50  0000 C CNN
F 2 "" H 7820 2050 50  0001 C CNN
F 3 "" H 7820 2050 50  0001 C CNN
	1    7820 2050
	1    0    0    -1  
$EndComp
Wire Notes Line
	1500 4450 5500 4450
Wire Notes Line
	5500 4450 5500 7000
Wire Notes Line
	5500 7000 1500 7000
Wire Notes Line
	1500 7000 1500 4450
Wire Notes Line
	1500 1000 1500 4000
Wire Notes Line
	1500 4000 7000 4000
Wire Notes Line
	7000 4000 7000 1000
Wire Notes Line
	1500 1000 7000 1000
Text Notes 1650 3650 0    50   ~ 0
Step Up Converter from battery input voltage (0.8-1.6V) to constance 3.3V
Text Notes 1650 3750 0    50   ~ 0
Enable signal is generated by undervoltage lockout circuit that protects the battery/accumulator against deep discharge 
Text Notes 1600 6700 0    50   ~ 0
Generation of negative operation voltage from positive (+3V3) to negative (-3V3)
Wire Notes Line
	7500 1000 7500 3000
Wire Notes Line
	7500 3000 9360 3000
Wire Notes Line
	9360 1000 7500 1000
Text Notes 7650 2750 0    50   ~ 0
Two seriell mignon cells with 1.5V
Text Notes 1600 6800 0    50   ~ 0
Optional Shottky Diode for latch up free operation
$Comp
L power:+BATT #PWR0128
U 1 1 5DC23891
P 1950 1310
F 0 "#PWR0128" H 1950 1160 50  0001 C CNN
F 1 "+BATT" H 1965 1483 50  0000 C CNN
F 2 "" H 1950 1310 50  0001 C CNN
F 3 "" H 1950 1310 50  0001 C CNN
	1    1950 1310
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR0129
U 1 1 5DA723BD
P 7820 1620
F 0 "#PWR0129" H 7820 1470 50  0001 C CNN
F 1 "+BATT" H 7835 1793 50  0000 C CNN
F 2 "" H 7820 1620 50  0001 C CNN
F 3 "" H 7820 1620 50  0001 C CNN
	1    7820 1620
	1    0    0    -1  
$EndComp
Wire Notes Line
	9360 3000 9360 1000
$Comp
L Connector:Conn_01x02_Male J401
U 1 1 5DA83C54
P 8230 1860
F 0 "J401" H 8202 1742 50  0000 R CNN
F 1 "Conn_01x02_Male" H 8202 1833 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 8230 1860 50  0001 C CNN
F 3 "~" H 8230 1860 50  0001 C CNN
	1    8230 1860
	-1   0    0    1   
$EndComp
Wire Wire Line
	7820 1760 8030 1760
Wire Wire Line
	8030 1860 7820 1860
Wire Wire Line
	7820 1620 7820 1760
Wire Wire Line
	7820 1860 7820 2050
$Comp
L power:GND #PWR0130
U 1 1 5DA9458F
P 8500 5790
F 0 "#PWR0130" H 8500 5540 50  0001 C CNN
F 1 "GND" H 8505 5617 50  0000 C CNN
F 2 "" H 8500 5790 50  0001 C CNN
F 3 "" H 8500 5790 50  0001 C CNN
	1    8500 5790
	1    0    0    -1  
$EndComp
Wire Notes Line
	7540 4320 7540 6320
Wire Notes Line
	7540 6320 9400 6320
Wire Notes Line
	9400 6320 9400 4320
Wire Notes Line
	9400 4320 7540 4320
Text Notes 7610 6240 0    50   ~ 0
Battery Voltage Measurement
Wire Wire Line
	8500 4660 8500 4830
Wire Wire Line
	8500 5650 8500 5790
$Comp
L power:+BATT #PWR0131
U 1 1 5DA945B7
P 8500 4660
F 0 "#PWR0131" H 8500 4510 50  0001 C CNN
F 1 "+BATT" H 8515 4833 50  0000 C CNN
F 2 "" H 8500 4660 50  0001 C CNN
F 3 "" H 8500 4660 50  0001 C CNN
	1    8500 4660
	1    0    0    -1  
$EndComp
$Comp
L Device:R R408
U 1 1 5DC23892
P 8500 4980
F 0 "R408" H 8570 5026 50  0000 L CNN
F 1 "1M" H 8570 4935 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8430 4980 50  0001 C CNN
F 3 "~" H 8500 4980 50  0001 C CNN
	1    8500 4980
	1    0    0    -1  
$EndComp
$Comp
L Device:R R409
U 1 1 5DA9CFC3
P 8500 5500
F 0 "R409" H 8570 5546 50  0000 L CNN
F 1 "1M" H 8570 5455 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8430 5500 50  0001 C CNN
F 3 "~" H 8500 5500 50  0001 C CNN
	1    8500 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 5130 8500 5230
Wire Wire Line
	8080 5230 8500 5230
Connection ~ 8500 5230
Wire Wire Line
	8500 5230 8500 5350
Text GLabel 8080 5230 0    50   Output ~ 0
AIN_U_BAT
NoConn ~ 2950 5450
NoConn ~ 3900 5550
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5DA61631
P 4800 1390
F 0 "#FLG0101" H 4800 1465 50  0001 C CNN
F 1 "PWR_FLAG" H 4800 1563 50  0000 C CNN
F 2 "" H 4800 1390 50  0001 C CNN
F 3 "~" H 4800 1390 50  0001 C CNN
	1    4800 1390
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 1550 4800 1390
Connection ~ 4800 1550
$EndSCHEMATC
