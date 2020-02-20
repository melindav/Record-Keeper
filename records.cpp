#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "Person.h"

using namespace std;
using Relatives = vector<Person>;

constexpr char filename[] = "record.txt";
enum class Selection { INIT = 1, SAVE, LOAD, NEW, DELETE, SEARCH_CITY,SEARCH_NAME, REPORT, QUIT };

int main()
{
	vector<Relatives> phonebook;
	string input;
	fstream fs;
	unsigned int relative_id = 0;
	Selection selection = Selection::INIT;
	bool read_from_file = false;

	fs.open(filename, fstream::in | fstream::out | fstream::app);
	if (fs.is_open()) {
		if (fs.peek() == ifstream::traits_type::eof())
			read_from_file = true; //Because the file is empty.	
		fs.close();
	}
	

	while (selection != Selection::QUIT) {
		do {
			cout << "=== Record Keeper ===\n1. Initialise\n2. Save to disk\n3. Read from disk" << endl <<
				"4. Add new person\n5. Remove a person\n6. Find people in a city\n7. Find people by name\n8. Print all contract info\n9. Exit program" << std::endl <<
				"Input: ";
			getline(cin, input);
		} while (input.find_first_of("123456789") && cout << "Invalid input." << endl);

		switch (selection = static_cast<Selection>(input[0] - '0')) {
		case Selection::INIT:
			do {
				cout << "This will clear all records in memory and in file.\nAre you sure you wish to continue (y/n)? ";
				getline(cin, input);
			} while (input.find_first_of("yn") && cout << "Invalid input." << endl);

			switch (input[0]) {
			case 'y':
				cout << "Clearing all records..." << endl;
				phonebook.clear();
				fs.open(filename, ofstream::out | ofstream::trunc);
				fs.close();
				break;

			case 'n':
				cout << "Returning to main menu." << endl;
				break;

			default:
				cout << "Invalid input." << endl;
				break;
			}
			break;

		case Selection::SAVE:
			// Saves everybody to file who hasn't already been saved.
			fs.open(filename, fstream::in | fstream::out | fstream::app);
			if (fs.is_open()) {
				for (auto& relatives : phonebook)
					for (auto& person : relatives)
						if (!person.is_written()) {
							person.set_written(true);
							fs << person << endl;
						}
				fs.close();
			}
			else
				cout << "Could not open file." << endl;
			break;

		case Selection::LOAD:
			fs.close();
			fs.open(filename, std::fstream::in);
			if (fs.is_open()) {
				phonebook.erase(remove_if(phonebook.begin(), phonebook.end(), [](auto& relatives) {
					relatives.erase(remove_if(relatives.begin(), relatives.end(), [](auto& person) {
						return person.is_written();
						}), relatives.end());
					return relatives.empty();
					}), phonebook.end());

				Person person;
				while (fs >> person) {
					person.set_written(true);

					if (person.get_relative() >= relative_id)
						relative_id = person.get_relative() + 1;

					auto relatives_iter = find_if(phonebook.begin(), phonebook.end(), [&person](auto& relatives) {
						return relatives.front().get_relative() == person.get_relative();
						});

					if (relatives_iter != phonebook.end())
						relatives_iter->push_back(person);
					else
						phonebook.emplace_back(Relatives{ person });
				}
				fs.close();
				read_from_file = true;
			}
			break;

		case Selection::NEW:
			if (!read_from_file && cout << "Please load phonebook from file (choose 3) before adding new persons." << endl)
				break;

			if (auto person = Person::new_record()) {
				string relative_name;
				cout << "If this person has a relative already recorded in the phone book, enter their name now.\n"
					"If not, just type quit.\nInput: ";
				getline(cin, relative_name);

				if (relative_name == "quit") {
					person.value().set_relative(relative_id++);
					phonebook.emplace_back(Relatives{ person.value() });
				}
				else {
					auto relatives_iter = find_if(phonebook.begin(), phonebook.end(), [&relative_name, &input](auto& relatives) {
						return relatives.end() != find_if(relatives.begin(), relatives.end(), [&relative_name, &input](auto& relative) {
							if (relative.get_fname() == relative_name || relative.get_lname() == relative_name) {
								do {
									cout << relative << endl << "Is this the person you meant (y/n)? ";
									getline(cin, input);
								} while (input.find_first_of("yn") && cout << "Invalid input." << endl);

								return input[0] == 'y';
							}
							});
						});

					if (relatives_iter != phonebook.end()) {
						person.value().set_relative(relatives_iter->front().get_relative());
						relatives_iter->push_back(person.value());
					}
					else {
						cout << "No relatives were found." << endl;
						person.value().set_relative(relative_id++);
						phonebook.emplace_back(Relatives{ person.value() });
					}
				}
			}
			break;

		case Selection::DELETE:
			cout << "Enter full name of person to remove from record: ";
			getline(cin, input);

			phonebook.erase(remove_if(phonebook.begin(), phonebook.end(), [&input](auto& relatives) {
				relatives.erase(remove_if(relatives.begin(), relatives.end(), [&input](auto& person) {
					return (input == person.get_fname() + " " + person.get_lname()) && cout << input << " removed." << endl;
					}), relatives.end());
				return relatives.empty();
				}), phonebook.end());
			break;

		case Selection::SEARCH_CITY:
			cout << "Enter city name to search for: ";
			getline(cin, input);
			for (auto& relatives : phonebook)
				for (auto& person : relatives)
					if (person.get_city() == input)
						cout << person << endl;
			break;

		case Selection::SEARCH_NAME:
			cout << "Enter the name of the person search for: ";
			getline(cin, input);
			for (auto& relatives : phonebook)
				for (auto& person : relatives)
					if (person.get_fname() == input || person.get_lname() == input)
						cout << person << endl;
			break;

		case Selection::REPORT:
			cout << "Printing all records in phonebook. Relatives are separated by newline." << endl;
			for (auto& relatives : phonebook) {
				for (auto& person : relatives)
					cout << person << endl;
				cout << endl;
			}
			break;

		case Selection::QUIT:
			cout << "Exiting program." << endl;
			break;

		default:
			cout << "Invalid input." << endl;
			break;
		}
	}
}