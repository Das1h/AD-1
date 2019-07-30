#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "utility.h"

const size_t MAX_KEY_LEN = 256;
const size_t MAX_VALUE_LEN = 256;
const size_t MAX_TIME_STAMP_LEN = 16;

typedef struct Data
{
	char* key;
	char* value;
	char* time_stamp;
} Data;

void free_data(Data* data)
{
	if(data != NULL)
	{
		free(data->key);
		data->key = NULL;

		free(data->value);
		data->value = NULL;

		free(data->time_stamp);
		data->time_stamp = NULL;
	}
	free(data);
}

Data* create_empty_data()
{
	Data* data = (Data*)malloc(sizeof(Data));

	data->key = (char *)malloc(sizeof(char) * MAX_KEY_LEN);
	data->value = (char *)malloc(sizeof(char) * MAX_VALUE_LEN);
	data->time_stamp = (char *)malloc(sizeof(char) * MAX_TIME_STAMP_LEN);

	return data;
}

Data* create_data(char *key, char* value)
{
	Data* data = create_empty_data();

	sprintf(data->key, "%s", key);
	sprintf(data->value, "%s", value);
	get_time_stamp(data->time_stamp);

	return data;
}

void print_data(Data* data)
{
	if(data == NULL)
	{
		puts("NULL");
	}
	else
	{
		printf("key = %s, value = %s, time_stamp = %s\n", data->key, data->value, data->time_stamp);
	}
}

void save_data(Data* data, FILE* fp)
{
	fprintf(fp, "%s %s %s\n", data->key, data->value, data->time_stamp);
}

void load_data(Data* data, FILE* fp)
{
	fscanf(fp, "%s %s %s", data->key, data->value, data->time_stamp);
}
