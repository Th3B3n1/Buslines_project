#include "menu.h"

int Menu::get_choice()
{
	int choice;
	std::cin >> choice;
	if (std::cin.fail())
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Nem megfelelo input." << std::endl;
		return -1;
	}
	std::cin.ignore();
	return choice;
}

void Menu::stop_menu()
{
	switch (get_choice())
	{
	case 1:
	{
		std::cout << "----------------------" << std::endl;
		std::cout << "Adja meg a megallo nevet: ";
		std::string stopname_to_create;
		std::getline(std::cin, stopname_to_create);
		bool creation = stops.add(Stop(stopname_to_create));
		if (creation)
		{
			std::cout << "Sikeres megallo letrehozas." << std::endl;
		}
		else
		{
			std::cout << "Sikertelen megallo letrehozas. Ez a megallo mar letezik." << std::endl;
		}
		break;
	}
	case 2:
	{
		std::cout << "----------------------" << std::endl;
		std::cout << "A megallok listaja:" << std::endl;
		stops.list();
		break;
	}
	case 3:
	{
		std::cout << "----------------------" << std::endl;
		std::cout << "Adja meg a torlendo megallo nevet: ";
		std::string stopname_to_delete;
		std::getline(std::cin, stopname_to_delete);

		std::shared_ptr<Stop> pStop = stops.get_stop_by_name(stopname_to_delete);
		if (!pStop)
		{
			std::cout << "Sikertelen megallo torles. Ilyen megallo nem letezik." << std::endl;
			break;
		}

		std::vector<Connection> connected_routes = routes.find_connections_by_connectionstop(pStop);
		if (!connected_routes.empty())
		{
			std::cout << "A megallo szerepel egy vagy tobb jaratban. Biztosan torli? (i/n) ";
			char confirm = std::cin.get();
			std::cin.ignore();
			if (confirm != 'i' && confirm != 'I')
			{
				break;
			}
		}
		bool deletion_code = stops.remove(Stop(stopname_to_delete));
		if (deletion_code)
		{
			routes.rebuild();
			std::cout << "Sikeres megallo torles." << std::endl;
		}
		else
		{
			std::cout << "Sikertelen megallo torles." << std::endl;
		}
		break;
	}
	default:
		break;
	}
}

void Menu::line_menu()
{
	switch (get_choice())
	{
	case 1:
	{
		int line_num;
		std::cout << "----------------------" << std::endl;
		std::cout << "Adja meg a jarat szamat: ";
		std::cin >> line_num;
		if (std::cin.fail() || line_num < 0)
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Nem megfelelo input." << std::endl;
			break;
		}
		std::cin.ignore();
		std::string type;
		std::cout << "Adja meg a jarat tipusat: ";
		std::getline(std::cin, type);
		if (type.empty())
		{
			std::cout << "Nem megfelelo input." << std::endl;
			break;
		}

		if (lines.contains(Line(line_num)))
		{
			std::cout << "Mar letezik ilyen jarat." << std::endl;
			break;
		}

		bool created = false;
		if (type == "Bus")
		{
			created = lines.add(Bus(line_num));
		}
		else if (type == "Tram")
		{
			created = lines.add(Tram(line_num));
		}
		else if (type == "Train")
		{
			created = lines.add(Train(line_num));
		}
		else if (type == "HEV")
		{
			created = lines.add(HEV(line_num));
		}
		else
		{
			created = lines.add(Line(line_num, type));
		}

		if (!created)
		{
			std::cout << "Sikertelen jarat letrehozas." << std::endl;
			break;
		}

		std::shared_ptr<Line> current_line = lines.get_line_by_number(line_num);
		if (current_line == nullptr)
		{
			std::cout << "Sikertelen jarat letrehozas." << std::endl;
			break;
		}

		while (true)
		{
			std::cout << "Adja meg a megallo nevet (befejezeshez 0-at irjon be): ";
			std::string stopname;
			std::getline(std::cin, stopname);
			if (stopname == "0")
			{
				break;
			}

			std::shared_ptr<Stop> pStop = stops.get_stop_by_name(stopname);
			if (!pStop)
			{
				std::cout << "A(z) " << stopname << " megallo nem talalhato az adatbazisba, hozzaadjam uj megallokent? (i/n) ";
				char confirm = std::cin.get();
				std::cin.ignore();
				if (confirm == 'i' || confirm == 'I')
				{
					bool code = stops.add(Stop(stopname));
					if (code)
					{
						std::cout << "Sikeres megallo letrehozas." << std::endl;
						pStop = stops.get_stop_by_name(stopname);
					}
					else
					{
						std::cout << "Sikertelen megallo letrehozas. Ez a megallo mar letezik." << std::endl;
						continue;
					}
				}
				else
				{
					continue;
				}
			}

			std::cout << "Adja meg az idot a terminusztol percekben: ";
			int time_minutes;
			std::cin >> time_minutes;
			if (std::cin.fail() || time_minutes < 0)
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Nem megfelelo formatum." << std::endl;
				break;
			}
			std::cin.ignore();

			if (!current_line->add_stop(pStop, time_minutes))
			{
				std::cout << "Nem megfelelo formatum." << std::endl;
				break;
			}
		}
		routes.rebuild();

		std::cout << "Sikeres jarat letrehozas." << std::endl;
		break;
	}
	case 2:
	{
		std::cout << "----------------------" << std::endl;
		std::cout << "A jaratok listaja:" << std::endl;
		lines.list();
		break;
	}
	case 3:
	{
		std::cout << "----------------------" << std::endl;
		std::cout << "Adja meg a torlendo jarat szamat: ";
		int line_to_delete;
		std::cin >> line_to_delete;
		if (std::cin.fail() || line_to_delete < 0)
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Nem megfelelo jarat szam." << std::endl;
			break;
		}
		std::cin.ignore();

		bool deletion_code = lines.remove(Line(line_to_delete));
		if (deletion_code)
		{
			routes.rebuild();
			std::cout << "Sikeres jarat torles." << std::endl;
		}
		else
		{
			std::cout << "Sikertelen jarat torles. Ilyen jarat nem letezik." << std::endl;
		}
		break;
	}
	case 4:
	{
		int line_num = 0;
		std::cout << "----------------------" << std::endl;
		std::cout << "Adja meg a jarat szamat: ";
		std::cin >> line_num;
		if (std::cin.fail() || line_num < 0)
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Nem megfelelo jarat szam." << std::endl;
			break;
		}
		std::cin.ignore();
		lines.list_line(line_num);
		break;
	}
	default:
		break;
	}
}

void Menu::schedule_menu()
{
	switch (get_choice())
	{
	case 1:
	{
		std::cout << "----------------------" << std::endl;
		std::cout << "Adja meg a jarat szamat: ";
		int line_num;
		std::cin >> line_num;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return;
		}
		std::cin.ignore();

		std::shared_ptr<Line> pLine = lines.get_line_by_number(line_num);
		if (pLine != nullptr)
		{
			std::cout << "Adja meg az indulasi datumot [YYYY-MM-DD HH:MM:SS]: ";
			std::string date_str;
			std::getline(std::cin, date_str);

			std::tm t = {};
			std::stringstream ss_dep(date_str);
			ss_dep >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
			if (ss_dep.fail())
			{
				std::cout << "Nem megfelelo formatum." << std::endl;
				return;
			}
			std::chrono::system_clock::time_point date = std::chrono::system_clock::from_time_t(std::mktime(&t));

			std::cout << "Adja meg a vegallomas nevet: ";
			std::string last_stop;
			std::getline(std::cin, last_stop);
			std::shared_ptr<Stop> pLastStop = stops.get_stop_by_name(last_stop);

			if (pLastStop != nullptr)
			{
				Schedule schedule(pLine, date, pLastStop);
				bool creation_code = schedules.add(schedule);
				if (creation_code)
				{
					std::cout << "Sikeres menetrend letrehozas." << std::endl;
				}
				else
				{
					std::cout << "Sikertelen menetrend letrehozas. Ilyen menetrend mar letezik." << std::endl;
				}
			}
			else
			{
				std::cout << "Nem letezik a megadott vegallomas." << std::endl;
			}
		}
		else
		{
			std::cout << "Nem letezik ezzel a jaratszammal jarat." << std::endl;
		}
		break;
	}
	case 2:
		std::cout << "----------------------" << std::endl;
		std::cout << "A menetrendek listaja:" << std::endl;
		schedules.list();
		break;
	case 3:
	{
		std::cout << "----------------------" << std::endl;
		std::cout << "Adja meg a jarat szamat: ";
		int line_num;
		std::cin >> line_num;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return;
		}
		std::cin.ignore();

		std::shared_ptr<Line> pLine = lines.get_line_by_number(line_num);
		if (pLine != nullptr)
		{
			std::cout << "Adja meg az indulasi datumot [YYYY-MM-DD HH:MM:SS]: ";
			std::string date_str;
			std::getline(std::cin, date_str);

			std::tm t = {};
			std::stringstream ss_dep(date_str);
			ss_dep >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
			if (ss_dep.fail())
			{
				std::cout << "Nem megfelelo formatum." << std::endl;
				return;
			}
			std::chrono::system_clock::time_point date = std::chrono::system_clock::from_time_t(std::mktime(&t));

			std::cout << "Adja meg a vegallomas nevet: ";
			std::string last_stop;
			std::getline(std::cin, last_stop);
			std::shared_ptr<Stop> pLastStop = stops.get_stop_by_name(last_stop);

			if (pLastStop != nullptr)
			{
				Schedule schedule(pLine, date, pLastStop);
				bool deletion_code = schedules.remove(schedule);
				if (deletion_code)
				{
					std::cout << "Sikeres menetrend torles." << std::endl;
				}
				else
				{
					std::cout << "Sikertelen menetrend torles. Ilyen menetrend nem letezik." << std::endl;
				}
			}
			else
			{
				std::cout << "Nem letezik a megadott vegallomas." << std::endl;
			}
		}
		else
		{
			std::cout << "Nem letezik ezzel a jaratszammal jarat." << std::endl;
		}
		break;
	}
	default:
		break;
	}
}

void Menu::find_routes()
{
	std::cout << "Find routes nem elerheto meg" << std::endl;
}

void Menu::route_menu()
{
	std::cout << "----------------------" << std::endl;
	std::cout << "Adja meg a kiindulasi megallot: " << std::endl;
	std::string startstop;
	std::getline(std::cin, startstop);
	std::cout << "Adja meg a cel megallot: " << std::endl;
	std::string endstop;
	std::getline(std::cin, endstop);

	std::cout << "Adja meg az indulasi datumot az alabbi formaba:" << std::endl;
	std::cout << "[YYYY-MM-DD HH:MM:SS]" << std::endl;
	std::string date_str;
	std::getline(std::cin, date_str);

	std::tm t = {};
	std::stringstream ss_dep(date_str);
	ss_dep >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
	if (ss_dep.fail())
	{
		std::cout << "Nem megfelelo formatum." << std::endl;
		return;
	}
	std::chrono::system_clock::time_point date = std::chrono::system_clock::from_time_t(std::mktime(&t));
	(void)date; // Suppress unused logic compiler warning since Route algorithm is not fully implemented
	// Hely a Route algorithmusnak.
	std::cout << "Nincsen megjelenitheto utvonal." << std::endl;
}

void Menu::save()
{
	stops.write_to_file("megallok.txt");
	lines.write_to_file("jaratok.txt");
	schedules.write_to_file("menetrendek.txt");
}

void Menu::save_menu()
{
	std::cout << "Szeretne menteni a valtoztatasokat? (i/n) ";
	char confirm = std::cin.get();
	std::cin.ignore();
	if (confirm == 'i' || confirm == 'I')
	{
		save();
		std::cout << "Sikeres mentes." << std::endl;
	}
	else
	{
		std::cout << "Nem tortent mentes." << std::endl;
	}
}

void Menu::run()
{
	int choice = -1;
	while (choice != 0)
	{
		std::cout << "\n1. Megallok kezelese\n2. Jaratok kezelese\n3. Menetrendek kezelese\n4. Utvonal keresese\n5. Adatok manualis mentese\n0. Kilepes\nValasszon menupontot: ";
		std::cin >> choice;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Nem megfelelo input." << std::endl;
			continue;
		}
		std::cin.ignore();
		switch (choice)
		{
		case 1:
			std::cout << "1. Uj megallo letrehozasa\n2. Megallok listazasa\n3. Megallo torlese\n0. Vissza\nValasszon menupontot: ";
			stop_menu();
			break;
		case 2:
			std::cout << "1. Uj jarat letrehozasa\n2. Jaratok listazasa\n3. Jarat torlese\n4. Jarat adatainak listazasa\n0. Vissza\nValasszon menupontot: ";
			line_menu();
			break;
		case 3:
			std::cout << "1. Uj menetrend letrehozasa\n2. Menetrendek listazasa\n3. Menetrend torlese\n0. Vissza\nValasszon menupontot: ";
			schedule_menu();
			break;
		case 4:
			route_menu();
			break;
		case 5:
			save_menu();
			break;
		default:
			break;
		}
	}
}

void Menu::exit()
{
	save_menu();
}