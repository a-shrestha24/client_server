# P4 File System Operations

## Overview

The `p4` program provides a set of file system operations that allow users to start a server, check file existence, load, store, and delete files.

---

## Commands

### 1. Start Server

./p4 server


## 2. p4 check
This command checks if the specified path at the given IP and port exists by running a stat system call on the file system.

Return Values:
0 → File exists
1 → File does not exist

## 3. p4 load
If the specified file exists, this command retrieves all values from the file and returns the contents to the user.

## 4. p4 store
This command allows the user to input data on the client side. The input continues until Ctrl + D is pressed. The data is then written into the specified file. If the file does not exist, a new file is created.

## 5. p4 delete
This command deletes the specified file if it exists.

Return Values:
0 → File successfully deleted
1 → File deletion failed

## Example usage
```
./p4 server  # Start the server
p4 check /path/to/file  # Check if a file exists
p4 load /path/to/file  # Load and display file contents
p4 store /path/to/file  # Store user input into a file
p4 delete /path/to/file  # Delete a file
```
