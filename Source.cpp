#include <iostream>
#include <fstream>
#include <string>
#include "dependencies/include/nlohmann/json.hpp"
#include <filesystem>


class Converter
{
public:
	void To_Txt(std::string filename)
	{
		std::filesystem::path p(filename);
		filename.erase(filename.size() - 5, 5); // удаляем .json
		rename(p, std::string(filename) + ".txt");
	}
	void To_JSON(std::string filename)
	{
		std::filesystem::path p(filename);
		filename.erase(filename.size() - 4, 4); // удаляем .txt
		rename(p, std::string(filename) + ".json");
	}
};


class Checker
{
private:
	std::string temp;
	std::string text;
	std::string type_of_vertice;
	double count = 0;
	int counter_id = 0;
	int total_id = 0;
	int index = 0;
	int step = 0;
	int from = 0; //
	int to = 0; // 
	int j = 0;
	int k = 0; //
	int number = 1;
	double num_of_sol = 0; // количество решений
	int num_of_comp = 0; // количество знаков сравнения
	int num_of_equals = 0; // количество знаков равно
	int num_of_math_sign = 0; // количесвто математических действий
	int num_of_edges = 0;
	int num_of_from = 0;
	int num_of_sign = 0;
	int num_of_exclam = 0;
	int err = 0;
public:
	void Check_logic(nlohmann::json objJson)
	{
		if (objJson["Vertices"][0]["Content"] != "Start")
		{
			std::cout << "Ошибка в терминальном блоке Start\n" << std::endl;
			err++;
		}

		for (int i = 0; i < size(objJson["Edges"]); i++)
		{
			if (objJson["Edges"][i]["To"] == 1)
			{
				std::cout << "Ошибка в блоке 'To' под номером: " << i+1 << std::endl;
				std::cout << "Получено значение начальной вершины\n" << std::endl;
				err++;
			}

			if (objJson["Edges"][i]["From"] == (size(objJson["Edges"]) - 1))
			{
				std::cout << "Ошибка в блоке 'From' под номером: " << i+1 << std::endl;
				std::cout << "Получено значение конечной вершины\n" << std::endl;
				err++;
			}

			int from = objJson["Edges"][i]["From"];
			int to = objJson["Edges"][i]["To"];
			if (objJson["Edges"][i]["Type"] == 0 and objJson["Vertices"][from - 1]["Type"] != 3)
			{
				std::cout << "Ошибка в значении 'Type', должно быть 3, Id " << from << "\n" << std::endl;
				err++;
			}
		}

		for (int i = 0; i < size(objJson["Vertices"]); i++)
		{
			if (objJson["Vertices"][i]["Type"] == 3) //проверка блока решения
			{
				int id_of_sol = objJson["Vertices"][i]["Id"];
				int total_type = 0;
				num_of_sol += 1;
				for (int i = 0; i < size(objJson["Edges"]); i++)
				{
					if (objJson["Edges"][i]["From"] == id_of_sol and (objJson["Edges"][i]["Type"] == 0 or objJson["Edges"][i]["Type"] == 1))
					{
						total_type += objJson["Edges"][i]["Type"];
						count += 1;
					}
				}

				if (count/num_of_sol != 2.0)
				{
					std::cout << "Ошибка в количестве веток блока решения" << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					num_of_sol = 0;
					count = 0;
					err++;
				}
				if (total_type == 2) {
					std::cout << "Ошибка, два соединения Edges имеют Type = 1" << std::endl;
					std::cout << "Ошибка в From: " << i + 1 << "\n" << std::endl;
					err++;
				}
				if (total_type == 0) {
					std::cout << "Ошибка, два соединения Edges имеют Type = 0" << std::endl;
					std::cout << "Ошибка в From: " << i + 1 << "\n" << std::endl;
					err++;
				}
				std::string s = objJson["Vertices"][i]["Content"];
				for (int i = 0; i < s.size(); i++)
				{
					if (s[i] == *">" or s[i] == *"<")
					{
						num_of_comp += 1;
					}
					if (s[i] == *"=")
					{
						num_of_equals += 1;
					}
					if (s[i] == *"!")
					{
						num_of_exclam += 1;
					}
				}
				if (num_of_comp >= 2)
				{
					std::cout << "Ошибка в количестве знаков сравнения, получено: " << num_of_comp << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					err++;
				}
				if (num_of_exclam >= 2)
				{
					std::cout << "Ошибка в количестве знаков восклицания, получено: " << num_of_comp << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					err++;
				}
				if (num_of_equals >= 3)
				{
					std::cout << "Ошибка в количестве знаков равно, получено: " << num_of_equals << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					err++;
				}
				num_of_equals = 0; // обнуляем счетчики
				num_of_comp = 0;
				num_of_exclam = 0;
			}

			if (objJson["Vertices"][i]["Type"] == 2) // проверка блока процесса
			{
				std::string str = objJson["Vertices"][i]["Content"];
				for (int i = 0; i < str.size(); i++)
				{
					if (str[i] == *"=")
					{
						num_of_equals += 1;
					}
					if (str[i] == *"+" or str[i] == *"*" or str[i] == *"-" or str[i] == *"/" or (str[i] == *"s" and str[i+1] == *"q" and str[i+2] == *"r" and str[i+3] == *"t"))
					{
						num_of_math_sign += 1;
					}
				}
				if (num_of_math_sign > 1)
				{
					std::cout << "Ошибка в количестве математических действий, получено: " << num_of_math_sign << std::endl;
					std::cout << "Ошибка в Id: " << i+1 << "\n" << std::endl;
					err++;
				}
				if (num_of_equals > 1)
				{
					std::cout << "Ошибка в количестве знаков равно, получено: " << num_of_equals << std::endl;
					std::cout << "Ошибка в Id: " << i+1 << "\n" << std::endl;
					err++;
				}
				num_of_math_sign = 0; // обнуляем счетчики
				num_of_equals = 0;
			}

			if (objJson["Vertices"][i]["Content"] == "Start")
			{
				if (objJson["Vertices"][i]["Type"] != 0)
				{
					std::cout << "Ошибка в значении Type блока 'Start'" << "\n" << std::endl;
					err++;
				}
			}

			if (objJson["Vertices"][i]["Content"] == "End")
			{
				if (objJson["Vertices"][i]["Type"] != 1)
				{
					std::cout << "Ошибка в значении Type блока 'End'" << "\n" << std::endl;
					err++;
				}
			}

			if (objJson["Vertices"][i]["Type"] == 0)
			{
				if (objJson["Vertices"][i]["Content"] != "Start")
				{
					std::cout << "Ошибка, получен 0 в значении Type, Id " << objJson["Vertices"][i]["Id"] << "\n" << std::endl;
					err++;
				}
			}

			if (objJson["Vertices"][i]["Type"] == 1)
			{
				if (objJson["Vertices"][i]["Content"] != "End")
				{
					std::cout << "Ошибка, получена 1 в значении Type, Id " << objJson["Vertices"][i]["Id"] << "\n" << std::endl;
					err++;
				}
			}

			if (objJson["Vertices"][i]["Type"] == 4 or objJson["Vertices"][i]["Type"] == 5) //проверка блока данных
			{
				std::string str = objJson["Vertices"][i]["Content"];
				for (int i = 0; i < str.size(); i++)
				{
					if (str[i] == *"=")
					{
						num_of_equals += 1;
					}
					if (str[i] == *"+" or str[i] == *"*" or str[i] == *"/" or (str[i] == *"s" and str[i + 1] == *"q" and str[i + 2] == *"r" and str[i + 3] == *"t"))
					{
						num_of_math_sign += 1;
					}
					if (str[i] == *";" or str[i] == *"'" or str[i] == *":")
					{
						num_of_sign += 1;
					}
				}
				if (num_of_math_sign > 0)
				{
					std::cout << "Ошибка, найдены математические действия в блоке данных" << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					err++;
				}
				if (num_of_equals > 0)
				{
					std::cout << "Ошибка, найдены знаки равно в блоке данных" << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					err++;
				}
				if (num_of_sign > 0)
				{
					std::cout << "Ошибка, найдены недопустимые знаки в блоке данных" << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					err++;
				}
				num_of_math_sign = 0; // обнуляем счетчики
				num_of_equals = 0;
				num_of_sign = 0;
			}
		}

		if (err == 0) {
			std::cout << "Ошибок нет" << std::endl;
		}
	}
	void Check_terminal_blocks(nlohmann::json objJson)
	{
		if (objJson["Vertices"][0]["Content"] != "Start")
		{
			std::cout << "Ошибка в терминальном блоке Start\n" << std::endl;
			err++;
		}

		for (int i = 0; i < size(objJson["Vertices"]); i++) {
			if (objJson["Vertices"][i]["Content"] == "Start")
			{
				if (objJson["Vertices"][i]["Type"] != 0)
				{
					std::cout << "Ошибка в значении Type блока 'Start'" << "\n" << std::endl;
					err++;
				}
			}

			if (objJson["Vertices"][i]["Content"] == "End")
			{
				if (objJson["Vertices"][i]["Type"] != 1)
				{
					std::cout << "Ошибка в значении Type блока 'End'" << "\n" << std::endl;
					err++;
				}
			}

			if (objJson["Vertices"][i]["Type"] == 0)
			{
				if (objJson["Vertices"][i]["Content"] != "Start")
				{
					std::cout << "Ошибка, получен 0 в значении Type, Id " << objJson["Vertices"][i]["Id"] << "\n" << std::endl;
					err++;
				}
			}

			if (objJson["Vertices"][i]["Type"] == 1)
			{
				if (objJson["Vertices"][i]["Content"] != "End")
				{
					std::cout << "Ошибка, получена 1 в значении Type, Id " << objJson["Vertices"][i]["Id"] << "\n" << std::endl;
					err++;
				}
			}
		}

		if (err == 0) {
			std::cout << "Ошибок в терминальных блоках нет" << std::endl;
		}
	};
	void Check_process_blocks(nlohmann::json objJson)
	{
		for (int i = 0; i < size(objJson["Vertices"]); i++) {
			if (objJson["Vertices"][i]["Type"] == 2) // проверка блока процесса
			{
				std::string str = objJson["Vertices"][i]["Content"];
				for (int i = 0; i < str.size(); i++)
				{
					if (str[i] == *"=")
					{
						num_of_equals += 1;
					}
					if (str[i] == *"+" or str[i] == *"*" or str[i] == *"-" or str[i] == *"/" or (str[i] == *"s" and str[i + 1] == *"q" and str[i + 2] == *"r" and str[i + 3] == *"t"))
					{
						num_of_math_sign += 1;
					}
				}
				if (num_of_math_sign > 1)
				{
					std::cout << "Ошибка в количестве математических действий, получено: " << num_of_math_sign << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					err++;
				}
				if (num_of_equals > 1)
				{
					std::cout << "Ошибка в количестве знаков равно, получено: " << num_of_equals << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					err++;
				}
				num_of_math_sign = 0; // обнуляем счетчики
				num_of_equals = 0;
			}
		}

		if (err == 0) {
			std::cout << "Ошибок в блоках процессов нет" << std::endl;
		}
	}
	void Check_solution_blocks(nlohmann::json objJson)
	{
		for (int i = 0; i < size(objJson["Edges"]); i++) {
			int from = objJson["Edges"][i]["From"];
			int to = objJson["Edges"][i]["To"];
			if (objJson["Edges"][i]["Type"] == 0 and objJson["Vertices"][from - 1]["Type"] != 3)
			{
				std::cout << "Ошибка в значении 'Type', должно быть 3, Id " << from << "\n" << std::endl;
				err++;
			}
		}

		for (int i = 0; i < size(objJson["Vertices"]); i++) {
			if (objJson["Vertices"][i]["Type"] == 3) //проверка блока решения
			{
				int id_of_sol = objJson["Vertices"][i]["Id"];
				int total_type = 0;
				num_of_sol += 1;
				for (int i = 0; i < size(objJson["Edges"]); i++)
				{
					if (objJson["Edges"][i]["From"] == id_of_sol and (objJson["Edges"][i]["Type"] == 0 or objJson["Edges"][i]["Type"] == 1))
					{
						total_type += objJson["Edges"][i]["Type"];
						count += 1;
					}
				}

				if (count / num_of_sol != 2.0)
				{
					std::cout << "Ошибка в количестве веток блока решения" << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					num_of_sol = 0;
					count = 0;
					err++;
				}
				if (total_type == 2) {
					std::cout << "Ошибка, два соединения Edges имеют Type = 1" << std::endl;
					std::cout << "Ошибка в From: " << i + 1 << "\n" << std::endl;
					err++;
				}
				if (total_type == 0) {
					std::cout << "Ошибка, два соединения Edges имеют Type = 0" << std::endl;
					std::cout << "Ошибка в From: " << i + 1 << "\n" << std::endl;
					err++;
				}
				std::string s = objJson["Vertices"][i]["Content"];
				for (int i = 0; i < s.size(); i++)
				{
					if (s[i] == *">" or s[i] == *"<")
					{
						num_of_comp += 1;
					}
					if (s[i] == *"=")
					{
						num_of_equals += 1;
					}
					if (s[i] == *"!")
					{
						num_of_exclam += 1;
					}
				}
				if (num_of_comp >= 2)
				{
					std::cout << "Ошибка в количестве знаков сравнения, получено: " << num_of_comp << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					err++;
				}
				if (num_of_exclam >= 2)
				{
					std::cout << "Ошибка в количестве знаков восклицания, получено: " << num_of_comp << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					err++;
				}
				if (num_of_equals >= 3)
				{
					std::cout << "Ошибка в количестве знаков равно, получено: " << num_of_equals << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					err++;
				}
				num_of_equals = 0; // обнуляем счетчики
				num_of_comp = 0;
				num_of_exclam = 0;
			}
		}

		if (err == 0) {
			std::cout << "Ошибок в блоках решений нет" << std::endl;
		}
	}
	void Check_data_blocks(nlohmann::json objJson)
	{
		for (int i = 0; i < size(objJson["Vertices"]); i++) {
			if (objJson["Vertices"][i]["Type"] == 4 or objJson["Vertices"][i]["Type"] == 5) //проверка блока данных
			{
				std::string str = objJson["Vertices"][i]["Content"];
				for (int i = 0; i < str.size(); i++)
				{
					if (str[i] == *"=")
					{
						num_of_equals += 1;
					}
					if (str[i] == *"+" or str[i] == *"*" or str[i] == *"/" or (str[i] == *"s" and str[i + 1] == *"q" and str[i + 2] == *"r" and str[i + 3] == *"t"))
					{
						num_of_math_sign += 1;
					}
					if (str[i] == *";" or str[i] == *"'" or str[i] == *":")
					{
						num_of_sign += 1;
					}
				}
				if (num_of_math_sign > 0)
				{
					std::cout << "Ошибка, найдены математические действия в блоке данных" << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					err++;
				}
				if (num_of_equals > 0)
				{
					std::cout << "Ошибка, найдены знаки равно в блоке данных" << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					err++;
				}
				if (num_of_sign > 0)
				{
					std::cout << "Ошибка, найдены недопустимые знаки в блоке данных" << std::endl;
					std::cout << "Ошибка в Id: " << i + 1 << "\n" << std::endl;
					err++;
				}
				num_of_math_sign = 0; // обнуляем счетчики
				num_of_equals = 0;
				num_of_sign = 0;
			}
		}

		if (err == 0) {
			std::cout << "Ошибок в блоках данных нет" << std::endl;
		}
	}
	bool Check_structure(std::string content) {
		for (k; k < content.size(); k++) {
			if (content[k] == 'I' and content[k + 1] == 'd') {
				total_id++;
			}
		}
		for (int i = 0; i < content.size(); i++) {
				if (content[i] == ' ' or content[i] == char(9)) continue;
				else {
					if (content[i] != '{' and (index == 0 or index == (5 + step))) {
						if (index == 0) {
							if (content[i] != '"') {
								std::cout << "Получен неверный символ в начале блок-схемы: " << content[i] << std::endl;
								return false;
							}
							else {
								std::cout << "Ошибка, отсутствует { в начале блок-схемы" << std::endl;
								return false;
							}
						}
						if (index == (5 + step)) {
							if (content[i] != '"') {
								std::cout << "Получен неверный символ перед {: " << content[i] << std::endl;
								std::cout << "Для Id " << counter_id + 1 << std::endl;
								return false;
							}
							else {
								std::cout << "Ошибка, отсутствует { для Id " << counter_id + 1 << std::endl;
								return false;
							}
						}
					}
					else {
						//std::cout << content[i] << " " << index << std::endl;
						i++;
						index++;
						//std::cout << content[i] << " " << index << std::endl;
						while (content[i] == ' ' or content[i] == char(9)) i++;
						//std::cout << content[i] << " " << index << std::endl;
						if ((content[i + 1] == ' ' or content[i + 1] == char(9)) and text == "End" and from == 0 and to == 0) {
							while (content[i + 1] == ' ' or content[i + 1] == char(9)) i++;
						}
						if (content[i + 1] == ']' and text == "End" and content[i] != ']' and !isalpha(content[i]) and !isdigit(content[i])) {
							j = i + 2;
							k = j;
							index = 0; // переходим к блоку "Edges"
							step = 0;
							break;
						}
						else {
							if (content[i + 1] != ']' and text == "End" and content[i] != ']' and !isalpha(content[i]) and !isdigit(content[i]) and counter_id == total_id) {
								std::cout << "Ошибка, отсутствует ] после }, Id " << counter_id << " или написаны лишние символы перед ней" << std::endl;
								return false;
							}
						}
					}

					if (content[i] == '"' and index == 1) {
						i++;
						while ((isalpha(content[i]) or isdigit(content[i]))) {
							temp += content[i];
							i++;
						}
						if (temp != "Vertices" or content[i] == ' ') {
							std::cout << "Ошибка в наименовании блока Vertices" << std::endl;
							return false;
						}
						index++;
						if (content[i] == '"' and index != 1) {
							i--;
						}
						else {
							std::cout << "Ошибка, отсутствует закрывающая кавычка для слова: " << std::endl;
							std::cout << temp << std::endl;
							return false;
						}
					}
					else {
						if (index == 1) {
							std::cout << "Ошибка, отсутствует открывающая кавычка или написан неверный символ перед блоком Vertices" << std::endl;
							return false;
						}
					}

					if (content[i] == ':' and index == 3) {
						i++;
						index++;
						while (content[i] == char(9) or content[i] == char(32)) {
							i++;
						}
						if (content[i] == '[' and index == 4) { index++; }
						else {
							if (index == 4) {
								std::cout << "Ошибка, отсутствует [ после: " << std::endl;
								std::cout << '"' + temp + '"' + ':' << std::endl;
								return false;
							}
						}
					}
					else {
						if (index == 3) {
							std::cout << "Ошибка, отсутствует двоеточие после: " << std::endl;
							std::cout << '"' + temp + '"' << std::endl;
							return false;
						}
					}

					if (content[i] == '"' and index == (6 + step))
					{
						i++;
						temp = "";
						while (isalpha(content[i]) or isdigit(content[i])) {
							temp += content[i];
							i++;
						}
						if (temp != "Id" or content[i] == ' ') {
							std::cout << "Ошибка в наименовании блока Id " << counter_id + 1 << std::endl;
							return false;
						}
						index++;
						if (content[i] == '"' and index != (6 + step)) {
							counter_id++;
							i--;
						}
						else {
							std::cout << "Ошибка, отсутствует закрывающая кавычка для Id " << counter_id + 1 << std::endl;
							return false;
						}
					}
					else {
						if (index == (6 + step)) {
							std::cout << "Ошибка, отсутствует открывающая кавычка для Id " << counter_id + 1 << " или написаны лишние символы перед ней" << std::endl;
							return false;
						}
					}

					if (content[i] == ':' and (index == (8 + step) or index == (12 + step))) {
						i++;
						temp = "";
						while (content[i] == ' ' or content[i] == char(9)) i++;
						while (isdigit(content[i]) or isalpha(content[i])) {
							if (isalpha(content[i])) {
								if (index == (8 + step)) {
									std::cout << "Ошибка, использованы буквы для Id " << counter_id << std::endl;
									return false;
								}
								if (index == (12 + step)) {
									std::cout << "Ошибка, использованы буквы для Type, Id " << counter_id << std::endl;
									return false;
								}
							}
							temp += content[i];
							i++;
						}
						if (temp == "") {
							if (index == (12 + step)) {
								std::cout << "Ошибка, пустое значение в блоке Type, Id " << counter_id << std::endl;
								return false;
							}
						}
						if (index == 12 + step) type_of_vertice = temp;
						index++;
						while (content[i] == ' ' or content[i] == char(9)) i++;
						if (content[i] == ',' and index != (8 + step)) { // не нужна проверка для числа Type
							if (temp != std::to_string(counter_id) and index != (13 + step)) {
								std::cout << "Ошибка в порядковом номере Id" << std::endl;
								std::cout << "Получено: " << temp << std::endl;
								std::cout << "Должно быть: " << counter_id << std::endl;
								return false;
							}
							else { i--; }
						}
						else {
							if (index == (9 + step)) {
								std::cout << "Ошибка, отсутствует запятая после номера Id " << counter_id << " или написаны лишние символы перед ней" << std::endl;
								return false;
							}
							if (index == (13 + step)) {
								std::cout << "Ошибка, отсутствует запятая после числа Type для Id " << counter_id << " или написаны лишние символы перед ней" << std::endl;
								return false;
							}
							std::cout << "Ошибка, отсутствует запятая для Id " << counter_id << std::endl;
							return false;
						}
					}
					else {
						if (index == (8 + step)) {
							std::cout << "Ошибка, отсутствует двоеточие после Id " << counter_id << " или написаны лишние символы перед ним" << std::endl;
							return false;
						}
						if (index == (12 + step)) {
							std::cout << "Ошибка, отсутствует двоеточие после Type для Id " << counter_id << " или написаны лишние символы перед ним" << std::endl;
							return false;
						}
					}

					if (content[i] == '"' and index == (10 + step))
					{
						i++;
						temp = "";
						while (isalpha(content[i]) or isdigit(content[i])) {
							temp += content[i];
							i++;
						}
						if (temp != "Type" or content[i] == ' ') {
							std::cout << "Ошибка в наименовании блока Type для Id " << counter_id << std::endl;
							return false;
						}
						index++;
						if (content[i] == '"' and index != (10 + step)) {
							i--;
						}
						else {
							std::cout << "Ошибка, отсутствует закрывающая кавычка для блока Type, Id " << counter_id << std::endl;
							return false;
						}
					}
					else {
						if (index == (10 + step)) {
							std::cout << "Ошибка, отсутствует открывающая кавычка для блока Type или написаны лишние символы перед ней, Id " << counter_id << std::endl;
							return false;
						}
					}

					if (content[i] == '"' and index == (14 + step))
					{
						i++;
						temp = "";
						while (isalpha(content[i]) or isdigit(content[i])) {
							temp += content[i];
							i++;
						}
						if (temp != "Content" or content[i] == ' ') {
							std::cout << "Ошибка в наименовании блока Content для Id " << counter_id << std::endl;
							return false;
						}
						index++;
						if (content[i] == '"' and index != (14 + step)) {
							i--;
						}
						else {
							std::cout << "Ошибка, отсутствует закрывающая кавычка для блока Content, Id " << counter_id << std::endl;
							return false;
						}
					}
					else {
						if (index == (14 + step)) {
							std::cout << "Ошибка, отсутствует открывающая кавычка для блока Content, Id " << counter_id << " или написаны лишние символы перед ней" << std::endl;
							return false;
						}
					}

					if (content[i] == ':' and index == (16 + step)) { i--; }
					else {
						if (index == (16 + step)) {
							std::cout << "Ошибка, отсутствует двоеточие после блока Content, Id " << counter_id << " или написаны лишние символы перед ним" << std::endl;
							return false;
						}
					}

					if (content[i] == '"' and index == (17 + step)) {
						i++;
						temp = "";
						while (content[i] != '"') {
							if (content[i] == '{' or content[i] == '}' or content[i] == ':' or content[i] == ';') {
								std::cout << "Получен недопустимый символ в блоке Content: " << content[i] << ", в Id " << counter_id << std::endl;
								std::cout << "Возможно отсутствует закрывающая кавычка для содержимого блока Content" << std::endl;
								return false;
							}
							temp += content[i];
							i++;
						}
						text = temp;
						index++;
						if (content[i] == '"' and index != (17 + step)) { i--; }
					}
					else {
						if (index == (17 + step)) {
							std::cout << "Ошибка, отсутствует открывающая кавычка для содержимого блока Content, Id " << counter_id << " или написаны лишние символы перед ней" << std::endl;
							return false;
						}
					}

					if (content[i] == '}' and index == (19 + step)) { i--; }
					else {
						if (index == (19 + step)) {
							std::cout << "Ошибка, отсутствует } после содержимого блока Content, Id " << counter_id << " или написаны лишние символы перед ней" << std::endl;
							return false;
						}
					}

					if (content[i] == ',' and index == (20 + step)) {
						step = step + 16;
						index++;
					}
					else {
						if (index == (20 + step)) {
							if (counter_id != total_id) {
								std::cout << "Ошибка, отсутствует запятая после }, Id " << counter_id << " или написаны лишние символы перед ней" << std::endl;
								return false;
							}
							else {
								std::cout << "Ошибка, в содержимом блоке Content, Id " << counter_id << " должно быть End" << std::endl;
								return false;
							}
						}
					}
				}
		}

		for (k; k < content.size(); k++) {
			if (content[k] == 'F' and content[k + 1] == 'r' and content[k + 2] == 'o' and content[k + 3] == 'm') {
				num_of_edges++;
			}
		}

		for (j; j < content.size(); j++) {
			if (content[j] == ' ' or content[j] == char(9)) continue;
			else {
				if (content[j] != ',' and (index == 0)) {
					if (index == 0) {
						std::cout << "Ошибка, отсутствует запятая после ], Id " << counter_id << " или написаны лишние символы перед ней" << std::endl;
						return false;
					}
				}
				else {
					j++;
					index++;
					while (content[j] == ' ' or content[j] == char(9)) j++;
					//std::cout << content[j] << " " << index << std::endl;
					/*if ((content[j + 1] == ' ' or content[j + 1] == char(9)) and (type_of_vertice == "1") and text == "End" and from == 0 and to == 0) {
						while (content[j + 1] == ' ' or content[j + 1] == char(9)) j++;
					} // ПЕРЕДЕЛАТЬ ДЛЯ ] FOR EDGES
					if (content[j + 1] == ']' and from == 999 and to == 0) {
						j = j + 1;
						break;
					}
					else {
						if (content[j + 1] != ']' and (type_of_vertice == "1") and text == "1End" and from == 0 and to == 0) {
							std::cout << "Ошибка, отсутствует ] после }, Id " << counter_id << std::endl;
							return false;
						}
					}*/
				}

				if (content[j] == '"' and index == 1) {
					j++;
					temp = "";
					while (isalpha(content[j]) or isdigit(content[j])) {
						temp += content[j];
						j++;
					}
					if (temp != "Edges" or content[j] == ' ') {
						std::cout << "Ошибка в наименовании блока Edges";
						return false;
					}
					index++;
					if (content[j] == '"' and index != 1) {
						j--;
					}
					else {
						std::cout << "Ошибка, отсутствует закрывающая кавычка для слова: " << std::endl;
						std::cout << temp << std::endl;
						return false;
					}
				}
				else {
					if (index == 1) {
						std::cout << "Ошибка, отсутствует открывающая кавычка или написан неверный символ перед блоком Edges" << std::endl;
						return false;
					}
				}

				if (content[j] == ':' and index == 3) {
					j++;
					index++;
					while (content[j] == char(9) or content[j] == char(32)) {
						j++;
					}
					if (content[j] == '[' and index == 4) { 
						index++;
						j++;
						while (content[j] == char(9) or content[j] == char(32)) { j++; }
					}
					else {
						if (index == 4) {
							std::cout << "Ошибка, отсутствует [ после  " << '"' + temp + '"' << " или написаны лишние символы перед ней" << std::endl;
;							return false;
						}
					}
				}
				else {
					if (index == 3) {
						std::cout << "Ошибка, отсутствует двоеточие после  " << '"' + temp + '"' << " или написаны лишние символы перед ним" << std::endl;
						return false;
					}
				}

				if (content[j] == '{' and index == (5 + step))
				{
					j++;
					index++;
					while (content[j] == char(9) or content[j] == char(32)) { j++;}
				}
				else {
					if (index == (5)) {
						std::cout << "Ошибка, отсутствует { послe [ в начале блока Edges или написаны лишние символы перед ней" << std::endl;
						return false;
					}
					
				}
				
				if (content[j] == '"' and index == (6 + step))
				{
					j++;
					temp = "";
					while (isalpha(content[j]) or isdigit(content[j])) {
						temp += content[j];
						j++;
					}
					if (temp != "From" or content[j] == ' ') {
						std::cout << "Ошибка в наименовании " << number << " блока From";
						return false;
					}
					index++;
					if (content[j] == '"' and index != (6 + step)) {
						j--;
						num_of_from++;
					}
					else {
						std::cout << "Ошибка, отсутствует закрывающая кавычка для " << number << " блока From" << std::endl;
						return false;
					}
				}
				else {
					if (index == (6 + step)) {
						std::cout << "Ошибка, отсутствует открывающая кавычка для " << number << " блока From или написаны лишние символы перед ней" << std::endl;
						return false;
					}
				}

				if (content[j] == ':' and (index == (8 + step) or index == (12 + step) or index == (16 + step))) {
					j++;
					temp = "";
					while (content[j] == ' ' or content[j] == char(9)) j++;
					while (isdigit(content[j]) or isalpha(content[j])) {
						if (isalpha(content[j])) {
							if (index == (8 + step)) {
								std::cout << "Ошибка, использованы буквы для From под номером " << number << std::endl;
								return false;
							}
							if (index == (12 + step)) {
								std::cout << "Ошибка, использованы буквы для To под номером " << number << std::endl;
								return false;
							}
							if (index == (16 + step)) {
								std::cout << "Ошибка, использованы буквы для Type в Edges под номером " << number << std::endl;
								return false;
							}
						}
						temp += content[j];
						j++;
					}
					if (temp == "") {
						if (index == (8 + step)) {
							std::cout << "Ошибка, пустое значение в блоке From под номером " << number << std::endl;
							return false;
						}
						if (index == (12 + step)) {
							std::cout << "Ошибка, пустое значение в блоке To под номером " << number << std::endl;
							return false;
						}
						if (index == (16 + step)) {
							std::cout << "Ошибка, пустое значение в блоке Type в Edges под номером " << number << std::endl;
							return false;
						}
					}
					if (index == (8 + step)) from = std::stoi(temp);
					if (index == (12 + step)) to = std::stoi(temp);
					index++;
					while (content[j] == ' ' or content[j] == char(9)) j++;
					if (content[j] == ',' and index != (8 + step)) {
						j--;
					}
					else {
						if (index == (9 + step)) {
							std::cout << "Ошибка, отсутствует запятая после " << number << " блока From или написаны лишние символы перед ней" << std::endl;
							return false;
						}
						if (index == (13 + step)) {
							std::cout << "Ошибка, отсутствует запятая после " << number << " блока To или написаны лишние символы перед ней" << std::endl;
							return false;
						}
					}
				}
				else {
					if (index == (8 + step)) {
						std::cout << "Ошибка, отсутствует двоеточие после " << number << " блока From или написаны лишние символы перед ним" << std::endl;
						return false;
					}
					if (index == (12 + step)) {
						std::cout << "Ошибка, отсутствует двоеточие после " << number << " блока To или написаны лишние символы перед ним" << std::endl;
						return false;
					}
					if (index == (16 + step)) {
						std::cout << "Ошибка, отсутствует двоеточие после " << number << " блока Type в Edges или написаны лишние символы перед ним" << std::endl;
						return false;
					}
				}
				
				if (content[j] == '"' and index == (10 + step))
				{
					j++;
					temp = "";
					while (isalpha(content[j]) or isdigit(content[j])) {
						temp += content[j];
						j++;
					}
					if (temp != "To" or content[j] == ' ') {
						std::cout << "Ошибка в наименовании " << number << " блока To";
						return false;
					}
					index++;
					if (content[j] == '"' and index != (10 + step)) {
						j--;
					}
					else {
						std::cout << "Ошибка, отсутствует закрывающая кавычка для " << number << " блока To" << std::endl;
						return false;
					}
				}
				else {
					if (index == (10 + step)) {
						std::cout << "Ошибка, отсутствует открывающая кавычка для " << number << " блока To или написаны лишние символы перед ней" << std::endl;
						return false;
					}
				}

				if (content[j] == '"' and index == (14 + step))
				{
					j++;
					temp = "";
					while (isalpha(content[j]) or isdigit(content[j])) {
						temp += content[j];
						j++;
					}
					if (temp != "Type" or content[j] == ' ') {
						std::cout << "Ошибка в наименовании " << number << " блока Type в Edges";
						return false;
					}
					index++;
					if (content[j] == '"' and index != (14 + step)) {
						j--;
					}
					else {
						std::cout << "Ошибка, отсутствует закрывающая кавычка для " << number << " блока Type в Edges" << std::endl;
						return false;
					}
				}
				else {
					if (index == (14 + step)) {
						std::cout << "Ошибка, отсутствует открывающая кавычка для " << number << " блока Type в Edges или написаны лишние символы перед ней" << std::endl;
						return false;
					}
				}

				if (content[j] == '}' and index == (17 + step)) { j--; }
				else {
					if (index == (17 + step)) {
						std::cout << "Ошибка, отсутствует } после содержимого " << number << " блока Type в Edges или написаны лишние символы перед ней" << std::endl;
						return false;
					}
				}

				if (content[j] == ',' and index == (18 + step) and to != counter_id) {
					step = step + 14;
					number++;
					j--;
				}
				else {
					if (index == (18 + step) and num_of_from != num_of_edges) {
						std::cout << "Ошибка, отсутствует запятая после } содержимого " << number << " блока Type в Edges или написаны лишние символы перед ней" << std::endl;
						return false;
					}
					if (index == (18 + step) and num_of_from == num_of_edges) {
						while (content[j] == char(9) or content[j] == char(32)) { j++; }
						if (content[j] != ']') {
							std::cout << "Ошибка, указано неверное значение для " << number << " блока To или отсутствует ] для блока Edges" << std::endl;
							std::cout << "Возможно использован неверный символ. Получено: " << content[j] << std::endl;
							return false;
						}
						else {
							j++;
							while (content[j] == char(9) or content[j] == char(32)) { j++; }
							if (content[j] != '}') {
								std::cout << "Ошибка, отсутствует } в конце блок-схемы или написаны лишние символы перед ней" << std::endl;
								return false;
							}
							else {
								j++;
								while ((content[j] == char(9) or content[j] == char(32)) and j != content.size()) {
									j++;
								}
								if (content[j] != char(0) and content[j] != char(32)) {
									std::cout << "Ошибка, написаны лишние символы после блок-схемы" << std::endl;
									return false;
								}
								else {
									return true;
								}
							}
						}
					}
				}
			}
		}
	}
};


class User_Interface
{
private:
	std::string name;
	std::string filename_json;
	std::string filename_txt;
	std::string filename_check;
public:
	std::string Enter_name()
	{
		std::cout << "Введите название файла:" << std::endl;
		std::cin >> name;

		return name;
	}
	int Show(nlohmann::json objJson)
	{
		name = Enter_name();

		filename_json = "JSON/" + name + ".json";
		filename_txt = "JSON/" + name + ".txt";
		filename_check = filename_json;

		std::ifstream file_check(filename_check);
		if (!file_check.is_open()) {
			std::cerr << "Не удалось открыть файл " << filename_check << std::endl;
			return 1;
		}
		file_check.close();

		Checker checker;

		Converter converter;

		converter.To_Txt(filename_json);

		// Открытие файла на чтение
		std::ifstream file(filename_txt);
		if (!file.is_open()) {
			std::cerr << "Не удалось открыть файл " << filename_txt << std::endl;
			return 1;
		}

		// Чтение содержимого файла в строку
		std::string line;
		std::string allContent;
		while (getline(file, line)) {
			allContent += line;
		}

		if (allContent == "") {
			std::cout << "Входной файл пуст" << std::endl;
			return 1;
		}
		// Закрытие файла
		file.close();

		converter.To_JSON(filename_txt);

		if (checker.Check_structure(allContent)) {
			std::fstream fileInput;
			fileInput.open(filename_json);

			fileInput >> objJson;
			bool flag = true;
			char choice;
			do {
			std::cout << "1. Проверить логику блок-схемы.\n";
			std::cout << "2. Проверить терминальные блоки.\n";
			std::cout << "3. Проверить блоки процессов.\n";
			std::cout << "4. Проверить блоки решений.\n";
			std::cout << "5. Проверить блоки данных.\n";
			std::cout << "Выберете действие: ";
			std::cin >> choice;
				switch (choice) {
					case '1':
						checker.Check_logic(objJson);
						flag = false;
						break;

					case '2':
						checker.Check_terminal_blocks(objJson);
						flag = false;
						break;

					case '3':
						checker.Check_process_blocks(objJson);
						flag = false;
						break;

					case '4':
						checker.Check_solution_blocks(objJson);
						flag = false;
						break;

					case '5':
						checker.Check_data_blocks(objJson);
						flag = false;
						break;

					default:
						std::cout << "Неверный формат ввода данных. Пожалуйста, попробуйте снова.\n";
						break;
				}
			} while (flag);

			fileInput.close();
		}
	}
};


int main()
{
	nlohmann::json objJson;

	setlocale(LC_ALL, "Russian");

	User_Interface user_interface;

	user_interface.Show(objJson);
	
}
