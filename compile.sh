#!/bin/bash
#compiltion limited to 4 files
if [[ $1 == "-h" ]]
then echo usage: compile.sh "<output_name> <input_name.c>"
exit 1
fi
avr-gcc -std=c99 -mmcu=atmega16 -Os -o $1.elf $2 $3 $4 $5
if [[ $? != 0 ]]  
then exit $?
fi
avr-objcopy -j .text -j .data -O ihex $1.elf $1.hex 
echo done
