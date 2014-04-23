Analisys for : C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Aeropuerto.c

Modules are: 

	"stdio.h"

This is a basic module.

	"conio.h"

	This module has no functions 
	This module has no modules 

	"stdlib.h"

This is a basic module.

	"ctype.h"

This is a basic module.

	"math.h"

This is a basic module.

	"time.h"

This is a basic module.

	"limits.h"

This is a basic module.

	"windows.h"

This is a basic module.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Functions are: 

	Function name: addqueue
	Returns: void
	The arguments are:
		queue 
		plane 

	Function name: apaddqueue
	Returns: void
	The arguments are:
		airport 
		char 

	Function name: apdelqueue
	Returns: plane
	The arguments are:
		airport 
		char 

	Function name: apempty
	Returns: int
	The arguments are:
		airport 
		char 

	Function name: apfull
	Returns: int
	The arguments are:
		airport 
		char 

	Function name: apsize
	Returns: int
	The arguments are:
		airport 
		char 

	Function name: conclude
	Returns: void
	The arguments are:
		airport 
		int 

	Function name: delqueue
	Returns: plane
	The arguments are:
		queue 

	Function name: empty
	Returns: int
	The arguments are:
		queue 

	Function name: fly
	Returns: void
	The arguments are:
		airport 
		plane 
		int 

	Function name: full
	Returns: int
	The arguments are:
		queue 

	Function name: idle
	Returns: void
	The arguments are:
		airport 
		int 

	Function name: initairport
	Returns: void
	The arguments are:
		airport 

	Function name: initqueue
	Returns: void
	The arguments are:
		queue 

	Function name: land
	Returns: void
	The arguments are:
		airport 
		plane 
		int 

	Function name: myrandomize
	Returns: void
	The arguments are:

	Function name: newplane
	Returns: void
	The arguments are:
		airport 
		int 
		int 

	Function name: randomnumber
	Returns: int
	The arguments are:
		double 

	Function name: refuse
	Returns: void
	The arguments are:
		airport 
		int 

	Function name: size
	Returns: int
	The arguments are:
		queue 

	Function name: start
	Returns: void
	The arguments are:
		int 
		double 
		double 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADTs are: 

	The name is : plane
	The variables are: 
		int id
		int tm
	The name is : queue
	The variables are: 
		int count
		int front
		int rear
		plane p[3]

	The name is : airport
	The variables are: 
		queue landing
		queue takeoff
		queue pl
		queue pt
		int idletime
		int takeoffwait
		int ntakeoff
		plane pln
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
It has no attributes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

GlobalVariables are: 

ARRIVE=0
DEPART=1
MAX=3
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Candidate classes are:

CandidateClass: addqueue
	Attributes are: 
	Methods are: 
			Method name: addqueue
	Returns: void
	The arguments are:
		queue 
		plane 
CandidateClass: airport
	Attributes are: 
			queue landing
			queue takeoff
			queue pl
			queue pt
			int idletime
			int takeoffwait
			int ntakeoff
			plane pln
	Methods are: 
			Method name: apaddqueue
	Returns: void
	The arguments are:
		airport 
		char 
			Method name: apdelqueue
	Returns: plane
	The arguments are:
		airport 
		char 
			Method name: apempty
	Returns: int
	The arguments are:
		airport 
		char 
			Method name: apfull
	Returns: int
	The arguments are:
		airport 
		char 
			Method name: apsize
	Returns: int
	The arguments are:
		airport 
		char 
			Method name: conclude
	Returns: void
	The arguments are:
		airport 
		int 
			Method name: fly
	Returns: void
	The arguments are:
		airport 
		plane 
		int 
			Method name: idle
	Returns: void
	The arguments are:
		airport 
		int 
			Method name: initairport
	Returns: void
	The arguments are:
		airport 
			Method name: land
	Returns: void
	The arguments are:
		airport 
		plane 
		int 
			Method name: newplane
	Returns: void
	The arguments are:
		airport 
		int 
		int 
			Method name: refuse
	Returns: void
	The arguments are:
		airport 
		int 
CandidateClass: apaddqueue
	Attributes are: 
	Methods are: 
			Method name: apaddqueue
	Returns: void
	The arguments are:
		airport 
		char 
CandidateClass: apdelqueue
	Attributes are: 
	Methods are: 
			Method name: apdelqueue
	Returns: plane
	The arguments are:
		airport 
		char 
CandidateClass: apempty
	Attributes are: 
	Methods are: 
			Method name: apempty
	Returns: int
	The arguments are:
		airport 
		char 
CandidateClass: apfull
	Attributes are: 
	Methods are: 
			Method name: apfull
	Returns: int
	The arguments are:
		airport 
		char 
CandidateClass: apsize
	Attributes are: 
	Methods are: 
			Method name: apsize
	Returns: int
	The arguments are:
		airport 
		char 
CandidateClass: conclude
	Attributes are: 
	Methods are: 
			Method name: conclude
	Returns: void
	The arguments are:
		airport 
		int 
CandidateClass: delqueue
	Attributes are: 
	Methods are: 
			Method name: delqueue
	Returns: plane
	The arguments are:
		queue 
CandidateClass: empty
	Attributes are: 
	Methods are: 
			Method name: empty
	Returns: int
	The arguments are:
		queue 
CandidateClass: fly
	Attributes are: 
	Methods are: 
			Method name: fly
	Returns: void
	The arguments are:
		airport 
		plane 
		int 
CandidateClass: full
	Attributes are: 
	Methods are: 
			Method name: full
	Returns: int
	The arguments are:
		queue 
CandidateClass: idle
	Attributes are: 
	Methods are: 
			Method name: idle
	Returns: void
	The arguments are:
		airport 
		int 
CandidateClass: initairport
	Attributes are: 
	Methods are: 
			Method name: initairport
	Returns: void
	The arguments are:
		airport 
CandidateClass: initqueue
	Attributes are: 
	Methods are: 
			Method name: initqueue
	Returns: void
	The arguments are:
		queue 
CandidateClass: land
	Attributes are: 
	Methods are: 
			Method name: land
	Returns: void
	The arguments are:
		airport 
		plane 
		int 
CandidateClass: myrandomize
	Attributes are: 
	Methods are: 
			Method name: myrandomize
	Returns: void
	The arguments are:
CandidateClass: newplane
	Attributes are: 
	Methods are: 
			Method name: newplane
	Returns: void
	The arguments are:
		airport 
		int 
		int 
CandidateClass: plane
	Attributes are: 
			int id
			int tm
	Methods are: 
			Method name: addqueue
	Returns: void
	The arguments are:
		queue 
		plane 
			Method name: apdelqueue
	Returns: plane
	The arguments are:
		airport 
		char 
			Method name: delqueue
	Returns: plane
	The arguments are:
		queue 
			Method name: fly
	Returns: void
	The arguments are:
		airport 
		plane 
		int 
			Method name: land
	Returns: void
	The arguments are:
		airport 
		plane 
		int 
CandidateClass: queue
	Attributes are: 
			int count
			int front
			int rear
			plane p[3]

	Methods are: 
			Method name: addqueue
	Returns: void
	The arguments are:
		queue 
		plane 
			Method name: delqueue
	Returns: plane
	The arguments are:
		queue 
			Method name: empty
	Returns: int
	The arguments are:
		queue 
			Method name: full
	Returns: int
	The arguments are:
		queue 
			Method name: initqueue
	Returns: void
	The arguments are:
		queue 
			Method name: size
	Returns: int
	The arguments are:
		queue 
CandidateClass: randomnumber
	Attributes are: 
	Methods are: 
			Method name: randomnumber
	Returns: int
	The arguments are:
		double 
CandidateClass: refuse
	Attributes are: 
	Methods are: 
			Method name: refuse
	Returns: void
	The arguments are:
		airport 
		int 
CandidateClass: size
	Attributes are: 
	Methods are: 
			Method name: size
	Returns: int
	The arguments are:
		queue 
CandidateClass: start
	Attributes are: 
	Methods are: 
			Method name: start
	Returns: void
	The arguments are:
		int 
		double 
		double 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
