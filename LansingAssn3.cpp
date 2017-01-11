//***************************************************************************
//  FILENAME:	 LansingAssn3.cpp
//  DESCRIPTION: This program manages data vehicles for a car rental company.
//               The user has the option to use data from a file about the
//               rental vehicles. If used the data from the file is stored into
//               an array of records. The user can display the current vehicles,
//               add vehicles, or delete vehicles from the array of records. 
//               When the user decides to exit the program the user has the 
//               option to save the data in the array of records to a file for
//               later use. 
//               file example format:
//               AA1234 12333 0
//               AA4321 2322 1
//
//  DATE:        November 10, 2014
//  DESIGNER:	 Robert Lansing
//  FUNCTIONS:	 programDescription - Displays the purpose of the program
//               useDataFile - Prompts for choice to use data file 
//               readFile - reads data file
//               menuPrompt - prompts user to select from menu
//               displayVehOnFile - displays vehicles in array
//               addVehicles - adds vehicle records to array
//               licInputFx - prompts for, reads in, and stores license number
//               mileageInputFx - prompts for, reads in, and stores mileage
//               rentedInputFx - prompts for, reads in, and stores rented status
//               deleteItem - deletes item from array
//               exitSave - saves file if user wishes to save changes
//***************************************************************************

#include <iostream> //for I/O
#include <iomanip>  //for output manipulators
#include <fstream>  //for stream variable
#include <string>   //for strings
#include <cctype>   //for character checking
#include <cmath>    //to use toupper function

using namespace std;

//global constants
const int LIC_MAX = 6, //max number of characters in license plate string
          MAX = 500, //max number of records in array of records
          L_NUM_TWO = 2;
const string RENTAL_FILE = "RENTALS.TXT";

//global structure definition
struct vehicleRec 
{
    string licenseNum;
    int mileage;
    bool rented;
};

// prototypes
void programDescription();
void useDataFile(bool& cont, int& count, vehicleRec vehInfoArray[]);
void readFile(ifstream& inFile, int& count, vehicleRec vehInfoArray[]);
void menuPrompt(int count, vehicleRec vehInfoArray[]);
void displayVehOnFile(int count, vehicleRec vehInfoArray[]);
void addVehicles(int& count, vehicleRec vehInfoArray[]);
string licInputFx();
int mileageInputFx();
bool rentedInputFx();
void deleteItem(int& count, vehicleRec vehInfoArray[]);
void exitSave(int count, vehicleRec vehInfoArray[]);
//*********************************************************************
// FUNCTION: Main
// DESCRIPTION: Declares local variables, calls other functions, exits program
// CALLS TO: programDescription, useDataFile, menuPrompt
//**********************************************************************

int main()
{
    
    //Variable declarations
    bool cont = true;  //used to determine if user will continue without file
    
    vehicleRec vehInfoArray[MAX];
    
    int count = 0; //declare and initialize count to 0 in case no input
                   //file is used
                          
    programDescription();
    
    useDataFile(cont, count, vehInfoArray);
    
    //if file is not present and user does not wish to continue
    if (!cont)   
    {
        cin.ignore();
        cin.get();
        return 1;
    }
    
    menuPrompt(count, vehInfoArray);
       
    

    cout << "Press [enter] to exit" << endl;
    cin.ignore();
    cin.get();
    return 0;
}
//*************************************************************************
//  FUNCTION:	 programDescription
//  DESCRIPTION: Displays program description
//  OUTPUT: 	 program purpose
//*************************************************************************
void programDescription()
{
    cout << "                 Car Rental Records Program" << endl
         << "----------------------------------------------------------------" 
         << endl
         << "This program manages the current rental vehicles of the company."
         << endl << endl
         << "Current data about the fleet of rental vehicles is displayed "
         << endl
         << "and updated using this program."
         << endl << endl
         << "The user is prompted to choose whether or not they wish to use"
         << endl
         << "data about the rental vehicles from a data file called RENTALS.TXT"
         << endl << endl
         << "The user can modify (add/delete) vehicles and save changes"
         << endl
         << "as desired."
         << endl
         << "----------------------------------------------------------------"
         << endl << endl << endl << endl;
}
//*********************************************************************
// FUNCTION: useDataFile
// DESCRIPTION: prompts user to decide whether or not to use data file, opens
//              and closes data file
// INPUT:
//       Parameters: cont - bool variable for
//                   decision to exit program if file does not exist, count -
//                   number of items in array, vehInfoArray - array of records
//                   containing vehicle information
// OUTPUT:
//        Parameters: cont - bool variable for decision to exit program if file
//                    does not exist, count - number of items in array
// CALLS TO: readFile
//**********************************************************************
void useDataFile(bool& cont, int& count, vehicleRec vehInfoArray[])
{   
    ifstream inFile;
    
    char choice,
         proceed;
       
    do
    {
        cout << "Load existing data from a file (Y/N)?";
        cin >> choice;
        
        choice = toupper(choice);
        
        if (choice == 'Y')
        {
            inFile.open(RENTAL_FILE.c_str());
            
            if (!inFile)
            {
                do
                {
                    cout << "File does not exist. Do you wish to proceed "
                         << "anyway (Y/N)?";
                    cin >> proceed;
                    
                    proceed = toupper(proceed);
                    
                    if (proceed == 'N')
                       cont = false;
                }
                while (proceed != 'N' and proceed != 'Y');
            }
            else
            {
                readFile(inFile, count, vehInfoArray);
                inFile.close();
            }    
        }
           
    }
    while (choice != 'N' and choice != 'Y');
       
}
//*********************************************************************
// FUNCTION: readFile
// DESCRIPTION: reads data into array of records from file
// INPUT:
//       Parameters: inFile - stream variable, count - number of items in array,
//                   vehInfoArray - array of records
//       File:       File contains license plate number, mileage, and rental
//                   status for one vehicle per line. License plate number is
//                   in all caps and formatted first two characters are letters
//                   followed by four numbers. The rental status is stored is
//                   bool value (1 or 0).
//                   format example:
//                   AA1234 50000 0
//                   YH4356 23050 1
// OUTPUT:
//        Parameters: inFile - ifstream variable, count - number of items, 
//                    vehInfoArray - array of records
//**********************************************************************
void readFile(ifstream& inFile, int& count, vehicleRec vehInfoArray[])
{
    int temp;      //used as temporary storage for boolean rented value
    
    
    inFile >> vehInfoArray[count].licenseNum; //Priming read
    
    while (inFile and count < MAX)
    {
        
        inFile >> vehInfoArray[count].mileage >> temp;
        
        //store bool value into rented variable
        vehInfoArray[count].rented = static_cast<bool>(temp);
        
        count++; 
        
        //if statement used so loop does not attempt store data out of bounds 
        //of array
        if (count < MAX)           
           inFile >> vehInfoArray[count].licenseNum;
    }
    
    if (count == MAX)
    {
        cout << "WARNING - There are " << MAX << " or more records in file."
             << endl
             << "All records over " << MAX << " will be ignored and not used."
             << endl << endl;
    }

    
}
//*********************************************************************
// FUNCTION: menuPrompt
// DESCRIPTION: Displays rental menu prompt.
// INPUT:
//       Parameters: count - number of items in array, vehInfoArray - array of
//                   records
// OUTPUT:
//        Parameters: vehInfoArray - array of records
// CALLS TO: displayVehOnFile, addVehicles, deleteItem, exitSave
//**********************************************************************
void menuPrompt(int count, vehicleRec vehInfoArray[])
{
    bool exit = false; //Used as text to exit loop
    
    char choice;
    
    do
    {
        cout << endl
             << endl
             << "                    RENTAL VEHICLES MENU"
             << endl
             << "************************************************************" 
             << endl
             << "S - Show vehicle list"
             << endl
             << "A - Add vehicle to list"
             << endl
             << "D - Delete vehicle"
             << endl
             << "E - Exit program"
             << endl
             << endl
             << "Please enter an option from above: ";
        cin >> choice;
        
        switch (choice)
        {
            case 's':
            case 'S':
                displayVehOnFile(count, vehInfoArray);
                break;
            case 'a':
            case 'A':
                addVehicles(count, vehInfoArray);
                break;
            case 'd':
            case 'D':
                deleteItem(count, vehInfoArray);
                break;
            case 'e':
            case 'E':
                exitSave(count, vehInfoArray);
                exit = true;
                break;
            default:
                cout << endl
                     << "Invalid selection - Valid selections are S, A, D, E"
                     << endl << endl;
        }
        
    }
    while (!exit);
    
}
//*********************************************************************
// FUNCTION: displayVehOnFile
// DESCRIPTION: Displays all vehicles stored in vehInfoArray (array of records)
// INPUT:
//       Parameters: count, vehInfoArray
//**********************************************************************
void displayVehOnFile(int count, vehicleRec vehInfoArray[])
{
    //display data stored in array. Count contains actual number of items
    //not starting at 0 for use of output to screen. if there are 4 items 
    //in array (array[0] through array[3]), count holds value of 4. 
    if (count == 0)
       cout << "There are no vehicles on file to display!" 
            << endl
            << endl;
            
    
    else
    {
        cout << "License" 
             << endl
             << "Number        Mileage        Rented"
             << endl
             << "--------      ---------      -------"
             << endl;
             
        for (int i = 0; i < count; i++)
        {
            cout << left << setw(14) << vehInfoArray[i].licenseNum << right 
                 << setw(9) << vehInfoArray[i].mileage << setw(6) << " ";
            
            //if else statement used to change boolean value to string for 
            //output     
            if (vehInfoArray[i].rented == true)
               cout << left << setw(6) << "yes" << endl;
               
            else                        
               cout << left << setw(6) << "no" << endl;
                 
            //pause so user can see records on screen and re-display heading
            if (i > 0 and i % 20 == 0)
            {
                cin.ignore();
                cin.get();
                
                cout << "License" 
                << endl
                << "Number        Mileage        Rented"
                << endl
                << "--------      ---------      -------"
                << endl;     
            }
        }
    }
    
    cin.ignore();
    cin.get();
    cout << endl << endl;
}
//*********************************************************************
// FUNCTION: addVehicles
// DESCRIPTION: adds vehicles to vehInfoArray
// INPUT:
//       Parameters: count - number of records, vehInfoArray - array of records
// OUTPUT:
//       Parameters: count - number of records, vehInfoArray - array of records
// CALLS TO: licInputFx, mileageInputFx, rentedInputfx
//**********************************************************************
void addVehicles(int& count, vehicleRec vehInfoArray[])
{
    char choice;  //used for yes or no to add another vehicle
    
    bool exit = false; //used in outer loop to determine if user wants to stop
                       //adding vehicles
                       
    
    if (count == MAX)
       cout << endl
            << endl
            << "Maximum rental vehicle record capacity reached! You must delete"
            << endl
            << "records to add room for new vehicles."
            << endl;
    else
       do
       {
           vehInfoArray[count].licenseNum = licInputFx();
           vehInfoArray[count].mileage = mileageInputFx();
           vehInfoArray[count].rented = rentedInputFx();
           count++;
           
           if (count < MAX)
              do
              {
                  cout << endl
                       << endl
                       << "Add another vehicle? (Y/N): ";
                  cin >> choice;
                  
                  choice = toupper(choice); //used so loop test is limited to 
                                            //two characters 'Y' or 'N'
                  
                  switch (choice)
                  {
                      case 'Y':
                          break;
                      case 'N':
                          exit = true;
                          break;
                      default:
                          cout << endl
                               << "Invalid selection. Choices are Y or N."
                               << endl;
                  }
                  
              }
              while (choice != 'Y' and choice != 'N');
              
              if (count == MAX)
                 cout << endl
                      << endl
                      << "Maximum rental vehicle record capacity reached! You "
                      << endl
                      << "must delete records to add new vehicles."
                      << endl;
              
              
       }
       while (!exit and count < MAX);
         
}
//*********************************************************************
// FUNCTION: licInputFx
// DESCRIPTION: prompts, reads in, and error checks user entered license string
// OUTPUT:
//        Return Val: string - validated user license plate string
//**********************************************************************
string licInputFx()
{
    int i = 0,
        size;
    
    string licenseNum;
    
    char item;
    
    bool format;
    
    
    cin.ignore(200, '\n'); //clear input stream so getline does not read 
                           //previous input from user menu

    do
    {
        
        format = true;
        
        cout << endl
             << endl
             << "Please the enter license plate number. Correct format is 6 "
             << endl
             << "characters long (2 letters followed by 4 numbers) with no" 
             << "spaces: ";
        getline(cin, licenseNum); //read in string including whitespaces if any
        
        size = licenseNum.length(); //measure length of string
        
        if (size > LIC_MAX)
        {
            cout << endl
                 << "Error - The license plate number you entered has too many"
                 << endl
                 << "characters."
                 << endl << endl;
                 
            format = false;
        }
        
        else if (size < LIC_MAX)
        {
            cout << endl
                 << "Error - The license plate number you entered does not have"
                 << endl
                 << "enough characters. "
                 << endl << endl;
                 
            format = false;      
        }
        
        while (i < size and format)
        {
            item = licenseNum[i]; //stored character of array for error checking
            
            if (i < L_NUM_TWO) //if the index is at place 0 or 1 in string
            {
                item = toupper(item); //make letter uppercase for two purposes
                                      //1. error checking 2. comparing strings
                                      //when searching for items to delete
                
                licenseNum[i] = item; //store uppercase letter into string 
                
                if (!isupper(item)) //if item is not a letter isupper will be
                {                   //false
                    cout << endl
                         << "Error - incorrect format. The first two characters"
                         << endl
                         << "of the license plate must be letters."
                         << endl << endl;
                         
                    format = false;
                
                }
            }
            
            else if (!isdigit(item)) 
            {
                cout << endl
                     << "Error - incorrect format. The last four characters"
                     << endl
                     << "of the license plate must be numbers."
                     << endl << endl;
                    
                format = false;
            }
                
            i++;
        }
    }
    while (!format);
    
    return licenseNum;
}
//*********************************************************************
// FUNCTION: mileageInputFx
// DESCRIPTION: prompts, reads in, and error checks user entered mileage
// OUTPUT:
//        Return Val: int - validated mileage entered by user
//**********************************************************************
int mileageInputFx()
{
    int mileage;
    
    bool format;
    
    
    do
    {
        format = true;
        
        cout << endl << endl
             << "Please enter the current vehicle mileage: ";
        cin >> mileage;
        
        if (mileage < 1 or !cin)
        {
           cout << endl << endl
                << "Error - Mileage must be greater than zero!"
                << endl << endl;
           
           
           //restores I/O if user inputs a letter after the prompt
           cin.clear();           
           cin.ignore(200, '\n');
           
           format = false;
        }

    }
    while (!format);
    
    return mileage;
}
//*********************************************************************
// FUNCTION: rentedInputFx
// DESCRIPTION: prompts, reads in, and error checks user entered rental status
// OUTPUT:
//        Return Val: bool - validated rental status entered by user
//**********************************************************************
bool rentedInputFx()
{
    char rentedPrompt;
    
    bool format,
         rentStatus;
         
    do
    {
        format = true;
        
        cout << endl << endl
             << "Is vehicle rented? (Y/N): ";
        cin >> rentedPrompt;
        
        rentedPrompt = toupper(rentedPrompt);
        
        switch (rentedPrompt)
        {
            case 'Y':
                rentStatus = true;
                break;
            case 'N':
                rentStatus = false;
                break;
            default:
                cout << endl << endl
                     << "Error - Invalid entry. Choices are Y or N.";
                format = false;
        }
    }
    while (!format);
    
    return rentStatus;
}
//*********************************************************************
// FUNCTION: deleteItem
// DESCRIPTION: deletes a record from vehInfoArray
// INPUT:
//       Parameters: count - number of items in array, vehInfoArray - vehicle
//                   records
// OUTPUT:
//       Parameters: count - number of items in array, vehInfoArray - vehicle
//                   records 
// CALLS TO: licInputFx
//**********************************************************************
void deleteItem(int& count, vehicleRec vehInfoArray[])
{
    string itemToFind;
    
    bool found;
    
    int i;
    
    
    
    if (count < 1)
       cout << endl << endl
            << "There are no vehicles on file!"
            << endl << endl;
       
    else
    {
         
        
        cout << endl << endl
             << "The following vehicles are on file:"
             << endl << endl;
        
        for (i = 0; i < count; i++)
            cout << vehInfoArray[i].licenseNum << " ";
            
        itemToFind = licInputFx();
        
        
        i = 0; //set i back to zero for use in loop
        found = false;
        
        while (i < count and !found)
        {
            if (vehInfoArray[i].licenseNum == itemToFind) //if item is found
               found = true;                              //exit loop
            else
               i++;
        }
        
        if (found)
        {
            vehInfoArray[i] = vehInfoArray[count - 1];
            count--;
            
            cout << endl << endl
                 << "Record for vehicle with license plate number " 
                 << itemToFind
                 << " has been deleted."
                 << endl << endl;
        }
        else
            cout << endl << endl
                 << "There is no vehicle with that license plate number on "
                 << "file."
                 << endl << endl;   
    }
}
//*********************************************************************
// FUNCTION: exitSave
// DESCRIPTION: prompts user to decide if changes should be saved, and saves
//              changes if the user wishes to save
// INPUT:
//       Parameters: count - number of items in array, vehInfoArray - vehicle
//                   records
// OUTPUT:
//       Parameters: count - number of items in array, vehInfoArray - vehicle
//                   records 
//**********************************************************************
void exitSave(int count, vehicleRec vehInfoArray[])
{
    ofstream outData;
    
    char choice;
    
    bool format;
    
    
    
    do
    {
        format = true;
        
        
        cout << endl << endl
             << "Do you wish to save/re-save? (Y/N): ";
        cin >> choice;
        
        switch (choice)
        {
            case 'y':
            case 'Y':
                outData.open(RENTAL_FILE.c_str());
                
                for (int i = 0; i < count; i++)
                    outData << vehInfoArray[i].licenseNum << " "
                            << vehInfoArray[i].mileage << " "
                            << vehInfoArray[i].rented << endl;
                            
                outData.close();
                
                cout << endl << endl
                     << "Data saved!"
                     << endl << endl;
                break;
            case 'n':
            case 'N':
                cout << endl << endl
                     << "Any changes made will not be saved!"
                     << endl << endl;
                break;
            default:
                cout << endl << endl
                     << "Invalid Choice - Please enter Y or N"
                     << endl << endl;
                format = false;
        }
        
    }
    while (!format);  
}

