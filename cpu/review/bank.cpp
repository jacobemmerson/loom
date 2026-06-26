#include "bank.h"

#include <string>
#include <iostream>
#include <mutex>

// Constructor implementation 
Account::Account(std::string name, std::string pin) : name(name), pin(pin) {}


// Deposit
void Account::deposit(int amount) {
    std::lock_guard<std::mutex> lock(mtx);
    total += amount;
}

// Withdraw
int Account::withdraw(int amount, std::string pin) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!check_pin(pin)) {
        std::cout << "Incorrect Pin" << std::endl;
        return 0;
    }

    if (amount > total) {
        std::cout << "Withdraw request is greater than total amount in bank account. Withdraw denied." << std::endl;
    }

    // if pin matches
    total -= amount;
    return amount;
}

// Check Balance
long Account::check_balance(std::string pin) {
    std::lock_guard<std::mutex> lock(mtx);
    if (!check_pin(pin)) {
        std::cout << "Incorrect Pin" << std::endl;
        return 0;
    }

    std::cout << "Current Balance: " << total << std::endl;

    return total;
}