# Image-Component-Labeling
Simulates labeling foreground elements of image by respective components.

Generates a random grid of 0's (background) and 1's (foreground) with chosen dimension/density by user. Using a stack/DFS (depth-first search) and a queue/BFS (breadth-first search), grids are scanned to label all foreground pixels adjacent to eachother as same component and in order in which they were labeled.

