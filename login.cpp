#include <fstream>
#include "HashTable.cpp"

class Login {
public:
	Login(string u = "", string p = "") {
		username = u;
		password = p;
		key = username + password;
	}
	string getKey() const {
		return key;
	}
	static unsigned int hash(const string& str) {
		unsigned int val = 0;
		for (unsigned int i = 0; i < str.length(); ++i) {
			val += str[i];
		}
		return val;
	}
private:
	string username;
	string password;
	string key;
};

int main() {
	HashTable<Login, string> accounts(10);
	Login login;
	string username;
	string password;
	ifstream fileIn;

	fileIn.open("password.dat");
	while (fileIn >> username >> password) {
		login = Login(username, password);
		accounts.insert(login);
	}
	fileIn.close();

	bool running = true;
	while (running) {
		cout << "Please log into your account, or enter 0" << endl
			<< "as your username to quit the program." << endl << endl
			<< "Username: ";
		cin >> username;
		if (username == "0") {
			running = false;
			continue;
		}
		cout << "Password: ";
		cin >> password;
		cout << endl;
		if (accounts.retrieve(username + password, login)) {
			cout << "Authentication successful";
		}
		else
			cout << "Authentication failure";
		cout << endl << endl;
	}
	return 0;
}