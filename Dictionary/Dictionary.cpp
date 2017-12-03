// ����������� ������� � ������ ���� string � ���������� ���� string
// �������� ������� �������� � ������� (��� ����������� ������� �������)
// ��� ������ ������ �� ����e ������������ �����������

// ����������� ������������ ������
#include "stdafx.h"
#include <clocale>
#include <iostream>
#include <string>
using namespace std;

// ����� ������� ������� LinkedList, ������������ � �������
#define NUM_LISTS 13
// ������ �������� ��� ���-������� ������ �������
#define ALPHABET_SIZE 128

// �����, ��������������� ����� ������ (������ �������)
class ListNode {
public:
	// ���� ���� ������ (���� ������ �������)
	string key;
	// �������� ���� ������ (�������� ������ �������)
	string value;
	// ������ �� ��������� ������ ������
	ListNode *next;
	// ����������� ������
	ListNode(string, string);
};

// ���������� ������������ ������ ���� ������
ListNode::ListNode(string key, string value) {
	this->key = key;
	this->value = value;
	this->next = NULL;
}

// �����, ��������������� ������ (��������������� ��������� ������ ��� ����������� ������� ������� � ������� �������)
class LinkedList {
	ListNode *head;
public:
	// ��� ������������ � ����������
	LinkedList();
	LinkedList(string, string);
	~LinkedList();

	// ��������� �������� ���� ������ (�������� �������) �� �����
	string get_value(string);
	
	// ��������� �������� ������ �� �����
	void set_value(string, string);
	
	// ������� �������� � ������ (������� � �������)
	void insert(string, string);

	// �������� �������� �� ������ (�������� �� �������)
	void remove(string);

	// ����� ��������� ������ �� �����
	void print();
};


// ����������� ��� ����������
LinkedList::LinkedList() {
	this->head = NULL;
}

// ����������� � ����������� ��� �������� ������ ���� ������������ �� �������
LinkedList::LinkedList(string key, string value) {
	ListNode *node = new ListNode(key, value);
	this->head = node;
}

// ���������� ������
LinkedList::~LinkedList() {
	ListNode *curr = this->head;
	// ������� ��������� ������ � ������ �� �� ������
	while (curr != NULL) {
		ListNode *next = curr->next;
		delete curr;
		curr = next;
	}
}

// ��������� �������� ���� ������ (�������� �������) �� �����
string LinkedList::get_value(string key) {
	string value = "";
	bool not_found = true;
	ListNode *curr = this->head;
	// ������� ��������� ������ � �����
	while (curr != NULL && not_found) {
		if (curr->key == key) {
			value = curr->value;
			not_found = false;
		}
		curr = curr->next;
	}
	return value;
}

// ��������� �������� ������ �� �����
void LinkedList::set_value(string key, string value) {
	ListNode *curr = this->head;
	bool not_set = true;
	// ������� ��������� ������ � ��������� �������� ��� ���������� ������
	while (curr != NULL && not_set) {
		if (curr->key == key) {
			curr->value = value;
			not_set = false;
		}
		curr = curr->next;
	}
}

// ������� �������� � ������ (������� � �������)
void LinkedList::insert(string key, string value) {
	ListNode *node = new ListNode(key, value);
	// ������� �������� � ������ ������
	node->next = this->head;
	this->head = node;
}

// �������� �������� �� ������ (�������� �� �������)
void LinkedList::remove(string key) {
	string value = "";
	bool not_found = true;
	ListNode *curr = this->head;
	ListNode *prev = this->head;
	// ������� ��������� ������ � �������� �������� ��� ���������� ������
	while (curr != NULL && not_found) {
		if (curr->key == key) {
			value = curr->value;
			not_found = false;
			break;
		}
		prev = curr;
		curr = curr->next;		
	}

	// �������� ���� ��� ������� ��� ������� � �������
	if (!not_found)
	{
		if (head == curr)
		{
			head = curr->next;
		}
		prev->next = curr->next;
		delete curr;
	}
}

// ����� �� ����� ����������� ������
void LinkedList::print() {
	ListNode *curr = this->head;
	while (curr != NULL) {
		cout << "(" << curr->key << ", " << curr->value << ")" << endl;
		curr = curr->next;
	}
}

// ����� � ��������������� ����������� �������
class Dictionary {
	// ������ ������� �������, � ������� �������� ���� �������
	LinkedList *lists[NUM_LISTS];
	// ��������� ������� �������� ������, � ������� �������� ������� � �������� ������
	int get_list_index(string);
	// ������ �������� ��� ������� �� �����
	unsigned long get_hashed_value(string);
	// ������� � ������� ������
	void insert_to_linked_list(int, string, string);
	// ������ �������� ������� �� �����
	void replace(int, string, string);
public:
	// ����������� � ���������� �������
	Dictionary();
	~Dictionary();
	// ��������� �������� �� �����
	string get(string);
	// ������� � �������
	void insert_or_replace(string, string);
	// �������� �� ������� �� �����
	void remove(string);
	// ����� ���������� �������
	void print();
};

// ����������� �������
Dictionary::Dictionary() {
	for (int i = 0; i < NUM_LISTS; i++) {
		this->lists[i] = new LinkedList();
	}
}

// ������������ ������ � ������������ �������
Dictionary::~Dictionary() {
	for (int i = 0; i < NUM_LISTS; i++) {
		LinkedList *list = this->lists[i];
		delete list;
		this->lists[i] = NULL;
	}
}

// ������ ���-������� �� ����� ��� ������ �������� ������
unsigned long Dictionary::get_hashed_value(string key) {
	unsigned long sum = 0;
	// ������� �������� ����� � ������ ���-�������
	for (unsigned int i = 0; i < key.length(); i++) {
		int val = key[i] - 'a';
		sum = (sum * ALPHABET_SIZE) + val;
	}
	return sum;
}

// ��������� ������� �������� ������, � ������� �������� ������� � �������� ������
int Dictionary::get_list_index(string key) {
	unsigned long hashed_value = get_hashed_value(key);
	int bucket_index = hashed_value % NUM_LISTS;
	return bucket_index;
}

// ��������� �������� �� �������� ������
string Dictionary::get(string key) {
	string value = "";
	int linkedList = get_list_index(key);
	if (linkedList >= 0 && linkedList < NUM_LISTS) {
		LinkedList *list = this->lists[linkedList];
		value = list->get_value(key);
	}
	return value;
}

// ������� � ������� ������ �� ��� ������, ����� ������� � �������� �������
void Dictionary::insert_to_linked_list(int listIndex, string key, string value) {
	if (listIndex >= 0 && listIndex < NUM_LISTS) {
		LinkedList *list = this->lists[listIndex];
		list->insert(key, value);
	}
}

// ������ ���� � ������ �� ������, ����� ������� � �������� �������
void Dictionary::replace(int listIndex, string key, string value) {
	if (listIndex >= 0 && listIndex < NUM_LISTS) {
		LinkedList *list = this->lists[listIndex];
		list->set_value(key, value);
	}
}

// ������� � ������� �� ����� ������� � �������� ������� ��� ���������� �������� �������
void Dictionary::insert_or_replace(string key, string value) {
	// ����� ������������� ��������
	string existing_value = get(key);
	int index = get_list_index(key);
	// ���������� ������ ����
	if (existing_value.empty()) {
		insert_to_linked_list(index, key, value);
	}
	else {
		// ���������� �������� ����
		replace(index, key, value);
	}
}

// �������� �������� �� ������� �� �����
void Dictionary::remove(string key) {
	string existing_value = get(key);
	// ����� ������ � ��������� ��������� �� �����
	int index = get_list_index(key);
	if (!existing_value.empty()) {
		lists[index]->remove(key);
	}
}

// ����� �� ����� ����������� �������
void Dictionary::print() {
	for (int i = 0; i < NUM_LISTS; i++) {
		LinkedList *list = this->lists[i];
		cout << "������ " << i << endl;
		list->print();
	}
}

int main() {
	// ������� ������� ���������
	setlocale(LC_ALL, "Russian");

	// ������ ����������� �������� �������� � ����� ����������� �� �����
	cout << "�������� ������� ��� Dictionary" << endl;
	cout << "���������� �������� � �������" << endl;
	Dictionary *dict = new Dictionary();
	dict->insert_or_replace("�����", "���");
	dict->insert_or_replace("����", "testString");
	dict->insert_or_replace("������", "�����������");
	dict->insert_or_replace("�����", "��");
	dict->insert_or_replace("����� ", "���");
	cout << endl;
	cout << "���������� �������:" << endl;
	dict->print();
	cout << endl;
	cout << "������ (�����) �������� �� �����" << endl;
	cout << "dict[�����] = " << dict->get("�����") << endl;
	cout << "dict[����] = " << dict->get("����") << endl;
	cout << "dict[������] = " << dict->get("������") << endl;
	cout << "dict[�����] = " << dict->get("�����") << endl;
	cout << endl;
	cout << "��������� ��������" << endl;
	cout << "dict[�����] = �� ���" << endl;
	dict->insert_or_replace("�����", "�� ���");
	cout << "���������� �������:" << endl;
	dict->print();
	cout << endl;
	cout << "�������� ��������" << endl;
	cout << "dict[����]->remove" << endl;
	dict->remove("����");
	cout << "���������� �������:" << endl;
	dict->print();

	// �������� ������� � ������� ������
	delete dict;

	// ����� ��� ��������� ����������� ������
	system("pause");
	return 0;
}

//int main()
//{
//	// ������� ������� ���������
//	setlocale(LC_ALL, "Russian");
//
//	std::cout << "����������� ������������� �������� ������� ��������� double" << std::endl;
//	
//	// ������ ������� ������� ����� ����� �� ����������� ����� std::cout
//	std::cout << "������� ������ �������:" << std::endl;
//
//	// ���������� ���������� - ������� �������
//	int size;
//
//	// ���� ������� ������� �� ������������ ������ ����������� ����� std::cin
//	std::cin >> size;
//
//	// �������� ������������ ����� ������� ������� �������������	
//	if (size <= 0)
//	{
//		std::cout << "�������� ������ �������!" << std::endl;
//		// �������� ������� ������������� �� ����� ������� (��� ��������� ����������� ������)
//		system("pause");
//
//		// ������� 1, ��� �������� ���������� ��������� � ��������
//		return 1;
//	}
//
//	// ���������� ������� ��� ��������� (��������� �� double)
//	double *values;
//
//	// ��������� ������ ��� �������
//	values = new double[size];
//
//	// ������������� ���������� ��������������� ����� �������� (��� ��������� ���������� ��������� �����)
//	srand(time(NULL));
//	
//	// ���������� ������� ���������������� �������	
//	for (int i = 0; i < size; i++)
//	{
//		// ��������� ������������ ����� �� 0 �� 100 (rand - ������� ��� ��������� ����� �� 0 �� ��������� RAND_MAX)
//		values[i] = (double)rand() / RAND_MAX * 100;
//	}
//
//	// ����� ������� ��������������� �����
//	std::cout << "��������������� ������:" << std::endl;
//	for (int i = 0; i < size; i++)
//	{		
//		std::cout << "values[" << i << "] = " << values[i] << std::endl;
//	}
//
//	// ����������, �������� ����������� �������� �������
//	double max;
//
//	// ������������� ���������� max ������ ��������� �������
//	max = values[0];
//
//	// ����������� ������������� �������� � �������
//	for (int i = 0; i < size; i++)
//	{
//		if (max < values[i])
//		{
//			max = values[i];
//		}		
//	}
//
//	// ����� ���������� ������ ��������� (���������)
//	std::cout << "max = " << max << std::endl;
//
//	// �������� ������� ������������� �� ����� ������� (��� ��������� ����������� ������)
//	system("pause");
//
//	// ������� 0, ��� �������� ���������� ��������� ��� ������
//    return 0;
//}

