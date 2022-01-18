#include "Main.h"
class Admin {
private:
	json admin_info{};
	static bool isAuth;
public:
	Admin() { // �����������
		ifstream file(admin_file);
		if (!file.is_open()) // �������� �� ������� �����
			OpenFileError(admin_file);
		else if (file.peek() == EOF) { // �������� �� ������� �������� ������
			cout << "\n������� �������������� �� ������! \n\t�����������\n";
			string login, password;
			cout << "\n�������: ";
			cout << "\n\t�����: ";
			getline(cin, login);
			cout << "\n\t������: ";
			getline(cin, password);
			admin_info.push_back({
				{"login", Crypt(login)},
				{"password", Crypt(password)} });
			Save();
			system("cls");
		}
		else { // �������� ������� ������
			string str, temp;
			while (getline(file, str))
				temp += str;
			if (!temp.empty())
				admin_info = json::parse(temp);
		}
		file.close();
	}
	void Save() { // ���������� ������ � ����
		ofstream file_write(admin_file);
		if (!file_write.is_open())
			OpenFileError(admin_file);
		else
			file_write << admin_info.dump(2);
		file_write.close();
	}
	bool Check(string login, string password) { // �������� ���������� ������ � ������
		if (admin_info[0]["login"] == Crypt(login) && admin_info[0]["password"] == Crypt(password))
			return 1;
		else
			return 0;
	}
	void Login() { // �����������
		system("cls");
		ifstream file(admin_file);
		if (!file.is_open()) // �������� �� ������� �����
			OpenFileError(admin_file);
		else if (!isAuth) {
			string login, password;
			cout << "\n\t����������� ��������������\n";
			cout << "\n\t�����: ";
			getline(cin, login);
			cout << "\n\t������: ";
			getline(cin, password);
			system("cls");
			if (Check(login, password)) {
				SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
				message = "\n\t�������� �����������!\n";
				isAuth = true;
				status = "�������������";
			}
			else {
				SetConsoleTextAttribute(handle, FOREGROUND_RED);
				message = "\n\t����������� �� ��������!\n";
			}
		}
		else {
			message = "\n\t�� ��� ������������!";
			SetConsoleTextAttribute(handle, 33);
		}
		file.close();
	}
	void Change() { // ����� ������
		system("cls");
		if (!isAuth)
			message = "\n\t��������� �������������� ��� ����� ������\n";
		else {
			string login, password;
			cout << "\n\t��������� ��������������\n";
			cout << "\n������� �����: ";
			cout << "\n\t�����: ";
			getline(cin, login);
			cout << "\n\t������: ";
			getline(cin, password);
			admin_info[0] = {
				{"login", Crypt(login)},
				{"password", Crypt(password)} };
			Save();
			system("cls");
		}
	}
	void LogOut() { // ����� 
		status = "�� �����������";
		isAuth = false;
		message = {};
		system("cls");
	}
	void UsersControlPanel() { // ������ ���������� ��������������
		int choice;
		User u;
		system("cls");
		cout << "\n 1. ���������������� ������������";
		cout << "\n 2. ������� ������������";
		cout << "\n 3. �������� ������������";
		cout << "\n >>> ";
		cin >> choice;
		cin.ignore();
		switch (choice) {
		case 1: {
			u.RegisterUser();
		}
			  break;
		case 2: {
			string login;
			cout << "������� ����� ������������: ";
			getline(cin, login);
			u.DeleteUser(login);
		}
			  break;
		case 3: {
			string login;
			cout << "������� ����� ������������: ";
			getline(cin, login);
			u.Edit(login);
		}
		}
		u.Save();
	}
	void TestControl() { // ���������� ������
		json temp_tests = ReadTests(tests_file);
		system("cls");
		cout << "\n\t�����\n\n";
		//����� ��������� � ������
		json test = ReadTests(tests_file);
		vector<string> categories;
		int cat;
		for (const auto& n : test.items()) {
			categories.push_back(n.key());
			cout << "\t" << categories.size() << ". " << categories[categories.size() - 1] << endl;
			for (const auto& l : test[n.key()].items())
				cout << "\t\t" << l.key() << endl;
		}
		//����� ��������� � ������
		cout << "\n 1. �������� ���������";
		cout << "\n 2. �������� ���� � ������������ ���������";
		cout << "\n >>> ";
		int choice;
		cin >> choice;
		cin.ignore();
		switch (choice) {
		case 1: {
			string category;
			cout << "\n������� �������� ���������: ";
			getline(cin, category);
			temp_tests[category];
		}
			  break;
		case 2: {
			string answers[3], question, test_name;
			int que_num, cat_ind;
			cout << "\n����� ���������� �� ����������!";
			cout << "\n������� ����� ���������(�� " << categories.size() << ", 0 - ������): ";
			cin >> cat_ind;
			cin.ignore();
			if (cat_ind != 0 || (cat_ind > 0 && cat_ind <= categories.size())) {
				cout << "\n������� �������� �����: ";
				getline(cin, test_name);
				cout << "\n������� ���������� ��������: ";
				cin >> que_num;
				cin.ignore();
				for (int i = 0; i < que_num; i++) {
					cout << "\n������� ������: ";
					getline(cin, question);
					cout << "������� 3 �������� ������: \n";
					for (int i = 0; i < 3; i++) {
						cout << i + 1 << ". ";
						getline(cin, answers[i]);
					}
					int right_answer;
					do {
						cout << "������� ����� ����������� ������: ";
						cin >> right_answer;
						cin.ignore();
					} while (right_answer < 1 || right_answer > 3);
					temp_tests[categories[cat_ind - 1]][test_name].push_back({
						{"question", question},
						{"answers", answers},
						{"right answer", answers[right_answer - 1]}
						});
				}

			}
		}
			  break;
		}
		ofstream write(tests_file);
		write << temp_tests.dump(2);
		write.close();
	}
	static bool auth_pos() { return isAuth; } // ������ ����������� ��������������
	void ShowStatistic() { // ���������� �� �������������
		system("cls");
		User users;
		json tests = ReadTests(tests_file);
		json user_base = users.GetUsers();
		for (const auto& categories : tests.items()) {
			cout << endl << categories.key() << ": " << endl;
			for (const auto& bot_categories : tests[categories.key()].items()) {
				cout << "\t" << bot_categories.key() << ": ";
				for (int i = 0; i < user_base.size(); i++) {
					if (user_base[i].contains("tests"))
						if (user_base[i]["tests"].contains(categories.key()))
							if (user_base[i]["tests"][categories.key()].contains(bot_categories.key())) {
								cout << "\n\t\t\t������������ ";
								SetConsoleTextAttribute(handle, 14);
								cout << (string)user_base[i]["name"];
								SetConsoleTextAttribute(handle, 15);
								cout << ":";
								if (user_base[i]["tests"][categories.key()][bot_categories.key()]["last question"] == user_base[i]["tests"][categories.key()][bot_categories.key()]["num questions"]) {
									double right = user_base[i]["tests"][categories.key()][bot_categories.key()]["right answers"];
									double nums = user_base[i]["tests"][categories.key()][bot_categories.key()]["num questions"];
									cout << "\n\t\t\t\t\t\t��������: ";
									SetConsoleTextAttribute(handle, 14);
									cout << nums;
									SetConsoleTextAttribute(handle, 15);
									cout << "\n\t\t\t\t\t\t���������� �������: ";
									SetConsoleTextAttribute(handle, 14);
									cout << right << "(" << (right / nums) * 100 << "%)";
									SetConsoleTextAttribute(handle, 15);
									cout << "\n\t\t\t\t\t\t������: ";
									SetConsoleTextAttribute(handle, 14);
									cout << user_base[i]["tests"][categories.key()][bot_categories.key()]["mark"];
									SetConsoleTextAttribute(handle, 15);
								}
								else cout << "\t�� ���������";
							}
				}
			}
		}
		cout << endl;
		system("pause");
	}
};