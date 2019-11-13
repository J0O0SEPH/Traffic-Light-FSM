# Traffic light Control System "FSM"
>Code writing and designing process was guided by Dr. Jonathan W Valvano. Using the following references:

-   Introduction to the MSP432 Microcontroller-- ISBN-10: 1512185671.
-   Real-Time Interfacing to the MSP432 Microcontroller-- ISBN-10: 1514676583.
-   [https://courses.edx.org/courses/course-v1:UTAustinX+UT.6.20x+2T2018/course/](https://courses.edx.org/courses/course-v1:UTAustinX+UT.6.20x+2T2018/course/)

Moor's Finite State Machine is implemented to design a control system for traffic lights between two intersections and pedesterian's lane.

Initial state: going north >>Green.
going east & pedestrians >>Red.

Proximity sensor on the west side
will allow going to east, and north>> Red.

The opposite applies for the south side.

Occupancy sensor allow pedestrians to move and both vehicles lanes >> Red.

Sensing vehicles approaching in ether sides will allow time for foot passengers to cross the road (Red flashes).

If traffic congestion, pass will be alternating between lanes.
## Development Procedure

Concepts of software development cycle were adopted throughtout the creation of DataFlow graph, Call graph, and Successive Refinement.
## Debugging Procedure

Keil's Logic analyzer used to detect the states of I/O pins, Timing is controlled with a delay function.
