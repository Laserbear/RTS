# RTS

super simple RTS game written in C++ without an engine

TODO:

* Define selection rectangles to control subsets of units
* Handle mouse events to draw rectangle to the screen
* Separate game ticks from graphics refresh so it looks smooth
* Find entities within selection
* Only selected entities receive target location

* Define terrain and randomly generate some obstacles/unpassable area
* Define pathing algorithm around walls
* Get entities to not run into one another

* Serialize commands like moving a group of entities to a target location
* Move computation/world ticks to server.c 
* Send back entity and world location data to client.c

* Parse commands in queue and efficiently merge overwriting commands so unit obeys latest

* define units a bit better, give them stats
* allow units to fight each other
* add unit sprites

* add notion of player ownership to units
* serverside: validate that a unit is owned by player before processing command to move it
* clientside: only select and highlight units you own

* Add main menu
* Add lobby to eventually host game configurations
* Add font
* Add UI tool bar
* Add Unit description
* Add minimap

* Add resources 
* Add gathering
* Add buildings
* Allow unit purchases
* Allow unit upgrades
* Add unit types
* Add unit maintenance costs
* Add roads
* Add supply lines which reduce maintenance cost

Day 1: Simple socket server + client scaffolding (only supporting text), drew window to screen, abstracted out entities, entities path to click, main game loop, FPS calculation

Day 2: Allowed selecting and highlighting units by drawing rectangle and then controlling those units, statically allocated memory for 5000 units, update entity positions 1/100th as fast as graphics refreshes 
