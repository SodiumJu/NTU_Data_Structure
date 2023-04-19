#!/bin/bash

cd "/Users/user/Desktop/DS HW2";
g++ main.cpp functions.cpp HashTable.cpp -o FindPasswords_User;
./FindPasswords_User;

# For batch inputs
#g++ main.cpp functions.cpp HashTable.cpp -o FindPasswords;
#./FindPasswords < "list_pa2.txt" > "result_pa2.txt";