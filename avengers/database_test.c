#include "MyDatabase.h"

int db_find_test(MyDatabase* db, char* key_type, char* key, int isNull)
{
	Data* data = find_data(db, key_type, key);
	return (isNull && data == NULL) || (!isNull && data != NULL);
}

int db_insert_test(MyDatabase* db, char* key, char* value)
{
	insert_data(db, create_data(key, value));
	return db_find_test(db, "key", key, 0) + db_find_test(db, "value", value, 0) == 2;
}

int db_delete_test(MyDatabase* db, char* key_type, char* key)
{
	delete_data(db, key_type, key);
	return db_find_test(db, key_type, key, 1);
}

int db_test(MyDatabase* db)
{
	int isSuccess = 1;

	isSuccess = isSuccess && db_find_test(db, "key", "0", 1);

	isSuccess = isSuccess && db_insert_test(db, "0", "value0");
	isSuccess = isSuccess && db_insert_test(db, "1", "value1");
	isSuccess = isSuccess && db_insert_test(db, "10", "value10");

	isSuccess = isSuccess && db_find_test(db, "key", "0", 0);
	isSuccess = isSuccess && db_find_test(db, "key", "1", 0);
	isSuccess = isSuccess && db_find_test(db, "key", "10", 0);
	isSuccess = isSuccess && db_find_test(db, "key", "2", 1);

	isSuccess = isSuccess && db_find_test(db, "value", "value0", 0);
	isSuccess = isSuccess && db_find_test(db, "value", "value1", 0);
	isSuccess = isSuccess && db_find_test(db, "value", "value10", 0);
	isSuccess = isSuccess && db_find_test(db, "value", "value2", 1);

	isSuccess = isSuccess && db_delete_test(db, "key", "0");
	isSuccess = isSuccess && db_delete_test(db, "key", "1");
	isSuccess = isSuccess && db_delete_test(db, "key", "10");
	isSuccess = isSuccess && db_delete_test(db, "key", "2");

	isSuccess = isSuccess && db_find_test(db, "key", "0", 1);
	isSuccess = isSuccess && db_find_test(db, "key", "1", 1);
	isSuccess = isSuccess && db_find_test(db, "key", "10", 1);
	isSuccess = isSuccess && db_find_test(db, "key", "2", 1);

	return isSuccess;
}

int main()
{
	MyDatabase* db = create_database();

	printf("%s\n", db_test(db) ? "Success" : "Failure");
}
