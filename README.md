# MemoraDB

### Overview

MemoraDB is an ultra-low-latency, in-memory key-value database engineered for applications where speed, reliability, and simplicity are paramount. Built in C with a focus on modern system design, MemoraDB leverages efficient data structures, atomic operations, and robust multi-threading to deliver microsecond-level performance.

MemoraDB is designed to be a standalone in-memory database with all its client forms, but we decided to make it also fully compliant with the **RESP3 protocol**, meaning it can serve as a drop-in replacement for Redis in many use cases and is compatible with any modern Redis/RESP client, CLI, and tool. Whether you're building caching layers, real-time session storage, analytics backplanes, or embedded systems, MemoraDB is designed to be simple to integrate, easy to extend, and blazingly fast.

> Developed & Maintained by [The HaiKaw Pr0tocol](https://github.com/The-HaiKaw-Pr0tocol) organization.

## MemoraDB's Logo
<div align="center">
    <img src="https://github.com/user-attachments/assets/3fc677fd-e5f2-4b1e-b851-9b93b6977b0e" alt="MemoraDB's Logo" width="1000">
</div>

## Authors

<div align="center">

<table>
  <tr>
    <td align="center">
        <img src="https://github.com/user-attachments/assets/18ff4153-f665-4426-b6ad-ad9717a08e1d" width="240px;" alt="Kawtar Taik"/><br />
        <b>Kawtar Taik</b> <br />
            <img src="https://github.com/user-attachments/assets/cd6bd36c-907c-49d2-a81b-5462c2e4142a" width="20" height="20" />
            <span>
                <a href="https://github.com/kei077"><img src="https://img.shields.io/badge/@kei077-8A2BE2?style=plastic"/></a>
            </span>
            <br /> <br />
            <a href="https://github.com/kei077" title="GitHub">
                <img src="https://img.shields.io/badge/-4B006E?style=flat&logo=github&logoColor=white" />
            </a>
            <a href="https://www.linkedin.com/in/kawtar-ta%C3%AFk-7544a11b9/" 
            title="LinkedIn">
                <img src="https://img.shields.io/badge/-4B006E.svg?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHNoYXBlLXJlbmRlcmluZz0iZ2VvbWV0cmljUHJlY2lzaW9uIiB0ZXh0LXJlbmRlcmluZz0iZ2VvbWV0cmljUHJlY2lzaW9uIiBpbWFnZS1yZW5kZXJpbmc9Im9wdGltaXplUXVhbGl0eSIgZmlsbC1ydWxlPSJldmVub2RkIiBjbGlwLXJ1bGU9ImV2ZW5vZGQiIHZpZXdCb3g9IjAgMCA1MTIgNTEyIj48cGF0aCBmaWxsPSIjZmZmIiBkPSJNNDc0LjkxOSAwSDM4LjU5MkMxNy43MiAwIDAgMTYuNTA0IDAgMzYuODQxVjQ3NS4xNEMwIDQ5NS40OTYgMTEuNjI5IDUxMiAzMi40OTIgNTEyaDQzNi4zMjdDNDg5LjcxOCA1MTIgNTEyIDQ5NS40OTYgNTEyIDQ3NS4xNFYzNi44NDFDNTEyIDE2LjUwNCA0OTUuODA5IDAgNDc0LjkxOSAwek0xOTUuMDQzIDE5NS4wNDNoNjguOTI4djM1LjEzNmguNzU1YzEwLjUwNS0xOC45NDUgNDEuNTQxLTM4LjE3NyA3OS45MjEtMzguMTc3IDczLjY1NSAwIDk0LjIxNCAzOS4xMDggOTQuMjE0IDExMS41Mzh2MTM1LjMyMWgtNzMuMTQ4VjMxNi44ODNjMC0zMi40MjctMTIuOTQ3LTYwLjg4My00My4yMjctNjAuODgzLTM2Ljc2OCAwLTU0LjI5NSAyNC44ODktNTQuMjk1IDY1Ljc1OHYxMTcuMTAzaC03My4xNDhWMTk1LjA0M3pNNzMuMTM5IDQzOC44NjFoNzMuMTQ4VjE5NS4wNDNINzMuMTM5djI0My44MTh6bTgyLjI4OS0zMjkuMTQ4YzAgMjUuMjU4LTIwLjQ1NyA0NS43MTUtNDUuNzE1IDQ1LjcxNS0yNS4yNTggMC00NS43MTUtMjAuNDU3LTQ1LjcxNS00NS43MTUgMC0yNS4yNTggMjAuNDU3LTQ1LjcxNSA0NS43MTUtNDUuNzE1IDI1LjI1OCAwIDQ1LjcxNSAyMC40NTcgNDUuNzE1IDQ1LjcxNXoiLz48L3N2Zz4=" />
            </a>
            <a href="mailto:kawtartaik123@gmail.com" 
            title="Email">
                <img src="https://img.shields.io/badge/-4B006E.svg?logo=data:image/svg+xml;base64,//48AHMAdgBnACAAeABtAGwAbgBzAD0AIgBoAHQAdABwADoALwAvAHcAdwB3AC4AdwAzAC4AbwByAGcALwAyADAAMAAwAC8AcwB2AGcAIgAgAHMAaABhAHAAZQAtAHIAZQBuAGQAZQByAGkAbgBnAD0AIgBnAGUAbwBtAGUAdAByAGkAYwBQAHIAZQBjAGkAcwBpAG8AbgAiACAAdABlAHgAdAAtAHIAZQBuAGQAZQByAGkAbgBnAD0AIgBnAGUAbwBtAGUAdAByAGkAYwBQAHIAZQBjAGkAcwBpAG8AbgAiACAAaQBtAGEAZwBlAC0AcgBlAG4AZABlAHIAaQBuAGcAPQAiAG8AcAB0AGkAbQBpAHoAZQBRAHUAYQBsAGkAdAB5ACIAIABmAGkAbABsAC0AcgB1AGwAZQA9ACIAZQB2AGUAbgBvAGQAZAAiACAAYwBsAGkAcAAtAHIAdQBsAGUAPQAiAGUAdgBlAG4AbwBkAGQAIgAgAHYAaQBlAHcAQgBvAHgAPQAiADAAIAAwACAANQAxADIAIAAzADIANwAuADUAMwAiAD4APABwAGEAdABoACAAZgBpAGwAbAA9ACIAIwBmAGYAZgAiACAAZAA9ACIATQAyADUANAAuADQAMQAgADIANwA0AC4AOQA3AGwAMQAwADAALgA5ADMALQAxADAAMAAuADkAMgAgADEANQAzAC4ANAA5ACAAMQA1ADMALgA0ADgASAAwAGwAMQA1ADMALgA0ADkALQAxADUAMwAuADQAOAAgADEAMAAwAC4AOQAyACAAMQAwADAALgA5ADIAegBNADUALgA4ADQAIAAwAGwAMgA0ADgALgA1ADcAIAAyADQAOAAuADUANgBMADUAMAAyAC4AOQA4ACAAMABIADUALgA4ADQAegBNADAAIAAyADkAMAAuADMAbAAxADMAMQAuADcANwAtADEAMwAxAC4ANwA4AEwAMAAgADIANgAuADcANQBWADIAOQAwAC4AMwB6AG0ANQAxADIAIAAxADIALgA2ADEATAAzADYANwAuADYAMQAgADEANQA4AC4ANQAyACAANQAxADIAIAAxADQALgAxADQAdgAyADgAOAAuADcANwB6ACIALwA+ADwALwBzAHYAZwA+AA==" />
            </a>
      <br />
    </td>
    <td align="center">
        <img src="https://github.com/user-attachments/assets/fb730dfb-b650-47f1-9810-e993a2e6f88d" width="240px;" alt="Haitam Bidiouane"/><br />
        <b>Haitam Bidiouane</b> <br />
            <img src="https://github.com/user-attachments/assets/cd6bd36c-907c-49d2-a81b-5462c2e4142a" width="20" height="20" />
            <span>
                <a href="https://github.com/sch0penheimer"><img src="https://img.shields.io/badge/@sch0penheimer-8A2BE2?style=plastic"/></a>
            </span>
            <br /> <br />
            <a href="https://github.com/sch0penheimer" title="GitHub">
                <img src="https://img.shields.io/badge/-4B006E?style=flat&logo=github&logoColor=white" />
            </a>
            <a href="https://www.linkedin.com/in/haitam-bidiouane" 
            title="LinkedIn">
                <img src="https://img.shields.io/badge/-4B006E.svg?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHNoYXBlLXJlbmRlcmluZz0iZ2VvbWV0cmljUHJlY2lzaW9uIiB0ZXh0LXJlbmRlcmluZz0iZ2VvbWV0cmljUHJlY2lzaW9uIiBpbWFnZS1yZW5kZXJpbmc9Im9wdGltaXplUXVhbGl0eSIgZmlsbC1ydWxlPSJldmVub2RkIiBjbGlwLXJ1bGU9ImV2ZW5vZGQiIHZpZXdCb3g9IjAgMCA1MTIgNTEyIj48cGF0aCBmaWxsPSIjZmZmIiBkPSJNNDc0LjkxOSAwSDM4LjU5MkMxNy43MiAwIDAgMTYuNTA0IDAgMzYuODQxVjQ3NS4xNEMwIDQ5NS40OTYgMTEuNjI5IDUxMiAzMi40OTIgNTEyaDQzNi4zMjdDNDg5LjcxOCA1MTIgNTEyIDQ5NS40OTYgNTEyIDQ3NS4xNFYzNi44NDFDNTEyIDE2LjUwNCA0OTUuODA5IDAgNDc0LjkxOSAwek0xOTUuMDQzIDE5NS4wNDNoNjguOTI4djM1LjEzNmguNzU1YzEwLjUwNS0xOC45NDUgNDEuNTQxLTM4LjE3NyA3OS45MjEtMzguMTc3IDczLjY1NSAwIDk0LjIxNCAzOS4xMDggOTQuMjE0IDExMS41Mzh2MTM1LjMyMWgtNzMuMTQ4VjMxNi44ODNjMC0zMi40MjctMTIuOTQ3LTYwLjg4My00My4yMjctNjAuODgzLTM2Ljc2OCAwLTU0LjI5NSAyNC44ODktNTQuMjk1IDY1Ljc1OHYxMTcuMTAzaC03My4xNDhWMTk1LjA0M3pNNzMuMTM5IDQzOC44NjFoNzMuMTQ4VjE5NS4wNDNINzMuMTM5djI0My44MTh6bTgyLjI4OS0zMjkuMTQ4YzAgMjUuMjU4LTIwLjQ1NyA0NS43MTUtNDUuNzE1IDQ1LjcxNS0yNS4yNTggMC00NS43MTUtMjAuNDU3LTQ1LjcxNS00NS43MTUgMC0yNS4yNTggMjAuNDU3LTQ1LjcxNSA0NS43MTUtNDUuNzE1IDI1LjI1OCAwIDQ1LjcxNSAyMC40NTcgNDUuNzE1IDQ1LjcxNXoiLz48L3N2Zz4=" />
            </a>
            <a href="mailto:h.bidiouane@gmail.com"
            title="Email">
                <img src="https://img.shields.io/badge/-4B006E.svg?logo=data:image/svg+xml;base64,//48AHMAdgBnACAAeABtAGwAbgBzAD0AIgBoAHQAdABwADoALwAvAHcAdwB3AC4AdwAzAC4AbwByAGcALwAyADAAMAAwAC8AcwB2AGcAIgAgAHMAaABhAHAAZQAtAHIAZQBuAGQAZQByAGkAbgBnAD0AIgBnAGUAbwBtAGUAdAByAGkAYwBQAHIAZQBjAGkAcwBpAG8AbgAiACAAdABlAHgAdAAtAHIAZQBuAGQAZQByAGkAbgBnAD0AIgBnAGUAbwBtAGUAdAByAGkAYwBQAHIAZQBjAGkAcwBpAG8AbgAiACAAaQBtAGEAZwBlAC0AcgBlAG4AZABlAHIAaQBuAGcAPQAiAG8AcAB0AGkAbQBpAHoAZQBRAHUAYQBsAGkAdAB5ACIAIABmAGkAbABsAC0AcgB1AGwAZQA9ACIAZQB2AGUAbgBvAGQAZAAiACAAYwBsAGkAcAAtAHIAdQBsAGUAPQAiAGUAdgBlAG4AbwBkAGQAIgAgAHYAaQBlAHcAQgBvAHgAPQAiADAAIAAwACAANQAxADIAIAAzADIANwAuADUAMwAiAD4APABwAGEAdABoACAAZgBpAGwAbAA9ACIAIwBmAGYAZgAiACAAZAA9ACIATQAyADUANAAuADQAMQAgADIANwA0AC4AOQA3AGwAMQAwADAALgA5ADMALQAxADAAMAAuADkAMgAgADEANQAzAC4ANAA5ACAAMQA1ADMALgA0ADgASAAwAGwAMQA1ADMALgA0ADkALQAxADUAMwAuADQAOAAgADEAMAAwAC4AOQAyACAAMQAwADAALgA5ADIAegBNADUALgA4ADQAIAAwAGwAMgA0ADgALgA1ADcAIAAyADQAOAAuADUANgBMADUAMAAyAC4AOQA4ACAAMABIADUALgA4ADQAegBNADAAIAAyADkAMAAuADMAbAAxADMAMQAuADcANwAtADEAMwAxAC4ANwA4AEwAMAAgADIANgAuADcANQBWADIAOQAwAC4AMwB6AG0ANQAxADIAIAAxADIALgA2ADEATAAzADYANwAuADYAMQAgADEANQA4AC4ANQAyACAANQAxADIAIAAxADQALgAxADQAdgAyADgAOAAuADcANwB6ACIALwA+ADwALwBzAHYAZwA+AA==" />
            </a>
      <br />
    </td>
  </tr>
</table>

</div>

---

## Maintainers
  
<div align="center">

  | GitHub Profile | Name                | Usernames |
  |:--------------:|---------------------|-----------|
  | <a href="https://github.com/kei077"><img src="https://avatars.githubusercontent.com/u/116844401?v=4" width="92px;" alt="kei077"></a> | Kawtar Taik        | <img src="https://github.com/user-attachments/assets/cd6bd36c-907c-49d2-a81b-5462c2e4142a" width="20" height="20" /> <a href="https://github.com/kei077"><img src="https://img.shields.io/badge/@kei077-8A2BE2?style=flat"/></a> |
  | <a href="https://github.com/sch0penheimer"><img src="https://avatars.githubusercontent.com/u/109217308?s=400&u=2524c131a291962f93f92fecb48653a04b0621dc&v=4" width="92px;" alt="sch0penheimer"></a> | Haitam Bidiouane   | <img src="https://github.com/user-attachments/assets/cd6bd36c-907c-49d2-a81b-5462c2e4142a" width="20" height="20" /> <a href="https://github.com/sch0penheimer"><img src="https://img.shields.io/badge/@sch0penheimer-8A2BE2?style=flat"/></a> |
 
</div>

---

## Contributors

<table>
<tr>
<td align="center">
  <a href="https://github.com/kei077">
    <img src="https://avatars.githubusercontent.com/u/116844401?v=4" width="60px;" alt="Kawtar Taik"/>
  </a>
  <br>
  <a href="https://github.com/kei077" title="GitHub">
    <img src="https://img.shields.io/badge/kei077-6b0e96?style=flat-square&logo=github&logoColor=white" />
  </a>
</td>
<td align="center">
  <a href="https://github.com/sch0penheimer">
    <img src="https://avatars.githubusercontent.com/u/109217308?s=400&u=2524c131a291962f93f92fecb48653a04b0621dc&v=4" width="60px;" alt="Haitam Bidiouane"/>
  </a>
  <br>
  <a href="https://github.com/sch0penheimer" title="GitHub">
    <img src="https://img.shields.io/badge/sch0penheimer-6b0e96?style=flat-square&logo=github&logoColor=white" />
  </a>
</td>
<td align="center">
  <a href="https://github.com/FlawlessAw">
    <img src="https://avatars.githubusercontent.com/u/222703336?v=4" width="60px;" alt="FlawlessAw"/>
  </a>
  <br>
  <a href="https://github.com/FlawlessAw" title="GitHub">
    <img src="https://img.shields.io/badge/FlawlessAw-6b0e96?style=flat-square&logo=github&logoColor=white" />
  </a>
</td>
<td align="center">
  <a href="https://github.com/Pouzani">
    <img src="https://avatars.githubusercontent.com/u/95249359?v=4" width="60px;" alt="Pouzani"/>
  </a>
  <br>
  <a href="https://github.com/Pouzani" title="GitHub">
    <img src="https://img.shields.io/badge/Pouzani-6b0e96?style=flat-square&logo=github&logoColor=white" />
  </a>
</td>
<td align="center">
  <a href="https://github.com/shady0503">
    <img src="https://avatars.githubusercontent.com/u/148286658?v=4" width="60px;" alt="shady0503"/>
  </a>
  <br>
  <a href="https://github.com/shady0503" title="GitHub">
    <img src="https://img.shields.io/badge/shady0503-6b0e96?style=flat-square&logo=github&logoColor=white" />
  </a>
</td>
<td align="center">
  <a href="https://github.com/MossabArektout">
    <img src="https://avatars.githubusercontent.com/u/115503659?v=4" width="60px;" alt="shady0503"/>
  </a>
  <br>
  <a href="https://github.com/MossabArektout" title="GitHub">
    <img src="https://img.shields.io/badge/MossabArektout-6b0e96?style=flat-square&logo=github&logoColor=white" />
  </a>
</td>
<td align="center">
  <a href="https://github.com/youssefbouraoui1">
    <img src="https://avatars.githubusercontent.com/u/155109081?v=4" width="60px;" alt="youssefbouraoui1"/>
  </a>
  <br>
  <a href="https://github.com/youssefbouraoui1" title="GitHub">
    <img src="https://img.shields.io/badge/youssefbouraoui1-6b0e96?style=flat-square&logo=github&logoColor=white" />
  </a>
</td>
</tr>
</table>

## Table of Contents

1. [Architecture and Design](#1-architecture-and-design)
    - [System Architecture](#11-system-architecture)
    - [Component Structure](#12-component-structure)
    - [System Libraries](#13-system-libraries)
2. [CLI and Client Documentation](#2-cli-and-client-documentation)
    - [Supported Commands](#21-supported-commands)
    - [Example Session](#22-example-session)
3. [Core Components](#4-core-components)
     - [Server Implementation](#41-server-implementation)
     - [Client Implementation](#42-client-implementation)
     - [Protocol Parser](#43-protocol-parser)
     - [RESP Parser](#44-resp-parser)
4. [Data Structures and Utilities](#5-data-structures-and-utilities)
     - [Hash Table Implementation](#51-hash-table-implementation)
     - [Linked List Implementation](#52-linked-list-implementation)
     - [Logging System](#53-logging-system)
     - [Project Branding](#54-project-branding)
5. [Testing Framework](#6-testing-framework)
     - [Test Infrastructure](#61-test-infrastructure)
     - [Unit Tests](#62-unit-tests)
     - [Integration Tests](#63-integration-tests)
6. [Build System and Development](#7-build-system-and-development)
     - [Makefile Configuration](#71-makefile-configuration)
     - [Build Script](#72-build-script)
     - [Development Workflow](#73-development-workflow)
7. [Network Protocol Implementation](#8-network-protocol-implementation)
     - [RESP Protocol Support](#81-resp-protocol-support)
     - [Client-Server Communication](#82-client-server-communication)
8. [Memory Management](#9-memory-management)


## 1. Architecture and Design

### 1.1 System Architecture

|![MemoraDB's Architecture](https://github.com/user-attachments/assets/7e5181d7-f350-4518-95f1-a348927b68ef)|
|:----------------------------------:|
|MemoraDB's System Architecture|

MemoraDB’s architecture is modular and layered, emphasizing clarity, maintainability, and extensibility. Below is a detailed breakdown of its core layers and components:

#### 1. Utilities Layer
- **Build System**: Automated with Makefile and build scripts for seamless compilation and setup.
- **Testing Framework**: Comprehensive unit tests (hash tables, lists) and integration tests (protocol parsing, system behavior) ensure reliability for every developed feature.

#### 2. Client Layer
- **CLI Client**: Provides a command-line interface for direct user interaction. It's the official MemoraDB Client (Only a C-Client for now).
- **RESP Client**: Any contemporary Redis client implememting RESP3.

#### 3. Server Layer
- **Memory Storage Layer**
  - **HashTable Storage**: In-memory key-value store optimized for fast lookups and updates.
  - **List Utilities**: Supports advanced data types and operations (e.g., linked lists for queues/stacks).
- **Command Processing Layer**
  - **Command Router**: Directs incoming commands to appropriate handlers.
  - **Command Handlers**: Execute logic for supported commands (GET, SET, LIST operations, etc.).
- **Core Parser Layer**
  - **Protocol Parser**: Decodes incoming RESP  messages.
  - **Token Parser & Type Detector**: Tokenizes commands and determines types for accurate execution.
- **Network Protocol Layer**
  - **TCP Server Socket**: Manages network communication using POSIX-compliant sockets.
  - **Connection Manager**: Handles client connections, typically with a thread-per-client model for concurrency.
  - **RESP Serializer**: Encodes responses in RESP format for protocol compliance.

#### Design Principles
- **Layered Separation**: Each layer has a distinct responsibility, reducing coupling and simplifying testing and extension.
- **Protocol Compliance**: Full RESP support ensures interoperability with all Redis tools / clients.
- **Thread-per-Client Model**: Enables concurrent client handling for scalability.

MemoraDB is architected for robustness and extensibility, with clear separation between storage, protocol handling, command processing, and utilities.

### 1.2 Component Structure

The project is organized into logical modules that encapsulate specific functionality:

```
src/
├── client/          ##-- MemoraDB Testing Client --##
├── server/          ##-- MemoraDB TCP Server --##
├── parser/          ##-- Core RESP3 parsing logic --##
└── utils/           ##-- Utility functions & data structures --##
```

Each module maintains clear interfaces and minimal dependencies, facilitating independent development and testing of individual components.

### 1.3 System Libraries

MemoraDB, and all of its core features: client, server, protocol handling, and testing infrastructure are implemented in standard C using only system libraries and POSIX APIs.

#### 1.3.1 Build and System Requirements:

- **Compiler:** GCC (or any modern C compiler supporting C99/C11)
- **Build System:** GNU Make (Makefile) and standard Bash (`build.sh`)
- **Supported OS:** Any POSIX-compliant system (Linux, BSD, macOS, etc.)

#### 1.3.2 System Libraries Used:

- `<stdio.h>` : Standard input/output
- `<stdlib.h>` : Memory allocation, process control
- `<string.h>` : String handling
- `<unistd.h>` : POSIX system calls and threading
- `<time.h>` : Time/date functions
- `<stdarg.h>` : Variadic arguments

#### 1.3.3 External Linker Flags:
- `-lpthread` — Used for multi-threading (required for thread-per-client server model)

#### 1.3.4 Third-Party Libraries:
- ***None***

MemoraDB is a standalone C project: no external packages, no third-party frameworks, no dynamic libraries, and no package managers required. All dependencies are either standard system libraries or POSIX APIs, guaranteeing straightforward compilation and deployment in embedded, server, and desktop environments.

---


## 2. CLI and Client Documentation

### 2.1 Supported Commands

MemoraDB (as of `August 12th, 2025`) supports a broad initial set of commands, all parsed and executed according to the RESP3 protocol. This guarantees compatibility with Redis clients and libraries.

| Command                                   | Arguments / Options                                           | Description                                     | RESP3 Type            |
|-------------------------------------------|---------------------------------------------------------------|-------------------------------------------------|-----------------------|
| `PING`                                    | none                                                          | Health check; returns `PONG`                    | Simple String         |
| `ECHO <message>`                          | message:string                                                | Echoes the input string                         | Bulk/String           |
| `SET <key> <value> [PX <milliseconds>]`   | key:string, value:string, PX:optional TTL in ms               | Sets key to value, with optional expiry         | Simple String         |
| `GET <key>`                               | key:string                                                    | Retrieves value for key                         | Bulk/String or Null   |
| `DEL <key> [key ...]`                     | one or more keys                                              | Deletes keys (string or list)                   | Integer (deleted cnt) |
| `RPUSH <list> <value> [value ...]`        | list:string, one or more values                               | Appends value(s) to end of list                 | Integer (length)      |
| `LPUSH <list> <value> [value ...]`        | list:string, one or more values                               | Prepends value(s) to start of list              | Integer (length)      |
| `LRANGE <list> <start> <end>`             | list:string, start:int, end:int (can be negative)             | Returns list elements in interval               | Array                 |
| `LLEN <list>`                             | list:string                                                    | Returns length of list                          | Integer               |
| `LPOP <list> [count]`                     | list:string, optional count:int                                | Pops 1 or N elements from head                  | Bulk/String or Array  |
| `BLPOP <list> <timeout>`                  | list:string, timeout:seconds (0 means block indefinitely)      | Blocking pop of 1 element from head             | Array or Null Bulk    |

Notes:
- SET with PX: expiry in milliseconds; expired keys are treated as nonexistent by GET.
- LPOP with a count returns an array of popped elements; single-arg LPOP returns a single bulk string or Null.
- BLPOP returns an array of two bulk strings: [list, element] when successful; returns Null Bulk on timeout. A timeout of 0 blocks indefinitely.

> [!IMPORTANT]
> The above table reflects all commands currently implemented, MemoraDB is still in ***Development*** mode, and will cover a much wider range of possible commands on release.


### 2.2 Example Session

The following example demonstrates the basic functionality available in MemoraDB through a typical client session:

```
$ ./client
Connected to MemoraDB server...

MemoraDB> PING
+PONG

MemoraDB> ECHO "Hello, MemoraDB!"
$16
Hello, MemoraDB!

MemoraDB> PING
+PONG

MemoraDB> ECHO "Testing protocol compliance"
$27
Testing protocol compliance

MemoraDB> SET mykey myvalue
+OK

MemoraDB> GET mykey
$7
myvalue

MemoraDB> RPUSH mylist first
:1

MemoraDB> LRANGE mylist 0 -1
*1
$5
first

MemoraDB> DEL mykey
:1

MemoraDB> exit
Connection closed.
```

The session demonstrates some of the core commands currently available in MemoraDB. All responses follow the RESP protocol format, ensuring compatibility with standard Redis clients. The PING command provides a simple health check mechanism, while ECHO allows for testing string transmission and protocol compliance.

---

## 4. Core Components

### 4.1 Server Implementation

The server component (server) implements the core database server functionality, including connection management, request processing, and response generation. The server is designed to handle multiple client connections efficiently while maintaining data consistency.

**Key Responsibilities:**
- TCP socket management and client connection handling
- Request routing and command processing
- Session management and client state tracking
- Error handling and graceful shutdown procedures

The server implementation provides the foundation for the database system, managing all aspects of client communication and ensuring that requests are processed correctly and responses are delivered reliably.

### 4.2 Client Implementation

The client component (client) serves dual purposes: it provides a client library for connecting to MemoraDB servers and serves as a testing tool for validating server functionality. The client implementation demonstrates proper usage patterns and provides reference code for integrating with MemoraDB.

**Features:**
- Connection establishment and management
- Command serialization and transmission
- Response parsing and handling
- Error recovery and connection pooling

The client implementation follows the same architectural principles as the server, ensuring consistency in code quality and design patterns throughout the project.

### 4.3 Protocol Parser

The protocol parser (parser) implements the core parsing logic for the RESP protocol. This component handles the low-level details of protocol parsing, including tokenization, type detection, and data extraction.

The parser is designed to be stateless and reentrant, allowing it to be used safely in different contexts throughout the application. It handles all aspects of RESP protocol compliance, including proper error detection and recovery.

### 4.4 RESP Parser

The RESP parser (resp_parser.c) provides specialized functionality for parsing and serializing RESP protocol messages. This component works in conjunction with the core parser to provide complete protocol support.

**Capabilities:**
- Parsing of all RESP data types (simple strings, errors, integers, bulk strings, arrays)
- Incremental parsing for streaming data
- Efficient serialization of response data
- Comprehensive error handling and validation

The RESP parser ensures full compatibility with Redis clients by implementing the protocol specification exactly as defined in the Redis documentation.

## 5. Data Structures and Utilities

### 5.1 Hash Table Implementation

The hash table implementation (hashTable.c) provides the primary storage mechanism for key-value pairs in MemoraDB. The implementation is optimized for performance and memory efficiency while providing the flexibility needed for database operations.

**Technical Specifications:**
- Separate chaining collision resolution for predictable performance
- Dynamic resizing based on load factor to maintain efficiency
- Optimized hash function for string keys
- Memory-efficient bucket management

The hash table automatically adjusts its size based on usage patterns, ensuring that performance remains consistent as the dataset grows or shrinks. The implementation includes comprehensive error handling and memory management to prevent leaks and ensure reliability.

### 5.2 Linked List Implementation

The linked list implementation (list.c) provides a fundamental data structure used throughout the codebase for managing dynamic collections. The implementation supports various list operations required by the database system.

**Features:**
- Efficient insertion and deletion operations
- Support for both forward and backward traversal
- Memory-efficient node management
- Generic data storage capabilities

The linked list serves as a building block for more complex data structures and provides the flexibility needed for implementing various database features.

### 5.3 Logging System

The logging system (log.c) provides comprehensive logging capabilities for debugging, monitoring, and troubleshooting. The system includes multiple log levels and formatting options to support different operational requirements.

**Logging Capabilities:**
- Multiple log levels (DEBUG, INFO, WARNING, ERROR)
- Configurable output destinations
- Timestamp and context information
- Performance-optimized logging operations

The logging system is designed to have minimal impact on performance while providing the information needed for effective system monitoring and debugging.

### 5.4 Project Branding

The project branding component (logo.c) provides visual identity elements for the MemoraDB project, including ASCII art logos and branding information that appears in application output.

This component demonstrates attention to user experience details and provides a professional appearance for the database system during startup and operation.

## 6. Testing Framework

### 6.1 Test Infrastructure

The testing framework (test_framework.c) provides a comprehensive infrastructure for validating all aspects of the MemoraDB system. The framework includes assertion macros, test execution management, and result reporting capabilities.

**Framework Features:**
- Assertion macros for various test conditions
- Test execution tracking and reporting
- Colored output for improved readability
- Summary reporting across all test suites

The test framework is designed to be simple to use while providing comprehensive coverage of system functionality.

### 6.2 Unit Tests

The project includes comprehensive unit tests for all major components:

**Hash Table Tests** (test_hashtable.c): Validates hash table operations including insertion, deletion, lookup, and resizing functionality.

**List Tests** (test_list.c): Tests linked list operations and ensures proper memory management and data integrity.

**Parser Tests** (test_parser.c): Validates RESP protocol parsing for all supported data types and error conditions.

Each unit test suite focuses on a specific component and provides thorough coverage of normal operations, edge cases, and error conditions.

### 6.3 Integration Tests

**Integration Test Suite** (integration_test.c): Tests the interaction between different system components to ensure proper system-level functionality.

**Ping-Echo Tests** (test_ping_echo.c): Validates client-server communication patterns and protocol compliance through basic command testing.

Integration tests ensure that the individual components work together correctly and that the system behaves properly under realistic usage scenarios.

## 7. Build System and Development

### 7.1 Makefile Configuration

The project uses a sophisticated Makefile that manages compilation of all components and provides convenient targets for development and testing operations.

**Build Targets:**
- `all`: Compiles both client and server executables
- `test`: Compiles all test files with appropriate dependencies
- `run-tests`: Executes the complete test suite with formatted output
- `headers`: Refreshes author/date header metadata in C/C headers via build.sh
- `clean`: Removes all generated binaries and temporary files

The Makefile includes intelligent dependency management and handles the different compilation requirements for various components.

### 7.2 Build Script

The build script (build.sh) provides automated project management capabilities including header updates, author tracking, and optional compilation. The script integrates with Git to track changes and maintain project metadata.

**Script Capabilities:**
- Automatic detection of modified files
- Header information updates with author and date tracking
- Interactive compilation prompting
- Git integration for project management

### 7.3 Development Workflow

The development environment is configured for efficient development with proper tooling and automation. The workflow includes:

**Code Organization**: Clear module separation with well-defined interfaces
**Testing Integration**: Automated test execution as part of the build process
**Documentation**: Comprehensive inline documentation and external documentation
**Version Control**: Git-based workflow with proper branching and tagging

## 8. Network Protocol Implementation

### 8.1 RESP Protocol Support

MemoraDB implements the complete RESP (Redis Serialization Protocol) specification, ensuring full compatibility with Redis clients. The implementation handles all RESP data types and maintains strict protocol compliance.

**Supported RESP Types:**
- Simple Strings: For basic text responses
- Errors: For error condition reporting
- Integers: For numeric responses
- Bulk Strings: For binary-safe string data
- Arrays: For complex multi-element responses

The protocol implementation includes proper handling of edge cases, error conditions, and streaming data scenarios.

### 8.2 Client-Server Communication

The communication system supports efficient client-server interaction through optimized network handling and protocol processing. The implementation includes:

**Connection Management**: Efficient handling of multiple concurrent connections
**Request Processing**: Optimized parsing and execution of client requests
**Response Generation**: Fast serialization and transmission of responses
**Error Handling**: Comprehensive error detection and recovery mechanisms

## 9. Memory Management

MemoraDB employs careful memory management strategies to ensure reliable operation and prevent memory leaks. The system includes:

**Allocation Tracking**: All memory allocations are tracked and properly freed
**Object Lifecycle Management**: Clear ownership and lifecycle rules for all objects
**Error Recovery**: Proper cleanup in error conditions to prevent resource leaks
**Performance Optimization**: Memory allocation patterns optimized for performance

The memory management approach ensures that the system remains stable and efficient even under heavy load conditions.

## 10. Contributing, Issues, and Pull Requests

We welcome contributions to MemoraDB! If you'd like to help improve the project, please review the following guidelines:

### 10.1 Pull Requests (PRs)

- Before submitting any PR, make sure it follows the conventions outlined in the project's [**CONTRIBUTIONS TEMPLATE**](./CONTRIBUTING.md) and [**PR TEMPLATE**](.github/PR_TEMPLATE.md).
- The maintainers will review your PR, provide feedback, and merge once approved.

### 10.2 Issues

- **Issuing Guidelines:** When opening an issue, please ensure it follows the project's **ISSUE TEMPLATE** (`.github/ISSUE_TEMPLATE`) for consistency and clarity.
- **Bug Reports:** Refer to the [Bug Report Template](.github/ISSUE_TEMPLATE/bug_report.md).
- **Feature Requests:** Refer to the [Feature Request Template](.github/ISSUE_TEMPLATE/feature_request.md).
- **Questions & Discussions:** Use the [Discussions](https://github.com/The-HaiKaw-Pr0tocol/MemoraDB/discussions) tab for general questions, design proposals, or technical Q&A.

Your feedback and contributions help make MemoraDB better—thank you for participating!

> © 2025 The HaiKaw Protocol. All rights reserved.
