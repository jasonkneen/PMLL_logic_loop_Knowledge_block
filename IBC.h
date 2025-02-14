#ifndef IBC_H
#define IBC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TRANSACTIONS 100
#define MAX_CHAIN_NAME 50

// Blockchain Transaction Structure
typedef struct {
    char tx_id[64];
    char source_chain[MAX_CHAIN_NAME];
    char destination_chain[MAX_CHAIN_NAME];
    char asset[10];
    double amount;
    char status[15];
    int suspicious;
} Transaction;

// IBC Ledger Structure
typedef struct {
    Transaction transactions[MAX_TRANSACTIONS];
    int transaction_count;
} IBCLedger;

// Function Prototypes
void init_ibc_ledger(IBCLedger *ledger);
void add_transaction(IBCLedger *ledger, const char *tx_id, const char *source, const char *destination, 
                     const char *asset, double amount, const char *status, int suspicious);
void print_ledger(const IBCLedger *ledger);
void detect_suspicious_transactions(const IBCLedger *ledger);

#endif
-// Function Prototypes
+/* Initialize a new IBC ledger
+ * @param ledger Pointer to ledger to initialize
+ * @return 0 on success, non-zero on error
+ */
-void init_ibc_ledger(IBCLedger *ledger);
+int init_ibc_ledger(IBCLedger *ledger);

+/* Add a new transaction to the ledger
+ * @param ledger Pointer to ledger
+ * @param transaction_id Unique transaction identifier
+ * @param source_chain Source blockchain identifier
+ * @param dest_chain Destination blockchain identifier
+ * @param asset_type Type of asset being transferred
+ * @param asset_amount Amount of asset
+ * @param tx_status Current transaction status
+ * @param is_suspicious Flag for suspicious activity
+ * @return 0 on success, non-zero on error
+ */
-void add_transaction(IBCLedger *ledger, const char *tx_id, const char *source,
-                     const char *destination, const char *asset, double amount,
-                     const char *status, int suspicious);
+int add_transaction(IBCLedger *ledger, const char *transaction_id,
+                   const char *source_chain, const char *dest_chain,
+                   const char *asset_type, double asset_amount,
+                   const char *tx_status, bool is_suspicious);

+/* Print the contents of the ledger
+ * @param ledger Pointer to ledger to print
+ */
 void print_ledger(const IBCLedger *ledger);

+/* Analyze and flag suspicious transactions
+ * @param ledger Pointer to ledger to analyze
+ * @return Number of suspicious transactions found
+ */
-void detect_suspicious_transactions(const IBCLedger *ledger);
+int detect_suspicious_transactions(const IBCLedger *ledger);
