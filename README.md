#Ur

Ur is supposed to be a God game, in which you influence the goings-on in a world through your believers, with open ended goals.

##What it is now

A NCURSES program (part of #PetraCURSES) that has a 2D city of defined size containing buildings and "beings" which can path through them. The beings can use doors etc and path using A*. Pathing is not as fast as I'd like, but it works.

Furniture is scattered around the map, and the beings go pick it up and bring it back to their starting rooms. If the room fills up the find a new one. They also steal from other being's rooms not part of their own. While I haven't made any pathing improvements, this does cut down on the amount of time it ends up taking.

##Plans

* Beings interact with the world
* Player can influence events
* City grows
* Multiple cities and areas
* Implement graphically in some way
