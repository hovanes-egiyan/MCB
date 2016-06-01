#!/bin/tcsh

# Set MATH alias - takes an arithmetic assignment statement
# as argument, e.g., newvar = var1 + var2
# Separate all items and operators in the expression with blanks
alias MATH 'set \!:1 = `echo "\!:3-$" | bc -l`' 

#if( $#argv == 2 ) then 
MATH x0 = -13.24
MATH y0 = 0.0
MATH xCenter = -10.24
MATH yCenter = 0.00 
foreach iPos (`seq 1 1 21`)
	set prefix="gluex022_perp"

	MATH xPosition0 = $x0 + ($iPos-1) * 0.3
	set xPosition = `echo "scale=2;$xPosition0/1.0" | bc` 
	set yPosition = 0.00

	echo "xPosition is " $xPosition " , yPosition is " $yPosition

	MATH xOffset = $xPosition - $xCenter
	MATH yOffset = $yPosition - $yCenter

	echo "xOffset = " $xOffset  " , yOffset = " $yOffset

	set fileName=$prefix"_"$xPosition"_"$yPosition".inp"

	sed -e "s/COL_X/$xOffset/"  -e "s/COL_Y/$yOffset/"  gluex022_perp_template.inp >! $fileName

	./linux/bin/mcbu $fileName sum /home/hovanes/GlueX/MCB
	./linux/bin/mcbu $fileName dif /home/hovanes/GlueX/MCB
	./linux/bin/mcbu $fileName amo /home/hovanes/GlueX/MCB
	./linux/bin/mcbu $fileName inc /home/hovanes/GlueX/MCB
end
#endif

