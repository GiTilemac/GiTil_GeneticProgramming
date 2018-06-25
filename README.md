# GiTil_GeneticProgramming
Computer Engineering and Informatics Department  
University of Patras  
Spring Semester 2016  
Computational Intelligence  

### Author: Tilemachos S. Doganis

### Short Description - Further comments in source files


 The purpose is to produce a simple program that when executed will solve a maze.
The program is in the form of a 1-3 Tree Data Structure and is executed top-down.

 In every generation a population of POPSIZE programs is produced.
Each one is evaluated by executing the program and evaluating the resulting
path using a fitness function. The most successful ones are then selected,
crossed-over and randomly mutated and the resulting Forest is evaluated again.

 The process repeats itself until a Forest includes a Tree which provides the
best possible solution according to the fitness criterium, or until MAX_GENS generations
have been produced, and the best solution up to that point is printed.

### Comments

- Edit Genetic Program Parameters using include/headers.h
As seen on the diagrams mutation_rate.jpg and population.jpg,
ideal mutation rate was found to be 0.9 and population size around 300
	
- Best Solution Tree is printed at the end
	
- Example Visualization can be seen using Tree_Visualization.jpg

### References
- *A Simple Genetic Algorithm (SGA)*, created by Stratos Georgopoulos
  based on the algorithm presented in the book:
  *Michalewicz, Z., "Genetic Algorithms + Data Structures = Evolution Programs",
  Springer-Verlag, 3rd edition, 1996.*

### Visualization  
#### Example Genetic Program output:  
![Alt text](/Tree_Visualization.jpg)  
#### Generations required to reach solution for different mutation rates:  
![Alt text](/mutation_rate.jpg)  
#### Generations required to reach solution for population size:  
![Alt text](/population.jpg)
