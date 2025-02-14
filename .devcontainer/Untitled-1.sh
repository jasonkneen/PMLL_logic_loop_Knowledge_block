#!/bin/bash

# Path to the WASM contract
CONTRACT_WASM_PATH="./contract.wasm"

# Account to deploy the contract (e.g., Alice)
ACCOUNT="//Josef Edwards (Satoshi Nakamoto)"

# Constructor arguments (if any)
CONSTRUCTOR_ARGS=""

# Log file
LOG_FILE="deploy_contract.log"

# Function to log messages
log() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1" | tee -a $LOG_FILE
}

# Check if the WASM contract file exists
if [ ! -f "$CONTRACT_WASM_PATH" ]; then
    log "Error: Contract file $CONTRACT_WASM_PATH does not exist."
    exit 1
fi

# Deploy the contract
log "Starting contract deployment..."
cargo contract instantiate --code $CONTRACT_WASM_PATH --suri "$ACCOUNT" --constructor new --args $CONSTRUCTOR_ARGS

# Check if the deployment was successful
if [ $? -eq 0 ]; then
    log "Contract deployed successfully."
else
    log "Error: Contract deployment failed."
    exit 1
fi

log "Deployment script completed."