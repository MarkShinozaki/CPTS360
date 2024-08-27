# Lab Rubrics 

### Pre-Lab 1 (1-12-23) + Lab 1 (1-19-23)

### [`Pre-Lab 1 Guide`](https://github.com/MarkShinozaki/CPTS360-SystemsProgrammingInUnix-Linux/blob/Lab-Rubrics/Pre-Lab%201(1-12-23)%20%2B%20Lab1(1-19-23)/Pre-Lab%201%20-%20Guide.jpeg)
#### Purpose:
- The Pre-Lab 1 Guide is designed to prepare students for Lab 1 by ensuring they understand the basics of stack frames, function calls, and memory addressing in C. This guide walks students through setting up their environment, writing C programs, and understanding the stack usage during function calls.

#### Key Tasks:

##### 1. Environment Setup:
- Ensure you have a 32-bit environment set up, as this lab involves generating 32-bit code.
- Instructions include using gcc -m32 for compiling the code.

##### 2. Writing and Understanding C Programs:
- Write simple C programs to print the values and addresses of function arguments.
- The guide emphasizes understanding how variables are passed and stored in memory.

##### 3. Stack Frame Analysis:
- The guide includes exercises where students print out the stack frame pointers (like `FP` in the C function).
- Students are asked to trace the stack frame link list and understand how function calls are stacked.

##### 4. Hexadecimal Printing:
- Convert and print addresses in hexadecimal to better understand memory layout.

#### Outcome:
- By the end of Pre-Lab 1, students should be comfortable with function calls, stack frames, and basic memory operations in C. This foundational knowledge is crucial for Lab 1.


### [`Lab Assignment 1 Guide`](https://github.com/MarkShinozaki/CPTS360-SystemsProgrammingInUnix-Linux/blob/Lab-Rubrics/Pre-Lab%201(1-12-23)%20%2B%20Lab1(1-19-23)/Lab%20Assignment%201%20-%20Guide.jpeg)
#### Purpose:
- The Lab Assignment 1 Guide builds on the knowledge from the Pre-Lab, focusing on applying those concepts in a more structured and comprehensive lab exercise.

#### Key Sections:

##### 1. Part 1: myprintf Function:
- The guide walks students through implementing a custom myprintf function, which requires them to manipulate stack frames to print variables. This simulates the behavior of the printf function but with a hands-on approach to understanding stack usage.

##### 2. Part 2: Partition Table:
- Students work with a simulated disk partition table. The tasks involve reading the partition table from a virtual disk, understanding how partitions are stored, and writing code to analyze and display partition information.

##### 3. Detailed Code Examples:
- The guide provides detailed examples and step-by-step instructions for writing the necessary C code. This includes how to manipulate pointers, read data from memory, and correctly handle function calls.

##### 4. Code Validation:
- The guide emphasizes testing and validating the written code against provided sample solutions. Students are encouraged to compile and run their code to ensure correctness.

#### Outcome:
- Lab Assignment 1 solidifies the students' understanding of stack operations and introduces them to more advanced topics like disk partitioning. The practical application of concepts learned in Pre-Lab 1 is crucial here.

---

### Pre-Lab 2 (1-26-23) + Lab 2 (2-2-23)

### [`Pre-Lab 2 Guide`](https://github.com/MarkShinozaki/CPTS360-SystemsProgrammingInUnix-Linux/blob/Lab-Rubrics/Pre-Lab%202(1-26-23)%20%2B%20Lab%202(2-2-23)/Pre-Lab%202%20-%20Guide.jpeg)
#### Purpose: 
- The Pre-Lab 2 Guide is designed to prepare students for Lab Assignment 2 by introducing them to the core concepts of process management within an operating system kernel simulation. The focus is on understanding how processes are managed, forked, and how the operating system handles them.

#### Key Concepts Covered:

##### 1. Process Structure:

- Students learn about the internal structure of processes (PROC) in an OS kernel simulation, including fields like `pid`, `ppid`, `priority`, and `status`.

##### 2. Process Initialization:

- The guide walks students through initializing the process management system, including setting up the initial process (P0) and forking child processes.

##### 3. Simulating Process Behavior:

- Students simulate basic operations such as process forking, waiting, sleeping, and waking up. These operations help in understanding the kernel-level process management.

##### 4. Running the Simulation:

- The guide provides steps to compile and run the provided simulation program, including commands to interact with the process management system, such as `fork`, `switch`, `exit`, `sleep`, and `wait`.

#### Outcome: 
- By completing Pre-Lab 2, students gain a foundational understanding of process management at the kernel level. They should be able to run and modify a basic OS kernel simulation, preparing them for the more advanced tasks in Lab Assignment 2.

### [`Lab Assignment 2 Guide`](https://github.com/MarkShinozaki/CPTS360-SystemsProgrammingInUnix-Linux/blob/Lab-Rubrics/Pre-Lab%202(1-26-23)%20%2B%20Lab%202(2-2-23)/Lab%20Assignment%202%20-%20Guide.jpeg)
#### Purpose: 
- The Lab Assignment 2 Guide focuses on building a more complex shell simulator (mysh) that handles Unix-like commands, process management, I/O redirection, and piping. This lab builds on the knowledge gained in Pre-Lab 2 and requires implementing key OS functionalities.

#### Key Sections:

##### 1. Building the Shell Simulator:

- The guide instructs students to create a shell program (`mysh`) that simulates the Unix shell. This includes parsing commands, forking processes, and executing commands using `execve()`.

##### 2. Handling I/O Redirection:

- Students implement I/O redirection, allowing their shell to handle commands that redirect input/output to and from files, such as `cmd > file`.

##### 3. Piping Commands:

- The lab covers piping, where the output of one command serves as the input to another (e.g., `cmd1 | cmd2`). This requires setting up and managing pipes between processes.

##### 4. Advanced Process Control:

- Beyond basic forking and execution, the lab emphasizes handling complex scenarios, including multiple pipes, chaining commands, and managing command execution order.

##### 5. Modifying the Base Code:

- Students start with a provided base code (lab2base.c) and modify it to meet the requirements of the assignment, ensuring they understand each component they add.

#### Outcome: 
- By the end of Lab Assignment 2, students should have a functioning Unix-like shell that can handle various command types, including those with I/O redirection and piping. This lab integrates knowledge from Pre-Lab 2 and challenges students to apply it in a real-world simulation.

--- 

### Pre-Lab 3 (2-9-23) + Lab 3 (2-16-23)

### [`Pre-Lab 3 Guide`](https://github.com/MarkShinozaki/CPTS360-SystemsProgrammingInUnix-Linux/blob/Lab-Rubrics/Pre-Lab%203(2-9-23)%20%2B%20Lab%203(2-16-23)/Pre-Lab%203%20-%20Guide.jpeg) 

#### Purpose: 
- The Pre-Lab 3 Guide is intended to prepare students for the upcoming Lab Assignment 3 by introducing them to the concept of parallelism using threads. The focus is on modifying a simple program to work correctly with multiple threads, ensuring proper synchronization and correct final results.

#### Key Tasks:

##### 1. Copy and Modify Example Program:

- Students are required to copy an example program (`C4.1.c`) from the textbook and modify it to work with an 8x8 matrix.
  
- The initial task is to compile and run this program, then record the total output value.

##### 2. Threaded Summation:

- The guide instructs students to modify the function to allow each thread to sum its corresponding row of the matrix and add it to a global total.

- A simulated interrupt (using `sleep(1)`) is introduced to highlight the potential issues that arise when multiple threads access and modify shared data without proper synchronization.

##### 3. Handling Thread Synchronization:

- Students are asked to modify the main function to join all threads and print the final total value after all threads have completed execution.

- The guide challenges students to notice that the total output might be incorrect due to race conditions.

##### 4. Correcting Synchronization Issues:

- Finally, students are prompted to devise a solution that ensures correct execution by reading another example program (`C4.3`) that introduces concepts like mutexes or other synchronization mechanisms.

#### Outcome: 
- The Pre-Lab 3 Guide sets the stage for Lab Assignment 3 by highlighting the importance of correct synchronization when working with threads. Students learn about race conditions and how to mitigate them, preparing them for the more complex threading tasks in the lab.

### [`Lab Assignment 3 Guide`](https://github.com/MarkShinozaki/CPTS360-SystemsProgrammingInUnix-Linux/blob/Lab-Rubrics/Pre-Lab%203(2-9-23)%20%2B%20Lab%203(2-16-23)/Lab%20Assignment%203%20-%20Guide.jpeg) 

#### Purpose: 
- Lab Assignment 3 is a programming project focused on solving a system of linear equations using Gauss elimination with partial pivoting, all within a multi-threaded context. This lab builds on the concepts of parallel processing learned in Pre-Lab 3 and introduces more complex problem-solving using threads.

#### Key Sections:

##### 1. Implementation of Gauss Elimination:

- The guide requires students to modify the program (C4.5.c) to solve a specific problem (Problem 4.5 in the textbook). The problem involves using Gauss elimination with partial pivoting to solve linear equations.

- Students must set the matrix size (N=8) and use a specified number of threads (NTHREADS), e.g., 4 threads, to parallelize the computation.

##### 2. Thread Management:

- The lab emphasizes managing multiple threads to work on different parts of the matrix simultaneously. Each thread will handle a part of the computation, demonstrating the benefits of parallelism in speeding up calculations.

##### 3. Understanding LU Decomposition:

- Although the main focus is on Gauss elimination, the guide encourages students to learn about LU decomposition as it’s a related concept that can be useful for solving linear systems.

#### Outcome:
- By completing Lab Assignment 3, students gain hands-on experience with multi-threaded programming applied to a mathematical problem. The lab reinforces the importance of correct thread management and synchronization while providing a practical application of parallel processing concepts.

### Pre-Lab 4 + Lab 4

### [`Pre-Lab 4 Guide`](https://github.com/MarkShinozaki/CPTS360-SystemsProgrammingInUnix-Linux/blob/Lab-Rubrics/Pre-Lab%204(0-0-)%20%2B%20Lab%204(3-2-23)/Pre-Lab%204%20-%20guide.jpeg)

#### Purpose: 
- The Pre-Lab 4 Guide is focused on introducing students to basic network programming using TCP (Transmission Control Protocol). The goal is to get students comfortable with socket programming, client-server communication, and handling multiple clients using a multi-threaded server.

#### Key Tasks:

##### 1. Modify the Client Program (`client.c`):

- The guide asks students to modify the `client.c` program so that it sends a line containing two numbers (e.g., "123 456") to the server.

- The server (server.c) must be modified to receive this line, compute the sum of the two numbers, and reply with a message that includes the original numbers and their sum (e.g., "123 456 sum=579").

##### 2. Modify the Server Program (server.c):

- The guide further instructs students to modify the `server.c` program to handle multiple clients simultaneously. This is achieved by converting the server into a multi-threaded server.

- When a client connects, the server should fork a child process to handle the client’s requests, allowing the parent server process to continue accepting new connections.

- The child process should handle the client’s requests and terminate when the client disconnects.

#### Outcome: 
- By completing the Pre-Lab 4 Guide, students gain hands-on experience with basic network programming concepts, including setting up a TCP connection, sending and receiving data over a network, and managing multiple client connections using multi-threading.

### [`Lab Assignment 4 Guide`](https://github.com/MarkShinozaki/CPTS360-SystemsProgrammingInUnix-Linux/blob/Lab-Rubrics/Pre-Lab%204(0-0-)%20%2B%20Lab%204(3-2-23)/Lab%20Assignment%204%20-%20Checklist.jpeg)

#### Purpose: 
- Lab Assignment 4 extends the concepts from Pre-Lab 4 by having students build a more advanced TCP-based client-server application. The assignment focuses on implementing a multi-threaded server capable of executing commands both on the server and client sides.

#### Key Sections:

##### 1. Implementing Command Sets:

- Students must implement two sets of commands:
  - Server-executed commands: These are commands that the server executes, but the output is displayed on the client’s side (e.g., `pwd`, `cd`, `ls`, `cp`).

  - Client-executed commands: These are commands that the client executes locally (e.g., `lpwd`, `lcd`, `lls`, `lcp`).

##### 2. File Copy Operations:

- The assignment requires implementing file copy commands (`cp`, `lcp`) that allow files to be copied either from the server to the client or locally on the client.

##### 3. Multi-Threaded Server:

- The server must be multi-threaded, allowing it to handle multiple client connections concurrently. Each client connection should be managed by a separate thread, ensuring that the server remains responsive to new connections even when processing existing ones.

##### Outcome:
- Lab Assignment 4 consolidates the students' understanding of network programming by challenging them to implement a fully functional multi-threaded client-server application. The task not only tests their ability to manage network communication but also their understanding of threading and process management.

--- 

### Lab 5 

### [`Lab 5 Guide`](https://github.com/MarkShinozaki/CPTS360-SystemsProgrammingInUnix-Linux/blob/Lab-Rubrics/Lab%205%20(3-23-23)/Lab%20Assignment%205%20-%20guide.jpeg)

#### Purpose: 
- The Lab 5 assignment is focused on working with a virtual disk image containing an EXT2 file system. The goal is to develop a C program called showblock that can display the disk blocks of a specified file within the EXT2 file system. This lab is designed to deepen the students' understanding of file systems, particularly how file data is organized and retrieved on disk.

#### Key Tasks:

##### 1. Implementing `showblock`:

- Students are required to write a C program, showblock, which accepts a file pathname as input and displays the corresponding disk block numbers for that file. The program needs to handle both direct and indirect block addressing, as well as double-indirect blocks.

##### 2. Traversing the EXT2 File System Tree:

- The guide emphasizes the importance of understanding the structure of the EXT2 file system. Students must traverse the file system tree to locate the inode of the specified file, as the inode contains all the information needed to retrieve the disk blocks.

##### 3. Reading and Validating the Disk Image:

Students must open and read the disk image file in Linux, treating it as a file descriptor. They need to read the superblock, group descriptor, and other file system structures to ensure the disk image is indeed an EXT2 file system.

##### 4. Algorithm Development:

- A step-by-step algorithm is provided to help students implement the necessary functions to read disk blocks, print directory contents, and search for file names. The guide outlines how to load and parse directory entries, convert logical block addresses to physical ones, and handle various levels of block indirection.

#### Outcome: 

- By completing Lab 5, students will gain hands-on experience with low-level file system operations, including inode handling, block management, and directory traversal. This lab reinforces their understanding of how file systems manage and retrieve data on disk.


---

### Lab 6

### [`Lab 6 Guide`](https://github.com/MarkShinozaki/CPTS360-SystemsProgrammingInUnix-Linux/blob/Lab-Rubrics/Lab%206%20(3-30-23)/Lab%20Assignment%206%20(Project%20start)%20-%20guide.jpeg) 

### [`Project - Level 1`](https://github.com/MarkShinozaki/CPTS360-SystemsProgrammingInUnix-Linux/tree/Lab-Rubrics/Lab%206%20(3-30-23)/Level%201)

### [`Project - Level 2`](https://github.com/MarkShinozaki/CPTS360-SystemsProgrammingInUnix-Linux/tree/Lab-Rubrics/Lab%206%20(3-30-23)/Level%202)

#### Purpose: 
- Lab 6 is the start of a more extensive project where students implement basic file system commands (ls, cd, and pwd) using the previously studied EXT2 file system. This project requires students to develop these commands by implementing the corresponding functions in C.

#### Key Sections:

##### 1. Project Setup:

- Students are instructed to download and extract the provided project files. The project involves modifying the util.c file to implement functions such as `iget(dev, ino)`, `iput(mip`), and `path2inode(pathname)`.

##### 2. Command Implementation:

- The core requirement of Lab 6 is to implement the `ls`, `cd`, and `pwd` commands. These commands are fundamental to interacting with file systems, and their implementation will require in-depth knowledge of navigating directory structures, resolving pathnames, and manipulating inodes.

##### 3. Reading and Understanding:

- The guide directs students to specific chapters in the textbook (Chapter 11.7.3) to understand the algorithms behind these commands. Students must understand how to list directory contents (`ls`), change directories (`cd`), and print the working directory (`pwd`).

##### 4. Sample Solution:

- A sample solution is provided as a reference for students to compare their implementation and ensure they meet the project requirements.

#### Outcome: 
- Lab 6 sets the foundation for building a minimal shell environment capable of basic file system navigation and management. Students will develop a deeper understanding of file system interaction at the programmatic level, preparing them for more complex operations and further expansion of the project.
























