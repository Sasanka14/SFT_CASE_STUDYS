
# 🏦 Bank Management System in C  

## 🔹 About the Project  
This **Bank Management System** is a simple and **secure** console-based program written in **C**. It allows users to **create accounts, deposit and withdraw money, check balances, and delete accounts**, all while ensuring **security with SHA-256 hashed PINs and masked input**.  

---

## ⚡ Features  
✅ **Secure Authentication** – PIN input is **hashed using SHA-256** and hidden during entry  
✅ **Account Management** – Create, delete, and manage multiple accounts  
✅ **Transaction Logging** – Tracks deposits, withdrawals, and account activity  
✅ **Data Persistence** – Stores accounts using **file handling** for long-term access  
✅ **Interest Calculation** – Applies interest to savings accounts automatically  
✅ **Cross-Platform Compatibility** – Works on **Windows**, with potential Linux support  
✅ **Prevents Duplicate Accounts** – Ensures unique **names and account numbers**  
✅ **Efficient Data Handling** – Uses **optimized search functions**  

---

## 🛠️ Installation & Usage  

### **🔹 Requirements**  
- **GCC Compiler** (MinGW for Windows)  
- **OpenSSL** (for SHA-256 hashing)  

### **🔹 Compile & Run**  
```sh
gcc bank.c -o bank.exe -lssl -lcrypto
./bank.exe
```
> **Note:** Use `-lssl -lcrypto` to link OpenSSL for hashing.

### **🔹 How to Use**  
1️⃣ **Run the program**  
2️⃣ **Choose an option** (Create Account, Deposit, Withdraw, etc.)  
3️⃣ **Follow the on-screen instructions**  
4️⃣ **Securely enter PINs** when required  

---

## 📁 File Structure  
```bash
📂 Bank-Management-System/
 ├── bank.c      # Main C source file
 ├── accounts.dat # Stores account data
 ├── transactions.log # Logs transactions
 ├── README.md   # Project documentation
```

---

## 🚀 Features to be Added  
📌 **Graphical User Interface (GUI)** – Upgrade to a GUI-based system  
📌 **Multi-User Access** – Allow multiple users to manage accounts  
📌 **Linux Support** – Use `termios.h` instead of `_getch()` for compatibility  
📌 **Enhanced Security** – Encrypt stored account data  

---

## 🏷️ Tags  
`C` `bank-management` `finance` `secure-banking` `account-management` `sha256` `file-handling` `transactions`  

---

## 📜 License  
🔒 This project is **open-source** under the **MIT License**. Feel free to use and improve it!  

---

## 👨‍💻 Author  
💡 **Sasanka Sekhar Kundu** – *Developer*  
🌐 GitHub: **[Your GitHub Profile](https://github.com/Sasanka14)**  
📧 Email: **sasankasekharkundu24@gmail.com**  
