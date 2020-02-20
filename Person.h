#pragma once

#include <string>
#include <optional>
#include <iostream>

using namespace std;
class Person
{
	friend ostream& operator<<(ostream& out, const Person& person) {
		return out << person.fname << " "<< person.lname << " " << person.email << " " << person.number << " " << person.city << " " << person.relative;
	}
	friend istream& operator>>(istream& is, Person& person) {
		return is >> person.fname >> person.lname >> person.email >> person.number >> person.city >> person.relative;
	}

private:
	string fname, lname, email, number, city;
	unsigned int relative; // Treating relative as a group ID.
	bool written;

public:
	Person(const string fname_ = "", const string lname_ = "", const string email_ = "", const string number_ = "", const string city_ = "") :
		fname{ fname_ }, lname{ lname_ }, email{ email_ }, number{ number_ }, city{ city_ }
	{
		written = false;
		relative = 0;
	};

	const string& get_fname() const {
		return fname;
	}
	const string& get_lname() const {
		return lname;
	}
	const string& get_city() const {
		return city;
	}

	void set_relative(const unsigned int relative_) {
		relative = relative_;
	}

	unsigned int get_relative() {
		return relative;
	}

	void set_written(const bool written_) {
		written = written_;
	}

	const bool is_written() const {
		return written;
	}

	static optional <Person> new_record() {
		Person person;
		cout << "=== Creating new record ===\nEnter quit at any time to quit." << endl;
		cout << "Enter person's first name: "; getline(cin, person.fname); if (person.fname == "quit") return nullopt;
		cout << "Enter person's last name: "; getline(cin, person.lname); if (person.lname == "quit") return nullopt;
		cout << "Enter person's email: "; getline(cin, person.email); if (person.email == "quit") return nullopt;
		cout << "Enter person's phone number: "; getline(cin, person.number); if (person.number == "quit") return nullopt;
		cout << "Enter person's city: "; getline(cin, person.city); if (person.city == "quit") return nullopt;
		return person;
	}
};

