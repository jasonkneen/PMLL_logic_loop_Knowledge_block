#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "memory_silo.h"
#include "PMLL_ARLL_EFLL.h"
#include "knowledge.h"

#define MAX_KNOWLEDGE_NODES 4096
#define SESSION_TIMEOUT_MS 5000
#define ENCRYPTION_KEY_SIZE 2048

typedef struct io_socket_t io_socket_t; // Forward declaration
typedef struct Graph Graph; // Forward declaration
struct RSA; // Forward declaration, assuming RSA is defined elsewhere

typedef struct {
    io_socket_t io_socket;
    PMLL_ARLL_EFLL_State state;
    Graph *knowledge_graph;
    struct RSA *encryption_key;
    memory_silo_t *persistent_silo;
} SessionContext;

// Function to save the state of the memory silo
void save_memory_silo(const char *filename, memory_silo_t *memory_silo);

// Function to load the state of the memory silo
memory_silo_t *load_memory_silo(const char *filename);

// Function to save session state with encryption
void save_session_state(SessionContext *ctx, const char *filename);

// Function to load session state with decryption
void load_session_state(SessionContext *ctx, const char *filename);

// Helper function to calculate checksum
unsigned long calculate_checksum(const unsigned char *data, size_t size);

// Helper function to write file atomically
int write_file_atomically(const char *filename, const unsigned char *data, size_t size);

#endif // PERSISTENCE_H
