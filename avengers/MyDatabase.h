#pragma once
#include "AVLTree.h"

typedef struct MyDatabase
{
	AVLTreeNode* key_tree;
	AVLTreeNode* value_tree;
	AVLTreeNode* time_stamp_tree;
} MyDatabase;

MyDatabase* create_database()
{
	MyDatabase* db = (MyDatabase *)malloc(sizeof(MyDatabase));

	db->key_tree = NULL;
	db->value_tree = NULL;
	db->time_stamp_tree = NULL;

	return db;
}

Data* insert_data(MyDatabase* db, Data* data)
{
	AVLTreeNode* node;
	
	node = create_node(data);
	db->key_tree = insert_node(db->key_tree, node, "key");

	node = create_node(data);
	db->value_tree = insert_node(db->value_tree, node, "value");

	node = create_node(data);
	db->time_stamp_tree = insert_node(db->time_stamp_tree, node, "time_stamp");

	return data;
}

Data* find_data(MyDatabase* db, char* key_type, char* key)
{
	AVLTreeNode* node;

	if(strcmp(key_type, "key") == 0)
	{
		return (node = find_node(db->key_tree, key_type, key)) == NULL ? NULL : node->data;
	}
	else if(strcmp(key_type, "value") == 0)
	{
		return (node = find_node(db->value_tree, key_type, key)) == NULL ? NULL : node->data;
	}
	else if(strcmp(key_type, "time_stamp") == 0)
	{
		return (node = find_node(db->time_stamp_tree, key_type, key)) == NULL ? NULL : node->data;
	}
	else
	{
		return NULL;
	}
}

Data* delete_data(MyDatabase* db, char* key_type, char* key)
{
	Data* data = find_data(db, key_type, key);

	if(data == NULL)
	{
		return NULL;
	}

	db->key_tree = delete_equal_data_node(db->key_tree, data);
	db->value_tree = delete_equal_data_node(db->value_tree, data);
	db->time_stamp_tree = delete_equal_data_node(db->time_stamp_tree, data);

	free_data(data);

	return data;
}

Data* db_query(MyDatabase* db, char* query)
{
	char cmd[16];
	char op1[256], op2[256];

	sscanf(query, "%s %s %s", cmd, op1, op2);

	if(strcmp(cmd, "show") == 0)
	{
		AVLTreeNode* tree;

		if(strcmp(op1, "key") == 0)
		{
			tree = db->key_tree;
		}
		else if(strcmp(op1, "value") == 0)
		{
			tree = db->value_tree;
		}
		else if(strcmp(op1, "time_stamp") == 0)
		{
			tree = db->time_stamp_tree;
		}

		if(strcmp(op2, "1") == 0)
		{
			show_tree_1d(tree);
		}
		else if(strcmp(op2, "2") == 0)
		{
			show_tree_2d(tree, 0);
		}
	}
	else if(strcmp(cmd, "insert") == 0)
	{
		return insert_data(db, create_data(op1, op2));
	}
	else if(strcmp(cmd, "find") == 0)
	{
		return find_data(db, op1, op2);
	}
	else if(strcmp(cmd, "delete") == 0)
	{
		return delete_data(db, op1, op2);
	}

	return NULL;
}

MyDatabase* save_database(MyDatabase* db, char* filename)
{
	FILE *fp = fopen(filename, "w");

	if(fp == NULL)
	{
		fprintf(stderr, "Error: File can't open.\n");
		return NULL;
	}

	save_node(db->key_tree, fp);

	fclose(fp);

	return db;
}

MyDatabase* load_database(MyDatabase* db, char* filename)
{
	FILE *fp = fopen(filename, "r");

	if(fp == NULL)
	{
		fprintf(stderr, "Error: File can't open.\n");
		return NULL;
	}

	Data* data = create_empty_data();
	while(fscanf(fp, "%s %s %s", data->key, data->value, data->time_stamp) != EOF)
	{
		insert_data(db, data);
		data = create_empty_data();
	}
	free_data(data);

	fclose(fp);

	return db;
}
