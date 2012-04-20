EESchema Schematic File Version 2  date 19/04/2012 12:41:44
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
LIBS:special
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
LIBS:Kicad1-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 1
Title ""
Date "19 apr 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ 3200 3500
Wire Wire Line
	3100 3500 3200 3500
Wire Wire Line
	3100 3300 3200 3300
Wire Wire Line
	3200 3300 3200 3600
Wire Wire Line
	5400 5200 4200 5200
Wire Wire Line
	4200 5200 4200 4600
Wire Wire Line
	4200 4600 4400 4600
Wire Wire Line
	5100 2350 5700 2350
Wire Wire Line
	5700 3100 5400 3100
Wire Wire Line
	4400 3200 4300 3200
Wire Wire Line
	4300 3200 4300 3300
Wire Wire Line
	4100 3150 4200 3150
Wire Wire Line
	3600 3000 3100 3000
Wire Wire Line
	4800 4050 4800 4100
Wire Wire Line
	4800 2600 4800 2700
Wire Wire Line
	4800 3600 4800 3500
Wire Wire Line
	4800 5000 4800 4900
Wire Wire Line
	3100 3100 3600 3100
Wire Wire Line
	3600 3100 3600 3150
Wire Wire Line
	4400 3000 4100 3000
Connection ~ 4200 3000
Wire Wire Line
	4600 2350 4200 2350
Wire Wire Line
	4200 2350 4200 3150
Wire Wire Line
	5700 2350 5700 3850
Wire Wire Line
	5700 3850 4200 3850
Wire Wire Line
	4200 3850 4200 4400
Wire Wire Line
	4200 4400 4400 4400
Connection ~ 5700 3100
Wire Wire Line
	5400 4500 5400 5500
Wire Wire Line
	5400 5500 3350 5500
Wire Wire Line
	3350 5500 3350 3200
Wire Wire Line
	3350 3200 3100 3200
Connection ~ 5400 5200
Wire Wire Line
	3100 3400 3200 3400
Connection ~ 3200 3400
$Comp
L GND #PWR1
U 1 1 4F8FF971
P 3200 3600
F 0 "#PWR1" H 3200 3600 30  0001 C CNN
F 1 "GND" H 3200 3530 30  0001 C CNN
	1    3200 3600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR2
U 1 1 4F8FF934
P 4300 3300
F 0 "#PWR2" H 4300 3300 30  0001 C CNN
F 1 "GND" H 4300 3230 30  0001 C CNN
	1    4300 3300
	1    0    0    -1  
$EndComp
$Comp
L VEE #PWR6
U 1 1 4F8FF914
P 4800 5000
F 0 "#PWR6" H 4800 5200 40  0001 C CNN
F 1 "VEE" H 4800 5150 40  0000 C CNN
	1    4800 5000
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR5
U 1 1 4F8FF90E
P 4800 4050
F 0 "#PWR5" H 4800 4150 30  0001 C CNN
F 1 "VCC" H 4800 4150 30  0000 C CNN
	1    4800 4050
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR4
U 1 1 4F8FF902
P 4800 3600
F 0 "#PWR4" H 4800 3700 30  0001 C CNN
F 1 "VCC" H 4800 3700 30  0000 C CNN
	1    4800 3600
	-1   0    0    1   
$EndComp
$Comp
L VEE #PWR3
U 1 1 4F8FF8FC
P 4800 2600
F 0 "#PWR3" H 4800 2800 40  0001 C CNN
F 1 "VEE" H 4800 2750 40  0000 C CNN
	1    4800 2600
	1    0    0    -1  
$EndComp
$Comp
L TL072 U?
U 1 1 4F8FF8D2
P 4900 4500
AR Path="/4F8FF8CA" Ref="U?"  Part="1" 
AR Path="/4F8FF8D2" Ref="U2"  Part="1" 
F 0 "U2" H 4850 4700 60  0000 L CNN
F 1 "TL072" H 4850 4250 60  0000 L CNN
	1    4900 4500
	1    0    0    -1  
$EndComp
$Comp
L TL072 U1
U 1 1 4F8FF8CA
P 4900 3100
F 0 "U1" H 4850 3300 60  0000 L CNN
F 1 "TL072" H 4850 2850 60  0000 L CNN
	1    4900 3100
	1    0    0    1   
$EndComp
$Comp
L R R?
U 1 1 4F8FF8AF
P 4850 2350
AR Path="/4F8FF8A5" Ref="R?"  Part="1" 
AR Path="/4F8FF8AF" Ref="R3"  Part="1" 
F 0 "R3" V 4930 2350 50  0000 C CNN
F 1 "10K" V 4850 2350 50  0000 C CNN
	1    4850 2350
	0    -1   -1   0   
$EndComp
$Comp
L R R?
U 1 1 4F8FF8AD
P 3850 3150
AR Path="/4F8FF8A5" Ref="R?"  Part="1" 
AR Path="/4F8FF8AD" Ref="R2"  Part="1" 
F 0 "R2" V 3930 3150 50  0000 C CNN
F 1 "10K" V 3850 3150 50  0000 C CNN
	1    3850 3150
	0    -1   -1   0   
$EndComp
$Comp
L R R1
U 1 1 4F8FF8A5
P 3850 3000
F 0 "R1" V 3930 3000 50  0000 C CNN
F 1 "10K" V 3850 3000 50  0000 C CNN
	1    3850 3000
	0    -1   -1   0   
$EndComp
$Comp
L CONN_6 P1
U 1 1 4F8FF0F4
P 2750 3250
F 0 "P1" V 2700 3250 60  0000 C CNN
F 1 "CONN_6" V 2800 3250 60  0000 C CNN
	1    2750 3250
	-1   0    0    -1  
$EndComp
$EndSCHEMATC
