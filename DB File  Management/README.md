# Database Configuration Updater – Automating DB File Management with Shell Scripting

## 📌 Overview
This project is a **Database Configuration Updater** built using **Linux Shell Scripting**.  
It automates the management of configuration files by:
- Replacing outdated host, username, and password values  
- Inserting backup information  
- Appending connection status  
- Deleting outdated entries  
- Previewing modifications  

The script also includes **automatic backup creation**, **logging with timestamps**, and **cross-platform compatibility** (Linux & macOS).

---

## 🚀 Features
- ✅ Automatic backup before every update  
- ✅ User input for new DB credentials (host, user, password)  
- ✅ Replace old credentials using `sed`  
- ✅ Insert backup host information  
- ✅ Append database connection status  
- ✅ Delete redundant/obsolete entries  
- ✅ Preview updated file (first 10 lines)  
- ✅ Logs all actions into `update_log.txt`  

---

## 📂 Project Structure
```

db_config_project/
│── db_config.conf              # Sample configuration file
│── db_config_update.sh         # Main automation script
│── update_log.txt              # Log file (created automatically)
│── db_config_backup_*.conf     # Timestamped backup files

````

---

## ⚙️ Usage

### 1. Clone Repository
```bash
git clone https://github.com/Sasanka14/db_case_study.git
cd db-config-updater
````

### 2. Make Script Executable

```bash
chmod +x db_config_update.sh
```

### 3. Prepare Sample Config

Edit `db_config.conf` with initial values:

```ini
host=old_host
user=old_user
password=old_pass
db_name=old_db
```

### 4. Run Script

```bash
./db_config_update.sh
```

### 5. Outputs

* Updated `db_config.conf` with new details
* Backup file created → `db_config_backup_<timestamp>.conf`
* Log file → `update_log.txt`

---

## 📝 Example Before vs After

**Before:**

```ini
host=old_host
user=old_user
password=old_pass
db_name=old_db
```

**After:**

```ini
host=192.168.1.2
backup_host=192.168.1.2
user=Sasanka
password=MySecret123
connection_status=active
```

---

## 📊 Analysis Visuals

* Performance Factors (Pie Chart)
* Challenges vs Solutions (Bar Chart)

*(Refer to full case study report for charts & detailed analysis.)*

---

## 🔮 Future Improvements

* Encrypt sensitive credentials
* Add Git-based version control for config files
* Multi-database support (MySQL, PostgreSQL, etc.)
* Integration with DevOps tools like **Ansible** or **Chef**
* Web-based UI for non-technical administrators

---

## 📚 References

* Silberschatz, A., Galvin, P. B., & Gagne, G. – *Operating System Concepts*
* Robbins, A. – *Bash Pocket Reference*
* Shotts, W. E. – *The Linux Command Line: A Complete Introduction*
* GNU Sed Documentation → [https://www.gnu.org/software/sed/manual](https://www.gnu.org/software/sed/manual)
* DigitalOcean Tutorials → [https://www.digitalocean.com/community/tutorials](https://www.digitalocean.com/community/tutorials)

---

## 👨‍💻 Author

**Sasanka Sekhar Kundu**
B.Tech CSE, ITM Skills University
