#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct {
    Contact contacts[100];
    int contactCount;
} AddressBook;

void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);
int validate_name(char name[]);
int validate_phone(char phone[],AddressBook *addressBook,int uniq,int skip);
int validate_mail(char mail[],AddressBook *addressBook,int uniq,int skip);
void sortcontact(AddressBook *addressBook);
int search_by_phone(AddressBook *addressBook);
int search_by_mail(AddressBook *addressBook);
int search_by_name(AddressBook *addressBook);
void print_table();
void saveContactsToFile(AddressBook *addressBook);
void loadContactsFromFile(AddressBook *addressBook);
void print_contact(AddressBook *addressBook,int sno,int ind);
void delete_operation(AddressBook *addressBook,int index);

#endif
