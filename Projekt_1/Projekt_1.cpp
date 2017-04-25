// Projekt_1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <time.h>

using namespace std;

double PCFreq = 0.0;
__int64 CounterStart = 0;
double wynik;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}

class Table
{
private:
	
public:
	int cnt; //iloœæ elementów w tablicy
	int *tab;

	Table()
	{
		cnt = 0;
		tab = new int[cnt];
	}

	void loadFromFile(string fileName) 
	{
		fstream file;
		file.open(fileName + ".txt", ios::in);
		file >> cnt;
		tab = new int[cnt];
		int count = 1;
		for (int i = 0; i < cnt; i++)
		{
			file >> tab[i];
		}
		//while (file >> tab[count++]) { }
		file.close();
	}

	void remove(int index)
	{
		int* t = new int[cnt - 1];
		for (int i = 0; i < index; i++)
		{
			t[i] = tab[i];
		}
		for (int i = index + 1; i < cnt; i++)
		{
			t[i - 1] = tab[i];
		}
		cnt--;
		delete[]tab;
		tab = t;
	}

	void add(int index, int value) 
	{
		cnt++;
		int * t = new int[cnt];
		for (int i = 0; i < index; i++)
		{
			t[i] = tab[i];
		}
		t[index] = value;
		for (int i = index + 1; i < cnt; i++)
		{
			t[i] = tab[i - 1];
		}
		delete[]tab;
		tab = t;
	}

	void generateTable(int size)
	{
		cnt = size;
		tab = new int[cnt];
		tab[0] = size;
		for (int i = 1; i < cnt; i++) {
			tab[i] = rand() % 50000 - 25000;
		}
	}

	bool IsValueInTable(int value)
	{
		for (int i = 0; i < cnt; i++)
		{
			if (value == tab[i]) return true;
		}
		return false;
	}

	void display()
	{
		if (cnt < 1) cout << "Lista jest pusta, nie mozna jej wyswietlic." << endl;
		else
		{
			for (int i = 0; i < cnt; i++) 
			{
				cout << tab[i] << " ";
			}
		}
	}
};

void displayMenu(string info)
{
	cout << endl;
	cout << info << endl;
	cout << "1.Wczytaj z pliku" << endl;
	cout << "2.Usun" << endl;
	cout << "3.Dodaj" << endl;
	cout << "4.Znajdz" << endl;
	cout << "5.Utworz losowo" << endl;
	cout << "6.Wyswietl" << endl;
	cout << "7.Test (pomiary)" << endl;
	cout << "0.Powrot do menu" << endl;
	cout << "Podaj opcje:";
}

Table myTab;

void menu_table()
{
	srand(time(NULL));
	char opt;
	string fileName;
	int index, value;

	do {
		displayMenu("--- TABLICA ---");
		opt = _getche();
		cout << endl;
		switch (opt) {
		case '1': //tutaj wczytytwanie tablicy z pliku
			cout << "Podaj nazwe zbioru: ";
			cin >> fileName;
			myTab.loadFromFile(fileName);
			myTab.display();
			break;

		case '2': //tutaj usuwanie elemenu z tablicy
			cout << "Podaj indeks: ";
			cin >> index;
			if (index < 0 || index >= myTab.cnt) 
				cout << "Tablica nie zawiera podanego indeksu." << endl;
			else 
				myTab.remove(index);
			myTab.display();
			break;

		case '3': //tutaj dodawanie elemetu do tablicy
			cout << "Podaj index: ";
			cin >> index;
			cout << "Podaj wartosc: ";
			cin >> value;
			
			if (index < 0 || index > myTab.cnt)
				cout << "Tablica nie zawiera podanego indeksu." << endl;
			else
				myTab.add(index, value);
			myTab.display();
			break;
			
		case '4': //tutaj znajdowanie elemetu w tablicy
			cout << "Podaj wartosc: ";
			cin >> value;
			if (myTab.IsValueInTable(value))
				cout << "Podana wartosc jest w tablicy.";
			else 
				cout << "Podanej wartosci NIE ma w tablicy.";
			break;
			
		case '5':  //tutaj generowanie tablicy
			cout << "Podaj ilosc elementow tablicy: ";
			cin >> value;
			if (value <= 0)
				cout << "Podaj poprawny rozmiar tablicy." << endl;
			else
				myTab.generateTable(value);
			myTab.display();
			break;

		case '6':  //tutaj wyœwietlanie tablicy
			myTab.display();
			break;
			
		case '7': //tutaj nasza funkcja do eksperymentów (pomiary czasów i generowanie daneych) - nie bêdzie testowana przez prowadz¹cego 
				  // mo¿na sobie tu dodaæ w³asne case'y
			//wynik = 0;
			int tab[] = { 1000,5000,10000,20000,50000 };
			for (int y = 0; y < 5; y++)
			{
				int size = tab[y];

				//dodawanie na pocz¹tku
				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myTab.generateTable(size);
					for (int i = 0; i < 100; i++)
					{
						StartCounter();
						myTab.add(0, i);
						wynik += GetCounter();
					}
				}
				cout << "Srednia dodawania na poczatek: " << wynik / (100 * 50) << endl;

				//dodawanie w randomowym miejscu
				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myTab.generateTable(size);
					for (int i = 0; i < 100; i++)
					{
						int randIndex = rand() % (size - 2) + 1;
						StartCounter();
						myTab.add(randIndex, i);
						wynik += GetCounter();
					}
				}
				cout << "Srednia dodawania w randomowym miejscu: " << wynik / (100 * 50) << endl;

				//dodawanie do koñcu
				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myTab.generateTable(size);
					for (int i = 0; i < 100; i++)
					{
						StartCounter();
						myTab.add(size + i, i);
						wynik += GetCounter();
					}
				}
				cout << "Srednia dodawania na ostatnim miejscu: " << wynik / (100 * 50) << endl;

				//-------USUWANIE----------------USUWANIE----------------USUWANIE----------------USUWANIE----------------
				//usuwanie z poczatku
				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myTab.generateTable(size);
					for (int i = 0; i < 100; i++)
					{
						StartCounter();
						myTab.remove(0);
						wynik += GetCounter();
					}
				}
				cout << "Srednia usuwania z poczatku: " << wynik / (100 * 50) << endl;

				//usuwanie w randomowym miejscu
				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myTab.generateTable(size);
					for (int i = 0; i < 100; i++)
					{
						int randIndex = rand() % (size - 2 - i) + 1;
						StartCounter();
						myTab.remove(randIndex);
						wynik += GetCounter();
					}
				}
				cout << "Srednia usuwania w randomowym miejscu: " << wynik / (100 * 50) << endl;

				//usuwanie do koñcu
				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myTab.generateTable(size);
					for (int i = 0; i < 100; i++)
					{
						StartCounter();
						myTab.remove(size - 1 - i);
						wynik += GetCounter();
					}
				}
				cout << "Srednia usuwania z ostaniego miejsca: " << wynik / (100 * 50) << endl;

				//-------------WYSZUKIWANIE------------------------

				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myTab.generateTable(size);
					for (int i = 0; i < 100; i++)
					{
						int randValue = rand() % 50000 - 25000;
						StartCounter();
						myTab.IsValueInTable(randValue);
						wynik += GetCounter();
					}
				}
				cout << "Srednia wyszukania wartosci: " << wynik / (100 * 50) << endl << endl;
			}
			break;
		}

	} while (opt != '0');
}

class ListElement
{
public:
	ListElement *next;
	ListElement *prev;
	int value;
};

class MyList
{
	ListElement *head, *tail;
public:
	int count;

	MyList()
	{
		head = tail = NULL;
		count = 0;
	}

	~MyList()
	{
		ListElement * element;
		while (head)
		{
			element = head->next;
			delete head;
			head = element;
		}
		count = 0;
	}

	void loadFromFile(string fileName)
	{
		fstream file;
		int key;
		int size;
		file.open(fileName + ".txt", ios::in);
		file >> size;
		//ListElement *firstElement = new ListElement();
		for (int i = 0; i < size; i++)
		{
			file >> key;
			addToEnd(key);
		}
		file.close();
	}

	void generateList(int size)
	{
		if (size > 0)
		{
			ListElement *firstElement = new ListElement();
			int key;
			head = firstElement;
			tail = firstElement;
			firstElement->next = NULL;
			firstElement->prev = NULL;
			firstElement->value = size;
			count = 1;
			for (int i = 1; i < size; i++)
			{
				key = rand() % 50000 - 25000;
				addToEnd(key);
			}
		}
	}

	bool IsValueInList(int value)
	{
		bool isThere = false;
		ListElement *element;
		element = head;
		for (int i = 0; i < count; i++)
		{
			if (element->value == value) { isThere = true; break; }
			else element = element->next;
		}
		return isThere;
	}

	void add(int index, int value)
	{
		if (index == count)
		{
			addToEnd(value);
		}
		else
		{
			ListElement *element;
			ListElement *newElement = new ListElement();
			element = head;

			for (int i = 0; i < index; i++)
			{
				element = element->next;
			}
			newElement->value = value;
			newElement->prev = element->prev;
			newElement->next = element;
			element->prev = newElement;
			if (newElement->prev) newElement->prev->next = newElement;
			else head = newElement;
			count++;
		}
	}
	
	void addToEnd(int value)
	{
		ListElement *newElement = new ListElement();
		newElement->prev = tail;
		newElement->next = NULL;
		if (tail) tail->next = newElement;
		tail = newElement;
		if (!head) head = tail;
		newElement->value = value;
		count++;
	}

	void addToBeggin(int value)
	{
		ListElement *newElement = new ListElement();
		newElement->prev = NULL;
		newElement->next = head;
		if (head) head->prev = newElement;
		head = newElement;
		if (!tail) tail = head;
		newElement->value = value;
		count++;
	}

	void remove(int value)
	{
		ListElement *element;
		element = head;
		while ((element->next != NULL) && (element->value != value)) 
		{ 
			element = element->next; 
		}

		if (element->value == value)
		{
			if (element->next) element->next->prev = element->prev;
			else tail = element->prev;
			if (element->prev) { element->prev->next = element->next; }
			else head = element->next;
			count--;
		}
	}
	
	void removeFromEnd()
	{
		ListElement *element;
		if (tail->prev)
		{
			element = tail->prev;
			element->next = NULL;
			delete[]tail;
			tail = element;
			count--;
		}
		else
		{
			delete[]tail;
			count--;
		}
	}

	void removeFromBeggin()
	{
		ListElement *element;
		if (head->next)
		{
			element = head->next;
			element->prev = NULL;
			delete[]head;
			head = element;
			count--;
		}
		else
		{
			delete[]head;
			count--;
		}

	}

	void display()
	{
		ListElement * element;
		if (count < 1) cout << "Lista jest pusta, nie mozna jej wyswietlic." << endl;
		else
		{
			element = head;
			while (element)
			{
				cout << element->value << " ";
				element = element->next;
			}
			cout << endl << endl;
			element = tail;
			while (element)
			{
				cout << element->value << " ";
				element = element->prev;
			}
		}
	}

};

MyList myList;

void menu_list()
{
	srand(time(NULL));
	char opt;
	string fileName;
	int index, value;

	do {
		displayMenu("--- LISTA ---");
		opt = _getche();
		cout << endl;
		switch (opt) {
		case '1': //tutaj wczytytwanie listy z pliku
			cout << "Podaj nazwe zbioru: ";
			cin >> fileName;
			myList.loadFromFile(fileName);
			myList.display();
			break;

		case '2': //tutaj usuwanie elemenu z listy
			if (myList.count == 0)
			{
				cout << "Nie mozna usunac elementu, poniewaz lista jest pusta" << endl;
				break;
			}
			cout << "Podaj wartosc:";
			cin >> value;
			myList.remove(value);
			myList.display();
			break;
		
		case '3': //tutaj dodawanie elemetu do listy
			cout << "Podaj index: ";
			cin >> index;
			cout << "Podaj wartosc: ";
			cin >> value;
			if (index < 0 || index > myList.count)
			{
				if (index < 0) myList.addToBeggin(value);
				else myList.addToEnd(value);
			}
			else
				myList.add(index, value);
			myList.display();
			break;

		case '4': //tutaj znajdowanie elemetu w liscie
			cout << "Podaj wartosc: ";
			cin >> value;
			if (myList.IsValueInList(value))
				cout << "Poadana wartosc znajduje sie w liscie.";
			else
				cout << "Podana wartosc NIE znajduje sie w liscie.";
			break;

		case '5':  //tutaj generowanie listy
			cout << "Podaj ilosc elementow tablicy: ";
			cin >> value;
			if (value <= 0)
				cout << "Podaj poprawny rozmiar tablicy." << endl;
			else
				myList.generateList(value);
			myList.display();
			break;

		case '6':  //tutaj wyœwietlanie listy
			myList.display();
			break;
			
		case '7': //tutaj nasza funkcja do eksperymentów (pomiary czasów i generowanie daneych) - nie bêdzie testowana przez prowadz¹cego
			// mo¿na sobie tu dodaæ w³asne case'y
			wynik = 0;
			int tab[] = { 1000,5000,10000,20000,50000 };
			
			for (int y = 0; y < 5; y++)
			{
				int size = tab[y];
				wynik = 0;
				//dodawanie na pocz¹tku
				for (int j = 0; j < 50; j++)
				{
					myList.generateList(size);
					for (int i = 0; i < 100; i++)
					{
						StartCounter();
						myList.addToBeggin(i);
						wynik += GetCounter();
					}
				}
				cout << "Srednia dodawania na poczatek: " << wynik / (100 * 50) << endl;

				//dodawanie w randomowym miejscu
				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myList.generateList(size);
					for (int i = 0; i < 100; i++)
					{
						int randIndex = rand() % (size - 2) + 1;
						StartCounter();
						myList.add(randIndex, i);
						wynik += GetCounter();
					}
				}
				cout << "Srednia dodawania w randomowym miejscu: " << wynik / (100 * 50) << endl;

				//dodawanie do koñcu
				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myList.generateList(size);
					for (int i = 0; i < 100; i++)
					{
						StartCounter();
						myList.addToEnd(i);
						wynik += GetCounter();
					}
				}
				cout << "Srednia dodawania na ostatnim miejscu: " << wynik / (100 * 50) << endl;

				//-------USUWANIE----------------USUWANIE----------------USUWANIE----------------USUWANIE----------------
				//usuwanie z poczatku
				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myList.generateList(size);
					for (int i = 0; i < 100; i++)
					{
						StartCounter();
						myList.removeFromBeggin();
						wynik += GetCounter();
					}
				}
				cout << "Srednia usuwania z poczatku: " << wynik / (100 * 50) << endl;

				//usuwanie w randomowym miejscu
				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myList.generateList(size);
					for (int i = 0; i < 100; i++)
					{
						int randValue = rand() % 50000 - 25000;
						StartCounter();
						myList.remove(randValue);
						wynik += GetCounter();
					}
				}
				cout << "Srednia usuwania po randomowej wartosci: " << wynik / (100 * 50) << endl;

				//usuwanie do koñcu
				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myList.generateList(size);
					for (int i = 0; i < 100; i++)
					{
						StartCounter();
						myList.removeFromEnd();
						wynik += GetCounter();
					}
				}
				cout << "Srednia usuwania z ostaniego miejsca: " << wynik / (100 * 50) << endl;

				//-------------WYSZUKIWANIE------------------------

				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myList.generateList(size);
					for (int i = 0; i < 100; i++)
					{
						int randValue = rand() % 50000 - 25000;
						StartCounter();
						myList.IsValueInList(randValue);
						wynik += GetCounter();
					}
				}
				cout << "Srednia wyszukania wartosci: " << wynik / (100 * 50) << endl << endl;
			}
			break;
		}

	} while (opt != '0');
}

class MyHeap {

public:
	int*wsk;
	int count;

	MyHeap()
	{
		count = 0;
		wsk = new int[count];
	}

	~MyHeap()
	{
		count = 0;
		delete[]wsk;
	}

	void loadFromFile(string fileName)
	{
		fstream file;
		int size;
		file.open(fileName + ".txt", ios::in);
		file >> size;
		if (count > 0)
		{
			count = 0;
			delete[]wsk;
		}
		wsk = new int[size + 100000];
		for (int i = 0; i < size; i++)
		{
			file >> wsk[i];
			push(wsk[i]);
		}
		file.close();
	}

	bool IsValueInHeap(int value)
	{
		bool isThere = false;
		for (int i = 0; i < count; i++)
		{
			if (value == wsk[i])
			{
				isThere = true;
				break;
			}
		}
		return isThere;
	}

	void generateHeap(int size)
	{
		if (size > 0)
		{
			if (count > 0)
			{
				count = 0;
				delete[]wsk;
			}
			wsk = new int[size + 100000];
			for (int i = 0; i < size; i++)
			{
				wsk[i] = (rand() % 100 +1);
				push(wsk[i]);
			}
		}
	}

	void push(int value)
	{
		count++;
		wsk[count - 1] = value;
		int indexOfSon, indexOfFather;
		indexOfSon = count - 1; 
		indexOfFather = (indexOfSon - 1) / 2; 

		while (indexOfSon > 0 && wsk[indexOfFather] < value)
		{
			wsk[indexOfSon] = wsk[indexOfFather];
			indexOfSon = indexOfFather;
			indexOfFather = (indexOfSon - 1) / 2;
		}
		wsk[indexOfSon] = value;
	}

	void pop(int value)
	{
		int index = 0;
		for (index; index < count; index++)
		{
			if (wsk[index] == value)
				break;
		}

		if (index == count - 1)
		{
			count--;
			wsk[count] = NULL;
		}

		if (index < count - 1)
		{
			wsk[index] = wsk[count - 1];
			wsk[count - 1] = NULL;
			count--;

			//if (2 * index > count - 1)
			if(wsk[index] > wsk[(index-1)/2])
			{
				while (index > 0 && wsk[index] > wsk[(index-1)/2])
				{
					if (wsk[index] > wsk[(index - 1) / 2])
					{
						int bufor = wsk[(index - 1) / 2];
						wsk[(index - 1) / 2] = wsk[index];
						wsk[index] = bufor;
					}
					index = (index - 1) / 2;
				}
			}
			else
			{
				while (index < count - 1 / 2)
				{
					if ((wsk[index] < wsk[2 * index + 1]) || (wsk[index] < wsk[2 * index + 2]))
					{
						if (wsk[2 * index + 1] > wsk[2 * index + 2])
						{
							int bufor = wsk[index];
							wsk[index] = wsk[2 * index + 1];
							wsk[2 * index + 1] = bufor;
							index = 2 * index + 1;
						}
						else
						{
							int bufor = wsk[index];
							wsk[index] = wsk[2 * index + 2];
							wsk[2 * index + 2] = bufor;
							index = 2 * index + 2;
						}
					}
					else break;
				}
			}	
		}
	}

	void display()
	{
		if (count < 1) cout << "Lista jest pusta, nie mozna jej wyswietlic." << endl;
		else
		{
			for (int i = 0; i < count; i++)
			{
				cout << wsk[i] << " ";
			}
		}
	}

	void display(string sp, string sn, int value)
	{
		string s;
		string cr, cl, cp;
		cr = cl = cp = "  ";
		cr[0] = 218; cr[1] = 196;
		cl[0] = 192; cl[1] = 196;
		cp[0] = 179;
		if (value < count)
		{
			s = sp;
			if (sn == cr) s[s.length() - 2] = ' ';
			display(s + cp, cr, 2 * value + 2);

			s = s.substr(0, sp.length() - 2);

			cout << s << sn << wsk[value] << endl;

			s = sp;
			if (sn == cl) s[s.length() - 2] = ' ';
			display(s + cp, cl, 2 * value + 1);
		}
	}
};

MyHeap myHeap;

void menu_heap()
{
	srand(time(NULL));
	char opt;
	string fileName;
	int index, value;

	do {
		displayMenu("--- KOPIEC BINARNY ---");
		opt = _getche();
		cout << endl;
		switch (opt) {
		case '1': //tutaj wczytytwanie kopca z pliku
			cout << "Podaj nazwe zbioru: ";
			cin >> fileName;
			myHeap.loadFromFile(fileName);
			myHeap.display("", "", 0);
			break;
			
		case '2': //tutaj usuwanie elemenu z kopca
			cout << "Podaj wartosc: ";
			cin >> value;
			myHeap.pop(value);
			myHeap.display("", "", 0);
			break;

		case '3': //tutaj dodawanie elemetu do kopca
			cout << "Podaj wartosc: ";
			cin >> value;
			myHeap.push(value);
			myHeap.display("", "", 0);
			break;
	
		case '4': //tutaj znajdowanie elemetu w tablicy
			cout << "Podaj wartosc: ";
			cin >> value;
			if (myHeap.IsValueInHeap(value))
				cout << "Podana wartosc znajduje sie w kopcu binarnym.";
			else
				cout << "Podana wartosc NIE znajduje sie w kopcu binarnym.";
			break;

		case '5':  //tutaj generowanie kopca
			cout << "Podaj ilosc elementow kopca: ";
			cin >> value;
			if (value <= 0)
				cout << "Podaj poprawny rozmiar tablicy." << endl;
			else
				myHeap.generateHeap(value);
			myHeap.display("", "", 0);
			break;
			
		case '6': //tutaj wyœwietlanie kopca
			myHeap.display("","",0);
			myHeap.display();
			break;
			
		case '7': //tutaj nasza funkcja do eksperymentów (pomiary czasów i generowanie daneych) - nie bêdzie testowana przez prowadz¹cego 
				  // mo¿na sobie tu dodaæ w³asne case'y

			int tab[] = { 1000,5000,10000,20000,50000 };
			for (int i = 0; i < 5; i++)
			{
				int size = tab[i];
				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myHeap.generateHeap(size);
					for (int k = 0; k < 100; k++)
					{
						int randValue = rand() % 50000 - 25000;
						StartCounter();
						myHeap.push(randValue);
						wynik += GetCounter();
					}
				}
				cout << "Srednia wstawiania: " << wynik / (100 * 50) << endl;

				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myHeap.generateHeap(size);
					for (int k = 0; k < 100; k++)
					{
						int randValue = rand() % 50000 - 25000;
						StartCounter();
						myHeap.pop(randValue);
						wynik += GetCounter();
					}
				}
				cout << "Srednia usuwania: " << wynik / (100 * 50) << endl;

				wynik = 0;
				for (int j = 0; j < 50; j++)
				{
					myHeap.generateHeap(size);
					for (int k = 0; k < 100; k++)
					{
						int randValue = rand() % 50000 - 25000;
						StartCounter();
						myHeap.IsValueInHeap(randValue);
						wynik += GetCounter();
					}
				}
				cout << "Srednia wyszukiwania: " << wynik / (100 * 50) << endl << endl;			
			}
			break;
		}

	} while (opt != '0');
}

int main(int argc, char* argv[])
{
	char option;
	do {
		cout << endl;
		cout << "==== MENU GLOWNE ===" << endl;
		cout << "1.Tablica" << endl;
		cout << "2.Lista" << endl;
		cout << "3.Kopiec" << endl;
		cout << "0.Wyjscie" << endl;
		cout << "Podaj opcje:";
		option = _getche();
		cout << endl;

		switch (option) {
		case '1':
			menu_table();
			break;

		case '2':
			menu_list();
			break;

		case '3':
			menu_heap();
			break;
		}

	} while (option != '0');

	return 0;
}

