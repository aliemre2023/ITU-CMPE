# Simple Filesystem Implementation

This project is a basic user-space file system.

## Getting Started

First, connect to the docker provided in the directory.

## Building and Running

```bash
# Compile the filesystem
make build

# Run individual commands
bin/main [command] [arguments]

# Available commands:
# mkfs - Create a new filesystem
# mkdir_fs [path] - Create a directory
# create_fs [path] - Create a file
# write_fs [path] "data" - Write to a file
# read_fs [path] - Read from a file
# ls_fs [path] - List directory contents
# delete_fs [path] - Delete a file
# rmdir_fs [path] - Remove a directory
```

## Testing

To test with custom inputs and expected outputs:

1. Add commands to commands.txt
2. Add expected results to expected_output.txt 
3. Run the test suite:

```bash
make check
```
('make check' runs the mini_fs.c file)
The output will be in output.txt and compared with your expected (expected_output.txt) results.

## Documentation

Screenshots are available in the `screenshot` folder, and a report is occur in the directory. Logs are available in the `tests/run_log.txt` file. Outputs are stored on `tests/output.txt`.