
typedef struct polynomial Polynomial;
typedef struct node Node;

struct node
{
    int coefficient;
    int order;
    struct node* next;
};

struct polynomial
{
    /* pointer to head node*/
    Node* head;
    /* result, =1 is polymom is valid, =0 otherwise 
       CreatePolynomial checks for validity too*/   
    int valid;
};


void CreatePolynomial(Polynomial *poly, const char* str);

const char* toString(Polynomial poly);

void AddPolynomial(Polynomial *result, Polynomial *poly1, Polynomial *poly2);

void SubtractPolynomial(Polynomial *result, Polynomial *poly1, Polynomial *poly2);

void MultiplyPolynomial(Polynomial *result, Polynomial *poly1, Polynomial *poly2);

