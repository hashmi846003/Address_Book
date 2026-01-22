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
        getchar();

        switch (choice)
        {
            case 'A': case 'a': add_contact(&abk);    break;
            case 'E': case 'e': edit_contact(&abk);   break;
            case 'S': case 's': search_contact(&abk); break;
            case 'L': case 'l': list_contacts(&abk);  break;
            case 'D': case 'd': delete_contact(&abk); break;
            case 'V': case 'v': save_contacts(&abk);  break;
            case 'X': case 'x': log_info(K_SUCCESS,"Exit");
			save_contacts(&abk);   
            log_info(K_SUCCESS,"Exit"); 
			break;
            default: log_warn(K_FAIL,"Invalid Choice");
        }
    }
    while (choice != 'X' && choice != 'x');

    return 0;
}
