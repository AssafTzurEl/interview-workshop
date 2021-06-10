```
// Receives map traversed so far, current position, target position.
// Returns success/failure.
Traverse(map, current, target):

    // Mark current location as traversed:
    map[current.x, current.y] = "2"

    if current == target:
        // We're done
        return true

    // else - attempt going "right" (unless there's a wall or it's already been traversed):
    if current.x + 1 < map.width && Map[current.x + 1, current.y] == "0":
        solved = Traverse(map, (current.x + 1, current.y), target)
        if solved:
            return true
    
    // else - attempt going "down":
    if current.y + 1 < map.length && Map[current.x, current.y + 1] == "0":
        solvedMap = Traverse(map, (current.x, current.y + 1), target)
        if solvedMap:
            return solvedMap

    // else - attempt going "left":
    if current.x - 1 >=0 && Map[current.x - 1, current.y] == "0":
        solvedMap = Traverse(map, (current.x - 1, current.y), target)
        if solvedMap:
            return solvedMap
        
    // else - attempt going "up":
    if current.y - 1 >= 0 && Map[current.x, current.y - 1] == "0":
        solvedMap = Traverse(map, (current.x, current.y - 1), target)
        if solvedMap:
            return solvedMap

    // All failed, backtrack:
    map[current.x, current.y] = "3"
    return false




// Usage:
maze = ...
startingPoint = (0, 0)
endPoint = (maze.width - 1, maze.height - 1)

success = Traverse(maze, startingPoint, endPoint)

if success:
    // map solved
else:
    // map cannot be solved
```