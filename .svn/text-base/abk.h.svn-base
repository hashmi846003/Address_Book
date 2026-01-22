#ifndef ADDRESS_BOOK_H
#define ADDRESS_BOOK_H

#define DEFAULT_FILE "telephone.csv"
#define DEFAULT_LIST_SIZE 32

#define SAVED 0
#define NOT_SAVED 1


#define INITIAL_SIZE 32
#define NAME_SIZE 32 // size of a contact name 
#define NUMBER_SIZE 32 // size of a phone number
#define EMAIL_SIZE 32 // size of an email address

#define PHONE_NUMBER_COUNT 5 // maximum number of phones per contact
#define EMAIL_ADDRESS_COUNT 5 // maximum number of email addresses per contact

#define DETAIL_SIZE 32
#define DETAIL_ITEM_COUNT 5

#define MSG_VALID_CHOICE "Please enter a valid choice.\n"

#define CLR_SCREEN system("clear") // command for clearing the console

#define WAIT_FOR_ENTER_KEY  while(getchar() != '\n')

typedef int bool_t;
#define FALSE 0
#define TRUE 1


typedef enum
{
	K_FAIL,
	K_ADDRESS_BOOK_NULL,
	K_FILE_NOT_AVAILABLE,
	K_FILE_COULD_NOT_BE_CREATED,
	K_FILENAME_NULL,
	K_MEMORY_ALLOCATION_FAILURE,
	K_SUCCESS,
	K_WRONG_FILE_FORMAT,
} return_t;

typedef struct
{
	char name[NAME_SIZE];
	char phone_numbers[PHONE_NUMBER_COUNT][NUMBER_SIZE];
	char email_addresses[EMAIL_ADDRESS_COUNT][EMAIL_SIZE];
} contact_detail_t;


typedef struct
{
	contact_detail_t *list;
	int size;
	int count;	
} abk_t;



return_t log_error(return_t result, char *msg);
return_t log_warn(return_t result, char *msg);
return_t log_info(return_t result, char *msg);

#endif /*  ADDRESS_BOOK_H */
