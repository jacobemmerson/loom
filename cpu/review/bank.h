#pragma once

#include <string>
#include <mutex>

// declarations

class Account {
    private:
        const std::string name;
        const std::string pin;
        long total = 0;

        std::mutex mtx;
        bool check_pin(std::string pin) const {return pin == this->pin;}
        
    public:
        Account(std::string name, std::string pin);

        void deposit(int amount);

        int withdraw(int amount, std::string pin);

        long check_balance(std::string pin);
};