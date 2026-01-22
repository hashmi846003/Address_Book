#include <stdio.h>
#include "abk.h"

void show_menu(void)
{
    printf("\n----- ADDRESS BOOK -----\n");
    printf("A  Add Contact\n");
    printf("E  Edit Contact\n");
    printf("S  Search Contact\n");
    printf("L  List Contacts\n");
    printf("D  Delete Contact\n");
    printf("V  Save Contacts\n");
    printf("X  Exit\n");
    printf("------------------------\n");
    printf("Choice: ");
}
