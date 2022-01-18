#include "Main.h"
class User {
private:
	json users_info{};
	static bool isAuth;
	static int user_id;
public:
	User() { // �����������
		ifstream file(users_file);
		if (!file.is_open()) // �������� �� ������� �����
			OpenFileError(users_file);
		else if (file.peek() == EOF) { // �������� �� ������� ���� �� ������ �������� ������������
			cout << "\n�� ������� �� ������ ������������!\n\t�����������\n";
			RegisterUser();
			system("cls");
		}
		else { // �������� ������� ������
			string str, temp;
			while (getline(file, str))
				temp += str;
			if (!temp.empty())
				users_info = json::parse(temp);
		}
		file.close();
	}
	void Login() { // ����������� ������������
		system("cls");
		ifstream file(users_file);
		if (!file.is_open()) // �������� �� ������� �����
			OpenFileError(users_file);
		else if (!isAuth) {
			string login, password;
			cout << "\n\t����������� ������������\n";
			cout << "\n\t�����: ";
			getline(cin, login);
			cout << "\n\t������: ";
			getline(cin, password);
			system("cls");
			user_id = Check(login, password);
			if (user_id != -1) {
				SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
				message = "\n\t�������� �����������!\n";
				isAuth = true;
				status = "������������ " + string(users_info[user_id]["name"]);
			}
			else {
				SetConsoleTextAttribute(handle, FOREGROUND_RED);
				message = "\n\t����������� �� ��������!\n";
			}
		}
		else {
			SetConsoleTextAttribute(handle, 33);
			message = "\n\t�� ��� ������������!";
		}
		file.close();
	}
	void RegisterUser() { // ����������� ������������
		string login, password, FIO, adress, telephone;
		cout << "\n�������: ";
		cout << "\n\t�����: ";
		getline(cin, login);
		cout << "\n\t������: ";
		getline(cin, password);
		cout << "\n\t���: ";
		getline(cin, FIO);
		cout << "\n\t�������� �����: ";
		getline(cin, adress);
		cout << "\n\t�������: ";
		getline(cin, telephone);
		bool is_here = true;
		for (int i = 0; i < users_info.size(); i++)
			if (users_info[i]["login"] == Crypt(login))
				is_here = false;
		if (is_here) {
			users_info.push_back({
				{"login", Crypt(login)},
				{"password", Crypt(password)},
				{"name", FIO},
				{"adress", adress},
				{"telephone", telephone} });
			Save();
		}
		else {
			SetConsoleTextAttribute(handle, FOREGROUND_RED);
			message = "\n������������ � ����� ������� ����������!\n";
		}
	}
	void DeleteUser(string login) { // �������� ������������
		for (size_t i = 0; i < users_info.size(); i++)
			if (users_info[i]["login"] == Crypt(login))
				users_info.erase(i);
	}
	int Check(string login, string password) { // �������� ���������� ������ � ������
		for (size_t i = 0; i < users_info.size(); i++)
			if (users_info[i]["login"] == Crypt(login) && users_info[i]["password"] == Crypt(password))
				return i;
		return -1;
	}
	void Edit(string login) { // ��������� ������������(������ �������� � ����������)
		DeleteUser(login);
		RegisterUser();
	}
	void LogOut() { // ����� 
		status = "�� �����������";
		isAuth = false;
		user_id = -1;
		message = {};
		system("cls");
	}
	void Save() { // ���������� ���� ������������� � ����
		ofstream file_write(users_file);
		if (!file_write.is_open())
			OpenFileError(users_file);
		else
			file_write << users_info.dump(2);
		file_write.close();
	}
	static bool auth_pos() { return isAuth; } // ������ ����������� ������������
	void Tests() { // ������������ ������������
		system("cls");
		json test = ReadTests(tests_file);
		vector<string> categories;
		int cat;
		for (const auto& n : test.items()) {
			categories.push_back(n.key());
			cout << "\t" << categories.size() << ". " << categories[categories.size() - 1] << endl;
			for (const auto& l : test[n.key()].items())
				cout << "\t\t" << l.key() << endl;
		}
		vector<string> bot_categories;
		cout << "\n�������� ���������: ";
		cin >> cat;
		cin.ignore();
		system("cls");
		if (cat > 0 && cat <= categories.size()) {
			cout << endl << categories[cat - 1] << endl;
			for (const auto& l : test[categories[cat - 1]].items()) {
				bot_categories.push_back(l.key());
				cout << "\t" << bot_categories.size() << ". " << bot_categories[bot_categories.size() - 1] << endl;
			}
			int bot_cat;
			cout << "\n�������� ����: ";
			cin >> bot_cat;
			cin.ignore();
			if (bot_cat > 0 && bot_cat <= bot_categories.size()) {
				double count = 0;
				int begin = 0;
				int my_answer;
				if (users_info[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]].contains("last question"))
					if (users_info[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["last question"] != users_info[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["num questions"]) {
						count = users_info[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["right answers"];
						begin = users_info[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["last question"];
					}
				for (int i = begin; i < test[categories[cat - 1]][bot_categories[bot_cat - 1]].size(); i++) {
					do {
						system("cls");
						cout << categories[cat - 1] << " >>> " << bot_categories[bot_cat - 1] << endl;
						cout << i + 1 << ". " << string(test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["question"]).c_str() << endl;
						cout << "\t1. " << string(test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["answers"][0]).c_str() << endl;
						cout << "\t2. " << string(test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["answers"][1]).c_str() << endl;
						cout << "\t3. " << string(test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["answers"][2]).c_str() << endl;
						cout << " >>> ";
						cin >> my_answer;
						cin.ignore();
						if (my_answer == -1)
							break;
					} while (my_answer < 1 || my_answer > 3);
					users_info[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["last question"] = i;
					if (my_answer == -1)
						break;
					else
						if (test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["answers"][my_answer - 1] == test[categories[cat - 1]][bot_categories[bot_cat - 1]][i]["right answer"])
							count++;
				}
				if (my_answer != -1)
					users_info[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["last question"] = users_info[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["last question"] + 1;
				users_info[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["num questions"] = test[categories[cat - 1]][bot_categories[bot_cat - 1]].size();
				users_info[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["right answers"] = (int)count;
				users_info[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["mark"] = (int)(12 * (double)(count / test[categories[cat - 1]][bot_categories[bot_cat - 1]].size()));
				Save();
				if (users_info[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["last question"] == users_info[user_id]["tests"][categories[cat - 1]][bot_categories[bot_cat - 1]]["num questions"]) {
					cout << "\n��������: " << test[categories[cat - 1]][bot_categories[bot_cat - 1]].size();
					cout << "\n���������� �������: " << count << "(" << (double)(count / test[categories[cat - 1]][bot_categories[bot_cat - 1]].size()) * 100 << "%)";
					cout << "\n������: " << 12 * (double)(count / test[categories[cat - 1]][bot_categories[bot_cat - 1]].size()) << endl;
					system("pause");
				}
			}
		}
	}
	void ShowStatistic() { // ���������� ����������� ������������
		if (users_info[user_id].contains("tests")) {
			for (const auto& categories : users_info[user_id]["tests"].items()) {
				cout << endl << categories.key() << ": " << endl;
				for (const auto& bot_categories : users_info[user_id]["tests"][categories.key()].items()) {
					cout << "\t" << bot_categories.key() << ": ";
					if (users_info[user_id]["tests"][categories.key()][bot_categories.key()]["last question"] == users_info[user_id]["tests"][categories.key()][bot_categories.key()]["num questions"]) {
						double right = users_info[user_id]["tests"][categories.key()][bot_categories.key()]["right answers"];
						double nums = users_info[user_id]["tests"][categories.key()][bot_categories.key()]["num questions"];
						cout << "\n\t\t\t��������: ";
						SetConsoleTextAttribute(handle, 14);
						cout << nums;
						SetConsoleTextAttribute(handle, 15);
						cout << "\n\t\t\t���������� �������: ";
						SetConsoleTextAttribute(handle, 14);
						cout << right << "(" << (right / nums) * 100 << "%)";
						SetConsoleTextAttribute(handle, 15);
						cout << "\n\t\t\t������: ";
						SetConsoleTextAttribute(handle, 14);
						cout << users_info[user_id]["tests"][categories.key()][bot_categories.key()]["mark"];
						SetConsoleTextAttribute(handle, 15);
					}
					else
						cout << "�� ���������" << endl;
				}
			}
		}
		cout << endl;
		system("pause");
	}
	json GetUsers() { return users_info; } // ��������� ���� �������������
};