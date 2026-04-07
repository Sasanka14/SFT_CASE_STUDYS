#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>  
#include <conio.h>  // For Windows secure PIN input

#define MAX_ACCOUNTS 100
#define MAX_NAME_LEN 50
#define PIN_LEN 4
#define HASH_LEN 65  

typedef enum { SAVINGS, CURRENT } AccountType;

typedef struct {
    char name[MAX_NAME_LEN];
    int accountNumber;
    char hashedPIN[HASH_LEN];
    float balance;
    AccountType accountType;
    time_t lastInterestApplied;
} Account;

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

// Function to securely input PIN (Windows)
void getMaskedPIN(char *pin) {
    printf("\nEnter your 4-digit PIN: ");
    
    for (int i = 0; i < PIN_LEN; i++) {
        pin[i] = _getch();  // Read without displaying
        printf("*");        // Show * for each character entered
    }
    
    pin[PIN_LEN] = '\0';  // Null-terminate the string
    printf("\n");  // Move to new line
}

// Hashing function for PIN security
void hashPIN(const char *pin, char *hashedPIN) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)pin, strlen(pin), hash);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hashedPIN + (i * 2), "%02x", hash[i]);
    }
    hashedPIN[64] = '\0';  
}

// Authenticate user with hashed PIN
int authenticateUser(int accountIndex) {
    char pin[PIN_LEN + 1], hashedInput[HASH_LEN];

    getMaskedPIN(pin);

    // Hash input PIN and compare with stored hash
    hashPIN(pin, hashedInput);
    if (strcmp(hashedInput, accounts[accountIndex].hashedPIN) == 0) {
        return 1;
    } else {
        printf("\nIncorrect PIN. Access Denied!\n");
        return 0;
    }
}

// Find an account by account number
int findAccount(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }
    return -1;
}

// Save accounts to file
void saveAccountsToFile() {
    FILE *file = fopen("accounts.dat", "wb");
    if (!file) {
        printf("\nError saving accounts!\n");
        return;
    }
    fwrite(&accountCount, sizeof(int), 1, file);
    fwrite(accounts, sizeof(Account), accountCount, file);
    fclose(file);
}

// Load accounts from file
void loadAccountsFromFile() {
    FILE *file = fopen("accounts.dat", "rb");
    if (!file) return;  
    fread(&accountCount, sizeof(int), 1, file);
    fread(accounts, sizeof(Account), accountCount, file);
    fclose(file);
}

// Function to check if the name is already in use
int isNameTaken(const char *name) {
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].name, name) == 0) {
            return 1; // Name already exists
        }
    }
    return 0; // Name is unique
}

// Create a new bank account with a unique name
void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("\nMaximum account limit reached.\n");
        return;
    }

    Account newAccount;
    getchar();

    // Check for unique name
    do {
        printf("\nEnter Your Name: ");
        fgets(newAccount.name, MAX_NAME_LEN, stdin);
        newAccount.name[strcspn(newAccount.name, "\n")] = '\0'; // Remove newline character

        if (isNameTaken(newAccount.name)) {
            printf("\nThis name is already registered! Please use a different name.\n");
        }
    } while (isNameTaken(newAccount.name));

    // Ensure unique account number
    do {
        printf("\nEnter a Unique Account Number: ");
        scanf("%d", &newAccount.accountNumber);
        if (newAccount.accountNumber <= 0) {
            printf("\nInvalid Account Number! Must be positive.\n");
        }
        if (findAccount(newAccount.accountNumber) != -1) {
            printf("\nAccount number already exists. Choose another.\n");
        }
    } while (newAccount.accountNumber <= 0 || findAccount(newAccount.accountNumber) != -1);

    // Securely input and hash the PIN
    char pin[PIN_LEN + 1];
    do {
        getMaskedPIN(pin);
    } while (strlen(pin) != PIN_LEN);

    hashPIN(pin, newAccount.hashedPIN);

    printf("\nSelect Account Type (0 = SAVINGS, 1 = CURRENT): ");
    scanf("%d", (int*)&newAccount.accountType);

    newAccount.balance = 0;
    newAccount.lastInterestApplied = time(NULL);

    // Store the new account
    accounts[accountCount++] = newAccount;
    saveAccountsToFile();

    printf("\nAccount Created Successfully!\n");
}


// Deposit money into an account
void depositAmount() {
    int accountNumber, index;
    float amount;
    printf("\nEnter Account Number: ");
    scanf("%d", &accountNumber);
    index = findAccount(accountNumber);
    if (index == -1) {
        printf("\nAccount Not Found!\n");
        return;
    }
    printf("\nEnter Amount to Deposit: ");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("\nInvalid Amount!\n");
        return;
    }
    accounts[index].balance += amount;
    saveAccountsToFile();
    printf("\nDeposit Successful! New Balance: Rs %.2f\n", accounts[index].balance);
}

// Withdraw money from an account
void withdrawAmount() {
    int accountNumber, index;
    float amount;
    printf("\nEnter Account Number: ");
    scanf("%d", &accountNumber);
    index = findAccount(accountNumber);
    if (index == -1 || !authenticateUser(index)) {
        return;
    }
    printf("\nEnter Amount to Withdraw: ");
    scanf("%f", &amount);
    if (amount <= 0 || amount > accounts[index].balance) {
        printf("\nInvalid or Insufficient Funds!\n");
        return;
    }
    accounts[index].balance -= amount;
    saveAccountsToFile();
    printf("\nWithdrawal Successful! New Balance: Rs %.2f\n", accounts[index].balance);
}

// Balance enquiry for an account
void balanceEnquiry() {
    int accountNumber, index;
    printf("\nEnter Account Number: ");
    scanf("%d", &accountNumber);
    index = findAccount(accountNumber);
    if (index == -1 || !authenticateUser(index)) {
        return;
    }
    printf("\nAccount Balance: Rs %.2f\n", accounts[index].balance);
}

// Delete an account
void deleteAccount() {
    int accountNumber, index;
    printf("\nEnter Account Number to Delete: ");
    scanf("%d", &accountNumber);
    index = findAccount(accountNumber);
    if (index == -1 || !authenticateUser(index)) {
        return;
    }

    for (int i = index; i < accountCount - 1; i++) {
        accounts[i] = accounts[i + 1];
    }
    accountCount--;
    saveAccountsToFile();
    printf("\nYour account has been successfully closed. We appreciate your trust and hope to serve you again in the future!\n");
}

// Display menu and handle user input
void menu() {
    int choice;
    do {
        printf("\n===== Bank Management System =====\n");
        printf("1. Create Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Balance Enquiry\n");
        printf("5. Delete Account\n");
        printf("6. Exit\n");
        printf("Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: depositAmount(); break;
            case 3: withdrawAmount(); break;
            case 4: balanceEnquiry(); break;
            case 5: deleteAccount(); break;
            case 6: printf("\nThank you for using our bank system!\n"); break;
            default: printf("\nInvalid Choice! Try again.\n");
        }
    } while (choice != 6);
}

// Main function
int main() {
    loadAccountsFromFile();
    menu();
    return 0;
}
