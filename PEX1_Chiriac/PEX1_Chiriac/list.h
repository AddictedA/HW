typedef struct s_node {
  char* data;
  struct s_node* next;
} node;

/* list_insert_tail: create a new node at the end of the linked list
   Parameters
   - node*: a pointer to the head of a linked list
   - char*: data to be stored in the new node
   Return: a pointer to the head of the linked list */
node* list_insert_tail(node*, char*);

/* list_remove: remove the node containing specific data; if multiple nodes
    contain the data, remove the node nearest to the head
   Parameters
   - node*: a pointer to the head of a linked list
   - char*: data that, if found in one of the nodes in the list, indicates
      the node to remove (in the case of multiple nodes containing the data,
	  remove the node nearest to the head)
   Return: a pointer to the head of the linked list */
node* list_remove(node*, char*);

/* list_print: print the data contained in each node in the linked list,
    starting with the head, where each node's data is printed on a separate
    line
   Parameter: node*: a pointer to the head of a linked list */
void list_print(node*);

/* list_get: retrieve the data stored in the <n>th node in the list
   Parameters:
   - node*: a pointer to the head of a linked list
   - int: the position of the node with the data to retrieve, where the head is
       at position 1
   Return: the data */
char* list_get(node*, int);

/* list_get_size: determine number of items in the list
    Parameters:
    - node*: a pointer to the head of a linked list
    Return: Number of items as integer*/
int list_get_size(node*);

/* listPrintHistory: formatted print of linked list required
   for reverse and number printing
   Parameters:
   - node*: a pointer to the head of a linked list */
void listPrintHistory(node* , int);