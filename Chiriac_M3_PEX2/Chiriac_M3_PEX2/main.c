//In this programming exercise you will create a Polynomial library to represent and manipulate polynomial expressions of the form, 4x3+1x2-3x+9.
//Author: Chiriac Adrian
//Date: 3/15/15
//Documentation:
//http://stackoverflow.com/questions/20020695/creating-a-polynomial-with-a-linked-list-in-c
//http ://stackoverflow.com/questions/1806074/c-extract-polynomial-coefficients
//http ://www.tutorialspoint.com/c_standard_library/c_function_strtol.htm
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "polynomial_library.h"

int read_from_file(Polynomial* library, const char* filename)
{
    int i, l, active;
    char line[1000];
    FILE* file;
    
    active = 0;
    file = fopen(filename, "r");
    if (file)    
    {        
        for(i=0; i<10; i++)
        {
            line[0] = 0;
            fgets(line, 1000, file);
            if (line[0]==0)
                break;

            /* Remove enter (LF) at ending of line */
            
            l = strlen(line);
            while (line[l-1]<=32 && l>0)
            {
                line[l-1] = 0;
                l--;
            }

            CreatePolynomial(&library[active], line);

            /* only valid data will be added into library of polynoms */

            if (library[active].valid)
                active++;
        }
        fclose(file);
    }

    return active;
}

void save_history(Polynomial* library, int active, const char* filename)
{
    int i;
    FILE* file;
    
    file = fopen(filename, "w");
    if (file)
    {
        for(i=0; i<active; i++)
        {
            fputs(toString(library[i]), file);
            fputs("\n", file);
        }
        fclose(file);
    }
}

int create_predefined(Polynomial* library)
{
    CreatePolynomial(&library[0], "5x^2+7x^1-9x^0" );    
    CreatePolynomial(&library[1], "3x^2-6x^1+4x^0" ); 
    CreatePolynomial(&library[2], "1x^2-2x^3+3x^2-11x^1-129x^0");    
    CreatePolynomial(&library[3], "12x^7-19x^6+1x^5-18x^4");
    CreatePolynomial(&library[4], "1x^3-9x^1");
    CreatePolynomial(&library[5], "x^2");
    return 6;
}

int addToLibrary(Polynomial* library, int active, Polynomial entered)
{
    if (active<10)
    {
        library[active] = entered;
        active++;                        
    } else {
        int i;

        for(i=0; i<10; i++)
            library[i] = library[i+1];
        library[9] = entered;
    }
    return active;
}

int enterIndex(int max_index)
{
    int index;
    do
    {
        printf("Enter index of polynom 1-%d or 0 to cancel: ", max_index);
        scanf("%d", &index);
        if (index<0 || index>max_index)
            printf("Invalid index is entered. Please try again.\n");
    }while(index<0 || index>max_index);

    return index-1;
}

int main(void)
{
    int i, active, choice, index1, index2;
    char buffer[128];
    Polynomial library[10];
    Polynomial entered;

    for(i=0; i<10; i++)
        library[i].head = 0;

    active = read_from_file(library, "polynoms.txt");

    if (active==0)
        active = create_predefined(library);

    do
    {
        printf("Menu: \n");
        printf("1. Enter new polynomial\n");
        printf("2. Add polynomials\n");
        printf("3. Subtract polynomials\n");
        printf("4. Multiply polynomials\n");
        printf("5. View History\n");
        printf("6. Erase History\n");
        printf("0. Quit\n");
        printf("\nChoose one item: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 0:
                break;
            case 1:
                scanf("%s", buffer);
                CreatePolynomial(&entered, buffer);
                if (entered.valid)
                {
                    active = addToLibrary(library, active, entered);
                    printf("\nPolynom is added\n");
                } else {
                    printf("\nInvalid polynomial is entered\n");
                }
                break;
            case 2:
                if (active>0)
                {
                    index1 = enterIndex(active);
                    if (index1>-1)
                    {
                        index2 = enterIndex(active);
                        if (index2>-1)
                        {
                            Polynomial result;
                            AddPolynomial(&result, &library[index1], &library[index2]);
                            active = addToLibrary(library, active, result);
                            printf("\nAddition performed.\nResult is added to library\n");
                        }
                    }
                } else {
                    printf("\nPolynomial library is empty\n");
                }
                break;
            case 3:
                if (active>0)
                {
                    index1 = enterIndex(active);
                    if (index1>-1)
                    {
                        index2 = enterIndex(active);
                        if (index2>-1)
                        {
                            Polynomial result;
                            SubtractPolynomial(&result, &library[index1], &library[index2]);
                            active = addToLibrary(library, active, result);
                            printf("\nSubtraction performed.\nResult is added to library\n");
                        }
                    }
                } else {
                    printf("\nPolynomial library is empty\n");
                }
                break;
            case 4:
                if (active>0)
                {
                    index1 = enterIndex(active);
                    if (index1>-1)
                    {
                        index2 = enterIndex(active);
                        if (index2>-1)
                        {
                            Polynomial result;
                            MultiplyPolynomial(&result, &library[index1], &library[index2]);
                            active = addToLibrary(library, active, result);
                            printf("\nMultiplying performed.\nResult is added to library\n");
                        }
                    }
                } else {
                    printf("\nPolynomial library is empty\n");
                }
                break;
            case 5:
                if (active>0)
                {
                    for(i=0; i<active; i++)
                        printf("%d. %s\n", i+1, toString(library[i])); 
                } else {
                    printf("\nPolynomial library is empty\n");
                }
                break;
            case 6:
                active = 0;
                break;
            default:
                break;
        }

    }while(choice!=0);

    save_history(library, active, "polynoms.txt");

    return 0;
}
