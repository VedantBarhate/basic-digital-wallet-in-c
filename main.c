#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

typedef struct transaction
{
    int from;
    int to;
    float amnt;
} Transaction;

typedef struct user
{
    int acc_no;
    char name[MAX];
    char email[MAX];
    char pswd[MAX];
    char trans_key[MAX];
    float balance;
} User;

int authentication_pg_input();
void authentication_page();
void login();
void create_acc();
Transaction build_ackno(int from, int to, float amnt);
int home_pg_input();
void home_page(User user);
void profile(User user);
void pay(User user);
void payment_process(User user, User recvr);
void see_transactions(User user);
void add_balance(User user);

int main()
{
    printf("\nWELCOME TO FAKE-PAY\n");
    authentication_page();
    return 0;
}

int authentication_pg_input()
{
    int choice;
    printf("1. Login\n2. Create Account\n3. Exit\n>");
    scanf("%d", &choice);
    if (choice < 1 || choice > 3)
    {
        printf("Invalid Input!...Plz. Try Again.\n");
        return authentication_pg_input();
    }
    return choice;
}

void authentication_page()
{
    printf("\n@ Authentication Page\n");
    int choice = authentication_pg_input();
    switch (choice)
    {
    case 1:
        login();
        break;
    case 2:
        create_acc();
        break;
    case 3:
        printf("\nExiting...\nThank You for using FAKE-PAY\n");
        exit(0);
        break;
    }
}

void login()
{
    int acc_no;
    char pswd[MAX];
    printf("\n@ Login_Page\n");
    printf("Account Number: ");
    scanf("%d", &acc_no);
    printf("Password: ");
    scanf("%s", pswd);
    char user_file_name[MAX];
    snprintf(user_file_name, sizeof(user_file_name), "database/users/%d.bin", acc_no);
    FILE *user_file = fopen(user_file_name, "rb");
    if (user_file == NULL)
    {
        printf("Account Not Found!...Try Again\n");
        return login();
    }
    else
    {
        User user;
        fread(&user, sizeof(User), 1, user_file);
        if (strcmp(pswd, user.pswd) == 0)
        {
            printf("Login Successfull...\n");
            return home_page(user);
        }
        else
        {
            printf("Login Failed!...Try Again\n");
            return login();
        }
    }
}

void create_acc()
{
    printf("\n@ Create_Account_Page\n");
    User user;
    int acc_no;
    char name[MAX], email[MAX], pswd[MAX], trans_key[MAX];
    float balance;
    Transaction transaction;
    FILE *lan_file = fopen("database/last_acc_no.txt", "r");
    int last;
    fscanf(lan_file, "%d", &last);
    fclose(lan_file);
    acc_no = last + 1;
    printf("Your account number is %d\n", acc_no);
    char user_file_name[MAX];
    snprintf(user_file_name, sizeof(user_file_name), "database/users/%d.bin", acc_no);
    FILE *user_file = fopen(user_file_name, "wb");
    user.acc_no = acc_no;
    printf("Name: ");
    scanf("%s", name);
    strcpy(user.name, name);
    printf("Email: ");
    scanf("%s", email);
    strcpy(user.email, email);
    printf("Password: ");
    scanf("%s", pswd);
    strcpy(user.pswd, pswd);
    printf("Transaction Key: ");
    scanf("%s", trans_key);
    strcpy(user.trans_key, trans_key);
    printf("Add balance: ");
    scanf("%f", &balance);
    user.balance = balance;
    fwrite(&user, sizeof(User), 1, user_file);
    fclose(user_file);
    transaction = build_ackno(0, acc_no, balance);
    char user_tfile_name[MAX];
    snprintf(user_tfile_name, sizeof(user_tfile_name), "database/transactions/%d_transactions.bin", acc_no);
    FILE *user_tfile = fopen(user_tfile_name, "ab");
    fwrite(&transaction, sizeof(Transaction), 1, user_tfile);
    fclose(user_tfile);
    printf("Account Successfully Created!!\n");
    lan_file = fopen("database/last_acc_no.txt", "w");
    fprintf(lan_file, "%d", acc_no);
    fclose(lan_file);
    login();
}

Transaction build_ackno(int from, int to, float amnt)
{
    Transaction transaction;
    transaction.from = from;
    transaction.to = to;
    transaction.amnt = amnt;
    return transaction;
}

int home_pg_input()
{
    int choice;
    printf("1. See Profile\n2. Pay\n3. See Transactions\n4. Add Balance\n5. Exit\n> ");
    scanf("%d", &choice);
    if (choice < 1 || choice > 5)
    {
        printf("Invalid Input!...Plz. Try Again.\n");
        return home_pg_input();
    }
    return choice;
}

void home_page(User user)
{
    int choice;
    char c;
    do
    {
        printf("\n@ Home\n");
        choice = home_pg_input();
        switch (choice)
        {
        case 1:
            profile(user);
            break;
        case 2:
            pay(user);
            break;
        case 3:
            see_transactions(user);
            break;
        case 4:
            add_balance(user);
            break;
        case 5:
            printf("\nExiting...\nThank You for using FAKE-PAY\n");
            break;
        }
        scanf("%c", &c);
        getchar();
        char user_file_name[MAX];
        snprintf(user_file_name, sizeof(user_file_name), "database/users/%d.bin", user.acc_no);
        FILE *user_file = fopen(user_file_name, "rb");
        fread(&user, sizeof(User), 1, user_file);
        fclose(user_file);
    } while (choice != 5);
}

void profile(User user)
{
    printf("\n@ Profile\n");
    printf("Account Number: %d\n", user.acc_no);
    printf("Name: %s\n", user.name);
    printf("Email: %s\n", user.email);
    printf("Balance: %f\n", user.balance);
}

void pay(User user)
{
    int to;
    printf("\n@ Pay\n");
    printf("Enter Account Number of Receiver: ");
    scanf("%d", &to);
    char recvr_file_name[MAX];
    snprintf(recvr_file_name, sizeof(recvr_file_name), "database/users/%d.bin", to);
    FILE *recvr_file = fopen(recvr_file_name, "rb");
    if (recvr_file == NULL)
    {
        printf("Account Not Found!...Try Again\n");
        return pay(user);
    }
    else
    {
        User recvr;
        fread(&recvr, sizeof(User), 1, recvr_file);
        fclose(recvr_file);
        payment_process(user, recvr);
    }
    fclose(recvr_file);
}

void payment_process(User user, User recvr)
{
    float amnt;
    printf("Your balance is %f\n", user.balance);
    printf("Enter Amount (0 to cancel): ");
    scanf("%f", &amnt);
    if (amnt <= 0)
    {
        printf("Transaction Canceled!!!\n");
    }
    else if (amnt <= user.balance)
    {
        char tkey[MAX];
        printf("Enter Transaction Key: ");
        scanf("%s", tkey);
        if (strcmp(tkey, user.trans_key) == 0)
        {
            printf("Transaction is being taking place...\n");
            user.balance = user.balance - amnt;
            recvr.balance = recvr.balance + amnt;
            char user_file_name[MAX], recvr_file_name[MAX];
            snprintf(user_file_name, sizeof(user_file_name), "database/users/%d.bin", user.acc_no);
            FILE *user_file = fopen(user_file_name, "wb");
            fwrite(&user, sizeof(User), 1, user_file);
            fclose(user_file);
            snprintf(recvr_file_name, sizeof(recvr_file_name), "database/users/%d.bin", recvr.acc_no);
            FILE *recvr_file = fopen(recvr_file_name, "wb");
            fwrite(&recvr, sizeof(User), 1, recvr_file);
            fclose(recvr_file);
            Transaction transaction = build_ackno(user.acc_no, recvr.acc_no, amnt);
            char user_tfile_name[MAX], recvr_tfile_name[MAX];
            snprintf(user_tfile_name, sizeof(user_tfile_name), "database/transactions/%d_transactions.bin", user.acc_no);
            FILE *user_tfile = fopen(user_tfile_name, "ab");
            fwrite(&transaction, sizeof(Transaction), 1, user_tfile);
            fclose(user_tfile);
            snprintf(recvr_tfile_name, sizeof(recvr_tfile_name), "database/transactions/%d_transactions.bin", recvr.acc_no);
            FILE *recvr_tfile = fopen(recvr_tfile_name, "ab");
            fwrite(&transaction, sizeof(Transaction), 1, recvr_tfile);
            fclose(recvr_tfile);
            printf("Transaction Done!!!\n");
        }
        else
        {
            printf("Validation Failed...Try Again\n");
            payment_process(user, recvr);
        }
    }
    else
    {
        printf("Insufficient Balance...Try Agian\n");
        payment_process(user, recvr);
    }
}

void see_transactions(User user)
{
    printf("\n@ Transactions\n");
    char tfile_name[MAX];
    snprintf(tfile_name, sizeof(tfile_name), "database/transactions/%d_transactions.bin", user.acc_no);
    FILE *tfile = fopen(tfile_name, "rb");
    Transaction transaction;
    int transaction_num = 1;
    while (fread(&transaction, sizeof(Transaction), 1, tfile) == 1)
    {
        printf("Transaction no.: %d\n", transaction_num);
        if (transaction.from == 0)
        {
            printf("    From: Balance Transfer\n");
        }
        else
        {
            printf("    From: %d\n", transaction.from);
        }
        printf("    To: %d\n", transaction.to);
        printf("    Amount: %f\n", transaction.amnt);
        transaction_num++;
    }
    fclose(tfile);
}

void add_balance(User user)
{
    float amnt;
    char user_file_name[MAX];
    printf("\n@ Add Balance\n");
    printf("Current Balance: %f\n", user.balance);
    printf("Enter amount to add: ");
    scanf("%f", &amnt);
    user.balance = user.balance + amnt;
    snprintf(user_file_name, sizeof(user_file_name), "database/users/%d.bin", user.acc_no);
    FILE *user_file = fopen(user_file_name, "wb");
    fwrite(&user, sizeof(User), 1, user_file);
    fclose(user_file);
    printf("Balance Updated\n");
    printf("Updated Balance: %f", user.balance);
}