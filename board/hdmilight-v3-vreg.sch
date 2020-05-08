EESchema Schematic File Version 4
LIBS:hdmilight-v3-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 8
Title "HDMI Light V3 - Voltage Regulator"
Date "2019-10-29"
Rev "3.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L hdmilight-v3-rescue:TPS65266-_SR U12
U 1 1 5DE449E2
P 9850 1750
F 0 "U12" H 10150 2450 50  0000 L CNN
F 1 "TPS65266" H 10000 2350 50  0000 L CNN
F 2 "Package_DFN_QFN:QFN-32-1EP_5x5mm_P0.5mm_EP3.1x3.1mm_ThermalVias" H 9850 1750 50  0001 C CNN
F 3 "" H 9850 1750 50  0001 C CNN
	1    9850 1750
	1    0    0    -1  
$EndComp
$Comp
L hdmilight-v3-rescue:TPS65266-_SR U12
U 2 1 5DE44A26
P 1600 1800
F 0 "U12" H 1850 2500 50  0000 C CNN
F 1 "TPS65266" H 1850 2400 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-32-1EP_5x5mm_P0.5mm_EP3.1x3.1mm_ThermalVias" H 1600 1800 50  0001 C CNN
F 3 "" H 1600 1800 50  0001 C CNN
	2    1600 1800
	1    0    0    -1  
$EndComp
$Comp
L hdmilight-v3-rescue:TPS65266-_SR U12
U 3 1 5DE44ACA
P 1500 4100
F 0 "U12" H 1750 4800 50  0000 C CNN
F 1 "TPS65266" H 1750 4700 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-32-1EP_5x5mm_P0.5mm_EP3.1x3.1mm_ThermalVias" H 1500 4100 50  0001 C CNN
F 3 "" H 1500 4100 50  0001 C CNN
	3    1500 4100
	1    0    0    -1  
$EndComp
$Comp
L hdmilight-v3-rescue:TPS65266-_SR U12
U 4 1 5DE44B2A
P 1500 6550
F 0 "U12" H 1700 7250 50  0000 C CNN
F 1 "TPS65266" H 1750 7150 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-32-1EP_5x5mm_P0.5mm_EP3.1x3.1mm_ThermalVias" H 1500 6550 50  0001 C CNN
F 3 "" H 1500 6550 50  0001 C CNN
	4    1500 6550
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C18
U 1 1 5DE44CDE
P 1100 1050
F 0 "C18" H 1192 1096 50  0000 L CNN
F 1 "22uF" H 1192 1005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1100 1050 50  0001 C CNN
F 3 "~" H 1100 1050 50  0001 C CNN
	1    1100 1050
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0132
U 1 1 5DE44D53
P 950 1050
F 0 "#PWR0132" H 950 800 50  0001 C CNN
F 1 "GND" H 955 877 50  0000 C CNN
F 2 "" H 950 1050 50  0001 C CNN
F 3 "" H 950 1050 50  0001 C CNN
	1    950  1050
	0    1    1    0   
$EndComp
Wire Wire Line
	1000 1050 950  1050
$Comp
L Device:C_Small C15
U 1 1 5DE44DB3
P 850 1950
F 0 "C15" H 942 1996 50  0000 L CNN
F 1 "0.1uF" H 942 1905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 850 1950 50  0001 C CNN
F 3 "~" H 850 1950 50  0001 C CNN
	1    850  1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  1850 850  1750
Wire Wire Line
	850  1750 1100 1750
$Comp
L power:GND #PWR0133
U 1 1 5DE44E7B
P 1600 2250
F 0 "#PWR0133" H 1600 2000 50  0001 C CNN
F 1 "GND" H 1605 2077 50  0000 C CNN
F 2 "" H 1600 2250 50  0001 C CNN
F 3 "" H 1600 2250 50  0001 C CNN
	1    1600 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 2150 1600 2200
Wire Wire Line
	850  2050 850  2200
Wire Wire Line
	850  2200 1600 2200
Connection ~ 1600 2200
Wire Wire Line
	1600 2200 1600 2250
$Comp
L Device:C_Small C27
U 1 1 5DE4511F
P 2450 1350
F 0 "C27" V 2221 1350 50  0000 C CNN
F 1 "0.047uF" V 2312 1350 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2450 1350 50  0001 C CNN
F 3 "~" H 2450 1350 50  0001 C CNN
	1    2450 1350
	0    1    1    0   
$EndComp
Wire Wire Line
	2100 1350 2350 1350
$Comp
L Device:L_Core_Iron L3
U 1 1 5DE453CF
P 3400 1550
F 0 "L3" V 3625 1550 50  0000 C CNN
F 1 "2.2uH/3.5A" V 3534 1550 50  0000 C CNN
F 2 "Inductor_SMD:L_Bourns-SRN4018" H 3400 1550 50  0001 C CNN
F 3 "~" H 3400 1550 50  0001 C CNN
	1    3400 1550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2100 1550 3050 1550
Wire Wire Line
	2550 1350 3050 1350
Wire Wire Line
	3050 1350 3050 1550
Connection ~ 3050 1550
$Comp
L Device:C_Small C32
U 1 1 5DE45605
P 3850 1750
F 0 "C32" H 3942 1796 50  0000 L CNN
F 1 "22uF" H 3942 1705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3850 1750 50  0001 C CNN
F 3 "~" H 3850 1750 50  0001 C CNN
	1    3850 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C35
U 1 1 5DE456AD
P 4250 1750
F 0 "C35" H 4342 1796 50  0000 L CNN
F 1 "22uF" H 4342 1705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4250 1750 50  0001 C CNN
F 3 "~" H 4250 1750 50  0001 C CNN
	1    4250 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 1550 3650 1550
Wire Wire Line
	3850 1550 3850 1650
Wire Wire Line
	3850 1550 4250 1550
Wire Wire Line
	4250 1550 4250 1650
Connection ~ 3850 1550
$Comp
L Device:R R47
U 1 1 5DE45980
P 3050 2300
F 0 "R47" H 3120 2346 50  0000 L CNN
F 1 "10K" H 3120 2255 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2980 2300 50  0001 C CNN
F 3 "~" H 3050 2300 50  0001 C CNN
	1    3050 2300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R50
U 1 1 5DE45A44
P 3400 1750
F 0 "R50" V 3500 1750 50  0000 C CNN
F 1 "10K" V 3300 1750 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3330 1750 50  0001 C CNN
F 3 "~" H 3400 1750 50  0001 C CNN
	1    3400 1750
	0    1    1    0   
$EndComp
Wire Wire Line
	3550 1750 3650 1750
Connection ~ 3650 1550
Wire Wire Line
	3650 1550 3850 1550
$Comp
L Device:R R43
U 1 1 5DE46084
P 2300 2150
F 0 "R43" H 2370 2196 50  0000 L CNN
F 1 "18K" H 2370 2105 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2230 2150 50  0001 C CNN
F 3 "~" H 2300 2150 50  0001 C CNN
	1    2300 2150
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C21
U 1 1 5DE46131
P 2300 2450
F 0 "C21" H 2392 2496 50  0000 L CNN
F 1 "1nF" H 2392 2405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2300 2450 50  0001 C CNN
F 3 "~" H 2300 2450 50  0001 C CNN
	1    2300 2450
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C24
U 1 1 5DE461EE
P 2600 2300
F 0 "C24" H 2692 2346 50  0000 L CNN
F 1 "DNI" H 2692 2255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2600 2300 50  0001 C CNN
F 3 "~" H 2600 2300 50  0001 C CNN
	1    2600 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 1950 2300 1950
Wire Wire Line
	2300 1950 2300 2000
Wire Wire Line
	2300 1950 2600 1950
Wire Wire Line
	2600 1950 2600 2200
Connection ~ 2300 1950
Wire Wire Line
	2300 2300 2300 2350
Wire Wire Line
	2600 2400 2600 2600
Wire Wire Line
	2600 2600 2450 2600
Wire Wire Line
	2300 2600 2300 2550
Wire Wire Line
	3050 1550 3250 1550
Wire Wire Line
	3050 1750 3250 1750
$Comp
L power:GND #PWR0134
U 1 1 5DE4ABC8
P 2450 2650
F 0 "#PWR0134" H 2450 2400 50  0001 C CNN
F 1 "GND" H 2455 2477 50  0000 C CNN
F 2 "" H 2450 2650 50  0001 C CNN
F 3 "" H 2450 2650 50  0001 C CNN
	1    2450 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 2600 2450 2650
Connection ~ 2450 2600
Wire Wire Line
	2450 2600 2300 2600
$Comp
L power:GND #PWR0135
U 1 1 5DE4B6B9
P 3050 2500
F 0 "#PWR0135" H 3050 2250 50  0001 C CNN
F 1 "GND" H 3055 2327 50  0000 C CNN
F 2 "" H 3050 2500 50  0001 C CNN
F 3 "" H 3050 2500 50  0001 C CNN
	1    3050 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 2450 3050 2500
Wire Wire Line
	3850 1850 3850 2050
Wire Wire Line
	3850 2050 4050 2050
Wire Wire Line
	4250 2050 4250 1850
$Comp
L power:GND #PWR0136
U 1 1 5DE4C19C
P 4050 2150
F 0 "#PWR0136" H 4050 1900 50  0001 C CNN
F 1 "GND" H 4055 1977 50  0000 C CNN
F 2 "" H 4050 2150 50  0001 C CNN
F 3 "" H 4050 2150 50  0001 C CNN
	1    4050 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 2050 4050 2150
Connection ~ 4050 2050
Wire Wire Line
	4050 2050 4250 2050
Wire Wire Line
	1600 1150 1600 1050
Text Notes 600  650  0    98   ~ 0
1.2V Supply For FPGA Core
Text Notes 2950 1000 0    50   ~ 0
Vin = 5V, min 4.5V, max 6.5V\nVout = 1.2V\nIout = 2A\nVripple = 1% = 0.012V\nFswitch = 1.09MHz\nTransient reponse to 1A load step = 5% = 0.24V
Text Notes 5000 2200 0    50   ~ 0
L = ((Vin_max - Vout) / (Iout * LIR)) * (Vout / (Vin_max * Fswitch))\n@2A L= ((6.5 - 1.2) / (2 * 0.2)) * (1.2 / (6.5 * 1090000)) = 2.2uH\n\nIripple = ((Vin_max - Vout) / L) * (Vout / (Vin_max * Fswitch))\n@2.2uH Iripple = ((6.5 - 1.2) / 0.0000022) * (1.2 / (6.5 * 1090000)) = 0.41A\n\nMin Cout for transient response = (2 * delta I) / (Fswitch * delta Vout) \n= (2 * 1) / (1090000 * 0.24) = 7.6uF\n\nMin Cout for ripple = (1 / (8 * Fswitch)) * ((1 / Vout_ripple) / Iout_ripple)\n= (1 / (8 * 1090000)) * ((1 / 0.012) / 0.41) = 23.3uF\n\nMax ESR of Cout = (Vout_ripple / Iout_ripple) = 0.012 / 0.41 = 29mOhm\n\nChosen Cout = 2x 22uF, each with ESR below 58mOhm @ 1MHz\n\nCompensation resistor Rc = (2 * pi * (Fswitch / 10) * Vout * Cout) / (Gm_ea * Vref * Gm_ps)\n= (2*pi * 109000 * 1.2 * 0.000044) / (0.00029 * 0.6 * 10) = 21K\nAdjust to common value = 22K\n\nCompensation capacitor Cc = (Rload * Cout) / Rc = (0.5 * 0.000066) / 22000 = 1.5nF
$Comp
L Device:R R51
U 1 1 5DE50C7B
P 9150 1800
F 0 "R51" H 9220 1846 50  0000 L CNN
F 1 "47K" H 9220 1755 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 9080 1800 50  0001 C CNN
F 3 "~" H 9150 1800 50  0001 C CNN
	1    9150 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 1550 9150 1550
Wire Wire Line
	9150 1550 9150 1650
$Comp
L power:GND #PWR0137
U 1 1 5DE51760
P 9750 2350
F 0 "#PWR0137" H 9750 2100 50  0001 C CNN
F 1 "GND" H 9755 2177 50  0000 C CNN
F 2 "" H 9750 2350 50  0001 C CNN
F 3 "" H 9750 2350 50  0001 C CNN
	1    9750 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 1950 9150 2250
Wire Wire Line
	9150 2250 9550 2250
Wire Wire Line
	10150 2250 10150 2100
Wire Wire Line
	9950 2100 9950 2250
Connection ~ 9950 2250
Wire Wire Line
	9950 2250 10150 2250
Wire Wire Line
	9850 2100 9850 2250
Connection ~ 9850 2250
Wire Wire Line
	9850 2250 9950 2250
Wire Wire Line
	9750 2100 9750 2250
Connection ~ 9750 2250
Wire Wire Line
	9750 2250 9850 2250
Wire Wire Line
	9650 2100 9650 2250
Connection ~ 9650 2250
Wire Wire Line
	9650 2250 9750 2250
Wire Wire Line
	9550 2100 9550 2250
Connection ~ 9550 2250
Wire Wire Line
	9550 2250 9650 2250
Wire Wire Line
	9750 2350 9750 2250
Text Notes 9000 2800 0    50   ~ 0
Fswitch (kHz) = 46657 * ((ROSC / 1000) ** -0.976)\nROSC = 47K, Fswitch = 1.09MHz
$Comp
L power:+1V2 #PWR0138
U 1 1 5DE5A3CE
P 4500 1450
F 0 "#PWR0138" H 4500 1300 50  0001 C CNN
F 1 "+1V2" H 4515 1623 50  0000 C CNN
F 2 "" H 4500 1450 50  0001 C CNN
F 3 "" H 4500 1450 50  0001 C CNN
	1    4500 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 1550 4500 1550
Wire Wire Line
	4500 1550 4500 1450
Connection ~ 4250 1550
Text HLabel 1000 1550 0    50   Input ~ 0
EN_1V2
Wire Wire Line
	1000 1550 1100 1550
$Comp
L Device:C_Small C30
U 1 1 5DE6168D
P 3400 2000
F 0 "C30" V 3600 2000 50  0000 C CNN
F 1 "DNI" V 3500 2000 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3400 2000 50  0001 C CNN
F 3 "~" H 3400 2000 50  0001 C CNN
	1    3400 2000
	0    1    1    0   
$EndComp
Wire Wire Line
	2100 1750 3050 1750
Wire Wire Line
	3650 1550 3650 1750
Connection ~ 3050 1750
Wire Wire Line
	3500 2000 3650 2000
Wire Wire Line
	3650 2000 3650 1750
Connection ~ 3650 1750
Wire Wire Line
	3300 2000 3050 2000
Wire Wire Line
	3050 1750 3050 2000
Connection ~ 3050 2000
Wire Wire Line
	3050 2000 3050 2150
$Comp
L power:+5V #PWR0139
U 1 1 5DE71F6B
P 1600 950
F 0 "#PWR0139" H 1600 800 50  0001 C CNN
F 1 "+5V" H 1615 1123 50  0000 C CNN
F 2 "" H 1600 950 50  0001 C CNN
F 3 "" H 1600 950 50  0001 C CNN
	1    1600 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 950  1600 1050
Connection ~ 1600 1050
$Comp
L Device:C_Small C36
U 1 1 5DE734DE
P 8800 1300
F 0 "C36" V 8571 1300 50  0000 C CNN
F 1 "2.2uF" V 8662 1300 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8800 1300 50  0001 C CNN
F 3 "~" H 8800 1300 50  0001 C CNN
	1    8800 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 950  9850 1100
$Comp
L power:+5V #PWR0140
U 1 1 5DE748C2
P 9850 750
F 0 "#PWR0140" H 9850 600 50  0001 C CNN
F 1 "+5V" H 9865 923 50  0000 C CNN
F 2 "" H 9850 750 50  0001 C CNN
F 3 "" H 9850 750 50  0001 C CNN
	1    9850 750 
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 750  9850 950 
Connection ~ 9850 950 
$Comp
L power:GND #PWR0141
U 1 1 5DE75CA5
P 8800 1500
F 0 "#PWR0141" H 8800 1250 50  0001 C CNN
F 1 "GND" V 8805 1372 50  0000 R CNN
F 2 "" H 8800 1500 50  0001 C CNN
F 3 "" H 8800 1500 50  0001 C CNN
	1    8800 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 1500 8800 1400
$Comp
L Device:R R52
U 1 1 5DE77163
P 10600 1300
F 0 "R52" H 10670 1346 50  0000 L CNN
F 1 "2K2" H 10670 1255 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 10530 1300 50  0001 C CNN
F 3 "~" H 10600 1300 50  0001 C CNN
	1    10600 1300
	1    0    0    -1  
$EndComp
Text HLabel 10850 1550 2    50   Input ~ 0
PGOOD
Wire Wire Line
	10350 1550 10600 1550
Wire Wire Line
	10600 1450 10600 1550
Connection ~ 10600 1550
Wire Wire Line
	10600 1550 10850 1550
Wire Wire Line
	9850 950  10600 950 
Wire Wire Line
	10600 950  10600 1150
$Comp
L Device:C_Small C25
U 1 1 5DE862BF
P 2350 3650
F 0 "C25" V 2121 3650 50  0000 C CNN
F 1 "0.047uF" V 2212 3650 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2350 3650 50  0001 C CNN
F 3 "~" H 2350 3650 50  0001 C CNN
	1    2350 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	2000 3650 2250 3650
$Comp
L Device:L_Core_Iron L1
U 1 1 5DE862C7
P 3300 3850
F 0 "L1" V 3525 3850 50  0000 C CNN
F 1 "4.7uH/2.1A" V 3434 3850 50  0000 C CNN
F 2 "Inductor_SMD:L_Bourns-SRN4018" H 3300 3850 50  0001 C CNN
F 3 "~" H 3300 3850 50  0001 C CNN
	1    3300 3850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2000 3850 2950 3850
Wire Wire Line
	2450 3650 2950 3650
Wire Wire Line
	2950 3650 2950 3850
Connection ~ 2950 3850
$Comp
L Device:C_Small C33
U 1 1 5DE862D2
P 3750 4050
F 0 "C33" H 3842 4096 50  0000 L CNN
F 1 "22uF" H 3842 4005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3750 4050 50  0001 C CNN
F 3 "~" H 3750 4050 50  0001 C CNN
	1    3750 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 3850 3550 3850
Wire Wire Line
	3750 3850 3750 3950
Connection ~ 3750 3850
$Comp
L Device:R R45
U 1 1 5DE862E5
P 2950 4600
F 0 "R45" H 3020 4646 50  0000 L CNN
F 1 "10K" H 3020 4555 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2880 4600 50  0001 C CNN
F 3 "~" H 2950 4600 50  0001 C CNN
	1    2950 4600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R48
U 1 1 5DE862EC
P 3300 4050
F 0 "R48" V 3400 4050 50  0000 C CNN
F 1 "47K" V 3200 4050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3230 4050 50  0001 C CNN
F 3 "~" H 3300 4050 50  0001 C CNN
	1    3300 4050
	0    1    1    0   
$EndComp
Wire Wire Line
	3450 4050 3550 4050
Connection ~ 3550 3850
Wire Wire Line
	3550 3850 3750 3850
$Comp
L Device:R R39
U 1 1 5DE862F6
P 2200 4500
F 0 "R39" H 2270 4546 50  0000 L CNN
F 1 "15K" H 2270 4455 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2130 4500 50  0001 C CNN
F 3 "~" H 2200 4500 50  0001 C CNN
	1    2200 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C19
U 1 1 5DE862FD
P 2200 4850
F 0 "C19" H 2292 4896 50  0000 L CNN
F 1 "1nF" H 2292 4805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2200 4850 50  0001 C CNN
F 3 "~" H 2200 4850 50  0001 C CNN
	1    2200 4850
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C22
U 1 1 5DE86304
P 2500 4650
F 0 "C22" H 2592 4696 50  0000 L CNN
F 1 "DNI" H 2592 4605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2500 4650 50  0001 C CNN
F 3 "~" H 2500 4650 50  0001 C CNN
	1    2500 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 4250 2200 4250
Wire Wire Line
	2200 4250 2200 4350
Wire Wire Line
	2200 4250 2500 4250
Wire Wire Line
	2500 4250 2500 4550
Connection ~ 2200 4250
Wire Wire Line
	2200 4650 2200 4750
Wire Wire Line
	2500 4750 2500 5050
Wire Wire Line
	2500 5050 2350 5050
Wire Wire Line
	2200 5050 2200 4950
Wire Wire Line
	2950 3850 3150 3850
Wire Wire Line
	2950 4050 3150 4050
$Comp
L power:GND #PWR0142
U 1 1 5DE86316
P 2350 5100
F 0 "#PWR0142" H 2350 4850 50  0001 C CNN
F 1 "GND" H 2355 4927 50  0000 C CNN
F 2 "" H 2350 5100 50  0001 C CNN
F 3 "" H 2350 5100 50  0001 C CNN
	1    2350 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 5050 2350 5100
Connection ~ 2350 5050
Wire Wire Line
	2350 5050 2200 5050
$Comp
L power:GND #PWR0143
U 1 1 5DE8631F
P 2950 4800
F 0 "#PWR0143" H 2950 4550 50  0001 C CNN
F 1 "GND" H 2955 4627 50  0000 C CNN
F 2 "" H 2950 4800 50  0001 C CNN
F 3 "" H 2950 4800 50  0001 C CNN
	1    2950 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 4750 2950 4800
$Comp
L power:GND #PWR0144
U 1 1 5DE86329
P 3950 4450
F 0 "#PWR0144" H 3950 4200 50  0001 C CNN
F 1 "GND" H 3955 4277 50  0000 C CNN
F 2 "" H 3950 4450 50  0001 C CNN
F 3 "" H 3950 4450 50  0001 C CNN
	1    3950 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 3850 4400 3750
$Comp
L Device:C_Small C28
U 1 1 5DE8633B
P 3300 4300
F 0 "C28" V 3500 4300 50  0000 C CNN
F 1 "DNI" V 3400 4300 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3300 4300 50  0001 C CNN
F 3 "~" H 3300 4300 50  0001 C CNN
	1    3300 4300
	0    1    1    0   
$EndComp
Wire Wire Line
	2000 4050 2950 4050
Wire Wire Line
	3550 3850 3550 4050
Connection ~ 2950 4050
Wire Wire Line
	3400 4300 3550 4300
Wire Wire Line
	3550 4300 3550 4050
Connection ~ 3550 4050
Wire Wire Line
	3200 4300 2950 4300
Wire Wire Line
	2950 4050 2950 4300
Connection ~ 2950 4300
Wire Wire Line
	2950 4300 2950 4450
Text Notes 600  3000 0    98   ~ 0
3.3V Supply For Microcontroller, FPGA I/O, ADV7611 I/O
$Comp
L power:+3V3 #PWR0145
U 1 1 5DE8CEC1
P 4400 3750
F 0 "#PWR0145" H 4400 3600 50  0001 C CNN
F 1 "+3V3" H 4415 3923 50  0000 C CNN
F 2 "" H 4400 3750 50  0001 C CNN
F 3 "" H 4400 3750 50  0001 C CNN
	1    4400 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C26
U 1 1 5DE8DBC6
P 2350 6100
F 0 "C26" V 2121 6100 50  0000 C CNN
F 1 "0.047uF" V 2212 6100 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2350 6100 50  0001 C CNN
F 3 "~" H 2350 6100 50  0001 C CNN
	1    2350 6100
	0    1    1    0   
$EndComp
Wire Wire Line
	2000 6100 2250 6100
$Comp
L Device:L_Core_Iron L2
U 1 1 5DE8DBCE
P 3300 6300
F 0 "L2" V 3525 6300 50  0000 C CNN
F 1 "4.7uH/2.1A" V 3434 6300 50  0000 C CNN
F 2 "Inductor_SMD:L_Bourns-SRN4018" H 3300 6300 50  0001 C CNN
F 3 "~" H 3300 6300 50  0001 C CNN
	1    3300 6300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2000 6300 2950 6300
Wire Wire Line
	2450 6100 2950 6100
Wire Wire Line
	2950 6100 2950 6300
Connection ~ 2950 6300
$Comp
L Device:C_Small C31
U 1 1 5DE8DBD9
P 3750 6500
F 0 "C31" H 3842 6546 50  0000 L CNN
F 1 "22uF" H 3842 6455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3750 6500 50  0001 C CNN
F 3 "~" H 3750 6500 50  0001 C CNN
	1    3750 6500
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C34
U 1 1 5DE8DBE0
P 4150 6500
F 0 "C34" H 4242 6546 50  0000 L CNN
F 1 "22uF" H 4242 6455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4150 6500 50  0001 C CNN
F 3 "~" H 4150 6500 50  0001 C CNN
	1    4150 6500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 6300 3550 6300
Wire Wire Line
	3750 6300 3750 6400
Wire Wire Line
	3750 6300 4150 6300
Wire Wire Line
	4150 6300 4150 6400
Connection ~ 3750 6300
$Comp
L Device:R R46
U 1 1 5DE8DBEC
P 2950 7050
F 0 "R46" H 3020 7096 50  0000 L CNN
F 1 "10K" H 3020 7005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2880 7050 50  0001 C CNN
F 3 "~" H 2950 7050 50  0001 C CNN
	1    2950 7050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R49
U 1 1 5DE8DBF3
P 3300 6500
F 0 "R49" V 3400 6500 50  0000 C CNN
F 1 "20K" V 3200 6500 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3230 6500 50  0001 C CNN
F 3 "~" H 3300 6500 50  0001 C CNN
	1    3300 6500
	0    1    1    0   
$EndComp
Wire Wire Line
	3450 6500 3550 6500
Connection ~ 3550 6300
Wire Wire Line
	3550 6300 3750 6300
$Comp
L Device:R R40
U 1 1 5DE8DBFD
P 2200 6950
F 0 "R40" H 2270 6996 50  0000 L CNN
F 1 "33K" H 2270 6905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2130 6950 50  0001 C CNN
F 3 "~" H 2200 6950 50  0001 C CNN
	1    2200 6950
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C20
U 1 1 5DE8DC04
P 2200 7300
F 0 "C20" H 2292 7346 50  0000 L CNN
F 1 "1nF" H 2292 7255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2200 7300 50  0001 C CNN
F 3 "~" H 2200 7300 50  0001 C CNN
	1    2200 7300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C23
U 1 1 5DE8DC0B
P 2500 7100
F 0 "C23" H 2592 7146 50  0000 L CNN
F 1 "DNI" H 2592 7055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2500 7100 50  0001 C CNN
F 3 "~" H 2500 7100 50  0001 C CNN
	1    2500 7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 6700 2200 6700
Wire Wire Line
	2200 6700 2200 6800
Wire Wire Line
	2200 6700 2500 6700
Wire Wire Line
	2500 6700 2500 7000
Connection ~ 2200 6700
Wire Wire Line
	2200 7100 2200 7200
Wire Wire Line
	2500 7200 2500 7500
Wire Wire Line
	2500 7500 2350 7500
Wire Wire Line
	2200 7500 2200 7400
Wire Wire Line
	2950 6300 3150 6300
Wire Wire Line
	2950 6500 3150 6500
$Comp
L power:GND #PWR0146
U 1 1 5DE8DC1D
P 2350 7550
F 0 "#PWR0146" H 2350 7300 50  0001 C CNN
F 1 "GND" H 2355 7377 50  0000 C CNN
F 2 "" H 2350 7550 50  0001 C CNN
F 3 "" H 2350 7550 50  0001 C CNN
	1    2350 7550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 7500 2350 7550
Connection ~ 2350 7500
Wire Wire Line
	2350 7500 2200 7500
$Comp
L power:GND #PWR0147
U 1 1 5DE8DC26
P 2950 7250
F 0 "#PWR0147" H 2950 7000 50  0001 C CNN
F 1 "GND" H 2955 7077 50  0000 C CNN
F 2 "" H 2950 7250 50  0001 C CNN
F 3 "" H 2950 7250 50  0001 C CNN
	1    2950 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 7200 2950 7250
Wire Wire Line
	3750 6600 3750 6800
Wire Wire Line
	3750 6800 3950 6800
Wire Wire Line
	4150 6800 4150 6600
$Comp
L power:GND #PWR0148
U 1 1 5DE8DC30
P 3950 6900
F 0 "#PWR0148" H 3950 6650 50  0001 C CNN
F 1 "GND" H 3955 6727 50  0000 C CNN
F 2 "" H 3950 6900 50  0001 C CNN
F 3 "" H 3950 6900 50  0001 C CNN
	1    3950 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 6800 3950 6900
Connection ~ 3950 6800
Wire Wire Line
	3950 6800 4150 6800
Wire Wire Line
	4150 6300 4400 6300
Wire Wire Line
	4400 6300 4400 6200
Connection ~ 4150 6300
$Comp
L Device:C_Small C29
U 1 1 5DE8DC3C
P 3300 6750
F 0 "C29" V 3500 6750 50  0000 C CNN
F 1 "DNI" V 3400 6750 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3300 6750 50  0001 C CNN
F 3 "~" H 3300 6750 50  0001 C CNN
	1    3300 6750
	0    1    1    0   
$EndComp
Wire Wire Line
	2000 6500 2950 6500
Wire Wire Line
	3550 6300 3550 6500
Connection ~ 2950 6500
Wire Wire Line
	3400 6750 3550 6750
Wire Wire Line
	3550 6750 3550 6500
Connection ~ 3550 6500
Wire Wire Line
	3200 6750 2950 6750
Wire Wire Line
	2950 6500 2950 6750
Connection ~ 2950 6750
Wire Wire Line
	2950 6750 2950 6900
Text HLabel 900  3850 0    50   Input ~ 0
EN_3V3
Wire Wire Line
	900  3850 1000 3850
$Comp
L Device:C_Small C13
U 1 1 5DE9A513
P 750 4250
F 0 "C13" H 842 4296 50  0000 L CNN
F 1 "0.1uF" H 842 4205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 750 4250 50  0001 C CNN
F 3 "~" H 750 4250 50  0001 C CNN
	1    750  4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 4050 750  4050
Wire Wire Line
	750  4050 750  4150
Wire Wire Line
	750  4350 750  4500
Wire Wire Line
	750  4500 1500 4500
Wire Wire Line
	1500 4500 1500 4450
$Comp
L power:GND #PWR0149
U 1 1 5DEADD43
P 1500 4550
F 0 "#PWR0149" H 1500 4300 50  0001 C CNN
F 1 "GND" H 1505 4377 50  0000 C CNN
F 2 "" H 1500 4550 50  0001 C CNN
F 3 "" H 1500 4550 50  0001 C CNN
	1    1500 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 4500 1500 4550
Connection ~ 1500 4500
$Comp
L Device:C_Small C16
U 1 1 5DEC2972
P 1000 3350
F 0 "C16" H 1092 3396 50  0000 L CNN
F 1 "22uF" H 1092 3305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1000 3350 50  0001 C CNN
F 3 "~" H 1000 3350 50  0001 C CNN
	1    1000 3350
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0150
U 1 1 5DEC2979
P 850 3350
F 0 "#PWR0150" H 850 3100 50  0001 C CNN
F 1 "GND" H 855 3177 50  0000 C CNN
F 2 "" H 850 3350 50  0001 C CNN
F 3 "" H 850 3350 50  0001 C CNN
	1    850  3350
	0    1    1    0   
$EndComp
Wire Wire Line
	900  3350 850  3350
Wire Wire Line
	1500 3450 1500 3350
Wire Wire Line
	1500 3350 1100 3350
$Comp
L power:+5V #PWR0151
U 1 1 5DEC2982
P 1500 3250
F 0 "#PWR0151" H 1500 3100 50  0001 C CNN
F 1 "+5V" H 1515 3423 50  0000 C CNN
F 2 "" H 1500 3250 50  0001 C CNN
F 3 "" H 1500 3250 50  0001 C CNN
	1    1500 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 3250 1500 3350
Connection ~ 1500 3350
Text Notes 3000 3500 0    50   ~ 0
Vin = 5V, min 4.5V, max 6.5V\nVout = 3.3V\nIout = 1.5A\nVripple = 1% = 0.033V\nFswitch = 1MHz\nTransient reponse to 1A load step = 5% = 0.66V
Text Notes 600  5450 0    98   ~ 0
1.8V Supply For ADV7611 Core
$Comp
L Device:C_Small C17
U 1 1 5DF0BDA4
P 1000 5800
F 0 "C17" H 1092 5846 50  0000 L CNN
F 1 "22uF" H 1092 5755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1000 5800 50  0001 C CNN
F 3 "~" H 1000 5800 50  0001 C CNN
	1    1000 5800
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0152
U 1 1 5DF0BDAB
P 850 5800
F 0 "#PWR0152" H 850 5550 50  0001 C CNN
F 1 "GND" H 855 5627 50  0000 C CNN
F 2 "" H 850 5800 50  0001 C CNN
F 3 "" H 850 5800 50  0001 C CNN
	1    850  5800
	0    1    1    0   
$EndComp
Wire Wire Line
	900  5800 850  5800
Wire Wire Line
	1500 5900 1500 5800
Wire Wire Line
	1500 5800 1100 5800
$Comp
L power:+5V #PWR0153
U 1 1 5DF0BDB4
P 1500 5700
F 0 "#PWR0153" H 1500 5550 50  0001 C CNN
F 1 "+5V" H 1515 5873 50  0000 C CNN
F 2 "" H 1500 5700 50  0001 C CNN
F 3 "" H 1500 5700 50  0001 C CNN
	1    1500 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 5700 1500 5800
Connection ~ 1500 5800
$Comp
L Device:C_Small C14
U 1 1 5DF1C9DC
P 750 6700
F 0 "C14" H 842 6746 50  0000 L CNN
F 1 "0.1uF" H 842 6655 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 750 6700 50  0001 C CNN
F 3 "~" H 750 6700 50  0001 C CNN
	1    750  6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 6500 750  6500
Wire Wire Line
	750  6500 750  6600
Wire Wire Line
	750  6800 750  6950
Wire Wire Line
	750  6950 1500 6950
Wire Wire Line
	1500 6950 1500 6900
$Comp
L power:GND #PWR0154
U 1 1 5DF2CFB3
P 1500 7000
F 0 "#PWR0154" H 1500 6750 50  0001 C CNN
F 1 "GND" H 1505 6827 50  0000 C CNN
F 2 "" H 1500 7000 50  0001 C CNN
F 3 "" H 1500 7000 50  0001 C CNN
	1    1500 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 6950 1500 7000
Connection ~ 1500 6950
$Comp
L power:+1V8 #PWR0155
U 1 1 5DF35797
P 4400 6200
F 0 "#PWR0155" H 4400 6050 50  0001 C CNN
F 1 "+1V8" H 4415 6373 50  0000 C CNN
F 2 "" H 4400 6200 50  0001 C CNN
F 3 "" H 4400 6200 50  0001 C CNN
	1    4400 6200
	1    0    0    -1  
$EndComp
Text HLabel 900  6300 0    50   Input ~ 0
EN_1V8
Wire Wire Line
	900  6300 1000 6300
Wire Wire Line
	3750 3850 4150 3850
Text Notes 3000 5900 0    50   ~ 0
Vin = 5V, min 4.5V, max 6.5V\nVout = 1.8V\nIout = 1.2A (from CN0282 ref design)\nVripple = 1% = 0.018V\nFswitch = 1MHz\nTransient reponse to 1A load step = 5% = 0.36V
Text Notes 5050 6900 0    50   ~ 0
L = ((Vin_max - Vout) / (Iout * LIR)) * (Vout / (Vin_max * Fswitch))\n@1.2A L= ((6.5 - 1.8) / (1.2 * 0.2)) * (1.8 / (6.5 * 1090000)) = 5.0uH\n\nIripple = ((Vin_max - Vout) / L) * (Vout / (Vin_max * Fswitch))\n@4.7uH Iripple = ((6.5 - 1.8) / 0.0000047) * (1.8 / (6.5 * 1090000)) = 0.25A\n\nMin Cout for transient response = (2 * delta I) / (Fswitch * delta Vout) = (2 * 1) / (1090000 * 0.36) = 5.1uF\n\nMin Cout for ripple = (1 / (8 * Fswitch)) * ((1 / Vout_ripple) / Iout_ripple) = (1 / (8 * 1090000)) * ((1 / 0.018) / 0.25) = 25uF\n\nMax ESR of Cout = (Vout_ripple / Iout_ripple) = 0.018 / 0.25 = 72mOhm\n\nChosen Cout = 2x 22uF, each with ESR below 144mOhm\n\nCompensation resistor Rc = (2 * pi * (Fswitch / 10) * Vout * Cout) / (Gm_ea * Vref * Gm_ps)\n= (2*pi * 109000 * 1.8 * 0.000044) / (0.00029 * 0.6 * 10) = 31K\nAdjust to common value 33K\n\nCompensation capacitor Cc = (Rload * Cout) / Rc = (1 * 0.000044) / 33000 = 1.33nF
Text Notes 5050 4500 0    50   ~ 0
L = ((Vin_max - Vout) / (Iout * LIR)) * (Vout / (Vin_max * Fswitch))\n@1.5A L= ((6.5 - 3.3) / (1.5 * 0.2)) * (3.3 / (6.5 * 1090000)) = 5.0uH\n\nIripple = ((Vin_max - Vout) / L) * (Vout / (Vin_max * Fswitch))\n@4.7uH Iripple = ((6.5 - 3.3) / 0.0000047) * (3.3 / (6.5 * 1090000)) = 0.32A\n\nMin Cout for transient response = (2 * delta I) / (Fswitch * delta Vout) = (2 * 1) / (1090000 * 0.66) = 2.8uF\n\nMin Cout for ripple = (1 / (8 * Fswitch)) * ((1 / Vout_ripple) / Iout_ripple) = (1 / (8 * 1090000)) * ((1 / 0.033) / 0.32) = 11uF\n\nMax ESR of Cout = (Vout_ripple / Iout_ripple) = 0.033 / 0.32 = 103mOhm\n\nChosen Cout = 1x 22uF with ESR below 103mOhm\n\nCompensation resistor Rc = (2 * pi * (Fswitch / 10) * Vout * Cout) / (Gm_ea * Vref * Gm_ps)\n= (2*pi * 109000 * 3.3 * 0.000022) / (0.00029 * 0.6 * 10) = 29K\nAdjust to common value 27K\n\nCompensation capacitor Cc = (Rload * Cout) / Rc = (1 * 0.000022) / 27000 = 0.81nF
$Comp
L Device:C_Small C37
U 1 1 5DFB0D9F
P 4150 4050
F 0 "C37" H 4242 4096 50  0000 L CNN
F 1 "DNI" H 4242 4005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4150 4050 50  0001 C CNN
F 3 "~" H 4150 4050 50  0001 C CNN
	1    4150 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 4300 4150 4300
Wire Wire Line
	4150 4300 4150 4150
Wire Wire Line
	3950 4300 3950 4450
Wire Wire Line
	4150 3950 4150 3850
Connection ~ 4150 3850
Wire Wire Line
	4150 3850 4400 3850
Wire Wire Line
	3750 4150 3750 4300
Wire Wire Line
	3750 4300 3950 4300
Connection ~ 3950 4300
$Comp
L Connector:TestPoint TP1
U 1 1 61BE6672
P 10750 1700
F 0 "TP1" V 10850 1900 50  0000 R CNN
F 1 "TestPoint" V 10950 2100 50  0000 R CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 10950 1700 50  0001 C CNN
F 3 "~" H 10950 1700 50  0001 C CNN
	1    10750 1700
	0    1    1    0   
$EndComp
$Comp
L Connector:TestPoint TP4
U 1 1 61D7274B
P 4400 6300
F 0 "TP4" V 4354 6488 50  0000 L CNN
F 1 "TestPoint" V 4445 6488 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 4600 6300 50  0001 C CNN
F 3 "~" H 4600 6300 50  0001 C CNN
	1    4400 6300
	0    1    1    0   
$EndComp
$Comp
L Connector:TestPoint TP2
U 1 1 61D733DB
P 4400 3850
F 0 "TP2" V 4354 4038 50  0000 L CNN
F 1 "TestPoint" V 4445 4038 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 4600 3850 50  0001 C CNN
F 3 "~" H 4600 3850 50  0001 C CNN
	1    4400 3850
	0    1    1    0   
$EndComp
Connection ~ 4400 3850
Wire Wire Line
	1300 1050 1200 1050
Wire Wire Line
	1600 1050 1300 1050
Connection ~ 1300 1050
$Comp
L Connector:TestPoint TP5
U 1 1 61D912CE
P 1300 1050
F 0 "TP5" H 1358 1168 50  0000 L CNN
F 1 "TestPoint" H 1358 1077 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.5mm" H 1500 1050 50  0001 C CNN
F 3 "~" H 1500 1050 50  0001 C CNN
	1    1300 1050
	1    0    0    -1  
$EndComp
Connection ~ 4400 6300
$Comp
L Device:LED D8
U 1 1 5F278759
P 9000 1800
F 0 "D8" V 8850 1950 50  0000 R CNN
F 1 "LED" V 8750 1950 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric" H 9000 1800 50  0001 C CNN
F 3 "~" H 9000 1800 50  0001 C CNN
	1    9000 1800
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D9
U 1 1 5F27A03E
P 10600 1850
F 0 "D9" V 10550 2050 50  0000 R CNN
F 1 "LED" V 10450 2050 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric" H 10600 1850 50  0001 C CNN
F 3 "~" H 10600 1850 50  0001 C CNN
	1    10600 1850
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R58
U 1 1 5F27A8CD
P 9000 1300
F 0 "R58" H 9070 1346 50  0000 L CNN
F 1 "2K2" H 9070 1255 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8930 1300 50  0001 C CNN
F 3 "~" H 9000 1300 50  0001 C CNN
	1    9000 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 1450 9000 1650
Wire Wire Line
	9000 1950 9000 2250
Wire Wire Line
	9000 2250 9150 2250
Connection ~ 9150 2250
Wire Wire Line
	9000 1150 9000 950 
Wire Wire Line
	9000 950  9850 950 
Wire Wire Line
	8800 950  9000 950 
Wire Wire Line
	8800 950  8800 1200
Connection ~ 9000 950 
Wire Wire Line
	10750 1700 10600 1700
Wire Wire Line
	10600 1550 10600 1700
Connection ~ 10600 1700
Wire Wire Line
	10150 2250 10600 2250
Wire Wire Line
	10600 2250 10600 2000
Connection ~ 10150 2250
$EndSCHEMATC
