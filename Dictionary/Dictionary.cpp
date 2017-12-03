// Реализуется словарь с ключом типа string и значениями типа string
// Значения словаря хранятся в списках (для оптимизации времени доступа)
// Для выбора списка по ключe используется хеширование

// Подключение заголовочных файлов
#include "stdafx.h"
#include <clocale>
#include <iostream>
#include <string>
using namespace std;

// Число связных списков LinkedList, используемых в словаре
#define NUM_LISTS 13
// Размер алфавита для хеш-фукнции ключей словаря
#define ALPHABET_SIZE 128

// Класс, соответствующий узлек списка (записи словаря)
class ListNode {
public:
	// Ключ узла списка (ключ записи словаря)
	string key;
	// Значение узла списка (значение записи словаря)
	string value;
	// Ссылка на следующую запись списка
	ListNode *next;
	// Конструктор класса
	ListNode(string, string);
};

// Реализация конструктора класса узла списка
ListNode::ListNode(string key, string value) {
	this->key = key;
	this->value = value;
	this->next = NULL;
}

// Класс, соответствующий списку (вспомогательная структура данных для оптимизации времени доступа к записям словаря)
class LinkedList {
	ListNode *head;
public:
	// Два конструктора и деструктор
	LinkedList();
	LinkedList(string, string);
	~LinkedList();

	// Получение значения узла списка (значения словаря) по ключу
	string get_value(string);
	
	// Установка значения списка по ключу
	void set_value(string, string);
	
	// Вставка значения в список (вставка в словарь)
	void insert(string, string);

	// Удаление значения из списка (удаление из словаря)
	void remove(string);

	// Вывод элементов списка на экран
	void print();
};


// Конструктор без параметров
LinkedList::LinkedList() {
	this->head = NULL;
}

// Конструктор с параметрами для создания одного узла одновременно со списком
LinkedList::LinkedList(string key, string value) {
	ListNode *node = new ListNode(key, value);
	this->head = node;
}

// Дестркутор списка
LinkedList::~LinkedList() {
	ListNode *curr = this->head;
	// Перебор элементов списка и очиска их из памяти
	while (curr != NULL) {
		ListNode *next = curr->next;
		delete curr;
		curr = next;
	}
}

// Получение значения узла списка (значения словаря) по ключу
string LinkedList::get_value(string key) {
	string value = "";
	bool not_found = true;
	ListNode *curr = this->head;
	// Перебор элементов списка и поиск
	while (curr != NULL && not_found) {
		if (curr->key == key) {
			value = curr->value;
			not_found = false;
		}
		curr = curr->next;
	}
	return value;
}

// Установка значения списка по ключу
void LinkedList::set_value(string key, string value) {
	ListNode *curr = this->head;
	bool not_set = true;
	// Перебор элементов списка и установка значения при совпадении ключей
	while (curr != NULL && not_set) {
		if (curr->key == key) {
			curr->value = value;
			not_set = false;
		}
		curr = curr->next;
	}
}

// Вставка значения в список (вставка в словарь)
void LinkedList::insert(string key, string value) {
	ListNode *node = new ListNode(key, value);
	// Вставка значения в начало списка
	node->next = this->head;
	this->head = node;
}

// Удаление значения из списка (удаление из словаря)
void LinkedList::remove(string key) {
	string value = "";
	bool not_found = true;
	ListNode *curr = this->head;
	ListNode *prev = this->head;
	// Перебор элементов списка и удаление значения при совпадении ключей
	while (curr != NULL && not_found) {
		if (curr->key == key) {
			value = curr->value;
			not_found = false;
			break;
		}
		prev = curr;
		curr = curr->next;		
	}

	// Удаление узла при условии его наличия в словаре
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

// Вывод на экран содержимого списка
void LinkedList::print() {
	ListNode *curr = this->head;
	while (curr != NULL) {
		cout << "(" << curr->key << ", " << curr->value << ")" << endl;
		curr = curr->next;
	}
}

// Класс с непосредственно реализацией словаря
class Dictionary {
	// Массив связных списков, в которых хранятся узлы словаря
	LinkedList *lists[NUM_LISTS];
	// Получение индекса связного списка, в котором хранится элемент с заданным ключом
	int get_list_index(string);
	// Расчет значения хеш фукнции по ключу
	unsigned long get_hashed_value(string);
	// Вставка в связный список
	void insert_to_linked_list(int, string, string);
	// Замена значения словаря по ключу
	void replace(int, string, string);
public:
	// Контсруктор и деструктор словаря
	Dictionary();
	~Dictionary();
	// Получения значения по ключу
	string get(string);
	// Вставка в словарь
	void insert_or_replace(string, string);
	// Удаление из словаря по ключу
	void remove(string);
	// Вывод содержания словаря
	void print();
};

// Конструктор словаря
Dictionary::Dictionary() {
	for (int i = 0; i < NUM_LISTS; i++) {
		this->lists[i] = new LinkedList();
	}
}

// Освобождение памяти в декструкторе словаря
Dictionary::~Dictionary() {
	for (int i = 0; i < NUM_LISTS; i++) {
		LinkedList *list = this->lists[i];
		delete list;
		this->lists[i] = NULL;
	}
}

// Расчет хеш-функции по ключу для выбора связного списка
unsigned long Dictionary::get_hashed_value(string key) {
	unsigned long sum = 0;
	// Перебор символов ключа и расчет хеш-функции
	for (unsigned int i = 0; i < key.length(); i++) {
		int val = key[i] - 'a';
		sum = (sum * ALPHABET_SIZE) + val;
	}
	return sum;
}

// Получение индекса связного списка, в котором хранится элемент с заданным ключом
int Dictionary::get_list_index(string key) {
	unsigned long hashed_value = get_hashed_value(key);
	int bucket_index = hashed_value % NUM_LISTS;
	return bucket_index;
}

// Получение значения из связного списка
string Dictionary::get(string key) {
	string value = "";
	int linkedList = get_list_index(key);
	if (linkedList >= 0 && linkedList < NUM_LISTS) {
		LinkedList *list = this->lists[linkedList];
		value = list->get_value(key);
	}
	return value;
}

// Вставка в связный список по его номеру, ключу словаря и значению словаря
void Dictionary::insert_to_linked_list(int listIndex, string key, string value) {
	if (listIndex >= 0 && listIndex < NUM_LISTS) {
		LinkedList *list = this->lists[listIndex];
		list->insert(key, value);
	}
}

// Замена узла в списке по номеру, ключу словаря и значению словаря
void Dictionary::replace(int listIndex, string key, string value) {
	if (listIndex >= 0 && listIndex < NUM_LISTS) {
		LinkedList *list = this->lists[listIndex];
		list->set_value(key, value);
	}
}

// Вставка в словарь по ключу словаря и значению словаря или обновление значения словаря
void Dictionary::insert_or_replace(string key, string value) {
	// Поиск существующего значения
	string existing_value = get(key);
	int index = get_list_index(key);
	// Добавление нового узла
	if (existing_value.empty()) {
		insert_to_linked_list(index, key, value);
	}
	else {
		// Обновление значения узла
		replace(index, key, value);
	}
}

// Удаление элемента из словаря по ключу
void Dictionary::remove(string key) {
	string existing_value = get(key);
	// Поиск списка с удаляемым элементои по ключу
	int index = get_list_index(key);
	if (!existing_value.empty()) {
		lists[index]->remove(key);
	}
}

// Вывод на экран содержимого словаря
void Dictionary::print() {
	for (int i = 0; i < NUM_LISTS; i++) {
		LinkedList *list = this->lists[i];
		cout << "Список " << i << endl;
		list->print();
	}
}

int main() {
	// Задание русской кодировки
	setlocale(LC_ALL, "Russian");

	// Прогон необходимых тестовых примеров и вывод результатов на экран
	cout << "Тестовые примеры для Dictionary" << endl;
	cout << "Добавление значения в словарь" << endl;
	Dictionary *dict = new Dictionary();
	dict->insert_or_replace("съешь", "ещё");
	dict->insert_or_replace("этих", "testString");
	dict->insert_or_replace("мягких", "французских");
	dict->insert_or_replace("булок", "да");
	dict->insert_or_replace("выпей ", "чаю");
	cout << endl;
	cout << "Содержимое словаря:" << endl;
	dict->print();
	cout << endl;
	cout << "Чтение (поиск) значения по ключу" << endl;
	cout << "dict[съешь] = " << dict->get("съешь") << endl;
	cout << "dict[этих] = " << dict->get("этих") << endl;
	cout << "dict[мягких] = " << dict->get("мягких") << endl;
	cout << "dict[булок] = " << dict->get("булок") << endl;
	cout << endl;
	cout << "Изменение значения" << endl;
	cout << "dict[съешь] = не ешь" << endl;
	dict->insert_or_replace("съешь", "не ешь");
	cout << "Содержимое словаря:" << endl;
	dict->print();
	cout << endl;
	cout << "Удаление значения" << endl;
	cout << "dict[этих]->remove" << endl;
	dict->remove("этих");
	cout << "Содержимое словаря:" << endl;
	dict->print();

	// Удаление словаря и очистка памяти
	delete dict;

	// Пауза для просмотра консольного вывода
	system("pause");
	return 0;
}

//int main()
//{
//	// Задание русской кодировки
//	setlocale(LC_ALL, "Russian");
//
//	std::cout << "Определение максимального значения массива элементов double" << std::endl;
//	
//	// Запрос размера массива через вывод на стандартный поток std::cout
//	std::cout << "Введите размер массива:" << std::endl;
//
//	// Объявление переменной - размера массива
//	int size;
//
//	// Ввод размера массива из стандартного потока консольного воода std::cin
//	std::cin >> size;
//
//	// Проверка правильности ввода размера массива пользователем	
//	if (size <= 0)
//	{
//		std::cout << "Неверный размер массива!" << std::endl;
//		// Ожидание нажатия пользователем на любую клавишу (для просмотра резульаттов работы)
//		system("pause");
//
//		// Возврат 1, что означает завершение программы с ошибокой
//		return 1;
//	}
//
//	// Объявления массива для обработки (указателя на double)
//	double *values;
//
//	// Выделение памяти для массива
//	values = new double[size];
//
//	// Инициализация генератора псевдослучаыных чисел временем (для избежания повторения случайных чисел)
//	srand(time(NULL));
//	
//	// Заполнение массива псевдослучайными числами	
//	for (int i = 0; i < size; i++)
//	{
//		// Генерация вещественных чисел от 0 до 100 (rand - функция для генерации чисел от 0 до константы RAND_MAX)
//		values[i] = (double)rand() / RAND_MAX * 100;
//	}
//
//	// Вывод массива псевдослучайных чисел
//	std::cout << "Сгенерированный массив:" << std::endl;
//	for (int i = 0; i < size; i++)
//	{		
//		std::cout << "values[" << i << "] = " << values[i] << std::endl;
//	}
//
//	// Переменная, хранящая минимальное значение массива
//	double max;
//
//	// Инициализация переменной max первым элементом массива
//	max = values[0];
//
//	// Определение максиамльного значения в массиве
//	for (int i = 0; i < size; i++)
//	{
//		if (max < values[i])
//		{
//			max = values[i];
//		}		
//	}
//
//	// Вывод результата работы алгоритма (максимума)
//	std::cout << "max = " << max << std::endl;
//
//	// Ожидание нажатия пользователем на любую клавишу (для просмотра резульаттов работы)
//	system("pause");
//
//	// Возврат 0, что означает завершение программы без ошибок
//    return 0;
//}

