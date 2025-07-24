/**
 * =====================================================
 * MemoraDB - In-Memory Database System
 * =====================================================
 * 
 * File                      : src/utils/logo.c
 * Module                    : Logo Display Utilities
 * Last Updating Author      : Weasel
 * Last Update               : 07/24/2025
 * Version                   : 1.0.0
 * 
 * Description:
 *  ASCII art logo for MemoraDB client interface.
 * 
 * 
 * Copyright (c) 2025 MemoraDB Project
 * =====================================================
 */

#include "logo.h"

void display_memoradb_logo() {
    printf("\033[1;35m");
    printf("\n\n");
    printf("███╗   ███╗ ███████╗ ███╗   ███╗  ██████╗  ██████╗   █████╗     ██████╗ ██████╗ \n");
    printf("████╗ ████║ ██╔════╝ ████╗ ████║ ██╔═══██╗ ██╔══██╗ ██╔══██╗    ██╔══██╗██╔══██╗\n");
    printf("██╔████╔██║ █████╗   ██╔████╔██║ ██║   ██║ ██████╔╝ ███████║    ██║  ██║██████╔╝\n");
    printf("██║╚██╔╝██║ ██╔══╝   ██║╚██╔╝██║ ██║   ██║ ██╔══██╗ ██╔══██║    ██║  ██║██╔══██╗\n");
    printf("██║ ╚═╝ ██║ ███████╗ ██║ ╚═╝ ██║ ╚██████╔╝ ██║  ██║ ██║  ██║    ██████╔╝██████╔╝\n");
    printf("╚═╝     ╚═╝ ╚══════╝ ╚═╝     ╚═╝  ╚═════╝  ╚═╝  ╚═╝ ╚═╝  ╚═╝    ╚═════╝ ╚═════╝ \n\n");
    printf("\n");
    printf("\033[0m"); 
    printf("\033[1;35m"); // Purple (bold)
    printf("╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                     Lightning-Fast In-Memory Database                    ║\n");
    printf("║__________________________________________________________________________║\n");
    printf("║                                                                          ║\n");
    printf("║ Authors:                                                                 ║\n");
    printf("║   @kei077 (Kawtar Taik)                                                  ║\n");
    printf("║   @sch0penheimer (Haitam Bidiouane)                                      ║\n");
    printf("║                                                                          ║\n");
    printf("║                        ▶ Running on port 6379 ◀                          ║\n");
    printf("╚══════════════════════════════════════════════════════════════════════════╝\n");
    printf("\033[0m");
    printf("\n");
}
