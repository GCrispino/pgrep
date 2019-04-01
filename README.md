# MAC 5742 - EP1 - Parallel Grep

## Overview

This repository contains the code for the first program exercise at **MAC 5742 - Introduction to Concurrent, Parallel and Distributed Programming** at IME-USP.

The exercise solution should contain a parallel implementation of the unix `grep` command: given a POSIX regular expression string, a number of threads and a directory path, it should search for matches for this regex in files inside the given directories, in a parallel implementation using the given number of threads.

## Building

To build the code you should just go to the `src` folder and run `make`:

```bash
$ cd src
$ make
```

This will generate an executable file called `pgrep` under the `src` folder

## Running

To run, follow this pattern:

```bash
$ ./pgrep <max_threads> <regex> <search_path>
```

This will generate an output in the format `<folder>/<file>: <line-number>`, for each match:

```bash
A/a.txt: 0
A/a.txt: 2
A/a.txt: 5
A/C/c.txt: 0
A//C/c.txt: 1
```
