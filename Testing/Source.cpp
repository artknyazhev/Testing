#include "Main.h"

//---------------------- ВСПОМОГАТЕЛЬНЫЕ ПЕРЕМЕННЫЕ ----------------------
HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
string admin_file = "admin.json"; // Путь к логам админа
string users_file = "users.json"; // Путь к логам пользователей
string tests_file = "tests.json"; // Путь к тестам
int crypt_key = 15;				  // Ключ шифрования. После изменения ключа следует очистить файлы admin.json и users.json
string status = "Не авторизован"; // Статус пользователя
string message = {};			  // Сообщение
//---------------------- ВСПОМОГАТЕЛЬНЫЕ ПЕРЕМЕННЫЕ ----------------------

void OpenFileError(string _message) { // Вывод ошибки открытия файла
	SetConsoleTextAttribute(handle, FOREGROUND_RED);
	cerr << "Ошибка чтения файла \"" << _message << "\"" << endl;
	SetConsoleTextAttribute(handle, 15);
	system("pause");
}
string Crypt(string no_crypt_string) { // Шифрование строки
	for (int i = 0; i < no_crypt_string.length(); i++)
		no_crypt_string[i] = no_crypt_string[i] ^ crypt_key;
	return no_crypt_string;
}
int AdminChoice() { // Панель админа
	int choice;
	system("cls");
	cout << message;
	SetConsoleTextAttribute(handle, 14);
	cout << "\n\t" << status << endl;
	SetConsoleTextAttribute(handle, 15);
	cout << "\n 1. Управление пользователями";
	cout << "\n 2. Просмотр статистики";
	cout << "\n 3. Управление тестами";
	cout << "\n 4. Смена логина и пароля";
	cout << "\n 5. Выйти из учетой записи";
	cout << "\n >>> ";
	cin >> choice;
	cin.ignore();
	message = {};
	return choice;
}
int UserChoice() { // Панель пользователя
	int choice;
	system("cls");
	cout << message;
	SetConsoleTextAttribute(handle, 15);
	cout << "\n\t" << status << endl;
	cout << "\n 1. Просмотр результатов";
	cout << "\n 2. Пройти тест";
	cout << "\n 3. Выйти из учетой записи";
	cout << "\n >>> ";
	cin >> choice;
	cin.ignore();
	message = {};
	return choice;
}
int MainChoice() { // Основная панель
	int control_choice;
	system("cls");
	cout << message;
	SetConsoleTextAttribute(handle, 15);
	cout << "\n\t" << status << endl;
	cout << "\n Авторизоваться как: ";
	cout << "\n 1. Администратор";
	cout << "\n 2. Пользователь";
	cout << "\n >>> ";
	cin >> control_choice;
	cin.ignore();
	return control_choice;
}
json ReadTests(string dest) { // Чтение тестов из файла
	json temp_tests;
	ifstream file(dest);
	if (!file.is_open())
		OpenFileError(dest);
	else {
		string str, temp;
		while (getline(file, str))
			temp += str;
		if (!temp.empty())
			temp_tests = json::parse(temp);
	}
	file.close();
	return temp_tests;
}
#include "User.h"
#include "Admin.h"
//------------ Переменные авторизации ------------
bool Admin::isAuth = false;
bool User::isAuth = false;
int User::user_id = -1;
//------------ Переменные авторизации ------------

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int log_choice;
	do {
		log_choice = MainChoice();
		switch (log_choice) {
		case 1: { // Для админа
			Admin a;
			int choice;
			a.Login();
			if (Admin::auth_pos())
				do {
					choice = AdminChoice();
					switch (choice) {
					case 1:
						a.UsersControlPanel();
						break;
					case 2:
						a.ShowStatistic();
						break;
					case 3:
						a.TestControl();
						break;
					case 4:
						a.Change();
						break;
					case 5:
						a.LogOut();
						  break;
					default:
						system("cls");
						break;
					}
				} while (choice != 5);
		}
			  break;
		case 2: { // Для пользователя
			User u;
			int choice;
			u.Login();
			if (User::auth_pos())
				do {
					choice = UserChoice();
					switch (choice) {
					case 1:
						u.ShowStatistic();
						break;
					case 2:
						u.Tests();
						break;
					case 3:
						u.LogOut();
						break;
					}
				} while (choice != 3);
		}
			  break;
		default:
			system("cls");
			break;
		}
	} while (log_choice != 0);
}