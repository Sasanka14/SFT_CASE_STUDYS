# Case Study: Database Configuration Updater
# Author: Sasanka Sekhar Kundu
# Description: Automates DB configuration updates with
#              backup, logging, and cross-platform support.

CONFIG_FILE="db_config.conf"
BACKUP_FILE="db_config_backup_$(date +%Y%m%d%H%M%S).conf"
LOG_FILE="update_log.txt"

# Cross-platform sed wrapper (Linux vs macOS)
sedi() {
    if sed --version >/dev/null 2>&1; then
        # GNU sed (Linux)
        sed -i "$@"
    else
        # BSD/macOS sed
        sed -i '' "$@"
    fi
}

# Check if config file exists
if [ ! -f "$CONFIG_FILE" ]; then
    echo "Error: $CONFIG_FILE not found!"
    exit 1
fi

# Create backup before making changes
cp "$CONFIG_FILE" "$BACKUP_FILE"
echo "[INFO] Backup created: $BACKUP_FILE" | tee -a $LOG_FILE

# Take new credentials from user
read -p "Enter new DB Host: " NEW_HOST
read -p "Enter new DB User: " NEW_USER
read -sp "Enter new DB Password: " NEW_PASS
echo

# 1. Substitute (s) → Update credentials
sedi "s/^host=.*/host=$NEW_HOST/" "$CONFIG_FILE"
sedi "s/^user=.*/user=$NEW_USER/" "$CONFIG_FILE"
sedi "s/^password=.*/password=$NEW_PASS/" "$CONFIG_FILE"
echo "[INFO] Credentials updated" | tee -a $LOG_FILE

# 2. Insert (i) → Insert backup info at line 2
sedi '2i\
backup_host=192.168.1.2
' "$CONFIG_FILE"
echo "[INFO] Backup info inserted" | tee -a $LOG_FILE

# 3. Append (a) → Append connection status at end
echo "connection_status=active" >> "$CONFIG_FILE"
echo "[INFO] Connection status appended" | tee -a $LOG_FILE

# 4. Delete (d) → Remove DB_NAME if present
sedi "/^db_name/d" "$CONFIG_FILE"
echo "[INFO] Old db_name entry deleted (if present)" | tee -a $LOG_FILE

# 5. Print (p) → Preview first 10 lines
echo "[INFO] Preview of updated config:" | tee -a $LOG_FILE
sed -n '1,10p' "$CONFIG_FILE" | tee -a $LOG_FILE

echo "[SUCCESS] Configuration update completed."

