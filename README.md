# Process Tree Simulation

## Overview

This project simulates a tree structure of processes using C++. Each node in the tree represents a process, and the tree structure allows for hierarchical organization and management of these processes. The project demonstrates the use of `fork()` to create new processes and provides functionality to inspect process information using system commands.

## Features

- **Process Tree Management**: Create and manage processes in a hierarchical tree structure.
- **Process Information**: Use system commands to retrieve and display information about specific processes.
- **Exception Handling**: Custom exception handling for better error management.

## Key Components

### Node Class

- Represents a node in the tree, corresponding to a process.
- Contains an ID, process ID (`pid`), parent node pointer, and a vector of child nodes.
- Methods to add children, set the process ID, and retrieve node information.

### Tree Class

- Manages a tree of `node` objects.
- Contains a head node (root of the tree) and tracks the maximum ID assigned to nodes.
- Methods to add child nodes, find nodes by ID, and execute the `ps` command on a node's process.

### Custom Exception

- `my_exp` class inherits from `std::exception` and allows throwing exceptions with custom messages.

## Installation
   ```bash
   git clone https://github.com/artemmolodoy1487/Procces-tree.git
   cd Procces-tree
   g++ -o process_tree main.cpp
   ./process_tree
```
