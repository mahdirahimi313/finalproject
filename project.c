#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

typedef struct Account account;


struct Account
{
    char username[51];
    char password[51];
    unsigned long int ID;
};

unsigned long int ID_Counter = 0; // variable to keep track of the ID
unsigned long int Current_ID = 0; // variable to keep track of the ID
unsigned long int message_counter = 1; // variable to keep track of the ID


void Start(); // Start the program
bool Is_User_Exist(); // return true if username and password is correct
bool Is_Open_File(const FILE *); // return true if file is open
bool Is_Valid_User(const account * ,FILE *); // return true if username and password exist in file
void Receive_User_Struct(account *); // receive the user struct from the user
bool Create_Account(); // create new account
bool Is_Duplicate_Account(const account *); // return true if username already exist
int Return_Menu_Number(); // return menu number
void Send_Message(); // send message to another user
void Write_Message_To_File(); // write message to file
void Write_Id_And_Message_Counter_To_File(); // write message to file
void Read_Id_And_Message_Counter_From_File(); // read message from file
void Edit_Message(); // edit message
void Edit_Message_From_File(); // edit message from file
void Manage_Show_Messages(); // manage show messages
void Show_All_Messages(); // show all messages
void Manage_Change_Password(); // manage change password
void Change_Password(); // change password


int main()
{
    Read_Id_And_Message_Counter_From_File(); // read id and message counter from file
    
    Start(); // start the program

    Write_Id_And_Message_Counter_To_File(); // write id and message counter to file

    return 0;
}



void Start()
{
    system("cls"); // clear the screen

    while (true) // until receive number 5 .
    {
        while(! Is_User_Exist() ) // enter this section when information incorrect .
        {
            printf("Do You Want To Create New Account ? ( Y / N ) : ");
            char choice; // choice of user .
            scanf("%c", &choice);
            fflush(stdin);

            if (choice == 'y' || choice == 'Y') 
            {
                if ( Create_Account() )
                break;
            }

        } // end of while(! Is_User_Exist() )

        int menu_number = Return_Menu_Number(); // return menu number .

        while (true) // continue until reach number 5 
        {
            switch (menu_number) // 
            {
                case 1: // send message to another user .
                    Send_Message();
                    break;

                case 2: // edit message .
                    Edit_Message();
                    break;

                case 3: // show all messages .
                    Manage_Show_Messages();
                    break;

                case 4: // change password .
                    Manage_Change_Password();
                    break;

                case 5:
                    printf("if you want return to menu, press 'Y'\n");
                    printf("if you want exit, press any key \n");

                    char choice;
                    fflush(stdin);
                    scanf("%c", &choice);
                    fflush(stdin);

                    if (choice == 'y' || choice == 'Y')
                    {
                        break;
                    }

                    else
                    {
                        return;
                    }
            }

            fflush(stdin);

            if (menu_number == 5)
            {
                break;
            }

            menu_number = Return_Menu_Number(); // return menu number .


        } // end of while(true)

    } // end of first while(true)

} // end of Start()


bool Is_User_Exist()
{
    system("cls");
    account user_struct;

    FILE * user_file = fopen("user.dat", "rb");

    if ( ! Is_Open_File(user_file) )
    {
        Receive_User_Struct(&user_struct);

        printf("!!   Invalid Username Or Password   !!\n"); 
        printf("Tap enter key to continue... ");
        getchar();

        fclose(user_file); // close file
        return false; // return false if username and password is incorrect
    }

    Receive_User_Struct(&user_struct);

    if ( Is_Valid_User(&user_struct, user_file) ) // return true if username and password exist.
    {
        printf("##   Login Successful   ##\n");
        printf("Tap Enter Key To Continue... ");
        getchar();

        fclose(user_file);
        return true;
    }

    else // return false if username and password is incorrect .
    {
        printf("!!   Invalid Username Or Password    !!\n");
        printf("Tap Enter Key To Continue... ");
        getchar();

        fclose(user_file);
        return false;
    }
}


bool Is_Open_File(const FILE * file_name) // return true if file is open .
{
    if(file_name == NULL)
    {
        return false;
    }

    else
    {
        return true;
    }
}


bool Is_Valid_User(const account * user_struct ,FILE * user_file) // return true if username and password exist
{
    account temp_struct;

    while(fread(&temp_struct, sizeof(account), 1, user_file))
    {
        if(strcmp(user_struct->username, temp_struct.username) == 0 && strcmp(user_struct->password, temp_struct.password) == 0)
        {
            Current_ID = temp_struct.ID;
            return true;
        }
    }

    return false;
}



void Receive_User_Struct(account * user_struct) // receive the user struct from the user
{
    system("cls"); // clear the screen
    printf("Enter UserName :  ");
    fgets(user_struct->username, 50, stdin); // receive username from user
    user_struct->username[strlen(user_struct->username) - 1] = '\0';
    fflush(stdin);

    printf("Enter Password :  ");
    fgets(user_struct->password, 50, stdin); // receive password from user
    user_struct->password[strlen(user_struct->password) - 1] = '\0';
    fflush(stdin);
}



bool Create_Account()
{
    FILE * user_file = fopen("user.dat", "rb");

    if (!Is_Open_File(user_file))
    {
        fclose(user_file);
        user_file = fopen("user.dat", "wb");

        account user_struct;

        Receive_User_Struct(&user_struct);

        user_struct.ID = ID_Counter;
        Current_ID = ID_Counter;
        ID_Counter++;

        fwrite(&user_struct, sizeof(account), 1, user_file);

        fclose(user_file);
    }
    
    else
    {
        fclose(user_file);

        account user_struct;

        Receive_User_Struct(&user_struct);

        if (Is_Duplicate_Account(&user_struct))
        {
            printf("!!   Duplicate Account   !!\n");
            printf("Tap Enter Key To Continue... ");
            fflush(stdin);
            getchar();
            return false;
        }
        
        user_file = fopen("user.dat", "ab");
        user_struct.ID = ID_Counter;
        Current_ID = ID_Counter;
        ID_Counter++;

        fwrite(&user_struct, sizeof(account), 1, user_file);

        fclose(user_file);
    }

    return true;
}


bool Is_Duplicate_Account(const account * user_struct)
{
    FILE * user_file = fopen("user.dat", "rb");
    account temp_struct;

    if (!Is_Open_File(user_file))
    {
        fclose(user_file);
        return false;
    }

    while(fread(&temp_struct, sizeof(account), 1, user_file))
    {
        if( (strcmp(user_struct->username, temp_struct.username) == 0 &&
            strcmp(user_struct->password, temp_struct.password) == 0) ||
            strcmp(user_struct->username, temp_struct.username) == 0)
        {
            fclose(user_file);
            return true;
        }
    }

    fclose(user_file);
    return false;
}




int Return_Menu_Number()
{
    system("cls");
    printf("1.Send Message\n");
    printf("2.Edit Message\n");
    printf("3.View All Message\n");
    printf("4.Change Password\n");
    printf("5.Exit\n");
    printf("Enter Number Of Menu : ");
    int number ;
    scanf("%d", &number);

    while (number < 1 || number > 5)
    {
        system("cls");
        printf("!!!   Invalid Number   !!!\n");
        printf("Enter Number Of Menu : ");
        scanf("%d", &number);
    }

    fflush(stdin);
    return number;
}


void Send_Message() // send message to another user
{
    while (true)
    {
        system("cls");
        printf("Send Message Menu : \n");
        printf("1.Send Message Menu\n");
        printf("2.Exit\n");
        printf("Enter Number Of Menu : ");
        int number ;

        scanf("%d", &number);

        while (number < 1 || number > 2)
        {
            system("cls");
            printf("!!!   Invalid Number   !!!\n");
            printf("Enter Number Of Menu : ");
            scanf("%d", &number);
        }

        switch (number)
        {
            case 1:
                Write_Message_To_File();
            break;

            case 2:
                return;
        }
    }
}




void Write_Message_To_File()
{
    FILE * message = fopen("message.txt" , "r");

    if ( ! Is_Open_File(message) )
    {
        fclose(message);
        message = fopen("message.txt" , "w");

        fprintf(message ,"%lu_%lu  " ,message_counter ,Current_ID);
        message_counter++;

        fflush(stdin);
        char character = fgetc(stdin);

        while (character != '\n')
        {
            fputc(character, message);
            character = fgetc(stdin);
        }

        fputc('\n', message);
    }

    else
    {
        fclose(message);
        message = fopen("message.txt" , "a");

        fprintf(message ,"%lu_%lu  " ,message_counter ,Current_ID);
        message_counter++;

        fflush(stdin);
        char character = fgetc(stdin);

        while (character != '\n')
        {
            fputc(character, message);
            character = fgetc(stdin);
        }

        fputc('\n', message);
    }

    fclose(message);
}


void Write_Id_And_Message_Counter_To_File()
{
    FILE * message_file = fopen("message_counter.txt" , "w");

    if (! Is_Open_File(message_file) )
    {
        perror("!!!   Can Not Open File In Write Id And Message Counter In File   !!!\n");
        exit(1);
    }

    fprintf(message_file , "%lu \t %lu" ,ID_Counter ,message_counter);
    fclose(message_file);
}


void Read_Id_And_Message_Counter_From_File()
{
    FILE * message_file = fopen("message_counter.txt" , "r");

    if (! Is_Open_File(message_file) )
    {
        perror("!!!   Can Not Open File In Read Id And Message Counter In File   !!!\n");
        fclose(message_file);
        return;
    }

    fscanf(message_file , "%lu %lu" ,&ID_Counter ,&message_counter);
    fclose(message_file);
}



void Edit_Message() // edit message
{
    while (true)
    {
        system("cls");
        printf("Edit Message Menu : \n");
        printf("1.Edit Message\n");
        printf("2.Exit\n");
        printf("Enter Number oO Menu : ");
        int number ;
        scanf("%d", &number);

        while (number < 1 || number > 2)
        {
            system("cls");
            printf("!!!   Invalid  Number   !!!\n");
            printf("Enter Number Of Menu : ");
            scanf("%d", &number);
        }

        switch (number)
        {
            case 1:
                Edit_Message_From_File();
                break;
            
            case 2 :
                return;
        }

    } // end of while(true)
}




void Edit_Message_From_File() // edit message from file
{
    system("cls");
    printf("Enter Message Id : ");
    unsigned long int entrance_message_number;
    scanf("%lu", &entrance_message_number);

    FILE * message_file = fopen("message.txt" , "r");

    if (! Is_Open_File(message_file) )
    {
        perror("!!!   Can Not Open File In Edit Message In File   !!!\n");
        fclose(message_file);
        return;
    }

    unsigned long int message_number;
    unsigned long int user_id;
    char ch;

    FILE * message_file_edit = fopen("message_edit.txt" , "w");

    if (! Is_Open_File(message_file_edit) )
    {
        perror("!!!   Can Not Open File In Edit Message In File   !!!\n");
        fclose(message_file);
        fclose(message_file_edit);
        return;
    }
    
    bool is_found = false;

    while (! feof(message_file))
    {
        fscanf(message_file ,"%lu %c %lu" , &message_number ,&ch ,&user_id);
        
        if (feof(message_file))
        {
            break;
        }

        if (message_number == entrance_message_number && user_id == Current_ID)
        {
            fflush(stdin);
            printf("Enter New Message : ");

            fprintf(message_file_edit , "%lu_%lu  " ,message_number ,user_id);
            
            char character = fgetc(stdin);

            while (character != '\n')
            {
                fputc(character, message_file_edit);
                character = fgetc(stdin);
            }

            fputc('\n', message_file_edit);

            character = fgetc(message_file);

            while (character != '\n')
            {
                character = fgetc(message_file);
            }

            is_found = true;
        }

        else
        {
            if (feof(message_file))
            {
                break;
            }
            
            fprintf(message_file_edit , "%lu_%lu " ,message_number ,user_id);

            ch = fgetc(message_file);

            while (ch != '\n')
            {
                ch = fgetc(message_file);
                fputc(ch, message_file_edit);
            }

        } // end of else

    } // end of while(! feof(message_file))

    fclose(message_file);
    fclose(message_file_edit);

    remove("message.txt");
    rename("message_edit.txt" , "message.txt");

    if (! is_found)
    {
        printf("!!!   This message was sent by another user   !!!\n");
        printf("Tap Enter Key To Continue\n");
        fflush(stdin);
        getchar();
    }

} // end of Edit_Message_From_File()



void Manage_Show_Messages() // manage show messages
{
    while (true)
    {
        system("cls");
        printf("Manage Show Messages Menu : \n");
        printf("1.Show All Messages\n");
        printf("2.Exit\n");
        printf("Enter Number oO Menu : ");
        int number ;
        scanf("%d", &number);

        while (number < 1 || number > 2)
        {
            system("cls");
            printf("!!!   Invalid  Number   !!!\n");
            printf("Enter Number Of Menu : ");
            scanf("%d", &number);
        }

        switch (number)
        {
            case 1:
                Show_All_Messages();
                break;
            
            case 2 :
                return;
        }

    } // end of while(true)
}



void Show_All_Messages() // show all messages
{
    system("cls");
    printf("All Messages : \n");
    FILE * message_file = fopen("message.txt" , "r");

    if (! Is_Open_File(message_file) )
    {
        perror("!!!   Can Not Open File In Show All Messages In File   !!!\n");
        fclose(message_file);
        return;
    }

    FILE * user_file = fopen("user.dat" , "rb");

    if (! Is_Open_File(user_file) )
    {
        perror("!!!   Can Not Open File In Show All Messages In File   !!!\n");
        fclose(message_file);
        fclose(user_file);
        return;
    }

    unsigned long int message_number;
    unsigned long int user_id;
    char ch;

    while (! feof(message_file))
    {
        fscanf(message_file ,"%lu %c %lu " , &message_number ,&ch ,&user_id);
        
        if (feof(message_file))
        {
            break;
        }
        
        fseek(user_file , user_id * sizeof(account) , SEEK_SET);
        account user;
        fread(&user , sizeof(account) , 1 , user_file);

        printf("%s _ %lu: " ,user.username ,message_number);
        
        ch = fgetc(message_file);
        
        while (ch != '\n')
        {
            printf("%c" ,ch);
            ch = fgetc(message_file);
        }

        fputc('\n', stdout);
    }
    
    fclose(message_file);
    fclose(user_file);

    printf("Tap Enter Key To Continue\n");
    fflush(stdin);
    getchar();

} // end of Show_All_Messages()




void Manage_Change_Password() // manage change password
{
    while (true)
    {
        system("cls");
        printf("Manage Change Password Menu : \n");
        printf("1.Change Password\n");
        printf("2.Exit\n");
        printf("Enter Number oO Menu : ");
        int number ;
        scanf("%d", &number);

        while (number < 1 || number > 2)
        {
            system("cls");
            printf("!!!   Invalid  Number   !!!\n");
            printf("Enter Number Of Menu : ");
            scanf("%d", &number);
        }

        switch (number)
        {
            case 1:
                Change_Password();
                break;
            
            case 2 :
                return;
        }

    } // end of while(true)
}



void Change_Password()
{
    printf("Enter New Password : ");
    char new_password[51];
    fflush(stdin);
    fgets(new_password , 50 , stdin);
    new_password[strlen(new_password) - 1] = '\0';

    FILE * user_file = fopen("user.dat" , "rb+");

    if (! Is_Open_File(user_file) )
    {
        perror("!!!   Can Not Open File In Change Password In File   !!!\n");
        fclose(user_file);
        return;
    }

    fseek(user_file , Current_ID * sizeof(account) , SEEK_SET);
    account user;
    fread(&user , sizeof(account) , 1 , user_file);


    strcpy(user.password , new_password);


    fseek(user_file , Current_ID * sizeof(account) , SEEK_SET);
    fwrite(&user , sizeof(account) , 1 , user_file);
    fclose(user_file);

    printf("Password Has Been Changed\n");
    printf("Tap Enter Key To Continue\n");
    fflush(stdin);
    getchar();

} // end of Change_Password()