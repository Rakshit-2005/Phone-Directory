# Phone-Directory
Phone Directory using Tree Data Structure 
A simple Phone Contact Manager written in C that uses a Binary Search Tree (BST) to store and manage contacts, and a Doubly Linked List to maintain call history.
Contacts are also saved in a file (Contact.txt) for persistence.

#Features
#Add Contact
Add a new contact with name and phone number.
Prevents duplicate names or phone numbers.
Validates phone numbers (must be in the range 7000000000 – 9999999999).
Saves contacts to file (Contact.txt).

#Display All Contacts
Displays all contacts in sorted order (A–Z) using inorder traversal of BST.

#Search Contact
Search a contact by name in the BST.

#Delete Contact
Delete a contact from both BST and file storage.

#Make Call
Records a call between caller and callee.
Stores call history in a doubly linked list.

#Display Call History
View the complete list of calls made (caller → callee).

#File Persistence
Contacts are stored in Contact.txt.
BST is rebuilt from file every time the program starts.
