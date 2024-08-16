# **CS3.304 - Advanced Operating Systems**
# Assignment - 1

The assignment contains 2 questions regarding system calls.

Commands used: *read()*, *write()*, *lseek()*, *stat()*, *fstat()*, *perror()*

## Q1

Q1 is about using system calls to reverse a large file (>1GB). There are 2 possible modes : 0 or 1.

Mode 0: Reverses the entire file

Mode 1 : Takes 2 more inputs from the command line: start_index and end index. The characters between start_index and end_index are copied as it is while the sections from start of file to start_index and end_index to end of file are reversed.

**Input format:**

./a.out <input file name> <flag> (either 0 or 1) <start_index> <end_index>

**Output format:**

The output file is named "0_<input file name>" if the input flag is 0, and
"1_<input file name>" if the input flag is 1.

## Q2

Q2 checks the following:
- The permissions for the two files and the directory
- Whether the content in the new file is the reverse of the old file.

**Input format:**

The path of newfile, oldfile, and directory is passed as a command-line argument.

./a.out <newfile_path> <oldfile_path> <directory_path>

**Output format:**

The output consists of 30 lines. The first 3 lines are:

Directory is created: Yes/No

Whether file contents are reversed in newfile: Yes/No

Both Files Sizes are Same : Yes/No

The next 27 lines are 3 sets of 9 lines each, which displays permissions of the new file, old file and directory respectively.



