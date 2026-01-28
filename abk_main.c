#include "abk.h"

int main(void)
{
    abk_t abk;
    char choice;

    init_address_book(&abk);

    do
    {
        show_menu();
        scanf(" %c", &choice);
        WAIT_FOR_ENTER_KEY;

        switch (choice)
        {
            case 'A': case 'a': 
            add_contact(&abk);    
            break;
            case 'E': case 'e': 
            edit_contact(&abk);   
            break;
            case 'S': case 's': 
            search_contact(&abk); 
            break;
            case 'L': case 'l': 
            list_contacts(&abk);  
            break;
            case 'D': case 'd': 
            delete_contact(&abk); 
            break;
            case 'V': case 'v': 
            save_contacts(&abk);  
            break;
            case 'X': case 'x':
                save_contacts(&abk);
                log_info(SUCCESS, "Exit");
                break;
            default:
                log_warn(FAILURE, "Invalid choice");
        }
    }
    while (choice != 'X' && choice != 'x');

    return 0;
}
