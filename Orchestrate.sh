#!/bin/bash
# File: Orchestrate.sh
# Description: Integrates consent collection, health checks, payload distribution, and binary deployment.

# Paths and Configuration
LOG_DIR="./logs"
BUILD_LOG="$LOG_DIR/build.log"
ORCHESTRA_LOG="$LOG_DIR/orchestra.log"
CONSENT_LOG_FILE="$LOG_DIR/consent_responses.log"
BINARIES_DIR="./binaries"
PORT_BASE=8080

# Components to orchestrate
COMPONENTS=("VECTOR_MATRIX" "MEMORY_SILO" "IO_SOCKET" "PML_LOGIC_LOOP" "UNIFIED_VOICE" "CROSS_TALK" "PERSISTENCE")

# URL/IP configuration for silos
SILO_DOMAIN="silo"
START_SILO=1
END_SILO=144000
RETRY_COUNT=3

# Payload message
PAYLOAD_MESSAGE=$(cat <<EOF
{
  "subject": "Deployment Notification",
  "body": "We are deploying the latest binaries to your silo. Please prepare to receive updates."
}
EOF
)

# Ensure directories exist
mkdir -p "$LOG_DIR" "$BINARIES_DIR"

# Logging utility
log() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1" | tee -a "$ORCHESTRA_LOG"
}

# Initialize counters
SUCCESS_PAYLOAD=0
FAILED_PAYLOAD=0
SUCCESS_BINARY=0
FAILED_BINARY=0
FAILED_HEALTH=0

# Health check function
health_check() {
    local silo=$1
    RESPONSE=$(curl -s -o /dev/null -w "%{http_code}" "https://silo$silo.$SILO_DOMAIN/health")
    if [ "$RESPONSE" == "200" ]; then
        return 0
    else
        log "Health check failed for silo$silo.$SILO_DOMAIN"
        FAILED_HEALTH=$((FAILED_HEALTH + 1))
        return 1
    fi
}

# Step 1: Execute Consent_request.sh
log "Starting consent request process..."
./Consent_request.sh
if [ $? -ne 0 ]; then
    log "ERROR: Consent_request.sh failed. Exiting."
    exit 1
fi

# Verify consent results
if [ ! -f "$CONSENT_LOG_FILE" ]; then
    log "ERROR: Consent log not found. Exiting."
    exit 1
fi
SUCCESS_COUNT=$(grep -c "SUCCESS" "$CONSENT_LOG_FILE")
FAILURE_COUNT=$(grep -c "ERROR" "$CONSENT_LOG_FILE")
log "Consent process completed. Success: $SUCCESS_COUNT, Failures: $FAILURE_COUNT"

# Step 2: Compile dependencies
log "Compiling dependencies..."
make clean all &>> "$BUILD_LOG"
if [ $? -ne 0 ]; then
    log "ERROR: Compilation failed. Check $BUILD_LOG for details."
    exit 1
fi
log "Compilation completed successfully."

# Validate compiled components
log "Validating compiled components..."
for component in "${COMPONENTS[@]}"; do
    if [ ! -f "./$component" ]; then
        log "ERROR: Missing executable for $component. Exiting."
        exit 1
    fi
    cp "./$component" "$BINARIES_DIR"
done
log "All components validated and prepared for distribution."

# Function to send payload message
send_payload() {
    local silo=$1
    local attempt=1

    while [ $attempt -le $RETRY_COUNT ]; do
        RESPONSE=$(curl -s -X POST "https://silo$silo.$SILO_DOMAIN/payload" \
            -H "Content-Type: application/json" \
            --data "$PAYLOAD_MESSAGE")

        if [ $? -eq 0 ] && [[ "$RESPONSE" == *"ACKNOWLEDGED"* ]]; then
            SUCCESS_PAYLOAD=$((SUCCESS_PAYLOAD + 1))
            echo "Payload sent to silo$silo.$SILO_DOMAIN"
            return 0
        fi

        attempt=$((attempt + 1))
        sleep 2
    done

    FAILED_PAYLOAD=$((FAILED_PAYLOAD + 1))
    return 1
}

# Function to deploy binaries
deploy_binaries() {
    local silo=$1
    local binary=$2
    local attempt=1

    while [ $attempt -le $RETRY_COUNT ]; do
        RESPONSE=$(curl -s -X POST "https://silo$silo.$SILO_DOMAIN/binary" \
            -H "Content-Type: application/octet-stream" \
            --data-binary "@$BINARIES_DIR/$binary")

        if [ $? -eq 0 ]; then
            SUCCESS_BINARY=$((SUCCESS_BINARY + 1))
            echo "Binary deployed to silo$silo.$SILO_DOMAIN"
            return 0
        fi

        attempt=$((attempt + 1))
        sleep 2
    done

    FAILED_BINARY=$((FAILED_BINARY + 1))
    return 1
}

# Step 3: Notify silos with payload
log "Performing health checks and sending payload notifications to silos..."
for silo in $(seq $START_SILO $END_SILO); do
    if health_check "$silo"; then
        send_payload "$silo" &
    else
        echo "Skipping silo$silo.$SILO_DOMAIN due to failed health check."
    fi
done
wait
log "Payload notifications complete."

# Step 4: Deploy binaries to silos
log "Deploying binaries to silos..."
for binary in "${COMPONENTS[@]}"; do
    for silo in $(seq $START_SILO $END_SILO); do
        if health_check "$silo"; then
            deploy_binaries "$silo" "$binary" &
        else
            echo "Skipping silo$silo.$SILO_DOMAIN due to failed health check."
        fi
    done
done
wait
log "Binary distribution complete."

# Summary
echo -e "\nSummary of Operations:"
echo "Payload Notifications - Success: $SUCCESS_PAYLOAD, Failed: $FAILED_PAYLOAD"
echo "Binary Deployments - Success: $SUCCESS_BINARY, Failed: $FAILED_BINARY"
echo "Health Checks - Failed: $FAILED_HEALTH"
log "Deployment process completed."
