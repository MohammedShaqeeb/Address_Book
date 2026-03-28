#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include <ctype.h>

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

//Function to print Table structure
void print_table()
{
    printf(GREEN"--------------------------------CONTACTS----------------------------------\n"RESET);
    printf(GREEN"--------------------------------------------------------------------------\n"RESET);
    printf(GREEN"|%-6.6s|%-20.20s|%-15.10s|%-28s|\n"RESET,"S.No","Name","Phone","Email-ID");
    printf(GREEN"--------------------------------------------------------------------------\n"RESET);
}
//Function to print a single contact
void print_contact(AddressBook *addressBook,int sno,int ind)
{
    printf(GREEN"|%-6d|%-20.20s|%-15.10s|%-28.28s|\n"RESET,sno,addressBook->contacts[ind].name,addressBook->contacts[ind].phone,addressBook->contacts[ind].email);
}
//Function which checks whether a name input is valid name or not 
int validate_name(char name[])
{
    int i=0;
    while(name[i])//Loop checks every single character 
    {
        if(!((name[i]>='a'&&name[i]<='z')||(name[i]>='A'&&name[i]<='Z')||(name[i]=='.')||(name[i]==' ')))
        {
            return 0;
        }
        i++;
    }
    return 1;
}
//Function which checks whether the phone number is valid or not
int validate_phone(char phone[],AddressBook *addressBook,int uniq,int skip)
{
    int len=strlen(phone);//Gets length of input
    if(len==10)//Phone number should be exactly 10 digits
    {
        int i=0;
        while(phone[i])//Checks whether every character is digit or not
        {
            if(!(isdigit(phone[i])))
            {
                return 0;
            }
            i++;
        }
        if(uniq==1)//Checks whether the number is already present in other contacts or not
        {
            for(int j=0;j<addressBook->contactCount;j++)
            {
                if(j==skip)
                {
                    continue;
                }
                if(strcmp(addressBook->contacts[j].phone,phone)==0)
                {
                    return 0;
                }
            }
        }
        return 1;
    }
    return 0;
}
//Function which checks whether the mail ID is valid or not
int validate_mail(char mail[],AddressBook *addressBook,int uniq,int skip)
{
    int i=0;
    if(islower(mail[0]))//Checks whether the first character is lowercase or not
    {
        while(mail[i]) //Checks whether every character is lowercase alphabet,'.','_','@' or digits
        {
            if(!(islower(mail[i])||mail[i]=='_'||mail[i]=='.'||isdigit(mail[i])||mail[i]=='@'))
            {
                return 0;
            }
            i++;
        }
        i=0;
        int ast_c=0;//Count variable for '@'
        int len=strlen(mail);//Gets the length of mail ID
        while(mail[i])//Checks how many '@' present in the mail ID
        {
            if(mail[i]=='@')
            {
                ast_c++;
            }
            i++;
        }
        //Checks whether mail ID ends in '@','.','_' or '@' is present more than or less than one
        if(mail[len-1]=='@'||ast_c!=1||mail[len-1]=='.'||mail[len-1]=='_')
        {
            return 0;
        }
        i=0;
        while(mail[i])
        {
            //Checks whether '.' and '@' comes together consecutively
            if(mail[i]=='.')
            {
                if(mail[i+1]=='.'||mail[i+1]=='@')
                {
                    return 0;
                }
            }
            i++;
        }
        i=0;
        int j,ch_count=0;//Character count variable
        while(mail[i])
        {
            if(mail[i]=='@')//Checks the characters after '@'
            {
                if(mail[i+1]=='.'||mail[i+1]=='_')//Checks for whether '.' or '_' comes after '@'
                {
                    return 0;
                }
                for(j=i+1;j<len;j++)//Checks whether atleast one '.' is present after '@'
                {
                    if(mail[j]=='.')
                    {
                        break;
                    }
                }
                if(j==len)
                {
                    return 0;
                }
                for(int k=j+1;k<len;k++)//Loop to count number of character after '.' which comes after '@'
                {
                    ch_count++;
                }
                if(ch_count<2)//Checks whether the domain is atleast 2 character length or not
                {
                    return 0;
                }
            }
            i++;
        }
        if(uniq==1)//Checks whether the user input mail ID is already present in other contacts or not
        {
            for(int j=0;j<addressBook->contactCount;j++)
            {
                if(j==skip)//condition to skip an iteration
                {
                    continue;
                }
                if(strcmp(addressBook->contacts[j].email,mail)==0)
                {
                    return 0;
                }
            }
        }
        return 1;
    }
    return 0;
} 
//Function which sorts the contacts in alphabetical order
void sortcontact(AddressBook *addressBook)
{
    int lim=addressBook->contactCount;//Gets total number of contacts
    for(int i=0;i<lim-1;i++)//Does simple selection sort using temporary Contact UDT
    {
        for(int j=i+1;j<lim;j++)
        {
            if((strcasecmp(addressBook->contacts[i].name,addressBook->contacts[j].name))>0)
            {
                Contact temp=addressBook->contacts[i];
                addressBook->contacts[i]=addressBook->contacts[j];
                addressBook->contacts[j]=temp;
            }
        }
    }
}
//Function to Display all contacts 
void listContacts(AddressBook *addressBook) 
{
    if(addressBook->contactCount==0)//Checking whether there is contact present or not
    {
        printf(YELLOW"No Contacts Saved\n"RESET);
        return;
    }
    sortcontact(addressBook); //Sorting contacts in Alphabetical order for listing
    print_table();
    // Sort contacts based on the chosen criteria
    for(int i=0;i<addressBook->contactCount;i++)
    {
        print_contact(addressBook,i+1,i);
    }
    printf(GREEN"--------------------------------------------------------------------------\n"RESET);
    printf(GREEN"--------------------------------------------------------------------------\n"RESET);
}
//Searching a contact by the contact name
int search_by_name(AddressBook *addressBook)
{
    if(addressBook->contactCount==0)//Checks atleast a single contact is present
    {
        return -2;
    }
    char s_name[50];//Temporary string to hold user input
    int c=0;//Count variable to warn invalid input
    do
    {
        if(c>0)
        {
            printf(YELLOW"INVALID NAME! RETYPE THE NAME!\n"RESET);
        }
        printf(WHITE"Enter the Name to be searched: "RESET);
        scanf(" %[^\n]",s_name);
        c++;
    } while(validate_name(s_name)!=1);//Loop goes on till valid name is typed
    
    int cnt_name[addressBook->contactCount];//Array to store all index of a name because name can have duplicate value
    int k=0;//Index of the name array
    for(int i=0;i<addressBook->contactCount;i++)//Storing index value of input name in array
    {
        if(strcasecmp(addressBook->contacts[i].name,s_name)==0)
        {
            cnt_name[k]=i;
            k++;
        }
    }
    if(k==0)//Name has no match
    {
        return -1;
    }
    else if(k==1)//Name has single match
    {
        int i=cnt_name[0];
        return i;
    }
    else//Name has many matches
    {
        print_table();
        for(int j=0;j<k;j++)
        {
            int i=cnt_name[j];
            print_contact(addressBook,j+1,i);
        }
        printf(GREEN"--------------------------------------------------------------------------\n"RESET);
        printf(GREEN"--------------------------------------------------------------------------\n"RESET);
        int ch;
        do
        {
            printf(BLUE"Select from the above Contacts by Serial No:\n"RESET);
            printf(WHITE"Enter the Choice: "RESET);
            scanf("%d",&ch);
        } while (ch<1||ch>k);//loop to select any one of the matches and return index after loop
        return cnt_name[ch-1];
    }
}
//Searching a contact by phone number
int search_by_phone(AddressBook *addressBook)
{
    if(addressBook->contactCount==0)//Checks whether any contact is present or not
    {
        return -2;
    }
    char s_phone[50];//Temporary string to store searching string
    int c=0;//Count variable to warn for invalid input
    do
    {
        if(c>0)
        {
            printf(YELLOW"INVALID PHONE NUMBER! RETYPE THE NUMBER!\n"RESET);
        }
        printf(WHITE"Enter the Phone number to be searched: "RESET);
        scanf("%s",s_phone);
        c++;
    } while (validate_phone(s_phone,addressBook,0,-1)!=1);//Loop goes on till valid number is typed
    //Searches the input number with every phone number in the Contacts
    for(int i=0;i<addressBook->contactCount;i++)
    {
        if(strcmp(addressBook->contacts[i].phone,s_phone)==0)
        {
            return i;
        }
    }
    return -1;
}
//Searching a contact by mail
int search_by_mail(AddressBook *addressBook)
{
    if(addressBook->contactCount==0)//Checking whether contacts are present or not
    {
        return -2;
    }
    char s_mail[50];//Temporary string to search a mail ID
    int c=0;//Count variable to check whether the input is valid or not
    do
    {
        if(c>0)//C greater than 0 warns the user to give valid input
        {
            printf(YELLOW"INVALID EMAIL ID! RETYPE THE EMAIL ID!\n"RESET);
        }
        printf(WHITE"Enter the Email ID to be searched: "RESET);
        scanf("%s",s_mail);
        c++;
    } while (validate_mail(s_mail,addressBook,0,-1)!=1);//Loop will goes on till a valid mail ID is entered to search
    //Compares every Mail ID of every contact and returns its index if present or returns -1
    for(int i=0;i<addressBook->contactCount;i++)
    {
        if(strcmp(addressBook->contacts[i].email,s_mail)==0)
        {
            return i;
        }
    }
    return -1;
}
//Function which backups the contacts from structure to file
void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fptr = fopen("contacts.csv","w");//Opening the file using Write mode
    if(fptr==NULL)//Validating whether file is open or not
    {
        printf(RED"FILE DOES NOT EXIST!\n"RESET);
        return;
    }
    int count=addressBook->contactCount;//Getting contact count from structure 
    fprintf(fptr,"#%d\n",count);
    for(int i=0;i<count;i++)//Backup every contact one by one to file
    {
        fprintf(fptr,"%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    fclose(fptr);//Closing the file
}
//Function which loads back the contacts from file to structure
void loadContactsFromFile(AddressBook *addressBook)
{
    FILE *fptr = fopen("contacts.csv","r");// Opening file in Read mode
    if(fptr==NULL)//Validating whether file is open or not
    {
        printf(RED"FILE DOES NOT EXIST!\n"RESET);
        return;
    }
    fscanf(fptr,"#%d\n",&addressBook->contactCount);
    for(int i=0;i<addressBook->contactCount;i++)//Loading the contacts one by one from file to structure
    {
        fscanf(fptr,"%[^,],%[^,],%[^\n]\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    fclose(fptr);//Closing the file
}
//Function to load back contacts to structure and initialize contact count 
void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}
//Function to take backup of the contacts from structure to file and exit program
void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}
//Contact creation happens in this function with validation
void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */
    int ct=addressBook->contactCount; //Getting the current contact count value
    char name[50];//Temporary string for name input
    char phone[20];//Temporary string for phone number input
    char mail[50];//Temporary string for mail input
    int c=0; //Count variable for number of have account of number times a value is entered
    //Getting Name of contact
    do
    {
        if(c>0) //Count value greater than 0 shows warning to type correct name
        {
            printf(YELLOW"INVALID NAME! RETYPE THE NAME!\n"RESET);
        }
        printf(WHITE"Enter the Name: "RESET);
        scanf(" %[^\n]",name);
        c++;
    } while (validate_name(name)!=1); //Loop is repeated until valid name is entered
    strcpy(addressBook->contacts[ct].name,name); //Valid name is stored in the structure
    c=0; //Re intialize count to use for next value
    //Getting Phone number of contact
    do
    {
        if(c>0) //Count value greater than 0 shows warning to type correct name
        {
            printf(YELLOW"INVALID PHONE NUMBER! RETYPE THE NUMBER!\n"RESET);
        }
        printf(WHITE"Enter the Phone Number: "RESET);
        scanf("%s",phone);
        c++;
    } while (validate_phone(phone,addressBook,1,-1)!=1);//Loop is repeated until valid Phone number is entered
    strcpy(addressBook->contacts[ct].phone,phone);//Valid Phone number is stored in the structure
    c=0;//Re intialize count to use for next value
    //Getting Email ID of contact
    do
    {
        if(c>0) //Count value greater than 0 shows warning to type correct name
        {
            printf(YELLOW"INVALID EMAIL ID! RETYPE THE EMAIL ID!\n"RESET);
        }
        printf(WHITE"Enter the Email ID: "RESET);
        scanf("%s",mail);
        c++;
    } while (validate_mail(mail,addressBook,1,-1)!=1);//Loop is repeated until valid mail ID is entered
    strcpy(addressBook->contacts[ct].email,mail);//Valid mail ID is stored in the structure
    (addressBook->contactCount)++; //Contact count is increased 
}

void searchContact(AddressBook *addressBook) //Contacta are searched here and displayed using this function
{
    /* Define the logic for search */
    int choice,ind;
    do
    {
        printf(BLUE"Search Menu:\n"RESET);
        printf(BLUE"Search by:\n1] Name\n2] Phone Number\n3] Mail\n4] Exit\n"RESET);
        printf(WHITE"Enter the Choice: "RESET);
        scanf("%d",&choice);//Input for choice selection
        switch(choice) 
        {
            case 1: //Searching the contact using Name
                ind=search_by_name(addressBook);//Returns the index or -1 or -2
                if(ind>=0)
                {
                    print_table();
                    print_contact(addressBook,1,ind);//Prints the contact using index
                    printf(GREEN"--------------------------------------------------------------------------\n"RESET);
                    printf(GREEN"--------------------------------------------------------------------------\n"RESET);
                }
                else if(ind==-2)
                {
                    printf(YELLOW"No Contacts Saved\n"RESET);
                }
                else
                {
                    printf(RED"NO MATCH FOUND!\n"RESET);
                }
                break;
            case 2: //Searching the contact using Phone number
                ind=search_by_phone(addressBook);//Returns Index of the contact or -1 or -2
                if(ind>=0)
                {
                    print_table();
                    print_contact(addressBook,1,ind);//Prints the contact using index
                    printf(GREEN"--------------------------------------------------------------------------\n"RESET);
                    printf(GREEN"--------------------------------------------------------------------------\n"RESET);
                }
                else if(ind==-2)
                {
                    printf(YELLOW"No Contacts Saved\n"RESET);
                }
                else
                {
                    printf(RED"NO MATCH FOUND!\n"RESET);
                }
                break;
            case 3: //Searching the contact using Mail ID
                ind=search_by_mail(addressBook);//Returns Index of the contact or -1 or -2
                if(ind>=0)
                {
                    print_table();
                    print_contact(addressBook,1,ind);//Prints the contact using index
                    printf(GREEN"--------------------------------------------------------------------------\n"RESET);
                    printf(GREEN"--------------------------------------------------------------------------\n"RESET);
                }
                else if(ind==-2)
                {
                    printf(YELLOW"No Contacts Saved\n"RESET);
                }
                else
                {
                    printf(RED"NO MATCH FOUND!\n"RESET);
                }
                break;
            case 4: //Exiting the search
                printf(GREEN"Search Exited\n"RESET);
                break;
            default:
                printf(YELLOW"Invalid choice. Please try again.\n"RESET);
        }
    }while(choice!=4);//Loop goes on until Exit option 4 is chosen
}
//Editing of contact happens in this function
void edit_operation(AddressBook *addressBook, int index)
{
    int choice;
    char name[50], phone[20], mail[20];//New Name/Phone Number/Email Id from user is scanned with these
    do
    {
        printf(BLUE"1] Name\n2] Phone Number\n3] Mail\n4] Exit\n"RESET);
        printf(BLUE"Select the option to Edit:\n"RESET);
        printf(WHITE"Enter the Choice: "RESET);
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:         //Editing of Name happens here
             {
                int c = 0;//Count variable to determine the validation 
                do 
                {
                    if (c > 1) //Will print INVALID only if the entered input is invalid 
                    {
                        printf(YELLOW"INVALID NAME! RETYPE\n"RESET);
                    }
                    printf("Enter the New Name: ");
                    scanf(" %[^\n]", name);//Input of new name
                    c++;
                } while (!validate_name(name));//Loop goes on until Valid input is provided
                strcpy(addressBook->contacts[index].name, name);//Editing the contact with new name
                printf(GREEN"Name Updated\n"RESET);
                break;
            }
            case 2:         //Editing of Phone number happens here
            {
                int c = 0;//Count variable to determine the validation 
                do 
                {
                    if (c > 1) //Will print INVALID only if the entered input is invalid
                    {
                        printf(YELLOW"INVALID PHONE! RETYPE\n"RESET);
                    }
                    printf("Enter the New Phone: ");
                    scanf("%s", phone);//Input of new Phone number
                    c++;
                } while (!validate_phone(phone, addressBook, 1,index));//Loop goes on until Valid input is provided
                strcpy(addressBook->contacts[index].phone, phone);//Editing the contact with new Phone number
                printf(GREEN"Phone Updated\n"RESET);
                break;
            }
            case 3:         //Editing of Email ID happens here
            {
                int c = 0;//Count variable to determine the validation 
                do {
                    if (c > 1) //Will print INVALID only if the entered input is invalid
                    {
                        printf(YELLOW"INVALID EMAIL! RETYPE\n"RESET);
                    }
                    printf("Enter the New Email: ");
                    scanf("%s", mail);//Input of new Email ID
                    c++;
                } while (!validate_mail(mail, addressBook, 1,index));//Loop goes on until Valid input is provided
                strcpy(addressBook->contacts[index].email, mail);//Editing the contact with new Email ID
                printf(GREEN"Email Updated\n"RESET);
                break;
            }
            case 4:         //Exiting Editing of contacts
                printf(GREEN"Edit Menu Exited\n"RESET);
                break;
            default:
                printf(YELLOW"Invalid choice. Try again.\n"RESET);
        }

    } while (choice != 4);//Loop goes on until Exit option is chosen
}

void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
    int choice,ind;
    do
    {
        printf(BLUE"Edit Menu:\n1] Name\n2] Phone Number\n3] Mail\n4] Exit\n"RESET);
        printf(WHITE"Enter the Choice: "RESET);
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:         //Searching the contact with help of name and editing
                ind=search_by_name(addressBook);//Returns Index of the contact or -1 or -2
                if (ind >= 0)
                {
                    edit_operation(addressBook, ind);//Editing contact using index
                    printf(GREEN"Successfully Edited\n"RESET);
                }
                else if (ind == -2)
                {
                    printf(YELLOW"No Contacts Saved\n"RESET);
                }
                else
                {
                    printf(RED"No Match Found\n"RESET);
                }
                break;
            case 2:         //Searching the contact with help of Phone number and editing
                ind=search_by_phone(addressBook);//Returns Index of the contact or -1 or -2
                if (ind >= 0)
                {
                    edit_operation(addressBook, ind);//Editing contact using index
                    printf(GREEN"Successfully Edited\n"RESET);
                }
                else if (ind == -2)
                {
                    printf(YELLOW"No Contacts Saved\n"RESET);
                }
                else
                {
                    printf(RED"No Match Found\n"RESET);
                }
                break;
            case 3:         //Searching the contact with help of Email ID and editing
                ind=search_by_mail(addressBook);//Returns Index of the contact or -1 or -2
                if (ind >= 0)
                {
                    edit_operation(addressBook, ind);//Editing contact using index
                    printf(GREEN"Successfully Edited\n"RESET);
                }
                else if (ind == -2)
                {
                    printf(YELLOW"No Contacts Saved\n"RESET);
                }
                else
                {
                    printf(RED"No Match Found\n"RESET);
                }
                break;
            case 4:         //Exiting Editing Operation
                printf(GREEN"Editing Exited\n"RESET);
                break;
            default:
                printf(YELLOW"Invalid choice. Please try again.\n"RESET);
        }
    } while (choice!=4);//Loop goes on until Exit is chosen
    saveContactsToFile(addressBook);//Saving the new contacts information to ensure modification
}

void delete_operation(AddressBook *addressBook,int index) //Function to perform Deletion
{
    //Simply left shifting the contact details on top of the contact to be deleted till last contact
    for(int i=index;i<(addressBook->contactCount)-1;i++)
    {
        addressBook->contacts[i]=addressBook->contacts[i+1];
    }
    (addressBook->contactCount)--;//Decrementing the contact count to ensure deletion
}
 
void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
    int choice,ind;
    do
    {
        printf(BLUE"Delete Menu:\n"RESET);
        printf(BLUE"Delete by:\n1] Name\n2] Phone Number\n3] Mail\n4] Exit\n"RESET);
        printf(BLUE"Enter the Contact to be Deleted:\n"RESET);
        printf(WHITE"Enter the Choice: "RESET);
        scanf("%d",&choice);
        switch(choice)      
        {
            case 1:             //Searching contact by name and deleting
                ind=search_by_name(addressBook);//Returns Index of the contact or -1 or -2
                if (ind >= 0)
                {
                    delete_operation(addressBook, ind);//Deleting contact using index
                    printf(GREEN"Successfully Deleted\n"RESET);
                }
                else if (ind == -2)
                {
                    printf(YELLOW"No Contacts Saved\n"RESET);
                }
                else
                {
                    printf(RED"No Match Found\n"RESET);
                }
                break;
            case 2:             //Searching contact by phone number and deleting
                ind=search_by_phone(addressBook);//Returns Index of the contact or -1 or -2
                if (ind >= 0)
                {
                    delete_operation(addressBook, ind);//Deleting contact using index
                    printf(GREEN"Successfully Deleted\n"RESET);
                }
                else if (ind == -2)
                {
                    printf(YELLOW"No Contacts Saved\n"RESET);
                }
                else
                {
                    printf(RED"No Match Found\n"RESET);
                }
                break;
            case 3:             //Searching contact by Email ID and deleting
                ind=search_by_mail(addressBook);//Returns Index of the contact or -1 or -2
                if (ind >= 0)
                {
                    delete_operation(addressBook, ind);//Deleting contact using index
                    printf(GREEN"Successfully Deleted\n"RESET);
                }
                else if (ind == -2)
                {
                    printf(YELLOW"No Contacts Saved\n"RESET);
                }
                else
                {
                    printf(RED"No Match Found\n"RESET);
                }
                break;
            case 4:             //Exiting Delete operation 
                printf(GREEN"Deletion Exited\n"RESET);
                break;
            default:
                printf(YELLOW"Invalid choice. Please try again.\n"RESET);
        }
    } while (choice!=4);//Loop goes on until Exit is chosen
    saveContactsToFile(addressBook);//Saving the new contacts information to ensure deletion 
}