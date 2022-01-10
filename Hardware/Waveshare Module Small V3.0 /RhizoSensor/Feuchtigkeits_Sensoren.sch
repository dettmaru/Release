EESchema Schematic File Version 5
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
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
L RhizoSensor-rescue:C C301
U 1 1 5AC49753
P 2150 1830
F 0 "C301" H 2175 1930 50  0000 L CNN
F 1 "1u" H 2175 1730 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 2188 1680 50  0001 C CNN
F 3 "" H 2150 1830 50  0000 C CNN
	1    2150 1830
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:GND #PWR0305
U 1 1 5D9FF8A6
P 2150 2260
F 0 "#PWR0305" H 2150 2010 50  0001 C CNN
F 1 "GND" H 2150 2110 50  0000 C CNN
F 2 "" H 2150 2260 50  0000 C CNN
F 3 "" H 2150 2260 50  0000 C CNN
	1    2150 2260
	1    0    0    -1  
$EndComp
$Comp
L RhizoSensor-rescue:GND #PWR0303
U 1 1 5AC49758
P 1900 2260
F 0 "#PWR0303" H 1900 2010 50  0001 C CNN
F 1 "GND" H 1900 2110 50  0000 C CNN
F 2 "" H 1900 2260 50  0000 C CNN
F 3 "" H 1900 2260 50  0000 C CNN
	1    1900 2260
	1    0    0    -1  
$EndComp
$Comp
L RhizoSensor-rescue:R R303
U 1 1 5AC4974D
P 1900 1830
F 0 "R303" H 2050 1750 50  0000 C CNN
F 1 "1M" H 2050 1820 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1830 1830 50  0001 C CNN
F 3 "" H 1900 1830 50  0000 C CNN
	1    1900 1830
	-1   0    0    1   
$EndComp
Wire Wire Line
	1390 1500 1440 1500
Wire Wire Line
	1320 1500 1390 1500
Text GLabel 2320 1500 2    39   Output ~ 0
AIN0.0
Text Label 1680 810  2    60   ~ 0
BottomPolygon
Wire Wire Line
	1390 1160 1390 1500
$Comp
L RhizoSensor-rescue:Poly #PWR0301
U 1 1 5AC4D88F
P 1390 1160
F 0 "#PWR0301" H 1390 910 50  0001 C CNN
F 1 "Poly" V 1440 940 50  0000 C CNN
F 2 "" H 1390 1160 50  0001 C CNN
F 3 "" H 1390 1160 50  0001 C CNN
	1    1390 1160
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:D D301
U 1 1 5AC49759
P 1590 1500
F 0 "D301" H 1590 1600 50  0000 C CNN
F 1 "1N4148" H 1590 1400 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-323_HandSoldering" H 1590 1500 50  0001 C CNN
F 3 "" H 1590 1500 50  0001 C CNN
	1    1590 1500
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:R R301
U 1 1 5AC4974E
P 1170 1500
F 0 "R301" V 1250 1500 50  0000 C CNN
F 1 "10k" V 1170 1500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1100 1500 50  0001 C CNN
F 3 "" H 1170 1500 50  0000 C CNN
	1    1170 1500
	0    1    1    0   
$EndComp
Connection ~ 1390 1500
Wire Wire Line
	1740 1500 1900 1500
Wire Wire Line
	2150 2260 2150 1980
Wire Wire Line
	1900 2260 1900 1980
Wire Wire Line
	1900 1680 1900 1500
Connection ~ 1900 1500
Wire Wire Line
	1900 1500 2150 1500
Wire Wire Line
	2150 1680 2150 1500
Connection ~ 2150 1500
Wire Wire Line
	2150 1500 2320 1500
Wire Wire Line
	920  1500 1020 1500
Connection ~ 3670 1500
$Comp
L RhizoSensor-rescue:R R305
U 1 1 5DAB82D8
P 3450 1500
F 0 "R305" V 3530 1500 50  0000 C CNN
F 1 "10k" V 3450 1500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3380 1500 50  0001 C CNN
F 3 "" H 3450 1500 50  0000 C CNN
	1    3450 1500
	0    1    1    0   
$EndComp
$Comp
L RhizoSensor-rescue:D D303
U 1 1 5DAB82E9
P 3870 1500
F 0 "D303" H 3870 1600 50  0000 C CNN
F 1 "1N4148" H 3870 1400 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-323_HandSoldering" H 3870 1500 50  0001 C CNN
F 3 "" H 3870 1500 50  0001 C CNN
	1    3870 1500
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:Poly #PWR0307
U 1 1 5DAB82FA
P 3670 1160
F 0 "#PWR0307" H 3670 910 50  0001 C CNN
F 1 "Poly" V 3720 940 50  0000 C CNN
F 2 "" H 3670 1160 50  0001 C CNN
F 3 "" H 3670 1160 50  0001 C CNN
	1    3670 1160
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:C C303
U 1 1 5DAB8308
P 4430 1830
F 0 "C303" H 4455 1930 50  0000 L CNN
F 1 "1u" H 4455 1730 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 4468 1680 50  0001 C CNN
F 3 "" H 4430 1830 50  0000 C CNN
	1    4430 1830
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:GND #PWR0311
U 1 1 5DAB8319
P 4430 2260
F 0 "#PWR0311" H 4430 2010 50  0001 C CNN
F 1 "GND" H 4430 2110 50  0000 C CNN
F 2 "" H 4430 2260 50  0000 C CNN
F 3 "" H 4430 2260 50  0000 C CNN
	1    4430 2260
	1    0    0    -1  
$EndComp
$Comp
L RhizoSensor-rescue:GND #PWR0309
U 1 1 5DAB8327
P 4180 2260
F 0 "#PWR0309" H 4180 2010 50  0001 C CNN
F 1 "GND" H 4180 2110 50  0000 C CNN
F 2 "" H 4180 2260 50  0000 C CNN
F 3 "" H 4180 2260 50  0000 C CNN
	1    4180 2260
	1    0    0    -1  
$EndComp
Wire Wire Line
	3670 1160 3670 1500
Text Label 3960 810  2    60   ~ 0
BottomPolygon
Text GLabel 4600 1500 2    39   Output ~ 0
AIN0.1
Wire Wire Line
	3600 1500 3670 1500
Wire Wire Line
	3670 1500 3720 1500
$Comp
L RhizoSensor-rescue:R R307
U 1 1 5DAB833A
P 4180 1830
F 0 "R307" H 4330 1750 50  0000 C CNN
F 1 "1M" H 4330 1820 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4110 1830 50  0001 C CNN
F 3 "" H 4180 1830 50  0000 C CNN
	1    4180 1830
	-1   0    0    1   
$EndComp
Wire Wire Line
	4430 1680 4430 1500
Wire Wire Line
	4430 1500 4600 1500
Wire Wire Line
	4020 1500 4180 1500
Wire Wire Line
	4180 1500 4430 1500
Wire Wire Line
	3200 1500 3300 1500
Wire Wire Line
	4180 1680 4180 1500
Wire Wire Line
	4430 2260 4430 1980
Wire Wire Line
	4180 2260 4180 1980
Connection ~ 4180 1500
Connection ~ 4430 1500
Connection ~ 5950 1500
$Comp
L RhizoSensor-rescue:R R309
U 1 1 5DABC4E1
P 5730 1500
F 0 "R309" V 5810 1500 50  0000 C CNN
F 1 "10k" V 5730 1500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5660 1500 50  0001 C CNN
F 3 "" H 5730 1500 50  0000 C CNN
	1    5730 1500
	0    1    1    0   
$EndComp
$Comp
L RhizoSensor-rescue:D D305
U 1 1 5DABC4F2
P 6150 1500
F 0 "D305" H 6150 1600 50  0000 C CNN
F 1 "1N4148" H 6150 1400 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-323_HandSoldering" H 6150 1500 50  0001 C CNN
F 3 "" H 6150 1500 50  0001 C CNN
	1    6150 1500
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:Poly #PWR0313
U 1 1 5DABC503
P 5950 1160
F 0 "#PWR0313" H 5950 910 50  0001 C CNN
F 1 "Poly" V 6000 940 50  0000 C CNN
F 2 "" H 5950 1160 50  0001 C CNN
F 3 "" H 5950 1160 50  0001 C CNN
	1    5950 1160
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:C C305
U 1 1 5DABC511
P 6710 1830
F 0 "C305" H 6735 1930 50  0000 L CNN
F 1 "1u" H 6735 1730 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 6748 1680 50  0001 C CNN
F 3 "" H 6710 1830 50  0000 C CNN
	1    6710 1830
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:GND #PWR0317
U 1 1 5DABC522
P 6710 2260
F 0 "#PWR0317" H 6710 2010 50  0001 C CNN
F 1 "GND" H 6710 2110 50  0000 C CNN
F 2 "" H 6710 2260 50  0000 C CNN
F 3 "" H 6710 2260 50  0000 C CNN
	1    6710 2260
	1    0    0    -1  
$EndComp
$Comp
L RhizoSensor-rescue:GND #PWR0315
U 1 1 5DABC530
P 6460 2260
F 0 "#PWR0315" H 6460 2010 50  0001 C CNN
F 1 "GND" H 6460 2110 50  0000 C CNN
F 2 "" H 6460 2260 50  0000 C CNN
F 3 "" H 6460 2260 50  0000 C CNN
	1    6460 2260
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 1160 5950 1500
Text Label 6240 810  2    60   ~ 0
BottomPolygon
Text GLabel 6880 1500 2    39   Output ~ 0
AIN0.2
Wire Wire Line
	5880 1500 5950 1500
Wire Wire Line
	5950 1500 6000 1500
$Comp
L RhizoSensor-rescue:R R311
U 1 1 5DABC543
P 6460 1830
F 0 "R311" H 6610 1750 50  0000 C CNN
F 1 "1M" H 6610 1820 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6390 1830 50  0001 C CNN
F 3 "" H 6460 1830 50  0000 C CNN
	1    6460 1830
	-1   0    0    1   
$EndComp
Wire Wire Line
	8990 2260 8990 1980
Wire Wire Line
	8740 1680 8740 1500
Wire Wire Line
	8740 2260 8740 1980
Wire Wire Line
	6710 1680 6710 1500
Wire Wire Line
	6710 1500 6880 1500
Wire Wire Line
	6300 1500 6460 1500
Wire Wire Line
	6460 1500 6710 1500
Wire Wire Line
	5480 1500 5580 1500
Wire Wire Line
	6460 1680 6460 1500
Text Label 8520 810  2    60   ~ 0
BottomPolygon
Text GLabel 9160 1500 2    39   Output ~ 0
AIN0.3
Wire Wire Line
	7760 1500 7860 1500
Wire Wire Line
	8580 1500 8740 1500
Wire Wire Line
	8740 1500 8990 1500
Wire Wire Line
	6710 2260 6710 1980
Wire Wire Line
	6460 2260 6460 1980
Wire Wire Line
	8230 1160 8230 1500
Wire Wire Line
	8160 1500 8230 1500
Wire Wire Line
	8230 1500 8280 1500
Wire Wire Line
	8990 1680 8990 1500
Wire Wire Line
	8990 1500 9160 1500
Connection ~ 6460 1500
Connection ~ 6710 1500
Connection ~ 8740 1500
Connection ~ 8230 1500
Connection ~ 8990 1500
$Comp
L RhizoSensor-rescue:Poly #PWR0319
U 1 1 5DABC575
P 8230 1160
F 0 "#PWR0319" H 8230 910 50  0001 C CNN
F 1 "Poly" V 8280 940 50  0000 C CNN
F 2 "" H 8230 1160 50  0001 C CNN
F 3 "" H 8230 1160 50  0001 C CNN
	1    8230 1160
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:D D307
U 1 1 5DABC583
P 8430 1500
F 0 "D307" H 8430 1600 50  0000 C CNN
F 1 "1N4148" H 8430 1400 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-323_HandSoldering" H 8430 1500 50  0001 C CNN
F 3 "" H 8430 1500 50  0001 C CNN
	1    8430 1500
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:GND #PWR0323
U 1 1 5DABC594
P 8990 2260
F 0 "#PWR0323" H 8990 2010 50  0001 C CNN
F 1 "GND" H 8990 2110 50  0000 C CNN
F 2 "" H 8990 2260 50  0000 C CNN
F 3 "" H 8990 2260 50  0000 C CNN
	1    8990 2260
	1    0    0    -1  
$EndComp
$Comp
L RhizoSensor-rescue:R R313
U 1 1 5DABC5A2
P 8010 1500
F 0 "R313" V 8090 1500 50  0000 C CNN
F 1 "10k" V 8010 1500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 7940 1500 50  0001 C CNN
F 3 "" H 8010 1500 50  0000 C CNN
	1    8010 1500
	0    1    1    0   
$EndComp
$Comp
L RhizoSensor-rescue:GND #PWR0322
U 1 1 5DABC5B3
P 8740 2260
F 0 "#PWR0322" H 8740 2010 50  0001 C CNN
F 1 "GND" H 8740 2110 50  0000 C CNN
F 2 "" H 8740 2260 50  0000 C CNN
F 3 "" H 8740 2260 50  0000 C CNN
	1    8740 2260
	1    0    0    -1  
$EndComp
$Comp
L RhizoSensor-rescue:R R314
U 1 1 5DABC5C1
P 8740 1830
F 0 "R314" H 8890 1750 50  0000 C CNN
F 1 "1M" H 8890 1820 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8670 1830 50  0001 C CNN
F 3 "" H 8740 1830 50  0000 C CNN
	1    8740 1830
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:C C307
U 1 1 5DABC5D2
P 8990 1830
F 0 "C307" H 9015 1930 50  0000 L CNN
F 1 "1u" H 9015 1730 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 9028 1680 50  0001 C CNN
F 3 "" H 8990 1830 50  0000 C CNN
	1    8990 1830
	-1   0    0    1   
$EndComp
Wire Notes Line
	520  500  9640 500 
Connection ~ 1390 3620
$Comp
L RhizoSensor-rescue:R R302
U 1 1 5DAC5FE4
P 1170 3620
F 0 "R302" V 1250 3620 50  0000 C CNN
F 1 "10k" V 1170 3620 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1100 3620 50  0001 C CNN
F 3 "" H 1170 3620 50  0000 C CNN
	1    1170 3620
	0    1    1    0   
$EndComp
$Comp
L RhizoSensor-rescue:D D302
U 1 1 5DAC5FF5
P 1590 3620
F 0 "D302" H 1590 3720 50  0000 C CNN
F 1 "1N4148" H 1590 3520 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-323_HandSoldering" H 1590 3620 50  0001 C CNN
F 3 "" H 1590 3620 50  0001 C CNN
	1    1590 3620
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:Poly #PWR0302
U 1 1 5DAC6006
P 1390 3280
F 0 "#PWR0302" H 1390 3030 50  0001 C CNN
F 1 "Poly" V 1440 3060 50  0000 C CNN
F 2 "" H 1390 3280 50  0001 C CNN
F 3 "" H 1390 3280 50  0001 C CNN
	1    1390 3280
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:C C302
U 1 1 5DAC6014
P 2150 3950
F 0 "C302" H 2175 4050 50  0000 L CNN
F 1 "1u" H 2175 3850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 2188 3800 50  0001 C CNN
F 3 "" H 2150 3950 50  0000 C CNN
	1    2150 3950
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:GND #PWR0306
U 1 1 5DAC6025
P 2150 4380
F 0 "#PWR0306" H 2150 4130 50  0001 C CNN
F 1 "GND" H 2150 4230 50  0000 C CNN
F 2 "" H 2150 4380 50  0000 C CNN
F 3 "" H 2150 4380 50  0000 C CNN
	1    2150 4380
	1    0    0    -1  
$EndComp
$Comp
L RhizoSensor-rescue:GND #PWR0304
U 1 1 5DAC6033
P 1900 4380
F 0 "#PWR0304" H 1900 4130 50  0001 C CNN
F 1 "GND" H 1900 4230 50  0000 C CNN
F 2 "" H 1900 4380 50  0000 C CNN
F 3 "" H 1900 4380 50  0000 C CNN
	1    1900 4380
	1    0    0    -1  
$EndComp
Wire Wire Line
	1390 3280 1390 3620
Text Label 1680 2930 2    60   ~ 0
BottomPolygon
Text GLabel 2320 3620 2    39   Output ~ 0
AIN0.4
Wire Wire Line
	1320 3620 1390 3620
Wire Wire Line
	1390 3620 1440 3620
$Comp
L RhizoSensor-rescue:R R304
U 1 1 5DAC6046
P 1900 3950
F 0 "R304" H 2050 3870 50  0000 C CNN
F 1 "1M" H 2050 3940 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1830 3950 50  0001 C CNN
F 3 "" H 1900 3950 50  0000 C CNN
	1    1900 3950
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:GND #PWR0318
U 1 1 5DAC6057
P 6710 4380
F 0 "#PWR0318" H 6710 4130 50  0001 C CNN
F 1 "GND" H 6710 4230 50  0000 C CNN
F 2 "" H 6710 4380 50  0000 C CNN
F 3 "" H 6710 4380 50  0000 C CNN
	1    6710 4380
	1    0    0    -1  
$EndComp
Wire Wire Line
	4430 4380 4430 4100
Wire Wire Line
	4180 3800 4180 3620
Wire Wire Line
	4180 4380 4180 4100
Wire Wire Line
	2150 3800 2150 3620
Wire Wire Line
	2150 3620 2320 3620
Wire Wire Line
	5950 3280 5950 3620
Wire Wire Line
	1740 3620 1900 3620
Wire Wire Line
	5950 3620 6000 3620
Wire Wire Line
	1900 3620 2150 3620
Wire Wire Line
	6710 3800 6710 3620
Wire Wire Line
	6710 3620 6880 3620
Wire Wire Line
	5880 3620 5950 3620
Wire Wire Line
	6300 3620 6460 3620
Wire Wire Line
	920  3620 1020 3620
Wire Wire Line
	1900 3800 1900 3620
Wire Wire Line
	5480 3620 5580 3620
Wire Wire Line
	6460 4380 6460 4100
Wire Wire Line
	6460 3620 6710 3620
Wire Wire Line
	6460 3800 6460 3620
Wire Wire Line
	6710 4380 6710 4100
Text Label 3960 2930 2    60   ~ 0
BottomPolygon
Text GLabel 4600 3620 2    39   Output ~ 0
AIN0.5
Text Label 6240 2930 2    60   ~ 0
BottomPolygon
Text GLabel 6880 3620 2    39   Output ~ 0
AIN0.6
$Comp
L RhizoSensor-rescue:GND #PWR0316
U 1 1 5DAC6094
P 6460 4380
F 0 "#PWR0316" H 6460 4130 50  0001 C CNN
F 1 "GND" H 6460 4230 50  0000 C CNN
F 2 "" H 6460 4380 50  0000 C CNN
F 3 "" H 6460 4380 50  0000 C CNN
	1    6460 4380
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 3620 3300 3620
Wire Wire Line
	4020 3620 4180 3620
Wire Wire Line
	4180 3620 4430 3620
$Comp
L RhizoSensor-rescue:R R312
U 1 1 5DAC6114
P 6460 3950
F 0 "R312" H 6610 3870 50  0000 C CNN
F 1 "1M" H 6610 3940 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 6390 3950 50  0001 C CNN
F 3 "" H 6460 3950 50  0000 C CNN
	1    6460 3950
	-1   0    0    1   
$EndComp
Wire Wire Line
	2150 4380 2150 4100
Wire Wire Line
	1900 4380 1900 4100
Wire Wire Line
	3670 3280 3670 3620
Wire Wire Line
	3600 3620 3670 3620
Wire Wire Line
	3670 3620 3720 3620
Wire Wire Line
	4430 3800 4430 3620
Wire Wire Line
	4430 3620 4600 3620
Connection ~ 1900 3620
Connection ~ 2150 3620
Connection ~ 4180 3620
Connection ~ 3670 3620
Connection ~ 4430 3620
Connection ~ 6460 3620
Connection ~ 5950 3620
Connection ~ 6710 3620
$Comp
L RhizoSensor-rescue:C C306
U 1 1 5DAC6137
P 6710 3950
F 0 "C306" H 6735 4050 50  0000 L CNN
F 1 "1u" H 6735 3850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 6748 3800 50  0001 C CNN
F 3 "" H 6710 3950 50  0000 C CNN
	1    6710 3950
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:Poly #PWR0308
U 1 1 5DAC6148
P 3670 3280
F 0 "#PWR0308" H 3670 3030 50  0001 C CNN
F 1 "Poly" V 3720 3060 50  0000 C CNN
F 2 "" H 3670 3280 50  0001 C CNN
F 3 "" H 3670 3280 50  0001 C CNN
	1    3670 3280
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:D D304
U 1 1 5DAC6156
P 3870 3620
F 0 "D304" H 3870 3720 50  0000 C CNN
F 1 "1N4148" H 3870 3520 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-323_HandSoldering" H 3870 3620 50  0001 C CNN
F 3 "" H 3870 3620 50  0001 C CNN
	1    3870 3620
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:GND #PWR0312
U 1 1 5DAC6167
P 4430 4380
F 0 "#PWR0312" H 4430 4130 50  0001 C CNN
F 1 "GND" H 4430 4230 50  0000 C CNN
F 2 "" H 4430 4380 50  0000 C CNN
F 3 "" H 4430 4380 50  0000 C CNN
	1    4430 4380
	1    0    0    -1  
$EndComp
$Comp
L RhizoSensor-rescue:R R306
U 1 1 5DAC6175
P 3450 3620
F 0 "R306" V 3530 3620 50  0000 C CNN
F 1 "10k" V 3450 3620 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3380 3620 50  0001 C CNN
F 3 "" H 3450 3620 50  0000 C CNN
	1    3450 3620
	0    1    1    0   
$EndComp
$Comp
L RhizoSensor-rescue:GND #PWR0310
U 1 1 5DAC6186
P 4180 4380
F 0 "#PWR0310" H 4180 4130 50  0001 C CNN
F 1 "GND" H 4180 4230 50  0000 C CNN
F 2 "" H 4180 4380 50  0000 C CNN
F 3 "" H 4180 4380 50  0000 C CNN
	1    4180 4380
	1    0    0    -1  
$EndComp
$Comp
L RhizoSensor-rescue:R R308
U 1 1 5DAC6194
P 4180 3950
F 0 "R308" H 4330 3870 50  0000 C CNN
F 1 "1M" H 4330 3940 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4110 3950 50  0001 C CNN
F 3 "" H 4180 3950 50  0000 C CNN
	1    4180 3950
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:C C304
U 1 1 5DAC61A5
P 4430 3950
F 0 "C304" H 4455 4050 50  0000 L CNN
F 1 "1u" H 4455 3850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 4468 3800 50  0001 C CNN
F 3 "" H 4430 3950 50  0000 C CNN
	1    4430 3950
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:R R310
U 1 1 5DAC61B6
P 5730 3620
F 0 "R310" V 5810 3620 50  0000 C CNN
F 1 "10k" V 5730 3620 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 5660 3620 50  0001 C CNN
F 3 "" H 5730 3620 50  0000 C CNN
	1    5730 3620
	0    1    1    0   
$EndComp
$Comp
L RhizoSensor-rescue:D D306
U 1 1 5DAC61C7
P 6150 3620
F 0 "D306" H 6150 3720 50  0000 C CNN
F 1 "1N4148" H 6150 3520 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-323_HandSoldering" H 6150 3620 50  0001 C CNN
F 3 "" H 6150 3620 50  0001 C CNN
	1    6150 3620
	-1   0    0    1   
$EndComp
$Comp
L RhizoSensor-rescue:Poly #PWR0314
U 1 1 5DAC61D8
P 5950 3280
F 0 "#PWR0314" H 5950 3030 50  0001 C CNN
F 1 "Poly" V 6000 3060 50  0000 C CNN
F 2 "" H 5950 3280 50  0001 C CNN
F 3 "" H 5950 3280 50  0001 C CNN
	1    5950 3280
	-1   0    0    1   
$EndComp
Wire Notes Line
	2800 500  2800 4740
Wire Notes Line
	520  500  520  4740
Wire Notes Line
	5080 500  5080 4740
Wire Notes Line
	9620 2620 9620 500 
Wire Notes Line
	7360 4740 520  4740
Text GLabel 920  1500 0    50   Input ~ 0
GPIO_PWM
Wire Notes Line
	11200 2620 11200 6500
Wire Notes Line
	11200 6500 7360 6500
Wire Notes Line
	520  2620 11200 2620
Wire Notes Line
	7360 500  7360 6500
Text Notes 590  2560 0    50   ~ 0
Moisture Sensor 1
Text Notes 2870 2560 0    50   ~ 0
Moisture Sensor 2\n
Text Notes 5190 2540 0    50   ~ 0
Moisture Sensor 3
Text Notes 7450 2530 0    50   ~ 0
Moisture Sensor 4
Text Notes 610  4670 0    50   ~ 0
Moisture Sensor 5
Text Notes 2900 4650 0    50   ~ 0
Moisture Sensor 6
Text Notes 5180 4620 0    50   ~ 0
Moisture Sensor 7
Wire Wire Line
	9280 5290 9280 5350
Wire Wire Line
	9280 5350 9380 5350
Text GLabel 8740 3790 0    50   Output ~ 0
AIN_MPX
Text GLabel 10030 4490 2    50   Input ~ 0
AIN0.7
Text GLabel 10030 4390 2    50   Input ~ 0
AIN0.6
Text GLabel 10030 4290 2    50   Input ~ 0
AIN0.5
Text GLabel 10030 4090 2    50   Input ~ 0
AIN0.3
Text GLabel 10030 4190 2    50   Input ~ 0
AIN0.4
Text GLabel 10030 3990 2    50   Input ~ 0
AIN0.2
Wire Wire Line
	9880 4490 10030 4490
Wire Wire Line
	8880 3790 8740 3790
Wire Wire Line
	9880 4090 10030 4090
Wire Wire Line
	9880 3890 10030 3890
Wire Wire Line
	9880 3790 10030 3790
Wire Wire Line
	9380 5410 9380 5350
Wire Wire Line
	9880 4190 10030 4190
Wire Wire Line
	10020 4890 9880 4890
Wire Wire Line
	9380 3220 9380 3490
Wire Wire Line
	9880 4390 10030 4390
Wire Wire Line
	9880 4290 10030 4290
Wire Wire Line
	10020 4990 9880 4990
Wire Wire Line
	10020 4790 9880 4790
Wire Wire Line
	9880 3990 10030 3990
$Comp
L power:+3.3V #PWR0112
U 1 1 5DAAF70A
P 9380 3220
F 0 "#PWR0112" H 9380 3070 50  0001 C CNN
F 1 "+3.3V" H 9395 3393 50  0000 C CNN
F 2 "" H 9380 3220 50  0001 C CNN
F 3 "" H 9380 3220 50  0001 C CNN
	1    9380 3220
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 5DAB020C
P 9380 5410
F 0 "#PWR0113" H 9380 5160 50  0001 C CNN
F 1 "GND" H 9385 5237 50  0000 C CNN
F 2 "" H 9380 5410 50  0001 C CNN
F 3 "" H 9380 5410 50  0001 C CNN
	1    9380 5410
	1    0    0    -1  
$EndComp
Text GLabel 10020 4790 2    50   Input ~ 0
MPX_CONTR_A
Text GLabel 10020 4990 2    50   Input ~ 0
MPX_CONTR_C
Text GLabel 10030 3890 2    50   Input ~ 0
AIN0.1
Text GLabel 10030 3790 2    50   Input ~ 0
AIN0.0
Text GLabel 10020 4890 2    50   Input ~ 0
MPX_CONTR_B
$Comp
L 4xxx:4051 U102
U 1 1 5DA5B501
P 9380 4390
F 0 "U102" H 9390 4470 50  0000 C CNN
F 1 "4051" H 9390 4380 50  0000 C CNN
F 2 "Package_SO:SOIC-16_3.9x9.9mm_P1.27mm" H 9380 4390 50  0001 C CNN
F 3 "http://www.intersil.com/content/dam/Intersil/documents/cd40/cd4051bms-52bms-53bms.pdf" H 9380 4390 50  0001 C CNN
	1    9380 4390
	-1   0    0    -1  
$EndComp
Text Notes 8050 6170 0    50   ~ 0
Analog Multiplexer to expant the analog input pins of\nthe nRF52840 SoC
Wire Wire Line
	9380 5350 9380 5290
Connection ~ 9380 5350
Wire Wire Line
	9380 5350 9930 5350
Wire Wire Line
	9930 5350 9930 4690
Wire Wire Line
	9930 4690 9880 4690
Text GLabel 920  3620 0    50   Input ~ 0
GPIO_PWM
Text GLabel 3200 1500 0    50   Input ~ 0
GPIO_PWM
Text GLabel 5480 1500 0    50   Input ~ 0
GPIO_PWM
Text GLabel 7760 1500 0    50   Input ~ 0
GPIO_PWM
Text GLabel 5480 3620 0    50   Input ~ 0
GPIO_PWM
Text GLabel 3200 3620 0    50   Input ~ 0
GPIO_PWM
$EndSCHEMATC
