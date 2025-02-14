
#### **Overview**  
This repository contains six tasks related to low-level programming and system programming in MINIX. Each task involves modifying or extending functionalities, such as process scheduling, memory management, and file system design.  

Each task is stored in its respective folder (`task1`, `task2`, ..., `task6`). Below is a description of each task.  

---

## **Task Descriptions**  

### **Task 1: Finding the Process with the Longest Ancestry Path**  
**Goal:**  
- Implement a function in MINIX that returns the **PID of the process with the longest ancestry path** leading to a process with no parent.  
- Additionally, return the length of this ancestry path.  
- The function should **ignore** a process with a given PID (provided as a parameter).  

---

### **Task 2: Modifying User Process Scheduling Algorithm**  
**Goal:**  
Modify the **user process scheduling algorithm** in MINIX (**version 2.XX**).  

**Requirements:**  
- Assign user processes to one of **three groups: A, B, C**.  
- Each group has a predefined **quantum allocation** (number of time quanta assigned per scheduling cycle).  
- The CPU should be allocated **cyclically** among the groups:  
  - A → B → C → A → B → C → ...  
- Within each group, processes should be scheduled **cyclically**.  

**System Calls to Implement:**  
1. **Assign a process** to a specific group (A, B, or C).  
2. **Set the quantum allocation** for each group.  

**Verification:**  
- Design a method to verify that the scheduling algorithm works correctly.  

---

### **Task 3: Synchronization with Semaphores**  
**Goal:**  
Implement **synchronization** using **semaphores** (only `P` and `V` operations) with **threads and global variables**.  

**System Description:**  
- **FIFO buffer** with **9 elements**.  
- **2 producers** and **2 consumers**.  
- Producers generate **integer values**.  
- **Producer A** can only add elements **if the sum of all values in the buffer is greater than 20**.  
- **Consumers** can remove elements **only if the buffer contains more than 3 elements**.  

**Implementation Note:**  
- Use the **Semaphore class** from `monitor.h` (available on the course website).  

---

### **Task 4: Synchronization with Monitors**  
**Goal:**  
Implement **Task 3** but using **monitors** instead of semaphores.  

---

### **Task 5: Implementing Worst-Fit Memory Allocation**  
**Goal:**  
Modify the **default memory allocation algorithm** in MINIX by allowing the user to choose between:  
1. **First-Fit Algorithm** (default MINIX behavior).  
2. **Worst-Fit Algorithm** (allocates the largest available free memory block).  

**Requirements:**  
- Implement the **Worst-Fit Algorithm**.  
- Compare and analyze the differences between **First-Fit** and **Worst-Fit** in system performance and memory usage.  

---

### **Task 6: Designing a Simple Fault-Tolerant File System**  
**Goal:**  
Design and partially implement a **simple file system** with the following constraints:  
- Designed for **memory cards** with a storage size of **2–4 KB** (choose a specific size).  
- **Atomic** 16-bit read/write operations.  
- **Resilient to power failure** (e.g., sudden removal of the card from the reader).  
- After a failure, the system should require only **minimal recovery steps** upon reinsertion.  
- **No concurrent access** (single-threaded use case).  

---

## **How to Use This Repository**  
Each task is implemented in a separate folder (`task1`, `task2`, ..., `task6`). The folder structure includes:  
- **Source code** for the solution.  
- **Documentation** explaining the implementation.  
- **Test cases** (if applicable).  

---

## **Contributing**  
Feel free to submit pull requests with improvements, additional test cases, or alternative implementations.  

---

