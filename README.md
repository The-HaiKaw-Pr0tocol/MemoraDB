> Last Updated: 16th July, 2025
# MemoraDB

MemoraDB is a lightweight, high-performance in-memory database project designed for ultra-fast memory based key-value storage.

## Features

- RESP command parsing
- Supported commands: `PING`, `ECHO`, `SET`, `GET`, `RPUSH`
- Expiry support via PX argument in `SET`
- Thread-per-client model using threads

## Project Structure

```
MemoraDB/
├── src/
│   ├── client/           # (Optional) client stub
│   ├── server/           # Main TCP server 
│   ├── parser/           # RESP protocol parser
│   ├── utils/            # Utility functions, logo printer
├── Makefile              # Project build file
├── build.sh              # Script for header updating and project building
```

## Building

To build the project, run:

```
./build.sh
```

You will be prompted to compile using the Makefile after headers are updated.

Alternatively, you can run manually with:

```
make
```

## Command Reference

-- to be added 

## Authors

- Kawtar Taik (@kei077)
- Haitam Bidiouane (@sch0penheimer)
