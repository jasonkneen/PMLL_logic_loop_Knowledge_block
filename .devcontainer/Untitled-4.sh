#!/bin/bash

# Path to the WASM contract
CONTRACT_WASM_PATH="./contract.wasm"

# Account to deploy the contract (e.g., Alice)
ACCOUNT="//Josef Edwards (Satoshi Nakamoto)"

# Constructor arguments (if any)
CONSTRUCTOR_ARGS=""

# Deploy the contract
cargo contract instantiate --code $CONTRACT_WASM_PATH --suri $ACCOUNT --constructor new --args $CONSTRUCTOR_ARGS