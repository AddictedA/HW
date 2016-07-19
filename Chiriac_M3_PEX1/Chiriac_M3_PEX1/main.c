#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define TYPE_SIZE 11
#define ACC_NUMBER_SIZE 32
#define VALIDITY_SIZE 8
#define REASON_SIZE 12

/*Make use of the time struct provided by time.h*/
time_t rawtime;
struct tm timeinfo;

typedef struct cards{
	/*Create the data struct of a credit card*/
	int month;
	int year;
	char type[TYPE_SIZE];		/*Mastercard is the longest type*/
	char number[ACC_NUMBER_SIZE];	/*assume dash after every digit*/
	char validity[VALIDITY_SIZE];		/*Invalid is the longest*/
	char reason[REASON_SIZE];		/*"Check Digit is the longest*/
} cards;
int sum_digit(int n){
	/*A simple function that returns the sum of the digit of the passed integer */
	int total = 0;
	while (n){
		total = total + n % 10;
		n = n / 10;
	}
	return total;
}
void damm(char *number, char *reason, char *validity){
	/*A function that test if a provided number sequence respects Damm's algorithm.
	Google or Wikipedia if you have no idea what that is.
	Since we are check card numbers we also update a card status from valid to invalid if the sequence is wrong.
	*/
	int TABLE[10][10] = {
		{ 0, 3, 1, 7, 5, 9, 8, 6, 4, 2 },
		{ 7, 0, 9, 2, 1, 5, 4, 8, 6, 3 },
		{ 4, 2, 0, 6, 8, 7, 1, 3, 5, 9 },
		{ 1, 7, 5, 0, 9, 8, 3, 4, 2, 6 },
		{ 6, 1, 2, 3, 0, 4, 5, 9, 7, 8 },
		{ 3, 6, 7, 4, 2, 0, 9, 5, 8, 1 },
		{ 5, 8, 6, 9, 7, 2, 0, 1, 3, 4 },
		{ 8, 9, 4, 5, 3, 6, 2, 0, 1, 7 },
		{ 9, 4, 3, 8, 6, 1, 7, 2, 0, 9 },
		{ 2, 5, 8, 1, 4, 3, 6, 7, 9, 0 }
	};
	int length, old = 0, new = 0, i, last;
	char reinit_reason[REASON_SIZE] = "CHECK DIGIT", reinit_validity[VALIDITY_SIZE] = "INVALID";

	length = strlen(number);
	for ( i = 0; i <= length-2; i++){
		new = number[i] - '0';
		old = TABLE[old][new];
	}
	last = number[length-1] - '0';
	if (old!=last){
		for (i = 0; i < VALIDITY_SIZE; i++){
			validity[i] = reinit_validity[i];
		}

		for (i = 0; i < REASON_SIZE; i++){
			reason[i] = reinit_reason[i];
		}
	}
}
void luhn(char *number, char *reason, char *validity){
	/*A function that test if a provided number sequence respects Luhn's algorithm.
	Google or Wikipedia if you have no idea what that is.
	Since we are check card numbers we also update a card status from valid to invalid if the sequence is wrong.
	*/
	int i, length, digit,sum =0,last,last_calc;
	char reinit_reason[REASON_SIZE] = "CHECK DIGIT", reinit_validity[VALIDITY_SIZE] = "INVALID";
	length = strlen(number);
	for (i = length - 2 ; i >=0; i=i-2){
		digit = number[i] - '0'; //The conversion is so stupid. I cannot find a useful function.
		sum += sum_digit(2*digit);
	}
	for (i = length - 3; i >= 0; i = i - 2){
		digit = number[i] - '0'; 
		sum += digit;
	}
	last_calc = (sum * 9) % 10;
	last = number[length-1] - '0';
	if (last != last_calc){
		for (i = 0; i < VALIDITY_SIZE; i++){
			validity[i] = reinit_validity[i];
		}
		for (i = 0; i < REASON_SIZE; i++){
			reason[i] = reinit_reason[i];
		}
	}
}
void init_valid(char *validity){
	/*Initializes the card atribute "Validity" as being valid.*/
	int i;
	char init_validity[VALIDITY_SIZE] = "VALID  ";
	for ( i = 0; i < VALIDITY_SIZE; i++){
		validity[i] = init_validity[i];
	}
}
void init_reason(char *reason){
	/*Initializes the card atribute "Reason" as being "    ".
	There's no reason for the card being valid*/
	int i;
	char init_reason[REASON_SIZE] = "           ";
	for (i = 0; i < REASON_SIZE; i++){
		reason[i] = init_reason[i];
	}
}
void check_date(int *month, int *year,int cmonth, int cyear, char *reason, char *validity){
	/*Cheks if card is expired. If expired it changes the atributes accordingly*/
	char reinit_reason[REASON_SIZE] = "CHECK DATE", reinit_validity[VALIDITY_SIZE] = "INVALID";
	int i;
	if (*year<cyear)	{
		for (i = 0; i < VALIDITY_SIZE; i++){
			validity[i] = reinit_validity[i];
		}
		for (i = 0; i < REASON_SIZE; i++){
			reason[i] = reinit_reason[i];
		}
	}
	if (*year==cyear){
		if (*month<=cmonth){
			for (i = 0; i < VALIDITY_SIZE; i++){
				validity[i] = reinit_validity[i];
			}
			for (i = 0; i < REASON_SIZE; i++){
				reason[i] = reinit_reason[i];
			}
		}
	}
}
void removechar(char *number){
	/*Removes the char '-' from the given string*/
	char *pr = number, *pw = number, c = '-';
	while (*pr) {
		*pw = *pr++;
		pw += (*pw != c);
	}
	*pw = '\0';
}
void check_ini_length(char *number, char *reason, char *validity){
	/*A function that checks if the card number is one of the possible valid lengths.
	Rredifines the atributes reagrding a card validity(Reason & Validity)*/
	char reinit_reason[REASON_SIZE] = "LENGTH", reinit_validity[VALIDITY_SIZE] = "INVALID";
	int i;
	if (strlen(number) != 13 && strlen(number) != 15 && strlen(number) != 16){
		for (i = 0; i < VALIDITY_SIZE; i++){
			validity[i] = reinit_validity[i];
		}
		for (i = 0; i < REASON_SIZE; i++){
			reason[i] = reinit_reason[i];
		}
	}
}
void check_prefix(char *number, char *reason, char *validity, char *type){
	/*A function that checks if the card number prefix is one of the possible valid combinations.
	Rredifines the atributes reagrding a card validity(Reason & Validity)*/
	char reinit_reason[REASON_SIZE] = "PREFIX", reinit_validity[VALIDITY_SIZE] = "INVALID";
	int i,length;
	length = strlen(number);
	if ((!(!strcmp( type,"VISA") && number[0] == '4' && (length == 13 || length == 16))) &&  //VISA
		(!((length == 16) && (number[0] == '5' && (number[1] == '1' || number[1] == '2' || number[1] == '3' || number[1] == '4' || number[1] == '5')) && (!strcmp(type, "MASTERCARD")))) && //MASTERCARD
		(!((length == 15) && (number[0] == '3' && (number[1] == '4' || number[1] == '7')) && (!strcmp(type, "AMEX")))) &&  //AMEX
		(!((length == 16) && (number[0] == '6' && number[1] == '0' && number[2] == '1' && number[3] == '1') && (!strcmp(type, "DISCOVER")))) &&   //DISCOVER
		(!((length == 15) && 
		((number[0] == '2' && number[1] == '1' && number[2] == '3' && number[3] == '1') || (number[0] == '1' && number[1] == '8' && number[2] == '0' && number[3] == '0')) &&
		(!strcmp(type, "JCB"))))) //JCB
	{
		for (i = 0; i < VALIDITY_SIZE; i++){
			validity[i] = reinit_validity[i];
		}
		for (i = 0; i < REASON_SIZE; i++){
			reason[i] = reinit_reason[i];
		}
	}
}

int main(){	

	FILE *input_file, *output_file;
	cards card;

	/*Create the current year and month variables*/
	int cmonth,cyear;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	cmonth = timeinfo.tm_mon + 1;
	cyear = (timeinfo.tm_year + 1900)%100;

	//Open the input and output files
	fopen_s(&input_file, "CCValidator list.txt", "r");
	fopen_s(&output_file, "Output.txt", "w");

	//Check the input and output files to see if they are oppened
	if (!input_file && !output_file)
		return 1;

	/*Take a line at the time(one card) and process it*/
	while (!feof(input_file)){
		fscanf_s(input_file, "%d %d %s %s", &card.month, &card.year, card.type,TYPE_SIZE, card.number,ACC_NUMBER_SIZE);

		/*Capitalize the letters in the card type*/
		_strupr_s(card.type, TYPE_SIZE);

		/*Eliminate "-" in the card number*/
		removechar(card.number);

		/*Assuming all cards are valid and initializing the appropriate value*/
		init_valid(card.validity);
		init_reason(card.reason);

		/*Check to see if card is expired*/
		check_date(&card.month, &card.year, cmonth, cyear, card.reason, card.validity);

		/*If still valid check to see if the number is a valid length*/
		if (strcmp(card.validity, "INVALID")){
			check_ini_length(card.number, card.reason, card.validity);
		}
		/*If still valid check to see if the number is a valid prefix*/
		if (strcmp(card.validity, "INVALID")){
			check_prefix(card.number, card.reason, card.validity, card.type);
		}

		/*If still valid check to see if the number is a valid digit*/
		if (strcmp(card.validity, "INVALID") && ((strcmp(card.type, "JCB"))))
		{
			luhn(card.number, card.reason, card.validity);
		}
		if (strcmp(card.validity, "INVALID") && (!(strcmp(card.type, "JCB"))))
		{
			damm(card.number, card.reason, card.validity);
		}

		/*Voila! The card has been check and is ready for writting*/
		fprintf(output_file, "%d/%d %s %s %s %s\n", card.month, card.year, card.type, card.number, card.validity, card.reason);
	}
	/*Close the files*/
	fclose(input_file);
	fclose(output_file);

	return 0;
}
