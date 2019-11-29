#include <stdio.h>
#include <mysql/mysql.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>


#define STR_HOST	"localhost"
#define STR_ID		"root"
#define STR_PW		"#hanback!" 
#define STR_DB		"BookManagement"

#define MANAGE_RENT	0
#define MANAGE_RETURN	1

struct Member
{
	int id;
	char name[40];
	char phone[40];
};

struct Book
{
	int id;
	char title[40];
	char author[40];
	char publisher[40];
	time_t date;	
	int price;
};

struct Management
{
	int id;
	int manageType;
	time_t date;
	int personId;
	int bookId;	
};

typedef struct Member Member;
typedef struct Book Book;
typedef struct Management Management;


void printCategory(void);
char getSelection(void);
void serveCategory(MYSQL *, char category);
void serveMemberCategory(MYSQL *);
void serveBookCategory(MYSQL*);
void serveRentalCategory(MYSQL*);
void printMemberItem(void);
void serveMemberItem(MYSQL*, char item);
void printBookItem(void);
void serveBookItem(MYSQL*, char item);
void printRentalItem(void);
void serveRentalItem(MYSQL*, char item);
void serveMemberAdd(MYSQL*);
void serveMemberSearch(MYSQL*);
void serveMemberEdit(MYSQL*);
void serveMemberRemove(MYSQL*);
void serveBookAdd(MYSQL*);
void serveBookSearch(MYSQL*);
void serveBookEdit(MYSQL*);
void serveBookRemove(MYSQL*);
void serveRentalRent(MYSQL*);
void serveRentalSearch(MYSQL*);
void serveRentalReturn(MYSQL*);

int getMemberInformation(Member * member);
int covertMemberAddQuery(char * query, Member * member);
void executeQuery(MYSQL * mysql, char * query);
int goOn(void);
void printQueryResult(MYSQL * mysql);
int getId(char * who);
void covertMemberUpdateQuery(char * query, Member * member);
void covertMemberRemoveQuery(char * query, int id);
int getBookInformation(Book * book);
void covertBookAddQuery(char * query, Book * book);
void covertBookUpdateQuery(char * query, Book * book);
void covertBookRemoveQuery(char * query, int id);
void covertManagementRentQuery(char * query, int idBook, int idMember);
void covertManagementReturnQuery(char * query, int idBook, int idMember);

void displayHeader(MYSQL_RES * mysqlRes);
void displayRow(MYSQL * mysql, MYSQL_ROW mysqlRow);

void init(MYSQL * mysql);
void destroy(MYSQL * mysql);

int main()
{
	MYSQL mysql;
	char category = 0;

	init(&mysql);

	printf("* Hi! This program is a Book Management System *\n");

	while(1)
	{
		printCategory();
		category = getSelection();	
		if(category=='q') break;
	
		serveCategory(&mysql, category);
	}

	destroy(&mysql);

	return 0;
}



void displayHeader(MYSQL_RES * res_ptr)
{
	MYSQL_FIELD *field_ptr;

	if(res_ptr==NULL)
	{
		printf("(!) MYSQL_RES is NULL\n");
		return;
	}

	printf("(i) Table Content.\n");

	while ((field_ptr = mysql_fetch_field(res_ptr)) != NULL)
	{
		printf("[%s]\t", field_ptr->name);
	}

	printf("\n");
	return;
}

void displayRow(MYSQL * mysql, MYSQL_ROW mysqlRow)
{
	unsigned int field_count =0;

	while (field_count < mysql_field_count(mysql))
	{
		if (mysqlRow[field_count]) 
			printf("%s ", mysqlRow[field_count]);
		else 
			printf("NULL");

		field_count++;
	}
	
	printf("\n");
	return;
}

int getMemberInformation(Member * member)
{
	printf("(i) Input Member Information\n");
	
	if(member==NULL) return 0;	
	
	printf("NAME:");
	scanf("%s", member->name);
	if(strlen(member->name)<1) return 0;

	printf("PHONE:");
	scanf("%s", member->phone); 
	if(strlen(member->phone)<1) return 0;

	return 1;	
}

int covertMemberAddQuery(char * query, Member * member)
{
	if(query==NULL) return 0;
	if(member==NULL) return 0;

	sprintf(query,"insert into member values (null, '%s', '%s')"
		, member->name, member->phone);

	return 1;
}

int goOn(void)
{
	char input[255]="";
	printf("(i) Continue? (y/n)\n");
	scanf("%s", input);
	if(input[0]=='y') return 1;
	
	return 0;	
}

void printQueryResult(MYSQL * mysql)
{
	printf("(i) printQueryResult called\n");

	MYSQL_RES * mysqlRes = mysql_use_result(mysql);

	printf("(i) mysql_use_result done\n");

	MYSQL_ROW sqlrow;
	if (mysqlRes) 
	{
		displayHeader(mysqlRes);
		while ((sqlrow = mysql_fetch_row(mysqlRes)))
		{
			displayRow(mysql, sqlrow);
		}

		if (mysql_errno(mysql))
		{
			fprintf(stderr, "(!) Retrive error: %s\n", 
				mysql_error(mysql));
		}
	}

	mysql_free_result(mysqlRes);

	printf("(i) printQueryReuslt will be returned\n");

	return;	
}

int getId(char * who)
{
	int id = 0;
	printf("(i) Input %s ID\n", who);
	scanf("%d", &id);
	return id;
}

void covertMemberUpdateQuery(char * query, Member * member)
{
	if(query==NULL) return;
	if(member==NULL) return;

	sprintf(query, 
		"update member set name='%s', phone='%s' where id=%d",  
		member->name, member->phone,  member->id);
	return; 
}

void covertMemberRemoveQuery(char * query, int id)
{
	if(query==NULL) return;
	if(id<0) return;

	sprintf(query, "delete from member where id=%d", id); 
	return; 
}

int getBookInformation(Book * book)
{
	printf("(i) Input Book Information\n");
	
	if(book==NULL) return 0;	
	
	printf("TITLE:");
	scanf("%s", book->title);
	if(strlen(book->title)<1) return 0;

	printf("AUTHOR:");
	scanf("%s", book->author); 
	if(strlen(book->author)<1) return 0;

	printf("PUBLISHER:");
	scanf("%s", book->publisher); 
	if(strlen(book->publisher)<1) return 0;

	printf("PRICE:");
	scanf("%d", &(book->price) ); 
	if(book->price <1 ) return 0;

	return 1;
}

void covertBookAddQuery(char * query, Book * book)
{
	if(query==NULL) return;
	if(book==NULL) return;

	sprintf(query,
	 "insert into book values (null, '%s', '%s', '%s', now(), %d)"
	 , book->title, book->author, book->publisher, book->price);

	return;
}

void covertBookUpdateQuery(char * query, Book * book)
{
	if(query==NULL) return;
	if(book==NULL) return;

	sprintf(query, 
		"update book set title='%s', author='%s', publisher='%s', date=now(), price=%d where id=%d",  
		book->title, book->author, book->publisher, book->price, book->id);
	return; 
}

void covertBookRemoveQuery(char * query, int id)
{
	if(query==NULL) return;
	if(id<0) return;

	sprintf(query, "delete from book where id=%d", id); 
	return; 
}

void covertManagementRentQuery(char * query, int idBook, int idMember)
{
	if(query==NULL) return;
	if(idBook<0) return;
	if(idMember<0) return;

	sprintf(query,
	 "insert into management values (null, %d, now(), %d, %d)"
	 , MANAGE_RENT, idMember, idBook);

	return;
}

void covertManagementReturnQuery(char * query, int idBook, int idMember)
{
	if(query==NULL) return;
	if(idBook<0) return;
	if(idMember<0) return;

	sprintf(query,
	 "insert into management values (null, %d, now(), %d, %d)"
	 , MANAGE_RETURN, idMember, idBook);

	return;
}

void serveMemberAdd(MYSQL * mysql)
{
	Member member;
	char query[255] = "";
	while(1)
	{
		memset(&member,0,sizeof(Member));
		strcpy(query,"");

		if(!getMemberInformation(&member))
			printf("(!) member Information is not valid\n");
		else
		{
			covertMemberAddQuery(query, &member);
			executeQuery(mysql, query);
		}

		if( !goOn() ) break;
	}

	return;
}


// display all member information
void serveMemberSearch(MYSQL * mysql)
{
	char query[255] = "select * from member";
	
	executeQuery(mysql, query);

	printQueryResult(mysql);

	return;
}

void serveMemberEdit(MYSQL * mysql)
{
	Member member;
	char query[255] = "";
	while(1)
	{
		memset(&member,0,sizeof(Member));
		strcpy(query,"");

		member.id = getId("Member");

		if(!getMemberInformation(&member))
			printf("(!) member Information is not valid\n");
		else
		{
			covertMemberUpdateQuery(query, &member);
			executeQuery(mysql, query);
		}

		if( !goOn() ) break;
	}

	return;
}

void serveMemberRemove(MYSQL* mysql)
{
	Member member;
	char query[255] = "";
	while(1)
	{
		memset(&member,0,sizeof(Member));
		strcpy(query,"");

		int id = getId("Member");

		covertMemberRemoveQuery(query, id);
		executeQuery(mysql, query);

		if( !goOn() ) break;
	}

	return;
}


void serveBookAdd(MYSQL * mysql)
{
	Book book;
	char query[255] = "";
	while(1)
	{
		memset(&book,0,sizeof(Book));
		strcpy(query,"");

		if(!getBookInformation(&book))
			printf("(!) Boom Information is not valid\n");
		else
		{
			covertBookAddQuery(query, &book);
			executeQuery(mysql, query);
		}

		if( !goOn() ) break;
	}

	return;
}


void serveBookSearch(MYSQL * mysql)
{
	char query[255] = "select * from book";
	
	executeQuery(mysql, query);

	printQueryResult(mysql);

	return;
}

void serveBookEdit(MYSQL * mysql)
{
	Book book;
	char query[255] = "";
	while(1)
	{
		memset(&book,0,sizeof(Book));
		strcpy(query,"");

		book.id = getId("Book");

		if(!getBookInformation(&book))
			printf("(!) Book Information is not valid\n");
		else
		{
			covertBookUpdateQuery(query, &book);
			executeQuery(mysql, query);
		}

		if( !goOn() ) break;
	}

	return;
}

void serveBookRemove(MYSQL * mysql)
{
	Book book;
	char query[255] = "";
	while(1)
	{
		memset(&book,0,sizeof(Book));
		strcpy(query,"");

		int id = getId("Book");

		covertBookRemoveQuery(query, id);
		executeQuery(mysql, query);

		if( !goOn() ) break;
	}

	return;
}

void serveRentalRent(MYSQL* mysql)
{
	Management management;
	char query[255] = "";
	int idBook = 0;
	int idMember = 0;

	while(1)
	{
		memset(&management,0,sizeof(Management));
		strcpy(query,"");

		idBook = getId("Book");	
		idMember = getId("Member");

		covertManagementRentQuery(query, idBook, idMember);
		executeQuery(mysql, query);

		if( !goOn() ) break;
	}

	return;
}

void serveRentalSearch(MYSQL * mysql)
{
	char query[255] = "select * from management";
	
	executeQuery(mysql, query);

	printQueryResult(mysql);

	return;
}

void serveRentalReturn(MYSQL * mysql)
{
	Management management;
	char query[255] = "";
	int idBook = 0;
	int idMember = 0;

	while(1)
	{
		memset(&management,0,sizeof(Management));
		strcpy(query,"");

		idBook = getId("Book");	
		idMember = getId("Member");

		covertManagementReturnQuery(query, idBook, idMember);
		executeQuery(mysql, query);

		if( !goOn() ) break;
	}

	return;
}

void printMemberItem(void)
{
	printf("(i) Select a member management item\n");
	printf("(1) Add\n");
	printf("(2) Search\n");
	printf("(3) Edit\n");
	printf("(4) Remove\n");
	printf("(q) Quit\n");
}

void serveMemberItem(MYSQL * mysql, char item)
{
	switch(item)
	{
	case '1':	// Add
		serveMemberAdd(mysql);
		break;
	case '2':	// Search
		serveMemberSearch(mysql);
		break;
	case '3':	// Edit
		serveMemberEdit(mysql);
		break;
	case '4':	// Remove
		serveMemberRemove(mysql);
		break;
	default:
		break;
	}
	return;
}

void printBookItem(void)
{
	printf("(i) Select a book management item\n");
	printf("(1) Add\n");
	printf("(2) Search\n");
	printf("(3) Edit\n");
	printf("(4) Remove\n");
	printf("(q) Quit\n");
}

void serveBookItem(MYSQL * mysql, char item)
{
	switch(item)
	{
	case '1':
		serveBookAdd(mysql);
		break;
	case '2':
		serveBookSearch(mysql);
		break;
	case '3':
		serveBookEdit(mysql);
		break;
	case '4':
		serveBookRemove(mysql);
		break;
	default:
		break;
	}
	return;
}

void printRentalItem(void)
{
	printf("(i) Select a rental management item\n");
	printf("(1) Rent\n");
	printf("(2) Search\n");
	printf("(3) Return\n");
	printf("(q) Quit\n");
}

void serveRentalItem(MYSQL * mysql, char item)
{
	switch(item)
	{
	case '1':
		serveRentalRent(mysql);
		break;
	case '2':
		serveRentalSearch(mysql);
		break;
	case '3':
		serveRentalReturn(mysql);
		break;
	default:
		break;
	}
	return;
}

void serveMemberCategory(MYSQL * mysql)
{
	char item = 0;

	while(1)
	{
		printMemberItem();
		item = getSelection();
		if(item=='q') break;
		
		serveMemberItem(mysql, item);
	}			

	return;
}

void serveBookCategory(MYSQL * mysql)
{
	char item = 0;

	while(1)
	{
		printBookItem();
		item = getSelection();
		if(item=='q') break;
		
		serveBookItem(mysql, item);
	}			

	return;
}

void serveRentalCategory(MYSQL * mysql)
{
	char item = 0;

	while(1)
	{
		printRentalItem();
		item = getSelection();
		if(item=='q') break;
		
		serveRentalItem(mysql, item);
	}			

	return;
}

void printCategory(void)
{
	printf("(i) Select a service category:\n");
	printf("(1) Member Mangement\n");
	printf("(2) Book Management\n");
	printf("(3) Rental Management\n");
	printf("(q) Quit\n");

	return;
}

char getSelection(void)
{
	char category[255]="";
	scanf("%s", category);
	return category[0];
}

void serveCategory(MYSQL * mysql, char category)
{
	switch(category)
	{
	case '1':
		serveMemberCategory(mysql);
		break;
	case '2':
		serveBookCategory(mysql);
		break;
	case '3':
		serveRentalCategory(mysql);
		break;
	default:
		break;
	}

	return;
}

void init(MYSQL * mysql)
{
	mysql_init(mysql);
	mysql_real_connect(mysql,STR_HOST, STR_ID, STR_PW, 
		STR_DB, 0,NULL, 0);

	return;
}

void executeQuery(MYSQL * mysql, char * query)
{
	int result = mysql_query(mysql, query);

	if(result==0) 	printf("(i) Execute Query Success\n");
	else 		printf("(!) Execute Query Failed:%s\n",query);	

	return;
}

void destroy(MYSQL * mysql)
{
	mysql_close(mysql);
	return;
}

