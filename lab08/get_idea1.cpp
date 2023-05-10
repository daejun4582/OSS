/*
 Authors: Haval Ahmed and Ryan Stevens
 Description: Program #1. Hex Dump Program
 Error Checking: Check if valid file type
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <bitset>

using namespace std;

void toHex(char * charArray[]);
void toBinary(char * charArray[]);

int main(int argc, char *argv[])
{
    // Check Argument Count
    if (argc == 2)
    {
        // Create Variable To Hold File Name
        ifstream fileName(argv[1]);
        // Check If Valid
        if (!fileName.is_open())
        {
            // Print Error.
            cout << "Error. Not Valid File Type Or File " << endl;
            return 0;
        }
        
        // Call Convert Function
        toHex(argv);
        
    }
    
    else if (argc == 3)
    {
        // Create Variable To Hold File Name
        ifstream fileName(argv[2]);
        // Check If Valid
        if (!fileName.is_open())
        {
            // Print Error
            cout << "Invalid File Type or File Is Not Open. " << endl;
            return 0;
        }
        
        // Call Convert Function
        toBinary(argv);
        
    }
    else if (argc < 1 || argc > 3)
        cout << "Error! Argument Count Not Valid. Please Re-Run With Valid Argument Count";
    
    return 0;
}

/*
 / toHex takes the array passed to it, which
 / is the file and converts each byte of info
 / and converts it to hex. It also prints
 / out the address and the ascii representation
 /                                           */
void toHex(char * charArray[])
{
    // Address Variable
    unsigned long address = 0;
    
    // Read 16 Characters At A Time
    const int sizeArray = 16;
    
    // ASCII Variable Size For Printing
    const int asSize = 21;
    
    // Array Of 16 Characters
    char * hexArray = new char[sizeArray];
    
    // Create A File
    ifstream filePassed(charArray[1]);
    
    while (filePassed.good() && (filePassed.read(hexArray, sizeArray) || (filePassed.gcount() > 0)) && (filePassed != '\0'))
    {
        // How Many Bytes Read In
        int sizeOfRead = filePassed.gcount();
        
        //display the address
        cout << setw(7) << setfill('0') << hex << address << ":";
        
        // Hex Code
        for (int i = 0; i < sizeArray; i++)
        {
            if( i % 2 == 0)
                cout << ' ';
            if( i < sizeOfRead)
                cout << setw(2) << (unsigned int)(unsigned char)hexArray[i];
        }
        
        // Put Space Between Next Column
        cout << " ";
        
        // Calculate Offset For Column Sizing
        int offset = 1 + (2 * (sizeArray - sizeOfRead));
        
        if (sizeOfRead < sizeArray)
            cout << setfill(' ') << setw(offset);
        
        for(int i = 0; i < sizeOfRead; i ++)
        {
            if(hexArray[i] < asSize) // lower than the printable ascii table
                cout << '.';
            else
                cout << hexArray[i];
        }
        
        // Print New Line
        cout << "\n";
        // Increment Address
        address += 16;
    }
    
    // Close File
    filePassed.close();
    
}


/*  toBinary takes the array passed to it, which
 / is the file and converts each byte of info
 / and converts it to binary. It also prints
 / out the address and the ascii representation
 /                                            */
void toBinary(char * charArray[])
{
    // Address Variable
    unsigned long address = 0;
    
    // Read 6 Characters At A Time
    const int sizeArray = 6;
    
    // 8 Bit Variable Variable
    const int eightCount = 8;
    
    // Max ASCII Size
    const int aSize = 21;
    
    // Array Of 6 Characters
    char * binaryArray = new char[sizeArray];
    
    // Create A File
    ifstream filePassed(charArray[2]);
    
    while (filePassed.good() && (filePassed.read(binaryArray, sizeArray) || (filePassed.gcount() > 0)) && (filePassed != '\0'))
    {
        // Read In
        int sizeOfRead = filePassed.gcount();
        
        //display the address
        cout << setw(7) << setfill('0') << hex << address << ": ";
        
        //display the bits
        for(int i = 0; i < sizeOfRead; i++)
            cout << bitset<8>((unsigned int)(unsigned char)(binaryArray[i])) << " ";
        
        // Calculate Offset For Colums
        int offset = 1 + (eightCount * (sizeArray - sizeOfRead)) + (sizeArray - sizeOfRead);
        
        if (sizeOfRead < sizeArray)
            cout << setfill(' ') << setw(offset);
        
        // Display the ascii
        for(int i = 0; i < sizeOfRead; i++)
        {
            if( binaryArray[i] < aSize)
                cout << '.';
            else
                cout << binaryArray[i];
        }
        
        // Print New Line
        cout << "\n";
        // Increment Address
        address += 6;
    }
    
    // Close File
    filePassed.close();
}