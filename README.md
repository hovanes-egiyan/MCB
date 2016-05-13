# MCB
A package to simulate coherent bremsstraulung called MCB (Monte-Carlo Bremsstrahlung). 
I got these version from Derek Glazier under a file name mcb.tgz which contained a directory called mcb05. 

To compile the program jsut type "make" in the top directory. One needs to have ROOT and CERNLIB installed 
on their system. Also "CERNROOT", "ROOTSYS", "TERMCAPLIB" variables need to be defined, eg

    	setenv CERNROOT /site/cernlib/x86_64_rhel7/2005
    	setenv TERMCAPLIB /usr/lib64

The program is run something like this: 
gluon:MCB> ./linux/bin/mcbu vgl010 dif /home/hovanes/GlueX/MCB
where "vgl010" is the name of the input file, "dif" is the mode of running, and "/home/hovanes/GlueX/MCB" 
is where the output ROOT file is placed. 

The output is a ROOT file. InspectTrees.C file is an example of using the the ROOT tree from Derek. 
