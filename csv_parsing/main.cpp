//
//  main.cpp
//  csv_parsing
//
//  Created by David Kapatsa on 09.12.2019.
//  Copyright © 2019 David Kapatsa. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include "errorHandler.hpp"

int main(int argc, const char * argv[]) {

    std::vector<std::string> names;
    std::string tempName;
    char tempChar = 'a';
    double tempNum;
    std::vector<std::vector<double>> numbers;
    std::vector<double> numbersRow;
    std::ifstream f(
    "/Users/davidkapatsa/Documents/Programming/4th_year/csv_parsing/csv_parsing/input.txt"
                    );
    ErrorHandler err;
    int rows{};
    int cols{};
    int currentRow{};
    int currentCol{};
    double number;
    
    try {
        //INITIAL CHECK: OPEN/EMPTY, COLUMNS, ROWS
        if (f.is_open() == 0){
            err.set(FILEOPEN_ERR);
            throw err;
        }
        while(getline(f, tempName, '\n')){
            ++rows;
        }
        if(rows == 0){
            err.set(EMPTY_FILE_ERR);
            throw err;
        }
        f.clear();
        f.seekg(0L, std::ios::beg);
        while (tempChar !='\n') {
            tempChar = f.get();
            if(tempChar == ',') ++cols;
        }
        if(cols == 0){
            err.set(EMPTY_FILE_ERR);
            throw err;
        }
        //IF ALL IS WELL, WE PROCEED
        --rows;
        ++cols;
        std::cout << "The text file contains " << cols <<
                        " columns(s) of data" << std::endl;
        std::cout << "The text file contains " << rows <<
                            " row(s) of data" << std::endl;
        
        f.clear();
        f.seekg(0L, std::ios::beg);
        
        //READING COLUMN NAMES FIRST
        for(int i = 0; i < cols-1; ++i){
            getline(f, tempName, ',');
            names.push_back(tempName);
            std::cout << names[i];
        }
        getline(f, tempName, '\n');
        names.push_back(tempName);
        
        //READING ROWS ONE BY ONE
        //POSSIBLE PROBLEMS:
        //1. NOT A NUMBER
        //2. LESS ELEMENTS THAN NO. OF COLUMNS
        //3. WRONG DELIMITER
        currentRow = 1;
        currentCol = 1;
        tempChar = ' ';
        
        for (; currentRow <= rows; ++currentRow, tempChar = ' ', currentCol = 1) {
            while(tempChar != '\n' && tempChar != '\377'){
                f >> tempNum; //get a number
                if (f.fail()) {
                    err.set(NOTNUM_ERR, currentRow, currentCol);
                    throw err;
                }
                numbersRow.push_back(tempNum); //record number into vector
                std::cout << std::endl << std::setprecision(10) << "Number read: " << tempNum;
                tempChar = f.get(); //get next character
                std::cout << std::endl << "Char read: " << tempChar;
                if(tempChar != ',' && tempChar != '\n' && tempChar != '\377'){
                    err.set(DELIM_ERR, currentRow, currentCol);
                    throw err;
                }
                std::cout << std::endl << "currentCol: " << currentCol;
                if((tempChar == '\n' || tempChar == '\377') && currentCol != cols){
                    err.set(END_OF_ROW_ERR, currentRow, currentCol);
                    throw err;
                }
                ++currentCol;
            }
            numbers.push_back(numbersRow);
            numbersRow.clear();
            std::cout << std::endl << "currentRow: " << currentRow;
        }
        
        //CONSOLE OUTPUT
        std::cout << std::endl << "OUTPUT:" << std::endl;
        for(int i = 0; i < cols; ++i){
            std::cout <<  std::setw(13) << names[i];
        }
        std::cout << std::endl;
        for(int i = 0; i < rows; ++i){
            for(int j = 0; j < cols; ++j){
            std::cout <<  std::setw(13) << numbers[i][j];
            }
            std::cout << std::endl;
        }
        
    } catch (ErrorHandler err) {
        std::cout << std::endl;
        switch (err.errCode) {
            case FILEOPEN_ERR:
                std::cout <<
                "ERROR: The text file cannot be opened" << std::endl;
                break;
            case EMPTY_FILE_ERR:
                std::cout <<
                "ERROR: The text file is empty" << std::endl;
                break;
            case NOTNUM_ERR:
                std::cout <<
                "ERROR: Not a number in row " << err.posRow <<
                ", column " << err.posCol << std::endl;
                break;
            case DELIM_ERR:
                std::cout <<
                "ERROR: Wrong delimiter in row " << err.posRow <<
                ", column " << err.posCol << std::endl;
                break;
            case END_OF_ROW_ERR:
                std::cout <<
                "ERROR: Number of columns in row " << err.posRow <<
                " is incorrect" << std::endl;
                break;
            default:
                break;
        }
        return -1;
    }
    
    return 0;
}

/*
try {
    std::cout << "Throwing an integer exception...\n";
    throw 42;
} catch (int i) {
    std::cout << " the integer exception was caught, with value: " << i << '\n';
}
*/
