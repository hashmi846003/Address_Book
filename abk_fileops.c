#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "abk.h"



static int valid_name(const char *name)
{
    int len = strlen(name);
    if (len < 4) return 0;

    for (int i = 0; i < len; i++)
        if (!isalpha((unsigned char)name[i]) && name[i] != ' ')
            return 0;

    return 1;
}

static int valid_phone(const char *phone)
{
    if (strlen(phone) != 10) return 0;
    if (phone[0] < '6' || phone[0] > '9') return 0;

    for (int i = 0; i < 10; i++)
        if (!isdigit((unsigned char)phone[i]))
            return 0;

    return 1;
}

static int valid_email(const char *email)
{
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');

    if (!at || strchr(at + 1, '@')) return 0;
    if (!dot || dot < at) return 0;
    if ((dot - at - 1) < 4) return 0;

    return 1;
}

static int is_duplicate(abk_t *abk, contact_t *c)
{
    for (int i = 0; i < abk->count; i++)
    {
        contact_t *e = &abk->list[i];
        if (!strcmp(e->name, c->name) &&
            !strcmp(e->phone, c->phone) &&
            !strcmp(e->email, c->email) &&
            !strcmp(e->address, c->address))
            return 1;
    }
    return 0;
}


void init_address_book(abk_t *abk)
{
    abk->count = 0;
    load_contacts(abk);
}

void load_contacts(abk_t *abk)
{
    FILE *fp = fopen(DEFAULT_FILE, "r");
    if (!fp)
    {
        log_warn(SUCCESS, "No existing contact file found");
        return;
    }

    while (abk->count < MAX_CONTACTS &&
           fscanf(fp, "%d,%31[^,],%31[^,],%31[^,],%63[^\n]\n",
                  &abk->list[abk->count].id,
                  abk->list[abk->count].name,
                  abk->list[abk->count].phone,
                  abk->list[abk->count].email,
                  abk->list[abk->count].address) == 5)
    {
        abk->count++;
    }

    fclose(fp);
    log_info(SUCCESS, "Contacts loaded");
}

void add_contact(abk_t *abk)
{
    if (abk->count >= MAX_CONTACTS)
        return (void)log_error(FAILURE, "Address book full");

    contact_t temp;

    printf("Name    : "); fgets(temp.name, NAME_SIZE, stdin);
    printf("Phone   : "); fgets(temp.phone, NUMBER_SIZE, stdin);
    printf("Email   : "); fgets(temp.email, EMAIL_SIZE, stdin);
    printf("Address : "); fgets(temp.address, ADDRESS_SIZE, stdin);

    temp.name[strcspn(temp.name, "\n")] = 0;
    temp.phone[strcspn(temp.phone, "\n")] = 0;
    temp.email[strcspn(temp.email, "\n")] = 0;
    temp.address[strcspn(temp.address, "\n")] = 0;

    if (!valid_name(temp.name))
        return (void)log_error(FAILURE,
            "Invalid Name: alphabets & spaces only, min 4 chars");

    if (!valid_phone(temp.phone))
        return (void)log_error(FAILURE,
            "Invalid Phone: 10 digits, starts with 6–9");

    if (!valid_email(temp.email))
        return (void)log_error(FAILURE,
            "Invalid Email format");

    if (is_duplicate(abk, &temp))
        return (void)log_warn(FAILURE,
            "Duplicate contact not saved");

    temp.id = (abk->count == 0) ? 1 :
              abk->list[abk->count - 1].id + 1;

    abk->list[abk->count++] = temp;
    log_info(SUCCESS, "Contact added");
}

return_t search_contact(abk_t *abk)
{
    int opt, found = 0;
    char key[50];

    if (!abk || abk->count == 0)
        return log_warn(FAILURE, "Address book empty");

    printf("\nSearch by:\n1.Name\n2.Phone\n3.Email\n4.Address\nOption: ");
    scanf("%d", &opt);
    WAIT_FOR_ENTER_KEY;

    if (opt < 1 || opt > 4)
        return log_error(FAILURE, "Invalid option");

    printf("Enter search key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;

    for (int i = 0; i < abk->count; i++)
    {
        char *match = NULL;

        if (opt == 1) match = strstr(abk->list[i].name, key);
        else if (opt == 2) match = strstr(abk->list[i].phone, key);
        else if (opt == 3) match = strstr(abk->list[i].email, key);
        else if (opt == 4) match = strstr(abk->list[i].address, key);

        if (match)
        {
            printf("\n%d | %s | %s | %s | %s",
                   abk->list[i].id,
                   abk->list[i].name,
                   abk->list[i].phone,
                   abk->list[i].email,
                   abk->list[i].address);
            found = 1;
        }
    }

    printf("\n");
    return found ? log_info(SUCCESS, "Search completed")
                 : log_info(SUCCESS, "No match found");
}

void list_contacts(abk_t *abk)
{
    if (!abk->count)
        return (void)log_warn(SUCCESS, "No contacts");

    for (int i = 0; i < abk->count; i++)
        printf("\n%d | %s | %s | %s | %s",
               abk->list[i].id,
               abk->list[i].name,
               abk->list[i].phone,
               abk->list[i].email,
               abk->list[i].address);
    printf("\n");
}



void delete_contact(abk_t *abk)
{
    int choice;
    int id;
    char name[NAME_SIZE];
    int found = 0;

    if (abk->count == 0)
        return (void)log_warn(FAILURE, "Address book empty");

    printf("\nDelete by:\n");
    printf("1. ID\n");
    printf("2. Name\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    WAIT_FOR_ENTER_KEY;

    if (choice == 1)
    {
        printf("Enter ID: ");
        scanf("%d", &id);
        WAIT_FOR_ENTER_KEY;

        for (int i = 0; i < abk->count; i++)
        {
            if (abk->list[i].id == id)
            {
                abk->list[i] = abk->list[abk->count - 1];
                abk->count--;
                found = 1;
                break;
            }
        }
    }
    else if (choice == 2)
    {
        printf("Enter Name: ");
        fgets(name, NAME_SIZE, stdin);
        name[strcspn(name, "\n")] = '\0';

        for (int i = 0; i < abk->count; i++)
        {
            
            if (strcmp(abk->list[i].name, name) == 0)
            {
                abk->list[i] = abk->list[abk->count - 1];
                abk->count--;
                found = 1;
                break;   
            }
        }
    }
    else
    {
        return (void)log_error(FAILURE, "Invalid delete option");
    }

    if (found)
        log_info(SUCCESS, "Contact deleted successfully");
    else
        log_warn(FAILURE, "Contact not found");
}



void edit_contact(abk_t *abk)
{
    int id;
    printf("Enter ID to edit: ");
    scanf("%d", &id);
    WAIT_FOR_ENTER_KEY;

    for (int i = 0; i < abk->count; i++)
    {
        if (abk->list[i].id == id)
        {
            contact_t *c = &abk->list[i];

            printf("New Phone: "); fgets(c->phone, NUMBER_SIZE, stdin);
            printf("New Email: "); fgets(c->email, EMAIL_SIZE, stdin);

            c->phone[strcspn(c->phone, "\n")] = 0;
            c->email[strcspn(c->email, "\n")] = 0;

            if (!valid_phone(c->phone) || !valid_email(c->email))
                return (void)log_error(FAILURE,
                    "Invalid phone/email");

            if (is_duplicate(abk, c))
                return (void)log_warn(FAILURE,
                    "Duplicate after edit");

            return (void)log_info(SUCCESS, "Contact updated");
        }
    }
    log_warn(FAILURE, "ID not found");
}

void save_contacts(abk_t *abk)
{
    FILE *fp = fopen(DEFAULT_FILE, "w");
    if (!fp)
        return (void)log_error(FAILURE, "File open failed");

    for (int i = 0; i < abk->count; i++)
        fprintf(fp, "%d,%s,%s,%s,%s\n",
                abk->list[i].id,
                abk->list[i].name,
                abk->list[i].phone,
                abk->list[i].email,
                abk->list[i].address);

    fclose(fp);
    log_info(SUCCESS, "Contacts saved");
}
