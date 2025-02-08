#!/bin/bash
# File: Orchestrate.sh
# Description: Final enhanced orchestrator that handles consent collection, health checks,
#              payload distribution, dynamic websocketing, cascading, and advanced logic loops.
#              Incorporates dynamic concurrency, pre-flight checks, rollback, notifications,
#              progress reporting, and graceful shutdown.

#####################################
# CONFIGURATION & GLOBAL VARIABLES  #
#####################################

# Script directory (dynamically resolved)
SCRIPT_DIR="${SCRIPT_DIR:-$(dirname "$0")}"
LOG_DIR="$SCRIPT_DIR/logs"
TIMESTAMP=$(date '+%Y%m%d%H%M%S')
BUILD_LOG="$LOG_DIR/build_${TIMESTAMP}.log"
ORCHESTRA_LOG="$LOG_DIR/orchestra_${TIMESTAMP}.log"
CONSENT_LOG_FILE="$LOG_DIR/consent_responses.log"
CONSENT_SCRIPT="$SCRIPT_DIR/Consent_request.sh"
BINARIES_DIR="$SCRIPT_DIR/binaries"
PREV_BINARIES_DIR="$SCRIPT_DIR/binaries_previous"  # For rollback backup
PORT_BASE=55878

# Components to orchestrate (binaries expected in BINARIES_DIR)
COMPONENTS=("VECTOR_MATRIX" "MEMORY_SILO" "IO_SOCKET" "PML_LOGIC_LOOP" "UNIFIED_VOICE" "CROSS_TALK" "PERSISTENCE" "ARLL" "EFLL")

# Silo configuration
SILO_DOMAIN="silo.example.com"
START_SILO=1
END_SILO=144000
BATCH_SIZE=1000
RETRY_COUNT=3

# Notification Options
SLACK_ENABLED=0
SLACK_WEBHOOK_URL="https://hooks.slack.com/services/YOUR/SLACK/WEBHOOK"
SLACK_CHANNEL="#deployments"
SLACK_USERNAME="OrchestrationBot"

EMAIL_ENABLED=0
EMAIL_TO="ops@example.com"
EMAIL_SUBJECT="Orchestrator Deployment Report"

REMOTE_SYSLOG_ENABLED=0
REMOTE_SYSLOG_SERVER="syslog.example.com"
REMOTE_SYSLOG_PORT=514

# Rollback Settings
ERROR_THRESHOLD=100   # Rollback if errors exceed this count
ERROR_COUNT=0

# Debug / Verbose flags (set via command-line options)
DEBUG=0
VERBOSE=0

# Dynamic concurrency – will be calculated in pre-flight
MAX_PARALLEL=1

##########################
# HELPER FUNCTIONS       #
##########################

usage() {
    cat <<EOF
Usage: $0 [OPTIONS]

Options:
  -v    Enable verbose logging.
  -d    Enable debug logging.
  -h    Show this help message.
EOF
}

# Send a Slack message if enabled
send_slack_message() {
    local message="$1"
    [ $SLACK_ENABLED -eq 1 ] || return 0
    curl -s -X POST -H 'Content-type: application/json' \
         --data "{\"channel\": \"${SLACK_CHANNEL}\", \"username\": \"${SLACK_USERNAME}\", \"text\": \"${message}\"}" \
         "$SLACK_WEBHOOK_URL" >/dev/null 2>&1
}

# Log function: prints to console, appends to ORCHESTRA_LOG, and optionally sends remote syslog/Slack
log() {
    local msg
    msg="$(date '+%Y-%m-%d %H:%M:%S') - $1"
    echo "$msg" | tee -a "$ORCHESTRA_LOG"
    if [ $REMOTE_SYSLOG_ENABLED -eq 1 ]; then
        logger -n "$REMOTE_SYSLOG_SERVER" -P "$REMOTE_SYSLOG_PORT" -t "Orchestrator" "$msg"
    fi
    # Send key events to Slack
    if [[ "$1" =~ ^(ERROR|ROLLBACK|INFO|Deployment) ]]; then
        send_slack_message "$msg"
    fi
}

# Debug logging helper
debug_log() {
    [ $DEBUG -eq 1 ] && log "[DEBUG] $1"
}

# Verbose logging helper
verbose_log() {
    [ $VERBOSE -eq 1 ] && log "[VERBOSE] $1"
}

# Trap SIGINT and SIGTERM to clean up background jobs gracefully
graceful_shutdown() {
    log "Received termination signal. Initiating graceful shutdown..."
    sleep 2
    pkill -P $$
    log "Shutdown complete. Exiting."
    exit 1
}
trap graceful_shutdown SIGINT SIGTERM

# Pre-flight: Check for required commands
preflight_checks() {
    local required_cmds=("curl" "timeout" "make" "logger")
    local missing=()
    for cmd in "${required_cmds[@]}"; do
        if ! command -v "$cmd" >/dev/null 2>&1; then
            missing+=("$cmd")
        fi
    done
    if [ ${#missing[@]} -gt 0 ]; then
        log "ERROR: Missing required commands: ${missing[*]}"
        exit 1
    fi
}

# Calculate dynamic parallelism based on CPU and free memory
calculate_dynamic_parallelism() {
    local cpu_count max_cpu_jobs free_mem max_mem_jobs
    cpu_count=$(nproc --all 2>/dev/null || echo 4)
    max_cpu_jobs=$(( cpu_count * 2 ))
    free_mem=$(free -m | awk '/^Mem:/{print $7}')  # available memory in MB
    [ -z "$free_mem" ] && free_mem=1024
    max_mem_jobs=$(( free_mem / 256 ))
    MAX_PARALLEL=$(( max_cpu_jobs < max_mem_jobs ? max_cpu_jobs : max_mem_jobs ))
    [ $MAX_PARALLEL -lt 1 ] && MAX_PARALLEL=1
    verbose_log "Calculated dynamic parallelism: CPU-based=$max_cpu_jobs, Mem-based=$max_mem_jobs, Final=$MAX_PARALLEL"
}

# Wait until the number of background jobs is less than MAX_PARALLEL
wait_for_slot() {
    while [ "$(jobs -rp | wc -l)" -ge "$MAX_PARALLEL" ]; do
        sleep 0.2
    done
}

# Backup current binaries for rollback purposes
backup_binaries_for_rollback() {
    if [ -d "$BINARIES_DIR" ]; then
        rm -rf "$PREV_BINARIES_DIR"
        cp -r "$BINARIES_DIR" "$PREV_BINARIES_DIR"
        debug_log "Existing binaries backed up to $PREV_BINARIES_DIR."
    fi
}

# Rollback: Restore previous binaries if error threshold is exceeded
rollback() {
    if [ -d "$PREV_BINARIES_DIR" ]; then
        log "ROLLBACK: Error count ($ERROR_COUNT) exceeded threshold. Restoring previous binaries."
        rm -rf "$BINARIES_DIR"
        mv "$PREV_BINARIES_DIR" "$BINARIES_DIR"
    else
        log "ERROR: No backup available for rollback."
    fi
    exit 1
}

##########################
# CORE FUNCTIONALITY     #
##########################

# Health check for a given silo using HTTP GET (with timeout)
health_check() {
    local silo="$1" attempt=1 RESPONSE
    while [ $attempt -le $RETRY_COUNT ]; do
        RESPONSE=$(timeout 5 curl -s -o /dev/null -w "%{http_code}" "https://silo$silo.$SILO_DOMAIN/health")
        if [ "$RESPONSE" == "200" ]; then
            return 0
        fi
        debug_log "Health check failed for silo$silo.$SILO_DOMAIN (Attempt $attempt/$RETRY_COUNT)"
        sleep 5
        attempt=$((attempt + 1))
    done
    log "ERROR: Health check failed for silo$silo.$SILO_DOMAIN after $RETRY_COUNT attempts."
    ((ERROR_COUNT++))
    return 1
}

# Check WebSocket status using websocat if available, fallback to curl
check_status_via_websocket() {
    local silo="$1" ws_url RESPONSE
    ws_url="wss://silo${silo}.$SILO_DOMAIN/status"
    if command -v websocat >/dev/null 2>&1; then
        RESPONSE=$(timeout 5 websocat "$ws_url" 2>/dev/null | grep -q "open" && echo "open" || echo "closed")
    else
        RESPONSE=$(timeout 5 curl -s --include "$ws_url" | grep -q "open" && echo "open" || echo "closed")
    fi
    echo "$RESPONSE"
}

# Run logic loops (PML_LOGIC_LOOP, ARLL, EFLL) for a silo with timeouts
run_logic_loops() {
    local silo_id="$1" PMLL_CHECK ARLL_CHECK EFLL_CHECK
    PMLL_CHECK=$(timeout 10 "$BINARIES_DIR/PML_LOGIC_LOOP" "$silo_id" 2>/dev/null || echo "ERROR")
    ARLL_CHECK=$(timeout 10 "$BINARIES_DIR/ARLL" "$silo_id" 2>/dev/null || echo "ERROR")
    EFLL_CHECK=$(timeout 10 "$BINARIES_DIR/EFLL" "$silo_id" 2>/dev/null || echo "ERROR")
    if [[ $PMLL_CHECK == "OK" && $ARLL_CHECK == "OK" && $EFLL_CHECK == "OK" ]]; then
        debug_log "Silo $silo_id: All logic loops OK."
    else
        log "ERROR: Silo $silo_id issues: PMLL=$PMLL_CHECK, ARLL=$ARLL_CHECK, EFLL=$EFLL_CHECK"
        ((ERROR_COUNT++))
    fi
}

# Process silos in batches with progress reporting and rollback checks
process_silos_in_batches() {
    local total_silos processed_silos last_report_time current_time batch_end i silo
    total_silos=$((END_SILO - START_SILO + 1))
    processed_silos=0
    last_report_time=$(date +%s)

    for (( i=START_SILO; i<=END_SILO; i+=BATCH_SIZE )); do
        batch_end=$(( i + BATCH_SIZE - 1 ))
        [ $batch_end -gt $END_SILO ] && batch_end=$END_SILO
        log "INFO: Processing silos from $i to $batch_end..."
        for silo in $(seq $i $batch_end); do
            wait_for_slot
            {
                if health_check "$silo"; then
                    ws_status=$(check_status_via_websocket "$silo")
                    if [ "$ws_status" == "open" ]; then
                        run_logic_loops "$silo"
                    else
                        log "ERROR: Silo $silo: Websocket status is closed."
                        ((ERROR_COUNT++))
                    fi
                fi
                processed_silos=$((processed_silos + 1))
                current_time=$(date +%s)
                if [ $((current_time - last_report_time)) -ge 30 ] || [ $processed_silos -eq $total_silos ]; then
                    log "INFO: Progress - Processed $processed_silos of $total_silos silos."
                    last_report_time=$current_time
                fi
                # Check if error threshold has been exceeded
                if [ "$ERROR_COUNT" -ge "$ERROR_THRESHOLD" ]; then
                    rollback
                fi
            } &
        done
        wait  # Wait for current batch to finish
    done
}

##########################
# MAIN SCRIPT EXECUTION  #
##########################

# Parse command-line options
while getopts ":vdh" opt; do
    case "${opt}" in
        v) VERBOSE=1 ;;
        d) DEBUG=1 ;;
        h) usage; exit 0 ;;
        *) usage; exit 1 ;;
    esac
done

# Run pre-flight checks and calculate dynamic concurrency
preflight_checks
calculate_dynamic_parallelism

# Ensure directories exist
mkdir -p "$LOG_DIR" "$BINARIES_DIR"

log "Deployment started..."
backup_binaries_for_rollback

# Consent Collection (run synchronously or you can parallelize if needed)
log "Starting consent request process..."
bash "$CONSENT_SCRIPT"
if [ $? -ne 0 ]; then
    log "ERROR: Consent_request.sh failed. Exiting."
    exit 1
fi
if [ ! -f "$CONSENT_LOG_FILE" ]; then
    log "ERROR: Consent log not found. Exiting."
    exit 1
fi
SUCCESS_COUNT=$(grep -c "SUCCESS" "$CONSENT_LOG_FILE" || echo 0)
FAILURE_COUNT=$(grep -c "ERROR" "$CONSENT_LOG_FILE" || echo 0)
log "INFO: Consent process completed. Success: $SUCCESS_COUNT, Failures: $FAILURE_COUNT"

# Compilation: Only if BINARIES_DIR is empty (conditional compilation)
if [ -z "$(ls -A "$BINARIES_DIR")" ]; then
    log "INFO: No binaries found. Starting compilation..."
    make clean all &>> "$BUILD_LOG"
    if [ $? -ne 0 ]; then
        log "ERROR: Compilation failed. Check $BUILD_LOG."
        exit 1
    fi
    log "INFO: Compilation completed successfully."
else
    log "INFO: Binaries detected. Skipping compilation."
fi

# Validate that each expected binary exists
log "INFO: Validating compiled components..."
for component in "${COMPONENTS[@]}"; do
    if [ ! -f "$BINARIES_DIR/$component" ]; then
        log "ERROR: Missing binary for $component. This component will be skipped."
        ((ERROR_COUNT++))
    fi
done

# Deploy each component by processing silos in batches
log "INFO: Deploying binaries to silos..."
for component in "${COMPONENTS[@]}"; do
    if [ -f "$BINARIES_DIR/$component" ]; then
        log "INFO: Deploying component: $component"
        process_silos_in_batches
    fi
done

log "INFO: Deployment process completed."

# Summary & Notification
summary="Deployment Summary:
- Total Silos Processed: $((END_SILO - START_SILO + 1))
- Consent: Success=$SUCCESS_COUNT, Failures=$FAILURE_COUNT
- Total Error Count: $ERROR_COUNT
- Rollback Triggered: $([ -d "$PREV_BINARIES_DIR" ] && [ ! -d "$BINARIES_DIR" ] && echo "YES" || echo "NO")"
log "INFO: $summary"

if [ $EMAIL_ENABLED -eq 1 ]; then
    echo "$summary" | mail -s "$EMAIL_SUBJECT" "$EMAIL_TO"
    log "INFO: Summary email sent to $EMAIL_TO."
fi

send_slack_message "Deployment Completed. Errors: $ERROR_COUNT (Threshold: $ERROR_THRESHOLD)"
echo "Deployment Complete."
exit 0

