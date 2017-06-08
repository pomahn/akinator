#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <ctype.h>
#include <assert.h>

#define TRASH 0xFEFEFEFE
#define CANARY 0xBADF00D
#define MAX_STR_LEN 100
#define HASH_TABLE_SIZE 10
#define TRASH_WORD "BLABLABLA"
#define WHERE printf ("File %s Line %d Function %s \n", __FILE__, __LINE__, __FUNCTION__);

typedef struct List_elem
    {
        double can1;
        char word[MAX_STR_LEN];
        //List_elem* prev;
        List_elem* right;
        List_elem* left;

        double can2;
    };



typedef struct List_header
    {
        double can1;
        int elem_amount;
        List_elem* head;
        double can2;
    };


List_elem* List_elem_ctor (char* word);

void List_elem_dtor (List_elem* This);

void Insert_right (List_elem* Where, List_elem* What);

void Insert_left (List_elem* Where, List_elem* What);

bool List_OK (const List_elem* This);

void Tree_save (List_header* This);

void Word_read (FILE* file, char* s);

List_elem* List_prefix_build ();

List_header* Tree_build ();

void List_prefix_save (List_elem* This);

void Tree_save_prefix (List_header* Head);

void Dotter_easy_show (List_elem* This);

void Tree_show (List_header* Header);

void Akinator(List_header* Header);

void New_akinator(List_header* Header);

FILE* save = NULL;
FILE* dotter_easy = NULL;
FILE* savee = NULL;

int main()
    {

        List_header* Header = Tree_build();
        printf ("%x\n", (int)Header);
        //Tree_save_prefix (Header);
        printf("\n\n NOW YOU ARE PLAYING AKINATOR \n\n WUAHAHAHAHHA \n\n\n");

        Akinator(Header);

        Tree_save_prefix (Header);


        Tree_show (Header);

        return 0;
    }



List_elem* List_elem_ctor (char* word)
    {
        List_elem* This = (List_elem*) calloc (1, sizeof (List_elem));
        This->can1 = CANARY;

        strcpy(This->word, word);
        printf("%s\n", This->word);
        //This->prev = NULL;
        This->right = NULL;
        This->left = NULL;

        This->can2 = CANARY;

        return This;
    }

void List_elem_dtor (List_elem* This)
    {

        This->can1 = TRASH;

        strcpy(This->word, TRASH_WORD);
        //This->prev = NULL;
        This->right = NULL;
        This->left = NULL;

        This->can2 = TRASH;
        free (This);
    }


void Insert_right (List_elem* Where, List_elem* What)
    {
        Where->right = What;
       // What->prev = Where;
    }


void Insert_left (List_elem* Where, List_elem* What)
    {
        Where->left = What;
       // What->prev = Where;
    }

//�������� �� ����� ������ ��� ���� ������ ������ ������� (�������� �������� ������� � ������ ������)
//���������� �����

void Tree_save_prefix (List_header* Header)
    {
        savee = fopen("tree_save.txt", "w");
        List_prefix_save (Header->head);
        fclose (savee);

    }


void List_prefix_save (List_elem* This)
    {
        printf("Save\n");
        if (This == NULL) {printf("NULL\n"); fprintf( savee, "nil "); return ;}

        fprintf (savee, "( %s ",  This->word );
        //printf("This->left = %x \n", (int)This->left);
        List_prefix_save (This->left);
        List_prefix_save (This->right);

        fprintf (savee, ") ");

    }

List_header* Tree_build ()
    {
        List_header* header = (List_header*) calloc (1, sizeof (List_header));



        save = fopen ("tree_save.txt", "r");

        header->head = List_prefix_build();

        fclose (save);

        return header;
    }


List_elem* List_prefix_build ()
    {
        char word[MAX_STR_LEN];
        Word_read (save, word);

        if (strcmp (word, "(") == 0)
            {
                printf("open\n");
                List_elem* new_list = NULL;
                Word_read (save, word);

                printf("asd = %s\n", word);

                new_list = List_elem_ctor (word);
                printf("new_list = %x\n", (int)new_list);

                printf("left\n");
                new_list->left = List_prefix_build ();

                printf("right\n");
                new_list->right = List_prefix_build ();

                Word_read (save, word);
                assert (word != ")");
                printf("close\n");

                return new_list;
            }

        if (strcmp (word,"nil") == 0)
            {
                printf("nil\n");
                return NULL;
            }
printf("ERROR FAIL \n");
    }



void Word_read (FILE* file, char* s)
    {
        char c = fgetc (file);

        int i=0;
        while (c == ' ') {c = fgetc (file);}

        while ((c != ' ') && (c != '\n') && (c != EOF))
            {
                s[i] = c;

                c = fgetc (file);

                i++;
            }
        s[i] = '\0';
    }


void Tree_show (List_header* Header)
    {
        printf ("dotteeeeeeeeeeeeeer\n");
        dotter_easy = fopen ("dotter.txt", "w");

        fprintf (dotter_easy, "digraph {  \n       ");

        Dotter_easy_show (Header->head);
        fprintf (dotter_easy, "\n }");

        fclose (dotter_easy);
    }


void Dotter_easy_show (List_elem* This)
    {
        if (This == NULL) { printf("Ha\n");return ;}

        if (This->left != NULL)
            {
                printf ("left\n");
                printf ("%s\n", This->left->word);
                fprintf (dotter_easy, "%s -> %s [label=\"n\"]\n",  This->word, This->left->word );
            }

        if (This->right != NULL)
            {
                printf ("right\n");
                fprintf (dotter_easy, "%s -> %s [label=\"y\"]\n",  This->word, This->right->word );
            }

        Dotter_easy_show (This->left);
        Dotter_easy_show (This->right);

    }




void Akinator(List_header* Header)
    {
        char new_attempt [MAX_STR_LEN] = "y";

        while (1)
            {


                if ( strcmp (new_attempt, "y") == 0 || strcmp (new_attempt, "Y") == 0)
                    {
                        New_akinator(Header);

                        printf ("Do you want to play again? \n Press y/n \n");

                        scanf ("%s", new_attempt);
                    }

                else break;

            }
    }


void New_akinator(List_header* Header)
    {
        char answer [MAX_STR_LEN] = "Nothing yet";
        char additional [MAX_STR_LEN] = "Blabla";

        List_elem* element = Header->head;

        while (1)
        {

        if ((element->left !=NULL) & (element->right !=NULL))     //�������� �� ��, �������� �� ���� ��������
            {
                printf ("%s ? \n Press y/n \n" , element->word );

                scanf ("%s", answer);

                if ( strcmp (answer, "y") == 0|| strcmp (answer, "Y") == 0)
                    {
                        element = element->right;
                    }
                else
                    {
                        element = element->left;
                    }
            }

        else
            {
                printf ("Is your object %s \n Press y/n \n", element->word);

                scanf ("%s", answer);

                if (strcmp (answer, "y") == 0 || strcmp (answer, "Y") == 0)
                    {
                        printf ("Yeeeeeeeeeeeeeeeeah\n");
                        return;
                    }
                else
                    {
                        printf ("What is your object? \n");
                        scanf ("%s", answer);

                        printf ("And what is the difference between \n %s and %s ? \n", answer, element->word);
                        scanf ("%s", additional);

                        element->right = List_elem_ctor (answer);
                        element->left = List_elem_ctor (element->word);

                        strcpy (element->word, additional);

                        printf("Addding process completed \n");
                        return;

                    }
            }
        }
    }



























