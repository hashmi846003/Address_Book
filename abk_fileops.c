#include <stdio.h>
#include <string.h>
#include "abk.h"

/* INIT */
void init_address_book(abk_t *abk)
{
    abk->count = 0;
    load_contacts(abk);
}

/* LOAD */
void load_contacts(abk_t *abk)
{
    FILE *fp = fopen(DEFAULT_FILE, "r");
    if (!fp)
        return;

    while (fscanf(fp,
        "%d,%31[^,],%31[^,],%31[^,],%63[^\n]\n",
        &abk->list[abk->count].id,
        abk->list[abk->count].name,
        abk->list[abk->count].phone,
        abk->list[abk->count].email,
        abk->list[abk->count].address) == 5)
    {
        abk->count++;
        if (abk->count >= MAX_CONTACTS)
            break;
    }
    fclose(fp);
}

/* ADD */
void add_contact(abk_t *abk)
{
    if (abk->count >= MAX_CONTACTS)
    {
        log_warn(K_FAIL, "Address Book Full");
        return;
    }

    contact_t *c = &abk->list[abk->count];
    c->id = (abk->count == 0) ? 1 : abk->list[abk->count-1].id + 1;

    printf("Name    : "); fgets(c->name, NAME_SIZE, stdin);
    printf("Phone   : "); fgets(c->phone, NUMBER_SIZE, stdin);
    printf("Email   : "); fgets(c->email, EMAIL_SIZE, stdin);
    printf("Address : "); fgets(c->address, ADDRESS_SIZE, stdin);

    c->name[strcspn(c->name,"\n")] = 0;
    c->phone[strcspn(c->phone,"\n")] = 0;
    c->email[strcspn(c->email,"\n")] = 0;
    c->address[strcspn(c->address,"\n")] = 0;

    abk->count++;
    log_info(K_SUCCESS, "Contact Added");
}

/* LIST */
void list_contacts(abk_t *abk)
{
    if (!abk->count)
    {
        log_warn(K_FAIL, "No Contacts");
        return;
    }

    for (int i = 0; i < abk->count; i++)
    {
        printf("ID:%d | %s | %s | %s | %s\n",
               abk->list[i].id,
               abk->list[i].name,
               abk->list[i].phone,
               abk->list[i].email,
               abk->list[i].address);
    }
    WAIT_FOR_ENTER_KEY;
}

/* SEARCH (name/phone/email/address) */
// void search_contact(abk_t *abk)
// {
//     char key[64];
//     int found = 0;

//     printf("Search: ");
//     fgets(key, sizeof(key), stdin);
//     key[strcspn(key,"\n")] = 0;

//     for (int i = 0; i < abk->count; i++)
//     {
//         if (strstr(abk->list[i].name, key) ||
//             strstr(abk->list[i].phone, key) ||
//             strstr(abk->list[i].email, key) ||
//             strstr(abk->list[i].address, key))
//         {
//             printf("ID:%d | %s | %s | %s | %s\n",
//                    abk->list[i].id,
//                    abk->list[i].name,
//                    abk->list[i].phone,
//                    abk->list[i].email,
//                    abk->list[i].address);
//             found = 1;
//         }
//     }

//     if (!found)
//         log_warn(K_FAIL, "No Match Found");

//     WAIT_FOR_ENTER_KEY;
// }
void search_contact(abk_t *abk)
{
    char key[64];
    int found = 0;

    printf("Search (name/phone/email/address): ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;  // remove newline

    printf("\n--- Search Results ---\n");

    for (int i = 0; i < abk->count; i++)
    {
        if (strstr(abk->list[i].name, key) ||
            strstr(abk->list[i].phone, key) ||
            strstr(abk->list[i].email, key) ||
            strstr(abk->list[i].address, key))
        {
            printf("ID:%d | %s | %s | %s | %s\n",
                   abk->list[i].id,
                   abk->list[i].name,
                   abk->list[i].phone,
                   abk->list[i].email,
                   abk->list[i].address);
            found = 1;
        }
    }

    if (!found)
        log_warn(K_FAIL, "No matching contact found");

    WAIT_FOR_ENTER_KEY;
}


/* EDIT (by ID) */
void edit_contact(abk_t *abk)
{
    int id;
    printf("Enter ID to edit: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < abk->count; i++)
    {
        if (abk->list[i].id == id)
        {
            printf("New Phone: "); fgets(abk->list[i].phone, NUMBER_SIZE, stdin);
            printf("New Email: "); fgets(abk->list[i].email, EMAIL_SIZE, stdin);

            abk->list[i].phone[strcspn(abk->list[i].phone,"\n")] = 0;
            abk->list[i].email[strcspn(abk->list[i].email,"\n")] = 0;

            log_info(K_SUCCESS, "Contact Updated");
            return;
        }
    }
    log_warn(K_FAIL, "ID Not Found");
}

/* DELETE (by ID) */
void delete_contact(abk_t *abk)
{
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < abk->count; i++)
    {
        if (abk->list[i].id == id)
        {
            abk->list[i] = abk->list[abk->count - 1];
            abk->count--;
            log_info(K_SUCCESS, "Contact Deleted");
            return;
        }
    }
    log_warn(K_FAIL, "ID Not Found");
}

/* SAVE */
void save_contacts(abk_t *abk)
{
    FILE *fp = fopen(DEFAULT_FILE, "w");
    if (!fp)
    {
        log_error(K_FAIL, "File Open Failed");
        return;
    }

    for (int i = 0; i < abk->count; i++)
    {
        fprintf(fp, "%d,%s,%s,%s,%s\n",
                abk->list[i].id,
                abk->list[i].name,
                abk->list[i].phone,
                abk->list[i].email,
                abk->list[i].address);
    }
    fclose(fp);
    log_info(K_SUCCESS, "Contacts Saved");
}
