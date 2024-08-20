#include<stdio.h>
#include<stdlib.h>
#include<time.h>

struct books
{
    int BookId;
    char BookName[100];
    char BookAuthor[100];
    char Date[20];
} Book;

struct students
{
    int StudentId;
    int BookId;
    char StudentName[100];
    char RentedBook[100];
    char StudentClass[20];
    char date[20];
} Student;

FILE *ptr;

void AddBook();
void ViewBookList();
void RemoveBook();
void IssueBook();
void ViewIssueBook();
void ReturnBook();

int main()
{
    int choice;
    while(1)
    {
        err:
        system("cls");
        printf("== Library Management System Using Array ==\n");
        printf("1. Add Book\n");
        printf("2. View Book List\n");
        printf("3. Remove Book\n");
        printf("4. Issue Book\n");
        printf("5. View Issued Book\n");
        printf("6. Return Book\n");
        printf("0. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if(choice == 0)
        {
            break;
        }
        else if(choice == 1)
        {
            AddBook();
        }
        else if(choice == 2)
        {
            ViewBookList();
        }
        else if(choice == 3)
        {
            RemoveBook();
        }
        else if(choice == 4)
        {
            IssueBook();
        }
        else if(choice == 5)
        {
            ViewIssueBook();
        }
        else if(choice == 6)
        {
            ReturnBook();
        }
        else
            {
                printf("Invalid Input\nPress Enter to Continue...");
            getchar();
            getchar();
            goto err;
            }
    }

    return 0;
}

void AddBook()
{
    char myDate[20];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(Book.Date, myDate);

    int idExists = 0;

    // Open the file in read mode to check for duplicate IDs
    ptr = fopen("books.txt", "rb");
    printf("Enter Book ID: ");
    scanf("%d", &Book.BookId);

    struct books tempBook;
    while (fread(&tempBook, sizeof(tempBook), 1, ptr) == 1)
    {
        if (tempBook.BookId == Book.BookId)
        {
            idExists = 1;
            break;
        }
    }
    fclose(ptr);

    if (idExists)
    {
        printf("This Book ID has already been stored.\n\nPress Enter to continue...");
        getchar(); // Wait for user input
        getchar(); // Capture the Enter key press
        return; // Exit the function to avoid adding a duplicate entry
    }

    // If the ID does not exist, proceed to add the book
    ptr = fopen("books.txt", "ab");
    printf("Enter Book name: ");
    fflush(stdin);
    gets(Book.BookName);

    printf("Enter Author name: ");
    fflush(stdin);
    gets(Book.BookAuthor);

    printf("Book Added Successfully\n\nPress Enter...");
    getchar();
    fwrite(&Book, sizeof(Book), 1, ptr);
    fclose(ptr);
}

void ViewBookList()
{
    system("cls");
    printf("== Available Books ==\n\n");
    printf("%-10s %-30s %-30s %-20s\n\n", "Book ID", "Book Name", "Book Author", "Date");

    ptr = fopen("books.txt", "rb");

    while(fread(&Book, sizeof(Book), 1, ptr) == 1)
    {
        printf("%-10d %-30s %-30s %-20s\n\n", Book.BookId, Book.BookName, Book.BookAuthor, Book.Date);
    }
    fclose(ptr);
    printf("Press Enter to return to the menu...");
    getchar(); // Waits for the user to press a key
    getchar(); // Ensures that the user actually presses a key before clearing the screen
}

void RemoveBook()
{
    int id;
    int flag = 0;
    system("cls");
    printf("== Remove Book ==\n\n");
    printf("Enter The Book ID to remove: ");
    scanf("%d", &id);

    FILE *ptr2;

    ptr = fopen("books.txt", "rb"); // rb = reading binary
    ptr2 = fopen("temp.txt", "wb"); // wb = Writing Binary

    while(fread(&Book, sizeof(Book), 1, ptr) == 1)
    {
        if(id == Book.BookId)
        {
            flag = 1;
        }
        else
        {
            fwrite(&Book, sizeof(Book), 1, ptr2); // copying every book name but the specific book id into the temp text file
        }
    }
    if(flag == 1)
    {
        printf("\n\nBook successfully Deleted !\n\nPress Enter...");
    }
    else
    {
        printf("\n\nBook Not Found\n\nPress Enter...");
    }
    getchar(); // Waits for the user to press a key
    getchar(); // Ensures that the user actually presses a key before clearing the screen

    fclose(ptr);
    fclose(ptr2);

    // Delete the original file then rename the temp file to books.txt
    remove("books.txt");
    rename("temp.txt", "books.txt");
}

void IssueBook()
{
    char myDate[20];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(myDate, "%02d/%02d/%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    strcpy(Student.date, myDate);

    int flag = 0;
    system("cls");
    printf("== Issue Book ==\n\n");
    printf("Enter The Book ID to Issue: ");
    scanf("%d", &Student.BookId);

    // Check if the book is already issued
    FILE *issuePtr = fopen("issue.txt", "rb");
    while (fread(&Student, sizeof(Student), 1, issuePtr) == 1)
    {
        if (Student.BookId == Book.BookId)
        {
            printf("This Book is already issued.\n\nPress Enter to return to the menu...");
            getchar();
            getchar();
            fclose(issuePtr);
            return;
        }
    }
    fclose(issuePtr);

    // Check if the book ID exists in the books.txt file
    ptr = fopen("books.txt", "rb");
    while (fread(&Book, sizeof(Book), 1, ptr) == 1)
    {
        if (Book.BookId == Student.BookId)
        {
            flag = 1;
            strcpy(Student.RentedBook, Book.BookName);
            break;
        }
    }
    fclose(ptr);

    if (flag == 0)
    {
        printf("Book not found with this ID\n\nPress Enter to return to the menu...");
        getchar();
        getchar();
        return;
    }

    // Issue the book if it is not already issued
    issuePtr = fopen("issue.txt", "ab"); // Append Binary
    printf("Enter Student Name: ");
    fflush(stdin);
    gets(Student.StudentName);

    printf("Enter Student Class: ");
    fflush(stdin);
    gets(Student.StudentClass);

    printf("Enter Student ID: ");
    fflush(stdin);
    scanf("%d", &Student.StudentId);

    fwrite(&Student, sizeof(Student), 1, issuePtr);
    fclose(issuePtr);

    printf("Book issued successfully.\n\nPress Enter to return to the menu...");
    getchar();
    getchar();
}

void ViewIssueBook()
{
    system("cls");
    printf("== Issued Books ==\n\n");
    printf("%-10s %-30s %-10s %-10s %-30s %-20s\n\n", "Book ID", "Student Name", "Class", "Student ID", "Book Name", "Issue Date");

    ptr = fopen("issue.txt", "rb");

    while(fread(&Student, sizeof(Student), 1, ptr) == 1)
    {
        printf("%-10d %-30s %-10s %-10d %-30s %-20s\n\n", Student.BookId, Student.StudentName, Student.StudentClass, Student.StudentId, Student.RentedBook, Student.date);
    }
    fclose(ptr);
    printf("Press Enter to return to the menu...");
    getchar(); // Waits for the user to press a key
    getchar(); // Ensures that the user actually presses a key before clearing the screen
}

void ReturnBook()
{
    int id, flag = 0;
    system("cls");
    printf("== Return Book ==\n\n");
    printf("Enter The Book ID to return: ");
    scanf("%d", &id);

    FILE *ptr2;

    ptr = fopen("issue.txt", "rb"); // rb = reading binary
    ptr2 = fopen("temp.txt", "wb"); // wb = Writing Binary

    while(fread(&Student, sizeof(Student), 1, ptr) == 1)
    {
        if(id == Student.BookId)
        {
            flag = 1;
        }
        else
        {
            fwrite(&Student, sizeof(Student), 1, ptr2); // copying every student record but the specific book id into the temp text file
        }
    }
    fclose(ptr);
    fclose(ptr2);

    if(flag == 1)
    {
        printf("\n\nBook returned successfully!\n\nPress Enter...");
    }
    else
    {
        printf("\n\nBook not found with this ID in issued records.\n\nPress Enter...");
    }
    getchar();
    getchar();

    // Delete the original issue.txt file then rename the temp file to issue.txt
    remove("issue.txt");
    rename("temp.txt", "issue.txt");
}
