# Maze Traversal

Given a two-dimentional array, navigate from a spawning point (e.g. [0, 0]) to the exit point without walking through walls.

The maze's cells are initialized with "0", except walls who appear as "1"s. The output should be a map of the way traversed.

Write your solution in pseudo-code.

## Example:

Given the following maze:
```
0 1 0 0 0 0
0 1 0 0 0 0
0 1 0 0 0 0
0 1 0 0 0 0
0 1 1 1 1 1
0 0 0 0 0 0
```
Start at [0. 0], reach [5, 5].

The solution would be:
```
2 1 0 0 0 0
2 1 0 0 0 0
2 1 0 0 0 0
2 1 0 0 0 0
2 1 1 1 1 1
2 2 2 2 2 2
```
Where "2" is the chosen path.