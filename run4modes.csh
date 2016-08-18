#!/bin/tcsh

set mcbMode=(sum dif amo inc)

if( $#argv == 1 ) then 
	set prefix = $1 
	set inputFile=$prefix.inp
	echo Input file is $inputFile
	foreach mode ( $mcbMode )
		echo Simulating MCB $mode for $inputFile input file
		./linux/bin/mcbu $inputFile $mode /home/hovanes/GlueX/MCB
	end
endif

