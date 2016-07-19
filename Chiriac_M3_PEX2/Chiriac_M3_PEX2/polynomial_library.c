// Added for supress warning of VS
// and scanf_s and etc, does not affect compilation by GCC 
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "polynomial_library.h"

Node* add_to_list(Node* node1, Node* node2)
/* Merges 2 list, to make it sorting order
   node1 contains only one element */
{
    if (node2)
    {
        if (node1->order < node2->order) {
            if (node2->next)
                node2->next = add_to_list(node2->next, node1);
            else
                node2->next = node1;

            return node2;
        }
    }
    node1->next = node2;
    return node1;
}

void CreatePolynomial(Polynomial *poly, const char* str)
   /* Constructor that converts a string to a Polynomial
      Also, it checks for validity and set poly->valid into 0,
      if string contains invalid data. Also, 
      it puts terms in sorted order
	*/
{    
    int i = 0;
    Polynomial next;

    poly->valid = 1;
    if (str[0]!=0)
    {
        int coefficient = 0;
        int exponent = 0;
        char* end_ptr;
        char* end_ptr2;
                
        /* for extra */
        if (str[0]=='-' && str[1]=='x')
        {
            end_ptr = (char*)&str[1];
            coefficient = -1;
        }
        /* for extra */
        if (str[0]=='x')
        {
            end_ptr = (char*)&str[0];
            coefficient = 1;
        } else {                    
            coefficient = strtol(str, &end_ptr, 10);
            if (!end_ptr || end_ptr==str)
            {
                poly->head = 0;            
                poly->valid = 0;
                printf("Invalid coeffient: %s\n", str);
                return;
            } 
        }

        /* Make sure there is x^ is present */
        if (end_ptr[0]!='x' || end_ptr[1]!='^')
        {
            poly->head = 0;            
            poly->valid = 0;
            printf("Should be \"x^\" here: %s\n", end_ptr);
            return;
        }

        end_ptr += 2;
                       
        exponent = strtol(end_ptr, &end_ptr2, 10);
        if (!end_ptr2 || end_ptr==end_ptr2)
        {
            poly->head = 0; 
            poly->valid = 0;
            printf("Invalid order: %s\n", end_ptr);
            return;
        }

        i = 0;
        /* check validity of operator between terms */
        if (end_ptr2[0]=='+')
            i = 1;
        else if (end_ptr2[0]!='-' && end_ptr2[0]!=0)
        {
            poly->head = 0; 
            poly->valid = 0;
            printf("Invalid operation: %s, should be + or -\n", end_ptr2);
            return;
        }
        
        /* 0 coefficient should not be stored */
        if (coefficient!=0)
        {                        
            poly->head = (Node*)malloc(sizeof(Node));
            poly->head->coefficient = coefficient;
            poly->head->order = exponent;
            poly->head->next = 0;

            CreatePolynomial(&next, &end_ptr2[i]);
            /* add new termn into list, make it ordered properly */
            poly->head = add_to_list(poly->head, next.head);
            //poly->head->next = next.head;
            poly->valid = next.valid;
        } else {
            /* if coefficient = 0, mive toi next term */
            CreatePolynomial(poly, &end_ptr2[i]);
        }
    } else {
        poly->head = 0;
        poly->valid = 1;
    }
}


const char* toString(Polynomial poly)
   /* returns a string representation of the polynomial.
      It assumes, terms in sorted order
	*/
{
    char* data = (char*)malloc(1001);
    data[0] = 0;

    if (poly.head)
    {            
        sprintf(data, "%dx^%d", poly.head->coefficient, poly.head->order);
        if (poly.head->next)
        {
            Polynomial nextPoly;
            char* nextString;

            nextPoly.head = poly.head->next;
            nextString = (char*)toString(nextPoly);
            if (poly.head->next->coefficient>=0)
                strcat(data, "+");
            strcat(data, nextString);
            free(nextString);
        }
    };

    return data;
}

void AddPolynomial(Polynomial *result, Polynomial *poly1, Polynomial *poly2)
   /* Adds two polynomials 
      It assumes, terms of poly1 and poly2 in sorted order   
    */
{
    Polynomial nextResult;
    Polynomial nextPoly1;                
    Polynomial nextPoly2;

    result->head = 0;
    result->valid = 1;

    /* if one of polymial is empty, return second one */
    if (!poly1->head)
        *result = *poly2;
    if (!poly2->head)
        *result = *poly1;
    else {
        /* Get coefficeients of most high ordered term */
        int o1 = poly1->head->order;
        int c1 = poly1->head->coefficient;
        int o2 = poly2->head->order;
        int c2 = poly2->head->coefficient;

        /* check orders */
        if (o1<o2)
        {      
            result->head = (Node*)malloc(sizeof(Node));
            result->head->coefficient = c2;
            result->head->order = o2;

            nextPoly1.head = poly1->head;
            nextPoly2.head = poly2->head->next;
            AddPolynomial(&nextResult, &nextPoly1, &nextPoly2);
            result->head->next = nextResult.head;
        }         
        else if (o1>o2)
        {            
            result->head = (Node*)malloc(sizeof(Node));
            result->head->coefficient = c1;
            result->head->order = o1;

            nextPoly1.head = poly1->head->next;
            nextPoly2.head = poly2->head;
            AddPolynomial(&nextResult, &nextPoly1, &nextPoly2);
            result->head->next = nextResult.head;
        } 
        else 
        {
            /* is there is same order of both terms */
            if (c1+c2!=0) /* check result is not 0*/
            {                
                result->head = (Node*)malloc(sizeof(Node));
                result->head->coefficient = c1+c2;
                result->head->order = o1;

                nextPoly1.head = poly1->head->next;
                nextPoly2.head = poly2->head->next;
                AddPolynomial(&nextResult, &nextPoly1, &nextPoly2);
                result->head->next = nextResult.head;
            } else {
                /* just skip this term */
                nextPoly1.head = poly1->head->next;
                nextPoly2.head = poly2->head->next;
                AddPolynomial(&nextResult, &nextPoly1, &nextPoly2);
                result->head = nextResult.head;
            }
        }
    }
}

void SubtractPolynomial(Polynomial *result, Polynomial *poly1, Polynomial *poly2)
   /* Subtracts two polynomials
      It assumes, terms of poly1 and poly2 in sorted order   
    */
{
    Polynomial nextResult;
    Polynomial nextPoly1;                
    Polynomial nextPoly2;

    result->head = 0;
    result->valid = 1;

    if (!poly1->head)
        *result = *poly2;
    if (!poly2->head)
        *result = *poly1;
    else {
        int o1 = poly1->head->order;
        int c1 = poly1->head->coefficient;
        int o2 = poly2->head->order;
        int c2 = poly2->head->coefficient;

        if (o1>o2)
        {      
            result->head = (Node*)malloc(sizeof(Node));
            result->head->coefficient = -c2;
            result->head->order = o2;

            nextPoly1.head = poly1->head;
            nextPoly2.head = poly2->head->next;
            SubtractPolynomial(&nextResult, &nextPoly1, &nextPoly2);
            result->head->next = nextResult.head;
        } 
        else if (o1<o2)
        {            
            result->head = (Node*)malloc(sizeof(Node));
            result->head->coefficient = c1;
            result->head->order = o1;

            nextPoly1.head = poly1->head->next;
            nextPoly2.head = poly2->head;
            SubtractPolynomial(&nextResult, &nextPoly1, &nextPoly2);
            result->head->next = nextResult.head;
        } 
        else 
        {
            if (c1-c2!=0)
            {                
                result->head = (Node*)malloc(sizeof(Node));
                result->head->coefficient = c1-c2;
                result->head->order = o1;

                nextPoly1.head = poly1->head->next;
                nextPoly2.head = poly2->head->next;
                SubtractPolynomial(&nextResult, &nextPoly1, &nextPoly2);
                result->head->next = nextResult.head;
            } else {
                nextPoly1.head = poly1->head->next;
                nextPoly2.head = poly2->head->next;
                SubtractPolynomial(&nextResult, &nextPoly1, &nextPoly2);
                result->head = nextResult.head;
            }
        }
    }
}

void MultiplyPolynomial(Polynomial *result, Polynomial *poly1, Polynomial *poly2)
   /* Multiplies two polynomials
      It assumes, terms of poly1 and poly2 in sorted order   
    */
{
    /* poly 2 polynimial does not have yet one term */
    if (poly2->head->next)
    {
        Polynomial tempResult1;
        Polynomial tempResult2;
        Polynomial current;
        Polynomial rest;

        /* decrease order of polynom */
        rest.head = poly2->head->next;
        
        /* create polynomial with one term */
        current.head = (Node*)malloc(sizeof(Node));
        current.head->coefficient = poly2->head->coefficient;
        current.head->order = poly2->head->order;
        current.head->next = 0;
                
        /* result of multiplying polynomial with current one term and second one */
        MultiplyPolynomial(&tempResult2, poly1, &current);
        /* result of multiplying polynomial with decreased order and second one */
        MultiplyPolynomial(&tempResult1, poly1, &rest);

        /* add result of both multiplying */
        AddPolynomial(result, &tempResult1, &tempResult2);
        free(current.head);

    } else {
        /* poly 2 polynimial has already one term */
        if (poly1->head->next)
        {
            Polynomial tempResult1;
            Polynomial tempResult2;

            Polynomial rest;
            Polynomial current;

            /* decrease order of polynom */
            rest.head = poly1->head->next;

            /* create polynomial with one term */
            current.head = (Node*)malloc(sizeof(Node));
            current.head->coefficient = poly1->head->coefficient;
            current.head->order = poly1->head->order;
            current.head->next = 0;

            /* result of multiplying polynomial with decreased order and second one */
            MultiplyPolynomial(&tempResult1, &rest, poly2);
            /* result of multiplying polynomial with current one term and second one */
            MultiplyPolynomial(&tempResult2, &current, poly2);

            /* add result of both multiplying */
            AddPolynomial(result, &tempResult1, &tempResult2);
            free(current.head);
        } else {
            /* easiest case, multipty 2 polymomials, each one has only one term */
            result->head = (Node*)malloc(sizeof(Node));

            /* multiply coefficients */
            result->head->coefficient = poly1->head->coefficient * poly2->head->coefficient;
            /* add orders */
            result->head->order = poly1->head->order + poly2->head->order;
            result->head->next = 0;
        }
    }
}