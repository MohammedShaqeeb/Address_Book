/*DOCUMENTATION:
  NAME: N.MOHAMMED SHAQEEB
  REG_NO: 25031_124
  START_DATE: 10-12-2025
  END_DATE: 14-12-2025
  DESC: This is an Address Book with a simple CRUDS operations, with data backup in Separate File and data re-loading using Concepts of Structures, Strings, Files I/O and Pointers 
*/
#include <stdio.h>
#include "contact.h"

// Text color macros
#define RESET   "\033[0m"
#define BLACK   "\033[0;30m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"


int main() 
{
    int choice;
    AddressBook addressBook;
    initialize(&addressBook); // Initialize the address book

    do 
    {
        //Main Menu
        printf(BLUE"\nAddress Book Menu:\n"RESET);
        printf(BLUE"1. Create contact\n"RESET);
        printf(BLUE"2. Search contact\n"RESET);
        printf(BLUE"3. Edit contact\n"RESET);
        printf(BLUE"4. Delete contact\n"RESET);
        printf(BLUE"5. List all contacts\n"RESET);
    	printf(BLUE"6. Save and Exit\n"RESET);		
        printf(WHITE"Enter your choice: "RESET);
        scanf("%d", &choice);
        
        switch (choice) 
        {
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                searchContact(&addressBook);
                break;
            case 3:
                editContact(&addressBook);
                break;
            case 4:
                deleteContact(&addressBook);
                break;
            case 5:          
                listContacts(&addressBook);
                break;
            case 6:
                printf(GREEN"Saving and Exiting...\n"RESET);
                saveContactsToFile(&addressBook);
                break;
            default:
                printf(YELLOW"Invalid choice. Please try again.\n"RESET);
        }
    } while (choice != 6);
    

    return 0;
}
