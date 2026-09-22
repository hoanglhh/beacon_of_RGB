# Beacons of RGB

Course project for **Data Structures and Algorithms 1** at Tampere University.

The project simulates a network of colored beacons connected by light beams and optical fibers. The main focus of the project was implementing efficient data structures and graph algorithms in C++.

## Technologies

* C++20
* C++ Standard Template Library (STL)
* Qt / Qt Creator
* Git

## What I Implemented

My implementation focused on the `Datastructures` class responsible for storing, querying, and processing beacon and fiber-network data.

Key functionality includes:

* Managing beacon information such as IDs, coordinates, names, and colors
* Efficient lookup and sorting of beacon data
* Managing directed light-beam relationships between beacons
* Representing an optical-fiber network as a graph
* Finding routes between points in the fiber network
* Finding routes with the fewest intermediate points
* Finding the lowest-cost route through the network
* Detecting cycles in the fiber network
* Using STL data structures and algorithms with attention to time complexity and performance

## Algorithms and Data Structures

The project provided practical experience with:

* Graph traversal
* Shortest-path algorithms
* Cycle detection
* Hash-based containers
* Ordered containers
* Sorting and searching
* C++ STL containers and algorithms

A major requirement of the project was selecting appropriate data structures so that operations remained efficient as the amount of stored data increased.

## Project Structure

The university provided the main application, command interface, and Qt graphical interface.

My implementation is primarily located in:

```text
datastructures.hh
datastructures.cc
```

These files contain the data structures, helper functions, and algorithms used by the application.

## Running the Project

The application can be run either with the Qt graphical interface or as a command-line program.

### Qt

Open the project in Qt Creator and build/run the project.

### Command Line

The project can also be compiled using a C++20-compatible compiler.

```bash
g++ -std=c++20 ...
```

The exact compilation command depends on which provided course files are included.

## Example Features

The program can:

* Add and query beacons
* Sort beacons by properties such as name or brightness
* Connect beacons using light beams
* Build networks of optical fibers
* Search for paths through the fiber network
* Calculate lowest-cost routes
* Detect cycles in the network

## What I Learned

This project strengthened my understanding of:

* Choosing data structures based on performance requirements
* Implementing graph algorithms in C++
* Working with existing interfaces and an existing codebase
* Analyzing algorithmic complexity
* Using STL containers and algorithms effectively

## Academic Context

This repository contains my implementation of a university course project. Parts of the application framework and graphical interface were provided as course material by Tampere University.

