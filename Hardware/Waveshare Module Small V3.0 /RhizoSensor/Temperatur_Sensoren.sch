EESchema Schematic File Version 5
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
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
L power:+3.3V #PWR0206
U 1 1 5DA27E41
P 6100 6390
F 0 "#PWR0206" H 6100 6240 50  0001 C CNN
F 1 "+3.3V" H 6115 6563 50  0000 C CNN
F 2 "" H 6100 6390 50  0001 C CNN
F 3 "" H 6100 6390 50  0001 C CNN
	1    6100 6390
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0207
U 1 1 5DA2872E
P 6100 7430
F 0 "#PWR0207" H 6100 7180 50  0001 C CNN
F 1 "GND" H 6105 7257 50  0000 C CNN
F 2 "" H 6100 7430 50  0001 C CNN
F 3 "" H 6100 7430 50  0001 C CNN
	1    6100 7430
	1    0    0    -1  
$EndComp
$Comp
L Device:C C207
U 1 1 5DA28D77
P 6100 6910
F 0 "C207" H 6215 6956 50  0000 L CNN
F 1 "100nF" H 6215 6865 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6138 6760 50  0001 C CNN
F 3 "~" H 6100 6910 50  0001 C CNN
	1    6100 6910
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 6390 6100 6600
Wire Wire Line
	6100 7430 6100 7290
$Comp
L Device:C C206
U 1 1 5DA2BC59
P 5700 6900
F 0 "C206" H 5815 6946 50  0000 L CNN
F 1 "1µ" H 5815 6855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5738 6750 50  0001 C CNN
F 3 "~" H 5700 6900 50  0001 C CNN
	1    5700 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 6750 5700 6600
Wire Wire Line
	5700 6600 6100 6600
Connection ~ 6100 6600
Wire Wire Line
	6100 6600 6100 6760
Wire Wire Line
	5700 7290 6100 7290
Connection ~ 6100 7290
Wire Wire Line
	6100 7290 6100 7060
Wire Wire Line
	5700 7050 5700 7290
$Comp
L Device:R R203
U 1 1 5DA2DAB4
P 4625 6890
F 0 "R203" H 4695 6936 50  0000 L CNN
F 1 "4k7" H 4695 6845 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4555 6890 50  0001 C CNN
F 3 "~" H 4625 6890 50  0001 C CNN
	1    4625 6890
	1    0    0    -1  
$EndComp
$Comp
L Device:R R205
U 1 1 5DA2F636
P 4965 6890
F 0 "R205" H 5035 6936 50  0000 L CNN
F 1 "4k7" H 5035 6845 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4895 6890 50  0001 C CNN
F 3 "~" H 4965 6890 50  0001 C CNN
	1    4965 6890
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0203
U 1 1 5DA3005B
P 4795 6535
F 0 "#PWR0203" H 4795 6385 50  0001 C CNN
F 1 "+3.3V" H 4810 6708 50  0000 C CNN
F 2 "" H 4795 6535 50  0001 C CNN
F 3 "" H 4795 6535 50  0001 C CNN
	1    4795 6535
	1    0    0    -1  
$EndComp
Text GLabel 5125 7300 2    50   Output ~ 0
SMCLK
Text GLabel 5115 7430 2    50   Output ~ 0
SMDATA
Wire Wire Line
	4625 6740 4625 6690
Wire Wire Line
	4625 6690 4795 6690
Wire Wire Line
	4965 6690 4965 6740
Wire Wire Line
	4795 6535 4795 6690
Connection ~ 4795 6690
Wire Wire Line
	4795 6690 4965 6690
Wire Wire Line
	4965 7040 4965 7300
Wire Wire Line
	4965 7300 5125 7300
Wire Wire Line
	4625 7040 4625 7430
Wire Wire Line
	4625 7430 5115 7430
Wire Wire Line
	7640 2050 7640 1990
Wire Wire Line
	8360 1990 8360 2050
$Comp
L Transistor_BJT:MMBT3904 Q201
U 1 1 5DA3BFFC
P 8260 2250
F 0 "Q201" H 8451 2296 50  0000 L CNN
F 1 "MMBT3904" H 8451 2205 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 8460 2175 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 8260 2250 50  0001 L CNN
	1    8260 2250
	1    0    0    1   
$EndComp
$Comp
L Device:C C201
U 1 1 5DA2C99E
P 7640 2200
F 0 "C201" H 7755 2246 50  0000 L CNN
F 1 "100pF" H 7755 2155 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 7678 2050 50  0001 C CNN
F 3 "~" H 7640 2200 50  0001 C CNN
	1    7640 2200
	-1   0    0    1   
$EndComp
Wire Wire Line
	8360 1990 7640 1990
Text Notes 8390 1940 2    50   ~ 0
Transistor Probe 1\n
Connection ~ 7950 2550
Wire Wire Line
	7950 2550 7950 2250
Wire Wire Line
	8360 2550 7950 2550
Wire Wire Line
	8360 2450 8360 2550
Wire Wire Line
	7950 2250 8060 2250
Wire Wire Line
	7640 2550 7950 2550
Wire Wire Line
	7640 2550 7640 2350
$Comp
L RhizoSensor-rescue:MAX6581-MAX6581_library U?
U 1 1 5DA47C6B
P 5130 2350
AR Path="/5DA47C6B" Ref="U?"  Part="1" 
AR Path="/5D991C37/5DA47C6B" Ref="U201"  Part="1" 
F 0 "U201" H 5130 2565 50  0000 C CNN
F 1 "MAX6581" H 5130 2474 50  0000 C CNN
F 2 "MAX:MAX6581TG9A&plus_" H 5030 2450 50  0001 C CNN
F 3 "" H 5030 2450 50  0001 C CNN
	1    5130 2350
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0126
U 1 1 5DA4A8F4
P 6130 1920
F 0 "#PWR0126" H 6130 1770 50  0001 C CNN
F 1 "+3.3V" H 6145 2093 50  0000 C CNN
F 2 "" H 6130 1920 50  0001 C CNN
F 3 "" H 6130 1920 50  0001 C CNN
	1    6130 1920
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0127
U 1 1 5DA4B31B
P 6000 4280
F 0 "#PWR0127" H 6000 4030 50  0001 C CNN
F 1 "GND" H 6005 4107 50  0000 C CNN
F 2 "" H 6000 4280 50  0001 C CNN
F 3 "" H 6000 4280 50  0001 C CNN
	1    6000 4280
	1    0    0    -1  
$EndComp
Text GLabel 6540 3000 2    50   Output ~ 0
SMCLK
Text GLabel 6540 3150 2    50   Output ~ 0
SMDATA
Wire Wire Line
	6130 3450 6130 1920
Wire Wire Line
	6000 2850 6000 4280
Wire Wire Line
	6130 3750 6130 3450
Connection ~ 6130 3450
Text GLabel 6530 3900 2    50   Input ~ 0
STBY
Connection ~ 7640 2550
Wire Wire Line
	7140 2400 7140 1990
Wire Wire Line
	7140 1990 7640 1990
Connection ~ 7640 1990
Connection ~ 2370 840 
$Comp
L Transistor_BJT:MMBT3904 Q203
U 1 1 5DA595E8
P 2060 1140
F 0 "Q203" H 2251 1186 50  0000 L CNN
F 1 "MMBT3904" H 2251 1095 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2260 1065 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 2060 1140 50  0001 L CNN
	1    2060 1140
	-1   0    0    -1  
$EndComp
Text Notes 1940 880  0    50   ~ 0
Transistor Probe 2\n\n
Wire Wire Line
	2370 1140 2260 1140
Wire Wire Line
	1960 840  2370 840 
Wire Wire Line
	2680 840  2370 840 
Wire Wire Line
	1960 1400 1960 1340
Wire Wire Line
	2370 840  2370 1140
Wire Wire Line
	1960 940  1960 840 
Wire Wire Line
	2680 840  2680 1040
Wire Wire Line
	2680 1340 2680 1400
$Comp
L Device:C C203
U 1 1 5DA59607
P 2680 1190
F 0 "C203" H 2795 1236 50  0000 L CNN
F 1 "100pF" H 2795 1145 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2718 1040 50  0001 C CNN
F 3 "~" H 2680 1190 50  0001 C CNN
	1    2680 1190
	1    0    0    -1  
$EndComp
Connection ~ 2360 1790
Text Notes 1960 1900 0    50   ~ 0
Transistor Probe 3\n\n\n
$Comp
L Transistor_BJT:MMBT3904 Q202
U 1 1 5DA643F3
P 2050 2090
F 0 "Q202" H 2241 2136 50  0000 L CNN
F 1 "MMBT3904" H 2241 2045 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2250 2015 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 2050 2090 50  0001 L CNN
	1    2050 2090
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C202
U 1 1 5DA64408
P 2670 2140
F 0 "C202" H 2785 2186 50  0000 L CNN
F 1 "100pF" H 2785 2095 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2708 1990 50  0001 C CNN
F 3 "~" H 2670 2140 50  0001 C CNN
	1    2670 2140
	1    0    0    -1  
$EndComp
Wire Wire Line
	2360 2090 2250 2090
Wire Wire Line
	1950 1790 2360 1790
Wire Wire Line
	2670 1790 2670 1990
Wire Wire Line
	1950 2350 1950 2290
Wire Wire Line
	2360 1790 2360 2090
Wire Wire Line
	2670 1790 2360 1790
Wire Wire Line
	1950 1890 1950 1790
Wire Wire Line
	2670 2290 2670 2345
Wire Wire Line
	1950 2350 2670 2350
Connection ~ 2380 2710
Text Notes 1950 2825 0    50   ~ 0
Transistor Probe 4\n\n\n
$Comp
L Transistor_BJT:MMBT3904 Q204
U 1 1 5DA667BF
P 2070 3010
F 0 "Q204" H 2261 3056 50  0000 L CNN
F 1 "MMBT3904" H 2261 2965 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2270 2935 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 2070 3010 50  0001 L CNN
	1    2070 3010
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C205
U 1 1 5DA667D4
P 2700 3060
F 0 "C205" H 2815 3106 50  0000 L CNN
F 1 "100pF" H 2815 3015 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2738 2910 50  0001 C CNN
F 3 "~" H 2700 3060 50  0001 C CNN
	1    2700 3060
	1    0    0    -1  
$EndComp
Wire Wire Line
	2380 3010 2270 3010
Wire Wire Line
	1970 2710 2380 2710
Wire Wire Line
	2700 2710 2700 2910
Wire Wire Line
	1970 3300 1970 3210
Wire Wire Line
	2380 2710 2380 3010
Wire Wire Line
	2700 2710 2380 2710
Wire Wire Line
	1970 2810 1970 2710
Wire Wire Line
	1970 3300 2700 3300
Wire Wire Line
	4630 2400 4145 2400
Wire Wire Line
	4145 2400 4145 840 
Wire Wire Line
	4145 840  2680 840 
Connection ~ 2680 840 
Wire Wire Line
	1960 1400 2680 1400
Wire Wire Line
	2680 1400 4020 1400
Wire Wire Line
	4020 1400 4020 2550
Wire Wire Line
	4020 2550 4630 2550
Connection ~ 2680 1400
Wire Wire Line
	4630 2700 3885 2700
Wire Wire Line
	3885 2700 3885 1790
Wire Wire Line
	3885 1790 2670 1790
Connection ~ 2670 1790
Wire Wire Line
	2670 2345 3755 2345
Wire Wire Line
	3755 2345 3755 2850
Wire Wire Line
	3755 2850 4630 2850
Connection ~ 2670 2345
Wire Wire Line
	2670 2345 2670 2350
Wire Wire Line
	4630 3000 3595 3000
Wire Wire Line
	3595 3000 3595 2710
Wire Wire Line
	3595 2710 2700 2710
Connection ~ 2700 2710
Wire Wire Line
	4625 3300 2700 3300
Wire Wire Line
	2700 3210 2700 3300
Connection ~ 2700 3300
Wire Wire Line
	3585 3450 4630 3450
Wire Wire Line
	4630 3600 3755 3600
Wire Wire Line
	4630 3750 3995 3750
Wire Wire Line
	3995 3750 3995 4560
Wire Wire Line
	3995 4560 2710 4560
Wire Wire Line
	4210 5120 4210 3900
Wire Wire Line
	4210 3900 4630 3900
Wire Wire Line
	2705 5120 4210 5120
Wire Wire Line
	4630 4050 4370 4050
Wire Wire Line
	4370 4050 4370 5510
Wire Wire Line
	4780 6070 4780 4380
Wire Wire Line
	4780 4380 5725 4380
Wire Wire Line
	5725 4380 5725 4050
Wire Wire Line
	2700 6070 4780 6070
Wire Wire Line
	2700 5510 4370 5510
Wire Wire Line
	3755 3600 3755 4220
Wire Wire Line
	3585 3660 3585 3450
Wire Wire Line
	2690 3660 2380 3660
Wire Wire Line
	2380 3660 2380 3960
Wire Wire Line
	1970 4220 2690 4220
Wire Wire Line
	2690 4160 2690 4220
Wire Wire Line
	1970 3760 1970 3660
Wire Wire Line
	1970 3660 2380 3660
Wire Wire Line
	2690 3660 2690 3860
Wire Wire Line
	1970 4220 1970 4160
Wire Wire Line
	2380 3960 2270 3960
$Comp
L Device:C C204
U 1 1 5DA667FA
P 2690 4010
F 0 "C204" H 2805 4056 50  0000 L CNN
F 1 "100pF" H 2805 3965 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2728 3860 50  0001 C CNN
F 3 "~" H 2690 4010 50  0001 C CNN
	1    2690 4010
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:MMBT3904 Q205
U 1 1 5DA667E5
P 2070 3960
F 0 "Q205" H 2261 4006 50  0000 L CNN
F 1 "MMBT3904" H 2261 3915 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2270 3885 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 2070 3960 50  0001 L CNN
	1    2070 3960
	-1   0    0    -1  
$EndComp
Text Notes 1980 3770 0    50   ~ 0
Transistor Probe 5\n\n\n
Connection ~ 2380 3660
Connection ~ 2690 4220
Wire Wire Line
	3755 4220 2690 4220
Connection ~ 2690 3660
Wire Wire Line
	2690 3660 3585 3660
Connection ~ 2400 5120
Text Notes 1965 4760 0    50   ~ 0
Transistor Probe 6\n\n\n\n
$Comp
L Transistor_BJT:MMBT3904 Q207
U 1 1 5DA9FCF5
P 2090 4820
F 0 "Q207" H 2281 4866 50  0000 L CNN
F 1 "MMBT3904" H 2281 4775 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2290 4745 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 2090 4820 50  0001 L CNN
	1    2090 4820
	-1   0    0    1   
$EndComp
$Comp
L Device:C C209
U 1 1 5DA9FD0A
P 2710 4770
F 0 "C209" H 2825 4816 50  0000 L CNN
F 1 "100pF" H 2825 4725 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2748 4620 50  0001 C CNN
F 3 "~" H 2710 4770 50  0001 C CNN
	1    2710 4770
	1    0    0    1   
$EndComp
Wire Wire Line
	2400 4820 2290 4820
Wire Wire Line
	1990 4560 1990 4620
Wire Wire Line
	2710 5120 2710 4920
Wire Wire Line
	1990 5120 2400 5120
Wire Wire Line
	1990 5020 1990 5120
Wire Wire Line
	2710 4620 2710 4560
Wire Wire Line
	1990 4560 2710 4560
Wire Wire Line
	2400 5120 2400 4820
Wire Wire Line
	2710 5120 2705 5120
Connection ~ 2705 5120
Wire Wire Line
	2705 5120 2400 5120
Connection ~ 2710 4560
Connection ~ 2390 6070
Text Notes 1970 5480 0    50   ~ 0
Transistor Probe 7\n
$Comp
L Transistor_BJT:MMBT3904 Q206
U 1 1 5DAA2928
P 2080 5770
F 0 "Q206" H 2271 5816 50  0000 L CNN
F 1 "MMBT3904" H 2271 5725 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2280 5695 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N3904.pdf" H 2080 5770 50  0001 L CNN
	1    2080 5770
	-1   0    0    1   
$EndComp
$Comp
L Device:C C208
U 1 1 5DAA293D
P 2700 5720
F 0 "C208" H 2815 5766 50  0000 L CNN
F 1 "100pF" H 2815 5675 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2738 5570 50  0001 C CNN
F 3 "~" H 2700 5720 50  0001 C CNN
	1    2700 5720
	1    0    0    1   
$EndComp
Wire Wire Line
	2390 5770 2280 5770
Wire Wire Line
	1980 5510 1980 5570
Wire Wire Line
	2700 6070 2700 5870
Wire Wire Line
	1980 6070 2390 6070
Wire Wire Line
	1980 5970 1980 6070
Wire Wire Line
	2700 5570 2700 5510
Wire Wire Line
	1980 5510 2700 5510
Wire Wire Line
	2390 6070 2390 5770
Wire Wire Line
	2700 6070 2390 6070
Connection ~ 2700 5510
Connection ~ 2700 6070
Wire Wire Line
	5630 3000 6540 3000
Wire Wire Line
	5630 3150 6540 3150
Wire Wire Line
	5630 3450 6130 3450
Wire Wire Line
	5630 2850 6000 2850
Wire Wire Line
	5630 3750 6130 3750
Wire Wire Line
	5630 3900 6530 3900
Wire Wire Line
	5630 2550 7640 2550
Wire Wire Line
	5630 2400 7140 2400
Wire Wire Line
	5725 4050 5630 4050
Connection ~ 4630 3300
NoConn ~ 4630 3150
NoConn ~ 5630 3600
NoConn ~ 5630 3300
NoConn ~ 5630 2700
$EndSCHEMATC
