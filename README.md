# **FSH (Flex Shell)**

FSH is a custom-built Linux shell written in C++ designed to offer a flexible, lightweight, and expandable command-line interface. This project marks my initial journey into understanding shell internals, process management, and system programming, with many more features planned for the future.

---

## 🚀 **Current Features**

- **Basic Command Execution:**  
  Run standard Linux commands like `ls`, `pwd`, `echo`, etc.

- **Argument Handling:**  
  Parse and manage command-line arguments for robust command execution.

- **Built-In Commands:**  
  - `exit`: Terminate the shell.  
  - `cd <directory>`: Change the current working directory.  
  - `history`: Display a list of previously executed commands.

- **Process Management:**  
  Leverage `fork()` and `execvp()` for command execution in separate processes.

- **Real-Time Clock:**  
  Display the current time in the shell prompt.

---

## 🔮 **Planned Features**

- **Environment Variable Support:**  
  Manage and export environment variables.

- **Input/Output Redirection:**  
  Support redirection operators like `>` and `<`.

- **Piped Commands:**  
  Enable command chaining with pipes (`|`).

- **Background Execution:**  
  Execute commands in the background using `&`.

- **Tab-Based Auto-Completion:**  
  Enhance usability with command and file name auto-completion.

- **Additional Enhancements:**  
  Continuous improvements and refinements as I dive deeper into shell programming.

---

## 🛠 **Installation**

### **Prerequisites**

- **GCC/G++ Compiler:**  
  Required to build the project.

- **Linux Operating System:**  
  Full compatibility and best performance are achieved on Linux.

### **Installation Steps**

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/abdullah4tech/fsh.git
   cd fsh
   ```

2. **Compile the Shell:**

   ```bash
   g++ fsh.cpp -o fsh
   ```

3. **Run FSH:**

   ```bash
   ./fsh
   ```

---

## 🤝 **Contributing**

Contributions are welcome! If you have ideas, fixes, or improvements, feel free to fork the repository, create a branch, and submit a pull request. Your input helps make FSH even better.

---

## 📜 **License**

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---