#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <math.h>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <fstream>
#include <conio.h>


#include "Header.h"
using namespace std;

char* substringLeft = new char[100];
char* substringRight = new char[100];

struct Node
{
	bool isVariable = 1;
	char* value = new char[10];
	double data = 0;
	Node* right = nullptr; Node* left = nullptr;
};

struct Stack {
	Node* head = nullptr;
	Node* tail = nullptr;

	Node* peek()
	{
		return tail;
	};

	void push(char* value)
	{
		Node* node = new Node;
		for (int i = 0; i < strlen(value); ++i)
		{
			node->value[i] = value[i];
		}
		node->value[strlen(value)] = '\0';
		if (tail == nullptr) {
			tail = node; head = node;
		}
		else {
			node->left = tail;
			tail->right = node;
			tail = node;
		}
	};

	Node* pop()
	{
		if (tail != nullptr) {
			Node* cur = tail;
			if (tail != head) {
				tail = (tail)->left;
				(tail)->right = nullptr;
			}
			else {
				tail = nullptr; head = nullptr;
			}
			return cur;
		}
	};
};

struct Tree
{
	Node* root = nullptr;

	Node* first(char* value) {
		root = new Node;
		for (int i = 0; i < strlen(value); ++i)
		{
			root->value[i] = value[i];
		}
		root->value[strlen(value)] = '\0';
		root->left = nullptr;
		root->right = nullptr;
		return root;
	}

	void print_tree(Node* p, int level)
	{
		if (p) {
			print_tree(p->left, level + 1);
			for (int i = 0; i < level; ++i)cout << "    ";
			cout << p->value << endl;
			print_tree(p->right, level + 1);
		}
	}
	void printPostorder(Node* node)
	{
		if (node == NULL)
			return;

		printPostorder(node->left);

		printPostorder(node->right);

		if (node->isVariable == 1)	cout << node->value << " ";
		else cout << node->data << " ";
	}
	bool variable_redefiner(Node* p, char v, float ins)
	{
		if (p) {
			variable_redefiner(p->left, v, ins);
			if (isalpha(p->value[0]) && v == p->value[0] && p->isVariable == 1 && strlen(p->value) == 1) {
				p->data = ins;
				p->isVariable = 0;
			}
			variable_redefiner(p->right, v, ins);

			return 1;
		}
		return 0;
	}
};

struct List {
	Node* head = NULL;
	Node* tail = NULL;

	void addLast(char* value) {
		Node* node = new Node;
		for (int i = 0; i < strlen(value); ++i)
		{
			node->value[i] = value[i];
		}
		node->value[strlen(value)] = '\0';
		node->right = NULL;
		if (head == NULL) {
			tail = node; head = node;
		}
		else {
			node->left = tail;
			tail->right = node;
			tail = node;
		}
	}

	Node* find(char* key)
	{
		Node* cur = head;
		while (cur)
		{
			if (strcmp(cur->value, key) == 0)break;
			cur = cur->right;
		}
		if (cur != NULL) { system("cls"); cout << "Here you are: " << cur->value << '\n'; }
		else cout << "There is no such element in list!" << endl;
		system("pause");
		system("cls");
		return cur;
	}

	bool remove(char* key)
	{
		if (Node* pkey = find(key)) {
			if (pkey == head) {
				head = (head)->right;
				(head)->left = NULL;
			}
			else if (pkey == tail) {
				tail = (tail)->left;
				(tail)->right = NULL;
			}
			else {
				(pkey->left)->right = pkey->right;
				(pkey->right)->left = pkey->left;
			}
			delete pkey;
			return true;
		}
		return false;
	}

	bool RemoveLast()
	{
		if (head == NULL) {
			cout << "There are no elements!" << endl << endl;
			return false;
		}
		else {
			Node* cur = tail;
			tail = (tail)->left;
			(tail)->right = NULL;
			delete cur;
		
			return true;
		}

	}
	void Print()
	{
		Node* cur = new Node;
		cur = head;
		while (cur) {
			cout << "Element: " << cur->value << endl;
			cur = cur->right;
		}
		return;
	}
};

Node* ExpressionTree(char* c, Stack*& stack)
{
	int cnt = 0;
	int i = 0;
	int iter = 0;
	while (c[i] != '\0') {
		if (c[i] == '(') {
			iter = i;
			cnt++; i++; while (c[i] != '\0') {
				if (c[i] == '(')cnt++;
				if (c[i] == ')')cnt--;
				if (cnt == 0 && i == strlen(c) - 1 && iter == 0) {
					for (int j = 0; j < strlen(c) - 2; ++j)c[j] = c[j + 1];
					c[strlen(c) - 2] = '\0';
					break;
				}
				if (cnt == 0)break;
				if (cnt != 0 && i == strlen(c) - 1) {
					cout << "Wrong string!\n";
					return NULL;
				}
				i++;
			}
			break;
		}
		i++;
	}
	i = 0;
	cnt = 0;
	if (strlen(c) == 1) {
		Node* node = new Node; node->value[0] = c[0]; node->value[1] = '\0';
		if (isdigit(c[0]))node->data = atof(c);
		return node;
	}

	while (c[i] != '\0') {

		if (c[i] == '(') {
			cnt++;
		}
		while (cnt != 0) {
			i++;
			if (c[i] == '(')cnt++;
			if (c[i] == ')')cnt--;
		}
		if (i == 0) {
			if (c[i] == '-') {
				Node* node = new Node;
				node->value[0] = '~'; node->value[1] = '\0';
				for (int j = i + 1; j < strlen(c); ++j) substringRight[j - (i + 1)] = c[j];
				substringRight[strlen(c) - (i + 1)] = '\0';

				node->right = ExpressionTree(substringRight, stack);
				return node;
			}
		}
		if (i != 0) {
			if (c[i] == '-' && c[i - 1] == '(') {
				Node* node = new Node;
				node->value[0] = '~'; node->value[1] = '\0';
				for (int j = i + 1; j < strlen(c); ++j) substringRight[j - (i + 1)] = c[j];
				substringRight[strlen(c) - (i + 1)] = '\0';

				node->right = ExpressionTree(substringRight, stack);
				return node;
			}
			if ((c[i] == '+' || c[i] == '-') && c[i - 1] != '(') {
				Node* node = new Node;
				bool isMinus = 0;
				if (c[i] == '-')isMinus = 1;

				if (isMinus) { node->value[0] = 45; node->value[1] = '\0'; }
				else { node->value[0] = 43; node->value[1] = '\0'; }

				char* temp = new char[20];
				for (int k = 0; k < strlen(c); ++k)temp[k] = c[k]; temp[strlen(c)] = '\0';
				int j = 0;
				for (j = 0; j < i; j++) {
					substringLeft[j] = c[j];
				}
				substringLeft[j] = '\0';

				for (j = i + 1; j < strlen(temp); ++j) substringRight[j - (i + 1)] = temp[j];
				substringRight[strlen(temp) - (i + 1)] = '\0';

				stack->push(substringRight);
				node->left = ExpressionTree(substringLeft, stack);

				for (int h = 0; h < strlen(stack->peek()->value); ++h) substringRight[h] = stack->peek()->value[h];
				substringRight[strlen(stack->peek()->value)] = '\0';
				stack->pop();

				node->right = ExpressionTree(substringRight, stack);
				return node;

			}

		}
		i++;
	}

	cnt = 0;
	i = 0;
	while (c[i] != '\0') {
		if (c[i] == '(') {
			cnt++;
		}
		while (cnt != 0) {
			i++;
			if (c[i] == '(')cnt++;
			if (c[i] == ')')cnt--;
		}
		if (c[i] == '*' || c[i] == '/') {
			Node* node = new Node;
			bool isDivide = 0;
			if (c[i] == '/')isDivide = 1;

			if (isDivide) { node->value[0] = 47; node->value[1] = '\0'; }
			else { node->value[0] = 42; node->value[1] = '\0'; }

			int j = 0;
			char* temp = new char[20];
			for (int k = 0; k < strlen(c); ++k)temp[k] = c[k]; temp[strlen(c)] = '\0';
			for (j = 0; j < i; j++) {
				substringLeft[j] = c[j];
			}
			substringLeft[j] = '\0';

			for (j = i + 1; j < strlen(temp); ++j) substringRight[j - (i + 1)] = temp[j];
			substringRight[strlen(temp) - (i + 1)] = '\0';

			stack->push(substringRight);
			node->left = ExpressionTree(substringLeft, stack);

			for (int h = 0; h < strlen(stack->peek()->value); ++h) substringRight[h] = stack->peek()->value[h];
			substringRight[strlen(stack->peek()->value)] = '\0';
			stack->pop();

			node->right = ExpressionTree(substringRight, stack);

			return node;
		}
		i++;
	}

	cnt = 0;
	i = 0;
	while (c[i] != '\0') {
		if (c[i] == '(') {
			cnt++;
		}
		while (cnt != 0) {
			i++;
			if (c[i] == '(')cnt++;
			if (c[i] == ')')cnt--;
		}

		if (c[i] == 'c' && c[i + 1] == 'o' && c[i + 2] == 's') {
			int k = i + 3;
			Node* node = new Node;
			node->value[0] = 'c'; node->value[1] = 'o'; node->value[2] = 's'; node->value[3] = '\0';
			for (k; k < strlen(c); ++k)
			{
				substringRight[k - (i + 3)] = c[k];
			}
			substringRight[strlen(c) - (i + 3)] = '\0';
			node->right = ExpressionTree(substringRight, stack);
			return node;
		}
		i++;
	}

	cnt = 0;
	i = 0;
	while (c[i] != '\0') {
		if (c[i] == '(') {
			cnt++;
		}
		while (cnt != 0) {
			i++;
			if (c[i] == '(')cnt++;
			if (c[i] == ')')cnt--;
		}

		if (c[i] == 's' && c[i + 1] == 'i' && c[i + 2] == 'n') {
			int k = i + 3;
			Node* node = new Node;
			node->value[0] = 's'; node->value[1] = 'i'; node->value[2] = 'n'; node->value[3] = '\0';
			for (k; k < strlen(c); ++k)
			{
				substringRight[k - (i + 3)] = c[k];
			}
			substringRight[strlen(c) - (i + 3)] = '\0';
			node->right = ExpressionTree(substringRight, stack);
			return node;
		}
		i++;
	}



	Node* node = new Node;
	i = 0;
	for (i; i < strlen(c); ++i) node->value[i] = c[i];
	node->value[strlen(c)] = '\0';
	if (c[0] == '~' || isdigit(c[0])) {
		double ins = 0;
		if (c[0] == '~') {
			c++;
			ins = (-1) * atof(c);
		}
		else ins = atof(c);
		node->data = ins;
		node->isVariable = 0;
	}

	return node;
}

char* getString()
{
	char* string = new char[100];
	cin.getline(string, 100);
	return string;
}

bool isSin(char f, char s, char t)
{
	if (f == 's' && s == 'i' && t == 'n')return 1;
	return 0;
}

bool isCos(char f, char s, char t)
{
	if (f == 'c' && s == 'o' && t == 's')return 1;
	return 0;
}

bool variableToFloat(char* v, double& ins, Tree*& tree)
{
	system("cls");
	int i = 0, k = 0;
	char tmp = 'o';
	while (v[i] != '\0') {
		cin.clear();
		if (isalpha(v[i]) && !isSin(v[i], v[i + 1], v[i + 2]) && !isCos(v[i], v[i + 1], v[i + 2])) {
			tree->printPostorder(tree->root); cout << endl << endl;
			cout << "Variable: " << v[i] << endl;
			cout << "Enter value: ";
			cin.clear();
			cin >> ins;
			tree->variable_redefiner(tree->root, v[i], ins);
			tmp = v[i];
			k = strlen(v);
			for (int j = 0; j < k; ++j) {
				if (v[j] == tmp) {
					for (int l = 0; l < k; ++l)v[l] = v[l + 1];
					v[k - 1] = '\0';
					k--;
					j = 0;
				}
			}
			system("cls");
		}
		i++;
	}
	system("cls");

	return 0;
}

double eval(Node* root)
{

	if (!root)
		return 0;


	if (!root->left && !root->right)
		return (root->data);


	double l_val = eval(root->left);

	double r_val = eval(root->right);

	if (root->value[0] == '+')
		return l_val + r_val;

	if (root->value[0] == '-')
		return l_val - r_val;

	if (root->value[0] == '~')
		return -(r_val);

	if (root->value[0] == '*')
		return l_val * r_val;

	if (root->value[0] == '/')
		return l_val / r_val;

	if (root->value[0] == 's')
		return sin(r_val);

	if (root->value[0] == 'c')
		return cos(r_val);
}

int main()
{
	cout << "Expression: ";
	Tree* tree = new Tree;
	Stack* stringstack = new Stack;
	char* expr = getString();
	char* temp = new char[100];

	strcpy(temp, expr);
	tree->root = ExpressionTree(expr, stringstack);
	int level = 0;
	tree->printPostorder(tree->root); cout << endl << endl;
	cout << "Tree itself: \n\n";
	tree->print_tree(tree->root, level);
	system("pause");
	double ins = 0;
	variableToFloat(temp, ins, tree);
	tree->printPostorder(tree->root); cout << endl << endl;
	cout << "Result = " << eval(tree->root) << endl;
	delete[] tree, stringstack, expr, temp;
	return 0;
}