Small implementation of the famous Conway's Game of Life cellular automaton, using ```ncurses``` terminal graphics libraries.

Some commands are available during execution:
```
  -[SPACE] -> generate a new random starting grid
  -'i'      -> show a small window with information about the current session.
  -'u'      -> hide the info window
  -'q'      -> terminate and exit the program
  -'c'      -> change the color of the cells
  -'b'      -> change the background color
  -'p'      -> enable step-by-step mode: to continue, press 'p'
  -'o'      -> enable continuous mode
  -'e'      -> increase the delay between iterations
  -'w'      -> decrease the time between iterations
```

It is possible to modify the size of the grid or the character of live/dead cells from the 'define' at the beginning of the program.
