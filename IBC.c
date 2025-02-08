#include "IBC.h"

// Initialize the IBC ledger
void init_ibc_ledger(IBCLedger *ledger) {
    ledger->transaction_count = 0;
}

// Add a transaction to the IBC ledger
void add_transaction(IBCLedger *ledger, const char *tx_id, const char *source, const char *destination, 
                     const char *asset, double amount, const char *status, int suspicious) {
    if (ledger->transaction_count < MAX_TRANSACTIONS) {
        Transaction *tx = &ledger->transactions[ledger->transaction_count++];
        strncpy(tx->tx_id, tx_id, sizeof(tx->tx_id));
        strncpy(tx->source_chain, source, sizeof(tx->source_chain));
        strncpy(tx->destination_chain, destination, sizeof(tx->destination_chain));
        strncpy(tx->asset, asset, sizeof(tx->asset));
        tx->amount = amount;
        strncpy(tx->status, status, sizeof(tx->status));
        tx->suspicious = suspicious;
    }
}

// Print all transactions in the ledger
void print_ledger(const IBCLedger *ledger) {
    printf("IBC Ledger Transactions:\n");
    for (int i = 0; i < ledger->transaction_count; i++) {
        printf("TX ID: %s | Source: %s | Destination: %s | Asset: %s | Amount: %.2f | Status: %s | Suspicious: %s\n",
               ledger->transactions[i].tx_id, ledger->transactions[i].source_chain, 
               ledger->transactions[i].destination_chain, ledger->transactions[i].asset, 
               ledger->transactions[i].amount, ledger->transactions[i].status,
               ledger->transactions[i].suspicious ? "Yes" : "No");
    }
}

// Detect suspicious transactions in the ledger
void detect_suspicious_transactions(const IBCLedger *ledger) {
    printf("Suspicious Transactions:\n");
    for (int i = 0; i < ledger->transaction_count; i++) {
        if (ledger->transactions[i].suspicious) {
            printf("ALERT! Suspicious Transaction Detected: %s from %s to %s\n",
                   ledger->transactions[i].tx_id, ledger->transactions[i].source_chain,
                   ledger->transactions[i].destination_chain);
        }
    }
}
