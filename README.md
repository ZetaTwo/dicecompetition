# DICE Programming Competition 2015

This is our solutions to the problems at [DICE Programming Competition 2015](dice.se/dice-programmeringstavling/).
We solved 8/10 problems which you can find in their respective subdirectory.
We would like to thank DICE for a really nice event and for the great prizes.
Below is a short description of the problems.

## Disclaimer

This code was written during a single day hackathon and does not necessarily faithfully represent our coding style or skill.

## ActiveRepulsorShield

You play a game where take turns picking any unmarked point in a X*Y grid and mark all points in a rectangle below and to the left of the point.
The player which is forced to take the last (upper left) point loses. The task was to implement the game and create an AI that performed better than a random player.

We simply created an AI which was almost random but re-randomed if it was about to voluntarily lose. This AI wins about 75% of the matches.

## Anagramatic

Given a list of words, print out all which are anagrams to each other together in descending order of length.

## DockingBay

This was exactly the [Eight queens puzzle](https://en.wikipedia.org/wiki/Eight_queens_puzzle) but phrased with space ships.
We solved it by trying all 8! (= 40320) permutations of columns and validating if an arrangement is valid.

## DroneSystems

The task was to implement a game in which you try to escape from a number of (level# * 10) robots which always walks towards you.
Whenever two robots collide they create a scrap pile. If a robot collides with a scrap pile, it is also destroyed. If you collide with a robot or scrap pile, you lose. You can walk in 8 directions and teleport to a random location. When all robots are dead you win and move on to the next level.

We implemented this as a web based game using Python with Flask on the backend and some jQuery on the frontend. Simply run the Python file and visit http://localhost:5000 with your browser to play.

## ObstacleSorting

The task was to sort a large CSV file (~600k rows) while never having more than 1000 rows in memory.

We did this through [External sorting](https://en.wikipedia.org/wiki/External_sorting) in which we first split the input into smaller files of 1000 rows each and then merged them pairwise, just like the merge step in merge sort.

## Pokejoined

The task was to find the longest list of Pokemon names such that the last letter of the previous name matches the first letter och the next name.

We did this by creating a graph with the letters as nodes and Pokemon names as edges and then using a recursive function which tried all outgoing edges and recursed on them.

## SpaceStation

This was basically 0..n [Knapsack Problem](https://en.wikipedia.org/wiki/Knapsack_problem) with "weight" relabeled as "cost" and "value" relabeled "importance".

We solved it by reducing it to regular 0-1 knapsack and solving it through dynamic programming

## StationEnergyGrid

The task was again to implement a game and create a bot which performed better than a random AI.
Players take turn in drawing a horisontal or vertical line between two neighbouring points in a 5*5 grid.
Whenever you finished a square, you got 1 point and got to take another move. The player with the most points won.

We solved this my simply checking if we could finish a square, otherwise we picked a random move.
