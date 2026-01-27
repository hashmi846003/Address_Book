#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "abk.h"


static int is_valid_name(const char *name)
{
    int len = strlen(name);

    if (len < 4)
        return 0;

    for (int i = 0; i < len; i++)
    {
        if (!isalpha(name[i]))
            return 0;
    }
    return 1;
}


static int is_valid_phone(const char *phone)
{
    if (strlen(phone) != 10)
        return 0;

    if (phone[0] < '6' || phone[0] > '9')
        return 0;

    for (int i = 0; i < 10; i++)
    {
        if (!isdigit(phone[i]))
            return 0;
    }
    return 1;
}


static int is_valid_email(const char *email)
{
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');

    if (!at || strchr(at + 1, '@'))
        return 0;

    if (!dot || dot < at)
        return 0;

    if ((dot - at - 1) < 4)
        return 0;

    return 1;
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


void add_contact(abk_t *abk)
{
    if (abk->count >= MAX_CONTACTS)
    {
        log_warn(K_FAIL, "Address Book Full");
        return;
    }

    contact_t temp;

    printf("Name    : ");
    fgets(temp.name, NAME_SIZE, stdin);
    printf("Phone   : ");
    fgets(temp.phone, NUMBER_SIZE, stdin);
    printf("Email   : ");
    fgets(temp.email, EMAIL_SIZE, stdin);
    printf("Address : ");
    fgets(temp.address, ADDRESS_SIZE, stdin);

    temp.name[strcspn(temp.name, "\n")] = 0;
    temp.phone[strcspn(temp.phone, "\n")] = 0;
    temp.email[strcspn(temp.email, "\n")] = 0;
    temp.address[strcspn(temp.address, "\n")] = 0;

    if (!is_valid_name(temp.name))
    {
        log_warn(K_FAIL, "Invalid Name (min 4 alphabets)");
        return;
    }

    if (!is_valid_phone(temp.phone))
    {
        log_warn(K_FAIL, "Invalid Phone (10 digits, starts 6–9)");
        return;
    }

    if (!is_valid_email(temp.email))
    {
        log_warn(K_FAIL, "Invalid Email Format");
        return;
    }

    temp.id = (abk->count == 0) ? 1 : abk->list[abk->count - 1].id + 1;
    abk->list[abk->count++] = temp;

    log_info(K_SUCCESS, "Contact Added");
}


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


void search_contact(abk_t *abk)
{
    char key[64];
    int found = 0;

    printf("Search (name/phone/email/address): ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;

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
        log_warn(K_FAIL, "No Matching Contact Found");

    WAIT_FOR_ENTER_KEY;
}


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
            printf("New Phone: ");
            fgets(abk->list[i].phone, NUMBER_SIZE, stdin);
            printf("New Email: ");
            fgets(abk->list[i].email, EMAIL_SIZE, stdin);

            abk->list[i].phone[strcspn(abk->list[i].phone, "\n")] = 0;
            abk->list[i].email[strcspn(abk->list[i].email, "\n")] = 0;

            if (!is_valid_phone(abk->list[i].phone) ||
                !is_valid_email(abk->list[i].email))
            {
                log_warn(K_FAIL, "Invalid Updated Data");
                return;
            }

            log_info(K_SUCCESS, "Contact Updated");
            return;
        }
    }
    log_warn(K_FAIL, "ID Not Found");
}


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
