#include "MyDatabase.h"

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		fprintf(stderr, "Error: Invalid arguments.\n");
		exit(1);
	}

	char* db_filename = argv[1];
	MyDatabase* db = create_database();

	load_database(db, db_filename);

	char input[256];
	Data* data;

	while(1)
	{
		printf("input > ");
		fgets(input, 256, stdin);

		if(strcmp(input, "exit\n") == 0 || strcmp(input, "quit\n") == 0)
		{
			save_database(db, db_filename);
			exit(0);
		}
		else
		{
			data = db_query(db, input);
			puts("===== 1 Query =====");
			print_data(data);
			puts("===================");
		}
	}

	return 0;
}
