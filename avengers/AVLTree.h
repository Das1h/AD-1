#pragma once
#include <string.h>
#include "Data.h"

typedef struct AVLTreeNode
{
	Data* data;
	struct AVLTreeNode* left;
	struct AVLTreeNode* right;
} AVLTreeNode;

int height(AVLTreeNode* node)
{
	return node == NULL ? 0 : max(height(node->left), height(node->right)) + 1;
}

int height_distance(AVLTreeNode* node)
{
	return height(node->left) - height(node->right);
}

AVLTreeNode* rotate_left(AVLTreeNode* node)
{
	AVLTreeNode* right = node->right;

	node->right = node->right->left;
	right->left = node;

	return right;
}

AVLTreeNode* rotate_right(AVLTreeNode* node)
{
	AVLTreeNode* left = node->left;

	node->left = node->left->right;
	left->right = node;

	return left;
}

AVLTreeNode* rotate_left_right(AVLTreeNode* node)
{
	node->left = rotate_left(node->left);
	node = rotate_right(node);
	return node;
}

AVLTreeNode* rotate_right_left(AVLTreeNode* node)
{
	node->right = rotate_right(node->right);
	node = rotate_left(node);
	return node;
}

AVLTreeNode* balance(AVLTreeNode* node)
{
	int h_d = height_distance(node);

	// 左の部分木のほうが高い
	if(h_d > 1)
	{
		// 左の子ノードの右の部分木の方が高い
		if(height_distance(node->left) < 0)
		{
			node = rotate_left_right(node);
		}
		else
		{
			node = rotate_right(node);
		}
	}
	// 右の部分木のほうが高い
	else if(h_d < -1)
	{
		// 右の子ノードの左の部分木の方が高い
		if(height_distance(node->right) > 0)
		{
			node = rotate_right_left(node);
		}
		else
		{
			node = rotate_left(node);
		}
	}

	return node;
}

AVLTreeNode* create_node(Data* data)
{
	AVLTreeNode* node = (AVLTreeNode*)malloc(sizeof(AVLTreeNode));

	node->data = data;

	node->left = NULL;
	node->right = NULL;

	return node;
}

AVLTreeNode* insert_node(AVLTreeNode* node, AVLTreeNode* new_node, char* key_type)
{
	if(node == NULL)
	{
		return new_node;
	}

	char* node_key;
	char* new_node_key;
	if(strcmp(key_type, "key") == 0)
	{
		node_key = node->data->key;
		new_node_key = new_node->data->key;
	}
	else if(strcmp(key_type, "value") == 0)
	{
		node_key = node->data->value;
		new_node_key = new_node->data->value;
	}
	else if(strcmp(key_type, "time_stamp") == 0)
	{
		node_key = node->data->time_stamp;
		new_node_key = new_node->data->time_stamp;
	}
	else
	{
		return NULL;
	}

	int cmp = strcmp(node_key, new_node_key);

	if(cmp >= 0)
	{
		node->left = insert_node(node->left, new_node, key_type);
	}
	else
	{
		node->right = insert_node(node->right, new_node, key_type);
	}

	return node = balance(node);
}

AVLTreeNode* find_node(AVLTreeNode* node, char* key_type, char* key)
{
	if(node == NULL)
	{
		return NULL;
	}

	char* node_key;
	if(strcmp(key_type, "key") == 0)
	{
		node_key = node->data->key;
	}
	else if(strcmp(key_type, "value") == 0)
	{
		node_key = node->data->value;
	}
	else if(strcmp(key_type, "time_stamp") == 0)
	{
		node_key = node->data->time_stamp;
	}
	else
	{
		return NULL;
	}

	int cmp = strcmp(node_key, key);

	if(cmp == 0)
	{
		return node;
	}
	else if(cmp < 0)
	{
		return find_node(node->right, key_type, key);
	}
	else
	{
		return find_node(node->left, key_type, key);
	}

	return NULL;
}

AVLTreeNode* find_equal_data_node(AVLTreeNode* node, Data* data)
{
	if(node == NULL)
	{
		return NULL;
	}

	if(node->data == data)
	{
		return node;
	}

	AVLTreeNode* left_result_node = find_equal_data_node(node->left, data);
	AVLTreeNode* right_result_node = find_equal_data_node(node->right, data);

	if(left_result_node != NULL)
	{
		return left_result_node;
	}
	else if(right_result_node != NULL)
	{
		return right_result_node;
	}
	else
	{
		return NULL;
	}
}

AVLTreeNode* find_min_node(AVLTreeNode* node)
{
	if(node == NULL)
	{
		return NULL;
	}

	if(node->left == NULL)
	{
		return node;
	}
	else
	{
		return find_min_node(node->left);
	}
}

AVLTreeNode* find_max_node(AVLTreeNode* node)
{
	if(node == NULL)
	{
		return NULL;
	}

	if(node->right == NULL)
	{
		return node;
	}
	else
	{
		return find_max_node(node->right);
	}
}

AVLTreeNode* delete_min_node(AVLTreeNode* node)
{
	if(node == NULL)
	{
		return NULL;
	}

	if(node->left == NULL)
	{
		AVLTreeNode* right_node = node->right;
		return right_node;
	}
	else
	{
		node->left = delete_min_node(node->left);
	}

	return node = balance(node);
}

AVLTreeNode* delete_max_node(AVLTreeNode* node)
{
	if(node == NULL)
	{
		return NULL;
	}

	if(node->right == NULL)
	{
		AVLTreeNode* left_node = node->left;
		return left_node;
	}
	else
	{
		node->right = delete_max_node(node->right);
	}

	return node = balance(node);
}

AVLTreeNode* delete_node(AVLTreeNode* node, char* key_type, char* key)
{
	if(node == NULL)
	{
		return NULL;
	}

	char* node_key;
	if(strcmp(key_type, "key") == 0)
	{
		node_key = node->data->key;
	}
	else if(strcmp(key_type, "value") == 0)
	{
		node_key = node->data->value;
	}
	else if(strcmp(key_type, "time_stamp") == 0)
	{
		node_key = node->data->time_stamp;
	}
	else
	{
		return NULL;
	}

	int cmp = strcmp(node_key, key);

	if(cmp == 0)
	{
		// 子ノードが2つある場合
		if(node->left != NULL && node->right != NULL)
		{
			AVLTreeNode* alt_node = find_max_node(node->left);

			node->left = delete_max_node(node->left);

			alt_node->left = node->left;
			alt_node->right = node->right;

			node = alt_node;
		}
		// 左しか子ノードがない場合
		else if(node->left != NULL)
		{
			return node->left;
		}
		// 右しか子ノードがない場合
		else if(node->right != NULL)
		{
			return node->right;
		}
		// 子ノードがない場合
		else
		{
			return NULL;
		}
	}
	else if(cmp > 0)
	{
		node->left = delete_node(node->left, key_type, key);
	}
	else
	{
		node->right = delete_node(node->right, key_type, key);
	}

	return node = balance(node);
}

AVLTreeNode* delete_equal_data_node(AVLTreeNode* node, Data* data)
{
	if(node == NULL)
	{
		return NULL;
	}

	if(node->data == data)
	{
		// 子ノードが2つある場合
		if(node->left != NULL && node->right != NULL)
		{
			AVLTreeNode* alt_node = find_max_node(node->left);

			node->left = delete_max_node(node->left);

			alt_node->left = node->left;
			alt_node->right = node->right;

			node = alt_node;
		}
		// 左しか子ノードがない場合
		else if(node->left != NULL)
		{
			return node->left;
		}
		// 右しか子ノードがない場合
		else if(node->right != NULL)
		{
			return node->right;
		}
		// 子ノードがない場合
		else
		{
			return NULL;
		}

		if(data == NULL)
		{
			puts("FUCK");
		}
	}
	else
	{
		node->left = delete_equal_data_node(node->left, data);
		node->right = delete_equal_data_node(node->right, data);
	}

	return node = balance(node);
}

void show_tree_2d(AVLTreeNode *node, int depth)
{
	if(node == NULL)
	{
		return;
	}

	int next_depth = depth + strlen(node->data->key) + strlen(node->data->value) + strlen(node->data->time_stamp) + 4;

	show_tree_2d(node->right, next_depth);

	for(int i = 0; i < depth; ++i)
	{
		printf(" ");
	}

	printf("(%s,%s,%s)\n", node->data->key, node->data->value, node->data->time_stamp);

	show_tree_2d(node->left, next_depth);

	return;
}

void show_tree_1d(AVLTreeNode *node)
{
	if(node == NULL)
	{
		return;
	}

	show_tree_1d(node->left);

	print_data(node->data);

	show_tree_1d(node->right);

	return;
}

void save_node(AVLTreeNode* node, FILE* fp)
{
	if(node == NULL)
	{
		return;
	}

	save_node(node->left, fp);

	save_data(node->data, fp);

	save_node(node->right, fp);
}

void get_all_node(AVLTreeNode *node, Data* result[], int* cnt)
{
	if(node == NULL)
	{
		return;
	}

	get_all_node(node->left, result, cnt);

	result[*cnt] = node->data;
	++(*cnt);

	get_all_node(node->right, result, cnt);

	return;
}
