#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Contact {
    char name[50];
    long long phoneNumber;
    struct Contact* left;
    struct Contact* right;
} Contact;

typedef struct CallRecord {
    char caller[50];
    char callee[50];
    struct CallRecord* next;
    struct CallRecord* prev;
} CallRecord;

// Contact* newNode(char name[], long long phone) {
//     Contact* temp = (Contact*)malloc(sizeof(Contact));
//     strcpy(temp->name, name);
//     temp->phoneNumber=phone;
//     temp->left = temp->right = NULL;
//     return temp;
// }

// // Function to insert a new node into the BST
// Contact* insert(Contact* root, char name[], long long phone) {
//     if (root == NULL) {
//         return createContactNode(name, phone);
//     }

//     int compare = strcmp(name, root->name);
//     if (compare < 0) {
//         root->left = insert(root->left, name, phone);
//     } else if (compare > 0) {
//         root->right = insert(root->right, name, phone);
//     }

//     return root;
// }

Contact* createContactNode(char name[],long long phoneNumber) {
    Contact* newContact = (Contact*)malloc(sizeof(Contact));
    if (newContact != NULL) {
        strcpy(newContact->name, name);
        newContact->phoneNumber = phoneNumber;
        newContact->left = NULL;
        newContact->right = NULL;
    }
    return newContact;
}

Contact* insertContact(Contact* root,char name[],long long phoneNumber) {
    if (root == NULL) {
        return createContactNode(name, phoneNumber);
    }

    int compareResult = strcasecmp(name, root->name);
    if (compareResult < 0) {
        root->left = insertContact(root->left, name, phoneNumber);
    } else if (compareResult > 0) {
        root->right = insertContact(root->right, name, phoneNumber);
    }

    return root;
}

void addContactToFile(const char* filename, char name[], long long phoneNumber) {
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%s %lld\n", name, phoneNumber);
    fclose(file);
}



Contact* buildBSTFromFile(Contact* root, char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }

    // Contact* root = NULL;
    char line[100];
    char name[50];
    long long phoneNumber;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%49s %lld", name, &phoneNumber) == 2) {
            root = insertContact(root, name, phoneNumber);
        }
    }

    fclose(file);
    return root;
}


bool contactExists(const char* filename, char name[], long long phone) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return false;  // File not found, contact doesn't exist
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char savedName[50];
        long long savedPhone;
        if (sscanf(line, "%49s %lld", savedName, &savedPhone) == 2) {
            if (strcmp(savedName, name) == 0 || savedPhone == phone) {
                fclose(file);
                return true;  // Contact with same name or phone number found
            }
        }
    }

    fclose(file);
    return false;  // Contact not found in the file
}


void displayContactsInOrder(Contact* root) {
    if (root != NULL) {
        displayContactsInOrder(root->left);
        printf("Name: %s, Phone Number: %lld\n", root->name, root->phoneNumber);
        displayContactsInOrder(root->right);
    }
}

void searchContact(Contact* root, const char* name) {
    if (root == NULL) {
        printf("Contact not found.\n");
        return;
    }

    int compareResult = strcmp(name, root->name);
    if (compareResult < 0) {
        searchContact(root->left, name);
    } else if (compareResult > 0) {
        searchContact(root->right, name);
    } else {
        printf("Name: %s, Phone Number: %lld\n", root->name, root->phoneNumber);
    }
}

Contact* deleteContact(Contact* root, const char* name) {
    if (root == NULL) {
        return root;
    }

    int compareResult = strcasecmp(name, root->name);
    if (compareResult < 0) {
        root->left = deleteContact(root->left, name);
    } else if (compareResult > 0) {
        root->right = deleteContact(root->right, name);
    } else {
        if (root->left == NULL) {
            Contact* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Contact* temp = root->left;
            free(root);
            return temp;
        }

        Contact* temp = root->right;
        while (temp->left != NULL) {
            temp = temp->left;
        }

        strcpy(root->name, temp->name);
        root->phoneNumber = temp->phoneNumber;
        root->right = deleteContact(root->right, temp->name);
    }
    
    return root;
}

void deletefromfile(const char* filename, char name[]) {
    // Update the file after deleting the contact from BST
    FILE* file = fopen(filename, "r");
    FILE* tempFile = fopen("temp.txt", "w");  // Use a unique name for the temporary file
    if (file == NULL || tempFile == NULL) {
        printf("Error opening files for deletion.\n");
        return;  // No need to return anything, just exit the function
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char savedName[50];
        long long savedPhone;  // This variable is not used in this function
        if (sscanf(line, "%49s %lld", savedName, &savedPhone) == 2) {
            if (strcmp(savedName, name) != 0) {  // Remove the comparison with phoneNumber
                fprintf(tempFile, "%s %lld\n", savedName, savedPhone);
            }
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("temp.txt", filename);
}

CallRecord* createCallRecord(const char* caller, const char* callee) {
    CallRecord* newRecord = (CallRecord*)malloc(sizeof(CallRecord));
    if (newRecord != NULL) {
        strcpy(newRecord->caller, caller);
        strcpy(newRecord->callee, callee);
        newRecord->next = NULL;
        newRecord->prev = NULL;
    }
    return newRecord;
}

void addCallRecord(CallRecord** head, const char* caller, const char* callee) {
    CallRecord* newRecord = createCallRecord(caller, callee);
    if (newRecord != NULL) {
        newRecord->next = *head;
        if (*head != NULL) {
            (*head)->prev = newRecord;
        }
        *head = newRecord;
    }
}

void displayCallHistory(CallRecord* head) {
    printf("Call History:\n");
    while (head != NULL) {
        printf("Caller: %s, Callee: %s\n", head->caller, head->callee);
        head = head->next;
    }
}

void freeContacts(Contact* root) {
    if (root != NULL) {
        freeContacts(root->left);
        freeContacts(root->right);
        free(root);
    }
}

void freeCallHistory(CallRecord* head) {
    CallRecord* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    Contact* phoneBook = NULL;
    char filename[] = "Contact.txt";
   phoneBook= buildBSTFromFile(phoneBook,"Contact.txt");
     FILE* file;
    // file = fopen("Contact.txt", "a+");
    // if (file == NULL) {
    //     printf("File cannot be opened\n");
    //     return 1;
    // }
    
    
    CallRecord* callHistory = NULL;
    int choice;
    char name[50];
    long long phoneNumber;
    char caller[50];
    char callee[50];
    
    do {
        printf("\nPhone Contact Manager\n");
        printf("1. Add Contact\n");
        printf("2. Display All Contacts (Sorted by Name)\n");
        printf("3. Search Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Make Call\n");
        printf("6. Display Call History\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                do{
                printf("Enter name: ");
                scanf("%49s", name);
                do{
                printf("Enter phone number: ");
                scanf("%lld", &phoneNumber);
                if (!(phoneNumber < 7000000000LL || phoneNumber > 9999999999LL)) {
                    
                    break;
                }
                else{
                    printf("Invalid phone number! Please enter a correct number\n");

                }
                }while(1);
                if (!contactExists(filename, name, phoneNumber)) {
                   
                    break;
                }
                else{
                     printf("Contact with the same name or phone number already exists.\n");
                }
                }while(1);
                // fprintf(file, "%s %lld\n", name, phoneNumber);
                if(phoneBook!=NULL){
                phoneBook = insertContact(phoneBook, name, phoneNumber);
                }
                addContactToFile(filename, name, phoneNumber);
                printf("Contact details added to file\n");
                
                break;
            case 2:
                 if(phoneBook!=NULL){
                printf("Contacts:\n");
                displayContactsInOrder(phoneBook);
                 }
                break;
            case 3:
                printf("Enter name to search: ");
                scanf("%49s", name);
                searchContact(phoneBook, name);
                break;
            case 4:
                printf("Enter name to delete: ");
                scanf("%49s", name);
                phoneBook = deleteContact(phoneBook, name);
                deletefromfile(filename,name);
                break;
            case 5:
                printf("Enter caller name: ");
                scanf("%49s", caller);
                printf("Enter callee name: ");
                scanf("%49s", callee);
                addCallRecord(&callHistory, caller, callee);
                printf("Call made from %s to %s recorded.\n", caller, callee);
                break;
            case 6:
                displayCallHistory(callHistory);
                break;
            case 7:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 7.\n");
        }
    } while (choice != 7);

    fclose(file);
    freeContacts(phoneBook);
    freeCallHistory(callHistory);

    return 0;
}
