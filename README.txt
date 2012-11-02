SPnKr

Created by: Michael Patrick Tkacik

This program is provided to you for free.
To the best of my knowledge it should be 
free of defects and should be completely
safe to use. However, by using this
program, you understand that you are 
accepting all responsiblility for any 
damages it may cause.

In general, see the GPL. I am releasing
this program under that license.

DEPENDENCIES (external programs needed)
-----------------
SM (SuperMONGO)
MOOG (http://www.as.utexas.edu/~chris/moog.html)
MSPAWN (available as a tarball where you got this)

INSTALLATION
---------------
To install this program, you need to 
unpackage the tarball if it exists.
Once you have done that, entering the
directory containing the source code
and typing 'make all' will compile
all of the source code provided.

Once make has finished, ensure the
correct file paths are included in
'config.txt' and type
'make install'

This may require root access.
Once this finishes, the program should
be installed and run when the command
'spnkr' is typed in a terminal.

INSTRUCTIONS FOR USE
--------------------------
SPnKr assumes that you are working
with lists of stellar absorption
lines formatted for MOOG. It will
ask you for the location of the FE-I
and FE-II lines lists as well as 
which mode you wish to run it in.

I attempted to design the interface
in such a way that it asks simple
questions and does not require the
user to have advanced knowledge of
their computer.

If it does not seem to be working,
ensure that the data in your config.txt
is correct for the computer you are
using to run this program and that
you have the necessary programs
installed and working.
