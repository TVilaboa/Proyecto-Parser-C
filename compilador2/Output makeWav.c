Analisys for : C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 3\WAV\makeWav.c

Modules are: 

	"stdio.h"

This is a basic module.

	"stdlib.h"

This is a basic module.

	"math.h"

This is a basic module.

	"make_wav.h"

	The functions in this module are: 

	Function name: write_little_endian
	Returns: void
	The arguments are:
		int word
		int num_bytes
		FILE wav_file

	 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

	Function name: write_wav
	Returns: void
	The arguments are:
		long num_samples
		int data
		int s_rate

	 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

	This module has no modules 

	"make_wav.c"

	The functions in this module are: 

	Function name: write_little_endian
	Returns: void
	The arguments are:
		int word
		int num_bytes
		FILE wav_file

	 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

	Function name: write_wav
	Returns: void
	The arguments are:
		long num_samples
		int data
		int s_rate

	 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

	The modules of this module are: 

	"assert.h"

This is a basic module.

	 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 


	"assert.h"

This is a basic module.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Functions are: 

MainFunction
	Global Sentences are :
	Function name: write_little_endian
	Returns: void
	The arguments are:
		int word
		int num_bytes
		FILE wav_file

	Function name: write_wav
	Returns: void
	The arguments are:
		long num_samples
		int data
		int s_rate

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
It has no ADTs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Variables are: 

	int buffer[88200]

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GlobalVariables are: 

BUF_SIZE=88200
S_RATE=44100
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Candidate classes are:

CandidateClass: main
	Attributes are: 
			int argc
			char argv
			int buffer[88200]

			int return
	Methods are: 
			Method name: main
	Returns: int
	The arguments are:
		int argc
		char argv
			Method name: write_wav
	Returns: void
	The arguments are:
		long num_samples
		int data
		int s_rate
CandidateClass: write_little_endian
	Attributes are: 
			int word
			int num_bytes
			FILE wav_file
	Methods are: 
			Method name: write_little_endian
	Returns: void
	The arguments are:
		int word
		int num_bytes
		FILE wav_file
CandidateClass: write_wav
	Attributes are: 
			long num_samples
			int data
			int s_rate
	Methods are: 
			Method name: write_wav
	Returns: void
	The arguments are:
		long num_samples
		int data
		int s_rate
			Method name: write_little_endian
	Returns: void
	The arguments are:
		int word
		int num_bytes
		FILE wav_file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
