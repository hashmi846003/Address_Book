#ifndef ADDRESS_BOOK_H
#define ADDRESS_BOOK_H

#include <stdio.h>

#define DEFAULT_FILE "telephone.csv"
#define MAX_CONTACTS 100

#define NAME_SIZE     32
#define NUMBER_SIZE   32
#define EMAIL_SIZE    32
#define ADDRESS_SIZE  64

#define WAIT_FOR_ENTER_KEY while(getchar()!='\n')

typedef enum
{
    K_FAIL,
    K_SUCCESS
} return_t;

/* Contact */
typedef struct
{
    int  id;
    char name[NAME_SIZE];
    char phone[NUMBER_SIZE];
    char email[EMAIL_SIZE];
    char address[ADDRESS_SIZE];
} contact_t;

/* Address book */
typedef struct
{
    contact_t list[MAX_CONTACTS];
    int count;
} abk_t;

/* menu */
void show_menu(void);

/* operations */
void init_address_book(abk_t *abk);
void load_contacts(abk_t *abk);
void add_contact(abk_t *abk);
void edit_contact(abk_t *abk);
void search_contact(abk_t *abk);
void list_contacts(abk_t *abk);
void delete_contact(abk_t *abk);
void save_contacts(abk_t *abk);

/* logging */
return_t log_info(return_t result, char *msg);
return_t log_warn(return_t result, char *msg);
return_t log_error(return_t result, char *msg);

#endif
