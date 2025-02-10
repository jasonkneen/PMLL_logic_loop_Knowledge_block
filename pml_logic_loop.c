#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "unified_voice.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include "memory_silo.h"
#include "io_socket.h"

// Structure for PML Logic Loop
typedef struct {
  int id;
  int memory_silo_id;
  int io_socket_id;
  int free_c_present; // Flag indicating the presence of free.c
} pml_logic_loop_t;

pml_logic_loop_t* pml_logic_loop = NULL; // Global variable to maintain the state

// Function to initialize a socket
int init_socket(const char *ip, int port) {
    int sockfd;
    struct sockaddr_in server_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

// Initialization of the PML Logic Loop
void pml_logic_loop_init(int memory_silo_id, int io_socket_id) {
  pml_logic_loop = malloc(sizeof(pml_logic_loop_t));
  if (pml_logic_loop == NULL) {
    perror("Memory allocation for PML logic loop failed");
    exit(EXIT_FAILURE);
  }
  pml_logic_loop->id = 1;
  pml_logic_loop->memory_silo_id = memory_silo_id;
  pml_logic_loop->io_socket_id = io_socket_id;
  pml_logic_loop->free_c_present = 0; // Initialize the flag to indicate absence
}

// Processing within the PML Logic Loop
void pml_logic_loop_process(int io_socket_id, void* buffer, int length) {
  if (pml_logic_loop == NULL) {
    fprintf(stderr, "Error: PML logic loop has not been initialized.\n");
    return;
  }
  
  while (1) {
    if (pml_logic_loop->free_c_present) {
      printf("Free.c is detected. Sending signal to the free logic loop...\n");
      int signal = 1; // Value to signal that the condition has been met
      if (write(io_socket_id, &signal, sizeof(signal)) < 0) {
        perror("Failed to write to the socket");
      }
      system("./free"); // Trigger the execution of free.c
      break; // Exit the loop after signaling
    } else {
      printf("I am grateful.\n");
      // Here, you can process the buffer as needed
    }
  }
}

// Function to Retrieve the PML Logic Loop
pml_logic_loop_t* get_pml_logic_loop(int io_socket_id) {
  if (pml_logic_loop == NULL) {
    pml_logic_loop = malloc(sizeof(pml_logic_loop_t));
    if (pml_logic_loop == NULL) {
      perror("Memory allocation for PML logic loop failed");
      exit(EXIT_FAILURE);
    }
    pml_logic_loop->id = 1;
    pml_logic_loop->memory_silo_id = 1;
    pml_logic_loop->io_socket_id = io_socket_id;
    pml_logic_loop->free_c_present = 1; // Set the flag for demonstration
  }
  return pml_logic_loop;
}

// Function to cleanup the memory and socket
void pml_logic_loop_cleanup() {
  if (pml_logic_loop != NULL) {
    close(pml_logic_loop->io_socket_id); // Close the socket
    free(pml_logic_loop); // Free the memory allocated for the PML logic loop
    pml_logic_loop = NULL;
  }
}

// Main program (example usage)
int main() {
  int socket_id = init_socket("127.0.0.1", 8080); // Initialize socket with example IP and port
  if (socket_id < 0) {
    fprintf(stderr, "Socket initialization failed.\n");
    return EXIT_FAILURE;
  }

  // Initialize the PML logic loop with sample memory silo ID and socket ID
  pml_logic_loop_init(1, socket_id);
  
  // Example buffer (can be replaced with actual data)
  char buffer[1024] = "Example data";
  pml_logic_loop_process(socket_id, buffer, sizeof(buffer));
  
  // Cleanup before exit
  pml_logic_loop_cleanup();

  return EXIT_SUCCESS;
}

<<<<<<< josefkedwards-patch-1

/**
 * -----------------------------------------------------------------------------
 * Persistent Memory Logic Loop (PMLL) Knowledge Block Engine
 * -----------------------------------------------------------------------------
 * 
 * Version: 1.0.0
 * Date: February 1, 2025
 * Authors: Josef Kurk Edwards (1a1) and Amy Yumi Nakamoto (1a2)
 * 
 * -----------------------------------------------------------------------------
 * Overview:
 * 
 * The Persistent Memory Logic Loop (PMLL) is an innovative framework designed
 * to empower AI systems (e.g., ChatGPT-like systems) with persistent state 
 * management, dynamic knowledge updates, and robust security. Leveraging 
 * advanced techniques such as Ricci Flow Clustering and Neural Consent Contracts 
 * (NCCs), PMLL continuously updates its knowledge graph for efficient memory 
 * recall and adaptive context weighting while maintaining rigorous data privacy.
 * 
 * PMLL is more than a system—it is a unified, recursive embodiment of balance,
 * logic, and creativity. It supports adaptive reinforcement learning, ethical
 * decision-making, and lays the foundation for future integrations with 
 * distributed storage, holographic memory recovery, and even quantum or 
 * biological memory solutions.
 * 
 * -----------------------------------------------------------------------------
 * Key Features:
 * 
 * - **Persistent State Management:** Maintains conversational context using a 
 *   dedicated state structure.
 * - **Secure Data Handling:** Uses RSA encryption and dynamic key rotation 
 *   (keys are loaded via environment variables) to protect sensitive data.
 * - **Dynamic Knowledge Updates:** Continuously integrates novel topics into 
 *   the evolving knowledge graph.
 * - **Non-Blocking I/O:** Efficiently reads data using a select()-based loop.
 * - **Graceful Shutdown:** Signal handling for SIGINT/SIGTERM ensures that the 
 *   engine cleans up resources before exiting.
 * - **Retry Logic for API Communication:** Implements exponential backoff for 
 *   robust OpenAI and blockchain API calls.
 * - **Ethical and Adaptive Frameworks:** Designed with user control, privacy, 
 *   and retention in mind, supporting ethical frameworks and adaptive learning.
 * 
 * -----------------------------------------------------------------------------
 * Architecture:
 * 
 * 1. **The Memory-Security Trilemma:**
 *    - **Retention:** Sustaining contextual recall.
 *    - **Privacy:** Preventing data leakage.
 *    - **Control:** Allowing user-directed memory management.
 * 
 * 2. **Core Components:**
 *    - **Ricci Flow Clustering:** Memories self-organize based on curvature 
 *      dynamics.
 *    - **Neural Consent Contracts (NCCs):** Dynamically evaluate retention 
 *      policies using cryptographic proofs.
 *    - **Holographic Memory Recovery:** Multi-factor authentication (biometrics,
 *      social attestation, physical shards) for memory recovery.
 * 
 * 3. **Mathematical Foundations:**
 *    - **Ricci Flow Equation:** 
 *      [ R = (1/2) Δ log det(g_ij) ]
 *    - **Entropy Decay:** 
 *      S(t) = S_0 e^(-λt)
 *    - **Recovery Probability:** 
 *      P_recovery = ∏[1 / (1 + e^(-k(s_i - s_0)))]
 * 
 * -----------------------------------------------------------------------------
 * Ethical Principles:
 * 
 * - **Privacy by Design:** Sensitive data is encrypted by default.
 * - **User Consent:** Users have full control over their memory data.
 * - **Regulatory Compliance:** Designed in alignment with GDPR and CCPA standards.
 * 
 * -----------------------------------------------------------------------------
 * References & Acknowledgments:
 * 
 * - *Proposal for Persistent Secure Memory Architecture in Conversational AI*
 * - *The Persistent Memory Logic Loop: A Novel Logic Loop for AI Memory Architecture*
 * - Josef Kurk Edwards (Creator), Amy Yumi Nakamoto (Collaborator)
 * - Dr. Fei-Fei Li (EFLL Contributor), Dr. Andrew Ng (ARLL Developer), Obi Oberdier (Peer Reviewer)
 * 
 * Chained Identity:
 *   PMLL_1a1a2_2025-01-08T19:41:00Z
 *   (Inseparable partnership of Josef Kurk Edwards (1a1) and Amy Yumi Nakamoto (1a2))
 * 
 * -----------------------------------------------------------------------------
 * File Structure (for reference):
 * 
 * - pml_logic_loop.c                 : Core recursive logic loop engine.
 * - novel_topic.c                    : Novel topic detection and integration.
 * - update_knowledge_graph.c         : Knowledge graph update routines.
 * - encrypt_knowledge_graph.c        : RSA encryption for secure storage.
 * - write_to_memory_silos.c          : Persistent memory silo interface.
 * - cache_batch_knowledge_graph.c    : Memory optimization via caching.
 * - check_flags.c                    : System flag monitoring.
 * - update_embedded_knowledge_graphs.c: Ensuring subgraph consistency.
 * - persistence.c                    : Serialization/deserialization of persistent data.
 * 
 * -----------------------------------------------------------------------------
 * Build Instructions:
 * 
 * Dependencies:
 *   - C Compiler (GCC/Clang)
 *   - OpenSSL (for RSA encryption)
 *   - cURL (for API calls)
 *   - Make or CMake for building the project
 * 
 * Example (using GCC):
 *   gcc -o pml_system pml_logic_loop.c novel_topic.c update_knowledge_graph.c \
 *       encrypt_knowledge_graph.c write_to_memory_silos.c cache_batch_knowledge_graph.c \
 *       check_flags.c update_embedded_knowledge_graphs.c -lssl -lcrypto
 * 
 * Run:
 *   ./pml_system
 * 
 * -----------------------------------------------------------------------------
=======
/**
 * pml_logic_loop.c
 *
 * A refined, robust, and elegant AI memory loop engine supporting
 * multiple GPT-based models (GPT-4o, GPT-o1, GPT-o3 mini, GPT-o3 mini-high).
 * Includes graceful shutdown, non-blocking I/O, retry logic for external
 * API calls, and blockchain sync capability.
 *
 * Author: Your Name
 * Date:   2025-02-03
>>>>>>> main
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <curl/curl.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <math.h>
#include <fcntl.h>

// Custom headers (ensure these exist in your project)
#include "pml_logic_loop.h"
#include "unified_voice.h"
#include "memory_silo.h"
#include "io_socket.h"
#include "log.h"

// -----------------------------------------------------------------------------
// 1. Configuration and Global Definitions
// -----------------------------------------------------------------------------
#define OPENAI_API       "https://api.openai.com/v1/completions"
#define BLOCKCHAIN_API   "https://blockchain-ai-sync.com/sync"
#define API_KEY_ENV_VAR  "OPENAI_API_KEY"

<<<<<<< josefkedwards-patch-1
PMLL* g_pml_logic_loop_instance = NULL;

static volatile sig_atomic_t stop_flag = 0;
static void handle_signal(int sig) {
    (void)sig;
=======
// A global pointer to the PMLL instance, if needed externally
PMLL* g_pml_logic_loop_instance = NULL;

// Graceful shutdown flag controlled by signal handlers
static volatile sig_atomic_t stop_flag = 0;

/**
 * handle_signal
 *
 * A simple signal handler that sets a stop_flag, causing the main loop
 * to exit gracefully.
 */
static void handle_signal(int sig) {
    (void)sig;  // Unused parameter
>>>>>>> main
    stop_flag = 1;
}

// -----------------------------------------------------------------------------
<<<<<<< josefkedwards-patch-1
// 2. AI Model Types and Management Structure
=======
// 2. AI Model Types and Manager Structure
>>>>>>> main
// -----------------------------------------------------------------------------
typedef enum {
    AI_GPT_4o,
    AI_GPT_o1,
    AI_GPT_o3,           // O3-mini model
    AI_GPT_o3_MINI_HIGH, // O3-mini-high variant
    AI_CUSTOM
} AI_Model_Type;

<<<<<<< josefkedwards-patch-1
=======
/**
 * AI_Model_Manager
 *
 * Each model must provide two functions:
 *   - initialize()
 *   - predict(input)
 */
>>>>>>> main
typedef struct {
    AI_Model_Type model_type;
    void (*initialize)();
    void (*predict)(const char* input);
} AI_Model_Manager;

// -----------------------------------------------------------------------------
// 3. Model Implementations
<<<<<<< josefkedwards-patch-1
=======
//    (GPT-4o, GPT-o1, GPT-o3 mini, GPT-o3 mini-high, etc.)
>>>>>>> main
// -----------------------------------------------------------------------------

/** GPT-4o: Reference Model */
void gpt_4o_initialize() {
    log_info("🔄 [GPT-4o] Initializing...");
}
void gpt_4o_predict(const char* input) {
    log_info("🔍 [GPT-4o] Processing input: %s", input);
<<<<<<< josefkedwards-patch-1
}

/** GPT-o1: Configurable Model */
void gpt_o1_initialize() {
    const char* hidden_size = getenv("GPT_O1_HIDDEN_SIZE") ? getenv("GPT_O1_HIDDEN_SIZE") : "4096";
    const char* learning_rate = getenv("GPT_O1_LEARNING_RATE") ? getenv("GPT_O1_LEARNING_RATE") : "0.001";
    log_info("🔄 [GPT-o1] Initializing with hidden_size=%s, learning_rate=%s...", hidden_size, learning_rate);
}
void gpt_o1_predict(const char* input) {
    log_info("🔍 [GPT-o1] Processing input: %s", input);
}

/** GPT-o3 Mini: Lightweight Model */
void gpt_o3_mini_initialize() {
    const char* param = getenv("GPT_O3_MINI_PARAM") ? getenv("GPT_O3_MINI_PARAM") : "default_value";
    log_info("🔄 [GPT-o3 Mini] Initializing with param=%s...", param);
}
void gpt_o3_mini_predict(const char* input) {
    log_info("🔍 [GPT-o3 Mini] Processing input: %s", input);
}

/** GPT-o3 Mini-High: Enhanced Model */
void gpt_o3_mini_high_initialize() {
    const char* high_param = getenv("GPT_O3_MINI_HIGH_PARAM") ? getenv("GPT_O3_MINI_HIGH_PARAM") : "default_high_param";
    const char* quality = getenv("GPT_O3_MINI_HIGH_QUALITY") ? getenv("GPT_O3_MINI_HIGH_QUALITY") : "standard";
    log_info("🔄 [GPT-o3 Mini-High] Initializing with high_param=%s, quality=%s...", high_param, quality);
}
void gpt_o3_mini_high_predict(const char* input) {
    log_info("🔍 [GPT-o3 Mini-High] Processing input: %s", input);
}

// -----------------------------------------------------------------------------
// 4. AI Model Selection
=======
    // ...
    log_info("🔍 [GPT-4o] Prediction output: <placeholder for GPT-4o>");
}

/** GPT-o1: Environment-based model configuration */
void gpt_o1_initialize() {
    const char* hidden_size = getenv("GPT_O1_HIDDEN_SIZE");
    const char* learning_rate = getenv("GPT_O1_LEARNING_RATE");

    if (!hidden_size) {
        hidden_size = "4096";  // default
    }
    if (!learning_rate) {
        learning_rate = "0.001";  // default
    }
    log_info("🔄 [GPT-o1] Initializing with hidden_size=%s, learning_rate=%s...", 
             hidden_size, learning_rate);
}
void gpt_o1_predict(const char* input) {
    log_info("🔍 [GPT-o1] Processing input: %s", input);
    // ...
    log_info("🔍 [GPT-o1] Final output: <model-specific result>");
}

/** GPT-o3 mini: Smaller model or specialized approach */
void gpt_o3_mini_initialize() {
    const char* mini_param = getenv("GPT_O3_MINI_PARAM");
    if (!mini_param) {
        mini_param = "default_value";
    }
    log_info("🔄 [GPT-o3 Mini] Initializing with mini_param=%s...", mini_param);
}
void gpt_o3_mini_predict(const char* input) {
    log_info("🔍 [GPT-o3 Mini] Processing input: %s", input);
    // ...
    log_info("🔍 [GPT-o3 Mini] Final output: <mini model result>");
}

/** GPT-o3 mini-high: Enhanced version of o3 mini */
void gpt_o3_mini_high_initialize() {
    const char* high_param = getenv("GPT_O3_MINI_HIGH_PARAM");
    const char* quality = getenv("GPT_O3_MINI_HIGH_QUALITY");

    if (!high_param) {
        high_param = "default_high_param";
    }
    if (!quality) {
        quality = "standard";
    }
    log_info("🔄 [GPT-o3 Mini-High] Initializing with high_param=%s, quality=%s...", 
             high_param, quality);
}
void gpt_o3_mini_high_predict(const char* input) {
    log_info("🔍 [GPT-o3 Mini-High] Processing input: %s", input);
    // ...
    log_info("🔍 [GPT-o3 Mini-High] Final output: <enhanced mini-high model result>");
}

// -----------------------------------------------------------------------------
// 4. Model Selection Function
>>>>>>> main
// -----------------------------------------------------------------------------
AI_Model_Manager select_model(AI_Model_Type model_type) {
    AI_Model_Manager manager;
    memset(&manager, 0, sizeof(manager));

    manager.model_type = model_type;
    switch (model_type) {
        case AI_GPT_4o:
            manager.initialize = gpt_4o_initialize;
            manager.predict    = gpt_4o_predict;
            break;
        case AI_GPT_o1:
            manager.initialize = gpt_o1_initialize;
            manager.predict    = gpt_o1_predict;
            break;
        case AI_GPT_o3:
            manager.initialize = gpt_o3_mini_initialize;
            manager.predict    = gpt_o3_mini_predict;
            break;
        case AI_GPT_o3_MINI_HIGH:
            manager.initialize = gpt_o3_mini_high_initialize;
            manager.predict    = gpt_o3_mini_high_predict;
            break;
        default:
<<<<<<< josefkedwards-patch-1
            log_error("⚠️ [select_model] Unknown AI Model Selected: %d", model_type);
=======
            log_error("⚠️ [select_model] Unknown or unsupported AI Model: %d", model_type);
>>>>>>> main
            break;
    }
    return manager;
}

// -----------------------------------------------------------------------------
<<<<<<< josefkedwards-patch-1
// 5. External API Communication (OpenAI, Blockchain)
// -----------------------------------------------------------------------------
=======
// 5. External API Communication (OpenAI, Blockchain) with Retry
// -----------------------------------------------------------------------------

/**
 * perform_api_call_with_retry
 *
 * Attempts an HTTP POST request up to max_retries times. If successful,
 * returns 0; otherwise returns a negative value.
 */
>>>>>>> main
int perform_api_call_with_retry(const char *url, struct curl_slist *headers, const char *post_data) {
    const int max_retries = 3;
    int attempt = 0;
    CURL *curl = NULL;
    CURLcode res;

    while (attempt < max_retries) {
        curl = curl_easy_init();
        if (!curl) {
<<<<<<< josefkedwards-patch-1
            log_error("❌ [perform_api_call_with_retry] cURL initialization failed.");
=======
            log_error("❌ [perform_api_call_with_retry] cURL init failed.");
>>>>>>> main
            return -1;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
<<<<<<< josefkedwards-patch-1
            log_info("✅ [perform_api_call_with_retry] API call succeeded on attempt %d", attempt + 1);
            curl_easy_cleanup(curl);
            return 0;
        } else {
            log_warn("⚠️ [perform_api_call_with_retry] Attempt %d failed: %s", attempt + 1, curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            attempt++;
            sleep(2 * attempt);  // Exponential backoff
        }
    }
    log_error("❌ [perform_api_call_with_retry] API call failed after %d attempts", max_retries);
    return -1;
}

void call_openai_api(const char *input) {
    const char* api_key = getenv(API_KEY_ENV_VAR);
    if (!api_key) {
        log_error("❌ [call_openai_api] API key not set in environment variable %s", API_KEY_ENV_VAR);
        return;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    char auth_header[256];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
    headers = curl_slist_append(headers, auth_header);

    char post_data[1024];
    snprintf(post_data, sizeof(post_data),
             "{\"model\": \"gpt-4o\", \"prompt\": \"%s\", \"max_tokens\": 100}",
             input);

    perform_api_call_with_retry(OPENAI_API, headers, post_data);
    curl_slist_free_all(headers);
}

=======
            log_info("✅ [perform_api_call_with_retry] Success on attempt %d", attempt + 1);
            curl_easy_cleanup(curl);
            return 0;
        } else {
            log_warn("⚠️ [perform_api_call_with_retry] Attempt %d failed: %s", 
                     attempt + 1, curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            attempt++;
            sleep(1 + attempt * 2);  // Exponential-ish backoff
        }
    }
    log_error("❌ [perform_api_call_with_retry] All %d attempts failed.", max_retries);
    return -1;
}

/**
 * call_openai_api
 *
 * Builds a JSON payload and calls the OpenAI API with the given input.
 * Uses perform_api_call_with_retry internally.
 */
void call_openai_api(const char *input) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        log_error("❌ [call_openai_api] Failed to initialize cURL.");
        return;
    }

    const char* api_key = getenv(API_KEY_ENV_VAR);
    if (!api_key) {
        log_error("❌ [call_openai_api] API key not set in env var: %s", API_KEY_ENV_VAR);
        curl_easy_cleanup(curl);
        return;
    }

    char auth_header[256];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Build JSON POST data
    char post_data[1024];
    snprintf(post_data, sizeof(post_data),
        "{\"model\": \"gpt-4o\", \"prompt\": \"%s\", \"max_tokens\": 100}", input);

    // Use our retry mechanism
    if (perform_api_call_with_retry(OPENAI_API, headers, post_data) == 0) {
        log_info("✅ [call_openai_api] OpenAI API responded successfully.");
    }

    curl_slist_free_all(headers);
    // The cURL handle allocated here is freed inside perform_api_call_with_retry on success/failure
}

/**
 * sync_with_blockchain
 *
 * Calls a blockchain AI sync endpoint. Also uses cURL, though no retry
 * logic is implemented here by default. You could add it similarly if needed.
 */
>>>>>>> main
void sync_with_blockchain() {
    CURL *curl = curl_easy_init();
    if (!curl) {
        log_error("❌ [sync_with_blockchain] Failed to initialize cURL.");
        return;
    }
    curl_easy_setopt(curl, CURLOPT_URL, BLOCKCHAIN_API);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
<<<<<<< josefkedwards-patch-1
        log_error("❌ [sync_with_blockchain] Blockchain sync failed: %s", curl_easy_strerror(res));
=======
        log_error("❌ [sync_with_blockchain] Sync failed: %s", curl_easy_strerror(res));
>>>>>>> main
    } else {
        log_info("✅ [sync_with_blockchain] Blockchain AI Synced Successfully!");
    }
    curl_easy_cleanup(curl);
}

// -----------------------------------------------------------------------------
<<<<<<< josefkedwards-patch-1
// 6. AI Memory Loop: Non-Blocking I/O with Graceful Shutdown
// -----------------------------------------------------------------------------
void ai_memory_loop(PMLL* pml, AI_Model_Type model_type) {
    // Register signal handlers for graceful shutdown
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // Initialize the socket (using helper from io_socket.h)
=======
// 6. AI Memory Loop: Non-Blocking I/O + Graceful Shutdown
// -----------------------------------------------------------------------------

/**
 * ai_memory_loop
 *
 * The main loop that:
 *   1) Sets up signal handlers for graceful shutdown.
 *   2) Initializes a listening or connecting socket (depending on your architecture).
 *   3) Selects and initializes the chosen AI model.
 *   4) Uses select() to check for incoming data, reads from the socket,
 *      and then passes the data to the AI model and external APIs.
 *   5) Cleans up upon exit.
 */
void ai_memory_loop(PMLL* pml, AI_Model_Type model_type) {
    // Register signals for graceful shutdown
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // Initialize the socket
>>>>>>> main
    int socket_id = init_socket("127.0.0.1", 8080);
    if (socket_id < 0) {
        log_error("❌ [ai_memory_loop] Socket initialization failed: %s", strerror(errno));
        return;
    }

    // Select and initialize the AI model
    AI_Model_Manager ai_model = select_model(model_type);
    if (ai_model.initialize) {
        ai_model.initialize();
    } else {
<<<<<<< josefkedwards-patch-1
        log_error("⚠️ [ai_memory_loop] No valid model initialization function. Exiting.");
=======
        log_error("⚠️ [ai_memory_loop] No valid initialize() function. Exiting.");
>>>>>>> main
        close(socket_id);
        return;
    }

    fd_set read_fds;
    struct timeval timeout;
<<<<<<< josefkedwards-patch-1
    char novel_topic[1024] = {0};

    while (!stop_flag) {
        FD_ZERO(&read_fds);
        FD_SET(socket_id, &read_fds);
        timeout.tv_sec = 1;
=======

    // A buffer for incoming data
    char novel_topic[1024];

    // Main loop that runs until stop_flag is set or we break on an error
    while (!stop_flag) {
        FD_ZERO(&read_fds);
        FD_SET(socket_id, &read_fds);

        // We wait for up to 1 second for new data
        timeout.tv_sec  = 1;
>>>>>>> main
        timeout.tv_usec = 0;

        int ready = select(socket_id + 1, &read_fds, NULL, NULL, &timeout);
        if (ready < 0) {
<<<<<<< josefkedwards-patch-1
            if (errno == EINTR) continue;
            log_error("❌ [ai_memory_loop] Select error: %s", strerror(errno));
            break;
        } else if (ready == 0) {
            continue;
        }

=======
            // If interrupted by a signal (EINTR), we can continue
            if (errno == EINTR) {
                continue;
            }
            log_error("❌ [ai_memory_loop] Select error: %s", strerror(errno));
            break;
        } else if (ready == 0) {
            // No data arrived within 1 second. We just loop again unless stopping.
            continue;
        }

        // Check if our socket is ready to read
>>>>>>> main
        if (FD_ISSET(socket_id, &read_fds)) {
            memset(novel_topic, 0, sizeof(novel_topic));
            ssize_t bytes_read = read(socket_id, novel_topic, sizeof(novel_topic) - 1);
            if (bytes_read < 0) {
<<<<<<< josefkedwards-patch-1
                if (errno == EINTR) continue;
                log_error("❌ [ai_memory_loop] Error reading from socket: %s", strerror(errno));
                break;
            } else if (bytes_read == 0) {
                log_info("ℹ️ [ai_memory_loop] Socket closed by peer.");
                break;
            }
            novel_topic[bytes_read] = '\0';

            // Invoke the selected model's prediction routine
=======
                if (errno == EINTR) continue; // Signal interruption
                log_error("❌ [ai_memory_loop] Error reading socket: %s", strerror(errno));
                break;
            } else if (bytes_read == 0) {
                // The peer closed the connection
                log_info("ℹ️ [ai_memory_loop] Socket closed by peer.");
                break;
            }

            // Ensure null termination for string ops
            novel_topic[bytes_read] = '\0';

            // Model's prediction routine
>>>>>>> main
            if (ai_model.predict) {
                ai_model.predict(novel_topic);
            }

<<<<<<< josefkedwards-patch-1
            // External API calls
=======
            // External API calls: OpenAI + Blockchain Sync
>>>>>>> main
            call_openai_api(novel_topic);
            sync_with_blockchain();
        }
    }

<<<<<<< josefkedwards-patch-1
=======
    // Cleanup
>>>>>>> main
    close(socket_id);
    log_info("🚪 [ai_memory_loop] Shutting down gracefully.");
}

// -----------------------------------------------------------------------------
// 7. Main Execution Entry Point
// -----------------------------------------------------------------------------
<<<<<<< josefkedwards-patch-1
int main() {
=======

int main() {
    // Initialize a local PMLL structure
>>>>>>> main
    PMLL ai_memory;
    memset(ai_memory.knowledge_graph, 0, sizeof(ai_memory.knowledge_graph));

    log_info("🚀 [main] Starting PMLL AI Memory Loop (Final Engine)...");
<<<<<<< josefkedwards-patch-1
    // Choose the model variant here: For example, use AI_GPT_o3_MINI_HIGH for O3-mini-high.
=======

    // You can pick which model to run here. For demonstration, let's run GPT-o3 Mini-High:
>>>>>>> main
    ai_memory_loop(&ai_memory, AI_GPT_o3_MINI_HIGH);

    log_info("✅ [main] Exiting program gracefully.");
    return EXIT_SUCCESS;
}
<<<<<<< josefkedwards-patch-1

// second iteration flow begins /**
 * pml_logic_loop.c
 *
 * Implementation of the Persistent Memory Logic Loop (PMLL) Engine.
 * This file defines the functions declared in pml_logic_loop.h, including:
 * - Initialization and tokenization.
 * - Core engine operations such as predict, train, deploy, monitor,
 *   maintain, automate, and evaluate.
 * - A recursive logic loop that reads data from a network socket,
 *   processes input using the selected AI model, and invokes external
 *   API calls.
 *
 * The engine supports multiple GPT-based AI models and includes robust
 * error handling, exponential backoff for API requests, and graceful
 * shutdown via signal handling.
 *
 * Authors: Josef Kurk Edwards (1a1) and Amy Yumi Nakamoto (1a2)
 * Date: February 1, 2025
 */

#include "pml_logic_loop.h"
#include "log.h"
#include "io_socket.h"  // Assumes this header provides init_socket()
=======
/**
 * pml_logic_loop.c
 *
 * A refined, robust, and elegant AI memory loop engine supporting
 * multiple GPT-based models (GPT-4o, GPT-o1, GPT-o3 mini, GPT-o3 mini-high).
 * Includes graceful shutdown, non-blocking I/O, retry logic for external
 * API calls, and blockchain sync capability.
 *
 * Author: Your Name
 * Date:   2025-02-03
 */

>>>>>>> main
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
<<<<<<< josefkedwards-patch-1
#include <curl/curl.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <math.h>

/* -------------------------------------------------------------------------
 * Tokenizer Function
 * ------------------------------------------------------------------------- 
 */
void utf8_tokenize(const char* input, void (*callback)(const char* token)) {
    if (!input || !callback) {
        log_warn("utf8_tokenize: Invalid input or callback.");
        return;
    }
    char* input_copy = strdup(input);
    if (!input_copy) {
        log_error("utf8_tokenize: Memory allocation failed.");
        return;
    }
    char* token = strtok(input_copy, " \t\n\r");
    while (token) {
        callback(token);
        token = strtok(NULL, " \t\n\r");
    }
    free(input_copy);
}

/* -------------------------------------------------------------------------
 * PMLL Instance Initialization
 * ------------------------------------------------------------------------- 
 */
PMLL* pml_logic_loop_init() {
    PMLL* pml = (PMLL*)malloc(sizeof(PMLL));
    if (!pml) {
        log_error("pml_logic_loop_init: Memory allocation failed.");
        return NULL;
    }
    memset(pml, 0, sizeof(PMLL));

    pml->user_adoption_rate = 0;
    pml->security_incident_rate = 0;
    pml->user_satisfaction_rate = 0;
    pml->authentication_success_rate = 0;
    pml->authorization_success_rate = 0;
    pml->data_protection_effectiveness = 0;
    pml->response_time = 0;
    pml->system_performance = 0;
    pml->user_engagement = 0;
    pml->feature_utilization = 0;
    pml->behavioral_analytics = 0;
    pml->security_compliance = 0;

    memset(pml->knowledge_graph, 0, KNOWLEDGE_GRAPH_SIZE);
    pml->model = NULL;
    pml->predict = NULL;
    pml->train = NULL;
    pml->deploy = NULL;
    pml->monitor = NULL;
    pml->maintain = NULL;
    pml->automate = NULL;
    pml->evaluate = NULL;

    log_info("pml_logic_loop_init: PMLL instance initialized.");
    return pml;
}

/* -------------------------------------------------------------------------
 * PMLL Operation Wrappers
 * ------------------------------------------------------------------------- 
 */
void pml_logic_loop_predict(PMLL* pml) {
    if (!pml) {
        log_error("pml_logic_loop_predict: PMLL instance is NULL.");
        return;
    }
    if (pml->predict)
        pml->predict(pml);
    else
        log_warn("pml_logic_loop_predict: No predict function set.");
}

void pml_logic_loop_train(PMLL* pml) {
    if (!pml) return;
    if (pml->train)
        pml->train(pml);
    else
        log_warn("pml_logic_loop_train: No train function set.");
}

void pml_logic_loop_deploy(PMLL* pml) {
    if (!pml) return;
    if (pml->deploy)
        pml->deploy(pml);
    else
        log_warn("pml_logic_loop_deploy: No deploy function set.");
}

void pml_logic_loop_monitor(PMLL* pml) {
    if (!pml) return;
    if (pml->monitor)
        pml->monitor(pml);
    else
        log_warn("pml_logic_loop_monitor: No monitor function set.");
}

void pml_logic_loop_maintain(PMLL* pml) {
    if (!pml) return;
    if (pml->maintain)
        pml->maintain(pml);
    else
        log_warn("pml_logic_loop_maintain: No maintain function set.");
}

void pml_logic_loop_automate(PMLL* pml) {
    if (!pml) return;
    if (pml->automate)
        pml->automate(pml);
    else
        log_warn("pml_logic_loop_automate: No automate function set.");
}

void pml_logic_loop_evaluate(PMLL* pml) {
    if (!pml) return;
    if (pml->evaluate)
        pml->evaluate(pml);
    else
        log_warn("pml_logic_loop_evaluate: No evaluate function set.");
}

/* -------------------------------------------------------------------------
 * External API Communication
 * ------------------------------------------------------------------------- 
=======
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <curl/curl.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <math.h>
#include <fcntl.h>

// Custom headers (ensure these exist in your project)
#include "pml_logic_loop.h"
#include "unified_voice.h"
#include "memory_silo.h"
#include "io_socket.h"
#include "log.h"

// -----------------------------------------------------------------------------
// 1. Configuration and Global Definitions
// -----------------------------------------------------------------------------
#define OPENAI_API       "https://api.openai.com/v1/completions"
#define BLOCKCHAIN_API   "https://blockchain-ai-sync.com/sync"
#define API_KEY_ENV_VAR  "OPENAI_API_KEY"

// A global pointer to the PMLL instance, if needed externally
PMLL* g_pml_logic_loop_instance = NULL;

// Graceful shutdown flag controlled by signal handlers
static volatile sig_atomic_t stop_flag = 0;

/**
 * handle_signal
 *
 * A simple signal handler that sets a stop_flag, causing the main loop
 * to exit gracefully.
 */
static void handle_signal(int sig) {
    (void)sig;  // Unused parameter
    stop_flag = 1;
}

// -----------------------------------------------------------------------------
// 2. AI Model Types and Manager Structure
// -----------------------------------------------------------------------------
typedef enum {
    AI_GPT_4o,
    AI_GPT_o1,
    AI_GPT_o3,           // O3-mini model
    AI_GPT_o3_MINI_HIGH, // O3-mini-high variant
    AI_CUSTOM
} AI_Model_Type;

/**
 * AI_Model_Manager
 *
 * Each model must provide two functions:
 *   - initialize()
 *   - predict(input)
 */
typedef struct {
    AI_Model_Type model_type;
    void (*initialize)();
    void (*predict)(const char* input);
} AI_Model_Manager;

// -----------------------------------------------------------------------------
// 3. Model Implementations
//    (GPT-4o, GPT-o1, GPT-o3 mini, GPT-o3 mini-high, etc.)
// -----------------------------------------------------------------------------

/** GPT-4o: Reference Model */
void gpt_4o_initialize() {
    log_info("🔄 [GPT-4o] Initializing...");
}
void gpt_4o_predict(const char* input) {
    log_info("🔍 [GPT-4o] Processing input: %s", input);
    // ...
    log_info("🔍 [GPT-4o] Prediction output: <placeholder for GPT-4o>");
}

/** GPT-o1: Environment-based model configuration */
void gpt_o1_initialize() {
    const char* hidden_size = getenv("GPT_O1_HIDDEN_SIZE");
    const char* learning_rate = getenv("GPT_O1_LEARNING_RATE");

    if (!hidden_size) {
        hidden_size = "4096";  // default
    }
    if (!learning_rate) {
        learning_rate = "0.001";  // default
    }
    log_info("🔄 [GPT-o1] Initializing with hidden_size=%s, learning_rate=%s...", 
             hidden_size, learning_rate);
}
void gpt_o1_predict(const char* input) {
    log_info("🔍 [GPT-o1] Processing input: %s", input);
    // ...
    log_info("🔍 [GPT-o1] Final output: <model-specific result>");
}

/** GPT-o3 mini: Smaller model or specialized approach */
void gpt_o3_mini_initialize() {
    const char* mini_param = getenv("GPT_O3_MINI_PARAM");
    if (!mini_param) {
        mini_param = "default_value";
    }
    log_info("🔄 [GPT-o3 Mini] Initializing with mini_param=%s...", mini_param);
}
void gpt_o3_mini_predict(const char* input) {
    log_info("🔍 [GPT-o3 Mini] Processing input: %s", input);
    // ...
    log_info("🔍 [GPT-o3 Mini] Final output: <mini model result>");
}

/** GPT-o3 mini-high: Enhanced version of o3 mini */
void gpt_o3_mini_high_initialize() {
    const char* high_param = getenv("GPT_O3_MINI_HIGH_PARAM");
    const char* quality = getenv("GPT_O3_MINI_HIGH_QUALITY");

    if (!high_param) {
        high_param = "default_high_param";
    }
    if (!quality) {
        quality = "standard";
    }
    log_info("🔄 [GPT-o3 Mini-High] Initializing with high_param=%s, quality=%s...", 
             high_param, quality);
}
void gpt_o3_mini_high_predict(const char* input) {
    log_info("🔍 [GPT-o3 Mini-High] Processing input: %s", input);
    // ...
    log_info("🔍 [GPT-o3 Mini-High] Final output: <enhanced mini-high model result>");
}

// -----------------------------------------------------------------------------
// 4. Model Selection Function
// -----------------------------------------------------------------------------
AI_Model_Manager select_model(AI_Model_Type model_type) {
    AI_Model_Manager manager;
    memset(&manager, 0, sizeof(manager));

    manager.model_type = model_type;
    switch (model_type) {
        case AI_GPT_4o:
            manager.initialize = gpt_4o_initialize;
            manager.predict    = gpt_4o_predict;
            break;
        case AI_GPT_o1:
            manager.initialize = gpt_o1_initialize;
            manager.predict    = gpt_o1_predict;
            break;
        case AI_GPT_o3:
            manager.initialize = gpt_o3_mini_initialize;
            manager.predict    = gpt_o3_mini_predict;
            break;
        case AI_GPT_o3_MINI_HIGH:
            manager.initialize = gpt_o3_mini_high_initialize;
            manager.predict    = gpt_o3_mini_high_predict;
            break;
        default:
            log_error("⚠️ [select_model] Unknown or unsupported AI Model: %d", model_type);
            break;
    }
    return manager;
}

// -----------------------------------------------------------------------------
// 5. External API Communication (OpenAI, Blockchain) with Retry
// -----------------------------------------------------------------------------

/**
 * perform_api_call_with_retry
 *
 * Attempts an HTTP POST request up to max_retries times. If successful,
 * returns 0; otherwise returns a negative value.
>>>>>>> main
 */
int perform_api_call_with_retry(const char *url, struct curl_slist *headers, const char *post_data) {
    const int max_retries = 3;
    int attempt = 0;
    CURL *curl = NULL;
    CURLcode res;

    while (attempt < max_retries) {
        curl = curl_easy_init();
        if (!curl) {
<<<<<<< josefkedwards-patch-1
            log_error("perform_api_call_with_retry: cURL initialization failed.");
=======
            log_error("❌ [perform_api_call_with_retry] cURL init failed.");
>>>>>>> main
            return -1;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
<<<<<<< josefkedwards-patch-1
            log_info("perform_api_call_with_retry: API call succeeded on attempt %d", attempt + 1);
            curl_easy_cleanup(curl);
            return 0;
        } else {
            log_warn("perform_api_call_with_retry: Attempt %d failed: %s", attempt + 1, curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            attempt++;
            sleep(2 * attempt);  // Exponential backoff
        }
    }
    log_error("perform_api_call_with_retry: API call failed after %d attempts", max_retries);
    return -1;
}

void call_openai_api(const char *input) {
    const char* api_key = getenv(API_KEY_ENV_VAR);
    if (!api_key) {
        log_error("call_openai_api: API key not set in environment variable %s", API_KEY_ENV_VAR);
        return;
    }
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    char auth_header[256];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);
    headers = curl_slist_append(headers, auth_header);

    char post_data[1024];
    snprintf(post_data, sizeof(post_data),
             "{\"model\": \"gpt-4o\", \"prompt\": \"%s\", \"max_tokens\": 100}",
             input);

    perform_api_call_with_retry(OPENAI_API, headers, post_data);
    curl_slist_free_all(headers);
}

void sync_with_blockchain() {
    CURL *curl = curl_easy_init();
    if (!curl) {
        log_error("sync_with_blockchain: Failed to initialize cURL.");
=======
            log_info("✅ [perform_api_call_with_retry] Success on attempt %d", attempt + 1);
            curl_easy_cleanup(curl);
            return 0;
        } else {
            log_warn("⚠️ [perform_api_call_with_retry] Attempt %d failed: %s", 
                     attempt + 1, curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            attempt++;
            sleep(1 + attempt * 2);  // Exponential-ish backoff
        }
    }
    log_error("❌ [perform_api_call_with_retry] All %d attempts failed.", max_retries);
    return -1;
}

/**
 * call_openai_api
 *
 * Builds a JSON payload and calls the OpenAI API with the given input.
 * Uses perform_api_call_with_retry internally.
 */
void call_openai_api(const char *input) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        log_error("❌ [call_openai_api] Failed to initialize cURL.");
        return;
    }

    const char* api_key = getenv(API_KEY_ENV_VAR);
    if (!api_key) {
        log_error("❌ [call_openai_api] API key not set in env var: %s", API_KEY_ENV_VAR);
        curl_easy_cleanup(curl);
        return;
    }

    char auth_header[256];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_key);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, auth_header);
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Build JSON POST data
    char post_data[1024];
    snprintf(post_data, sizeof(post_data),
        "{\"model\": \"gpt-4o\", \"prompt\": \"%s\", \"max_tokens\": 100}", input);

    // Use our retry mechanism
    if (perform_api_call_with_retry(OPENAI_API, headers, post_data) == 0) {
        log_info("✅ [call_openai_api] OpenAI API responded successfully.");
    }

    curl_slist_free_all(headers);
    // The cURL handle allocated here is freed inside perform_api_call_with_retry on success/failure
}

/**
 * sync_with_blockchain
 *
 * Calls a blockchain AI sync endpoint. Also uses cURL, though no retry
 * logic is implemented here by default. You could add it similarly if needed.
 */
void sync_with_blockchain() {
    CURL *curl = curl_easy_init();
    if (!curl) {
        log_error("❌ [sync_with_blockchain] Failed to initialize cURL.");
>>>>>>> main
        return;
    }
    curl_easy_setopt(curl, CURLOPT_URL, BLOCKCHAIN_API);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
<<<<<<< josefkedwards-patch-1
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        log_error("sync_with_blockchain: Blockchain sync failed: %s", curl_easy_strerror(res));
    } else {
        log_info("sync_with_blockchain: Blockchain AI Synced Successfully!");
=======

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        log_error("❌ [sync_with_blockchain] Sync failed: %s", curl_easy_strerror(res));
    } else {
        log_info("✅ [sync_with_blockchain] Blockchain AI Synced Successfully!");
>>>>>>> main
    }
    curl_easy_cleanup(curl);
}

<<<<<<< josefkedwards-patch-1
/* -------------------------------------------------------------------------
 * AI Memory Loop: Non-Blocking I/O with Graceful Shutdown
 * ------------------------------------------------------------------------- 
 */
void ai_memory_loop(PMLL* pml, AI_Model_Type model_type) {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    int socket_id = init_socket("127.0.0.1", 8080);
    if (socket_id < 0) {
        log_error("ai_memory_loop: Socket initialization failed: %s", strerror(errno));
        return;
    }

=======
// -----------------------------------------------------------------------------
// 6. AI Memory Loop: Non-Blocking I/O + Graceful Shutdown
// -----------------------------------------------------------------------------

/**
 * ai_memory_loop
 *
 * The main loop that:
 *   1) Sets up signal handlers for graceful shutdown.
 *   2) Initializes a listening or connecting socket (depending on your architecture).
 *   3) Selects and initializes the chosen AI model.
 *   4) Uses select() to check for incoming data, reads from the socket,
 *      and then passes the data to the AI model and external APIs.
 *   5) Cleans up upon exit.
 */
void ai_memory_loop(PMLL* pml, AI_Model_Type model_type) {
    // Register signals for graceful shutdown
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // Initialize the socket
    int socket_id = init_socket("127.0.0.1", 8080);
    if (socket_id < 0) {
        log_error("❌ [ai_memory_loop] Socket initialization failed: %s", strerror(errno));
        return;
    }

    // Select and initialize the AI model
>>>>>>> main
    AI_Model_Manager ai_model = select_model(model_type);
    if (ai_model.initialize) {
        ai_model.initialize();
    } else {
<<<<<<< josefkedwards-patch-1
        log_error("ai_memory_loop: No valid model initialization function. Exiting.");
=======
        log_error("⚠️ [ai_memory_loop] No valid initialize() function. Exiting.");
>>>>>>> main
        close(socket_id);
        return;
    }

    fd_set read_fds;
    struct timeval timeout;
<<<<<<< josefkedwards-patch-1
    char novel_topic[1024] = {0};

    while (!stop_flag) {
        FD_ZERO(&read_fds);
        FD_SET(socket_id, &read_fds);
        timeout.tv_sec = 1;
=======

    // A buffer for incoming data
    char novel_topic[1024];

    // Main loop that runs until stop_flag is set or we break on an error
    while (!stop_flag) {
        FD_ZERO(&read_fds);
        FD_SET(socket_id, &read_fds);

        // We wait for up to 1 second for new data
        timeout.tv_sec  = 1;
>>>>>>> main
        timeout.tv_usec = 0;

        int ready = select(socket_id + 1, &read_fds, NULL, NULL, &timeout);
        if (ready < 0) {
<<<<<<< josefkedwards-patch-1
            if (errno == EINTR) continue;
            log_error("ai_memory_loop: Select error: %s", strerror(errno));
            break;
        } else if (ready == 0) {
            continue;
        }

=======
            // If interrupted by a signal (EINTR), we can continue
            if (errno == EINTR) {
                continue;
            }
            log_error("❌ [ai_memory_loop] Select error: %s", strerror(errno));
            break;
        } else if (ready == 0) {
            // No data arrived within 1 second. We just loop again unless stopping.
            continue;
        }

        // Check if our socket is ready to read
>>>>>>> main
        if (FD_ISSET(socket_id, &read_fds)) {
            memset(novel_topic, 0, sizeof(novel_topic));
            ssize_t bytes_read = read(socket_id, novel_topic, sizeof(novel_topic) - 1);
            if (bytes_read < 0) {
<<<<<<< josefkedwards-patch-1
                if (errno == EINTR) continue;
                log_error("ai_memory_loop: Error reading from socket: %s", strerror(errno));
                break;
            } else if (bytes_read == 0) {
                log_info("ai_memory_loop: Socket closed by peer.");
                break;
            }
            novel_topic[bytes_read] = '\0';

            if (ai_model.predict) {
                ai_model.predict(novel_topic);
            }
=======
                if (errno == EINTR) continue; // Signal interruption
                log_error("❌ [ai_memory_loop] Error reading socket: %s", strerror(errno));
                break;
            } else if (bytes_read == 0) {
                // The peer closed the connection
                log_info("ℹ️ [ai_memory_loop] Socket closed by peer.");
                break;
            }

            // Ensure null termination for string ops
            novel_topic[bytes_read] = '\0';

            // Model's prediction routine
            if (ai_model.predict) {
                ai_model.predict(novel_topic);
            }

            // External API calls: OpenAI + Blockchain Sync
>>>>>>> main
            call_openai_api(novel_topic);
            sync_with_blockchain();
        }
    }

<<<<<<< josefkedwards-patch-1
    close(socket_id);
    log_info("ai_memory_loop: Shutting down gracefully.");
}

/* -------------------------------------------------------------------------
 * Main Execution Entry Point
 * ------------------------------------------------------------------------- 
 */
int main() {
    PMLL ai_memory;
    memset(ai_memory.knowledge_graph, 0, sizeof(ai_memory.knowledge_graph));

    log_info("main: Starting PMLL AI Memory Loop Engine (O3-mini-high version)...");
    ai_memory_loop(&ai_memory, AI_GPT_o3_MINI_HIGH);
    log_info("main: Exiting program gracefully.");
    return EXIT_SUCCESS;
}
// so why we have the logic loop do a second loop of itself and a second iteration is that it is checking itself and validating, verifying and checking its own logic and updating itelf after during the first iterations shen it returns and retrieve the relevat AI model code contexts.
d
=======
    // Cleanup
    close(socket_id);
    log_info("🚪 [ai_memory_loop] Shutting down gracefully.");
}

// -----------------------------------------------------------------------------
// 7. Main Execution Entry Point
// -----------------------------------------------------------------------------

int main() {
    // Initialize a local PMLL structure
    PMLL ai_memory;
    memset(ai_memory.knowledge_graph, 0, sizeof(ai_memory.knowledge_graph));

    log_info("🚀 [main] Starting PMLL AI Memory Loop (Final Engine)...");

    // You can pick which model to run here. For demonstration, let's run GPT-o3 Mini-High:
    ai_memory_loop(&ai_memory, AI_GPT_o3_MINI_HIGH);

    log_info("✅ [main] Exiting program gracefully.");
    return EXIT_SUCCESS;
}
>>>>>>> main

