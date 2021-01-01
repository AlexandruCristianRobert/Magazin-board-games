#include <iostream> 
#include <algorithm>// pentru functii push_back,begin,end,back
#include <vector>// folosim vector
using namespace std;
class board_game// clasa jocului
{
	string nume;//variabile de tip privat, nu l-am specificat, deci e default setat ca private
	// variabilele si functiile private pot fi folosite doar in interiorul clasei curente( in cazul asta le putem folosi doar in clasa board_game
	int pret;
	int nr_juc_min;
	int nr_juc_max;
	int nr_vanzari;
public:// variabile si functii publice( noi avem doar functii) pot fi folosite oriunde in program ( de ex in clasa client, magazin sau in int main)
	// pentru a accesa variabilele private in afara clasei vom face functii publice ce le acceseaza sau modifica
	//  functiile set modifica valoarea
	// functiile get returneaza valoarea
	board_game(string bnume = "Anonim") :nr_juc_min(2), nr_juc_max(3), pret(1), nr_vanzari(0)// cand creezi un joc el va primi automat numele anonim, si va primi valorile alea  din paranteze, chit ca le scrii aici sau intre acoloade mai jos e acelasi lucru
	{
		nume = bnume;//pentru ca noi am creat un joc cu un  nume din exterior, ii setam variabila nume sa primeasca numele din exterior, iar daca nu am dat niciun nume cand l-am creat, el primeste automat numele anonim.
	}
	friend istream& operator >>(istream& i, board_game& bg)// suprascrierea citirii
	{
		cout << "Nume joc:";
		i >> bg.nume;
		cout << "Pret:";
		i >> bg.pret;
		cout << "Numar minim de jucatori:";
		i >> bg.nr_juc_min;
		cout << "Numar maxim de jucatori:";
		i >> bg.nr_juc_max;
		return  i;
	}
	void set_vanzari()// functie set, face nr_vanzari=nr_vanzari+1 cand e apelata
	{
		nr_vanzari++;
	}
	int get_vanzari()//returneaza valoarea lui nr_vanzari vezi ca are int in loc de void pentru ca returneaza o variabila de tip int
	{
		return nr_vanzari;
	}
	int get_pret()
	{
		return pret;
	}
	string get_nume()//aici e string pentru ca returneaza un string
	{
		return nume;
	}
	friend ostream& operator <<(ostream& o, board_game& bg) //  suprascrierea afisarii
	{
		o << endl;
		o << "Joc: " << bg.nume << endl;
		o << "Pret: " << bg.pret << endl;
		o << "Numar de jucatori intre: " << bg.nr_juc_min << " si " << bg.nr_juc_max << endl;
		return o;
	}
};
class Client
{
	string nume;//variabile private 
	vector<board_game> jocuri_cumparate;//vectorul de jocuri al unui client, tinem jocurile cumparate de acesta
	int nr_jocuri;//  nr jocuri cumparate
	int total;//  total bani dati pe jocuri
public:
	Client(string nume) :nr_jocuri(0), total(0)
	{
		this->nume = nume;//  pentru ca primeste un parametru nume care e scris la fel cu variabila privata nume, folosim this-> ca sa stie ca vrem sa ii dam variabilei private nume valoarea parametrului nume
	}
	void cumpara_joc(board_game i) // functie cand cumpara un joc
	{
		jocuri_cumparate.push_back(i);// adauga jocul la capatul vectorului de jocuri. Deci daca ai un vector cu 5 jocuri, mai adauga un joc pe pozitia 6
		nr_jocuri++; // adunam la nr de jocuri cumparate
		total += i.get_pret();// exemplu de aplicare a unei functii get. pentru ca nu putem accesa pretul jocului, este variabila privata, folosim functia get_pret care returneaza valoarea pretului
	}
	int get_nr_jocuri()
	{
		return nr_jocuri;
	}
	string get_nume()
	{
		return nume;
	}
	friend ostream& operator <<(ostream& x, Client& c)// suprascrierea afisarii unui client
	{
		x << "Nume client: " << c.nume << endl;
		x << "Lista jocuri cumparate: " << endl;
		for (auto i = c.jocuri_cumparate.begin();i != c.jocuri_cumparate.end();++i)
		{
			x << *i << endl; // aici de exemplu programul stie sa foloseasca suprascrierea afisarii unui joc pentru ca *i este un joc.
		}
		x << "In total, el a cumparat " << c.nr_jocuri << " jocuri in valoare de " << c.total << " RON." << endl;
		return x;
	}
};
class Magazin
{
	vector<board_game> jocuri;
	vector<Client> clienti;
	int nr_jocuri;
	int total_vanzari;
public:
	Magazin() :nr_jocuri(0), total_vanzari(0) {}
	void primire_joc(board_game joc)
	{
		bool k = true;
		for (auto i = jocuri.begin();i != jocuri.end();++i)
		{
			if ((*i).get_nume() == joc.get_nume())// daca numele jocului se regaseste printre jocurile din inventar nu il adaugam.
			{
				k = false;
				break;
			}
		}
		if (k)// daca k e adevarat
		{
			jocuri.push_back(joc);
			nr_jocuri++;
			cout << "Jocul a fost adaugat cu succes in inventar." << endl;
		}
		else
			cout << "Jocul este deja in inventar" << endl;
	}
	void vinde_joc(string nume_client, string nume_joc)//  vindem un joc in functie de numele lor
	{
		bool finalizare = false;
		bool verif = false;
		for (auto i = clienti.begin();i != clienti.end();++i)// daca gasim un client in lista de clienti ii vindem lui jocul. De ex daaca deja avem un client Alex care a cumaprat un joc, nu mai facel alt client Alex, ii vindem tot lui alex pe care deja il avem
		{
			if ((*i).get_nume() == nume_client)
			{
				for (auto j = jocuri.begin();j != jocuri.end();++j)
				{
					if ((*j).get_nume() == nume_joc)
					{
						(*i).cumpara_joc(*j);
						(*j).set_vanzari();
						total_vanzari += (*j).get_pret();
						finalizare = true;
						break;
					}
				}
				verif = true;
				break;
			}
		}
		if (verif == false)// daca nu am gasit niciun client cu numele ala, verificam daca avem un joc cu numele introdus, dupa creez un client cu numele ala si ii adaug jocul
		{
			for (auto j = jocuri.begin();j != jocuri.end();++j)
			{
				if ((*j).get_nume() == nume_joc)
				{
					Client c(nume_client);
					clienti.push_back(c);
					clienti.back().cumpara_joc(*j);
					total_vanzari += (*j).get_pret();//  aici adun la totalul vanzarilor magazinului
					(*j).set_vanzari();// adunam la totalul vanzarilor jocului
					finalizare = true;
					break;
				}
			}

		}
		if (finalizare == true)
			cout << "Jocul a fost cumparat!" << endl;
		else
			cout << "Jocul nu a fost cumparat deoarece s-a introdus un nume invalid" << endl;
	}
	void afisare_cel_mai_vandut_joc()//  afiseaza jocurile care s-au vandut de cele mai multe ori
	{
		int max = 0;
		for (auto i = jocuri.begin();i != jocuri.end();++i)
		{
			if ((*i).get_vanzari() > max)
			{
				max = (*i).get_vanzari();
			}
		}
		cout << "Jocul/jocurile care s-au vandut de cele mai multe ori (" << max << " ori) sunt: " << endl;
		for (auto i = jocuri.begin();i != jocuri.end();++i)
		{
			if ((*i).get_vanzari() == max)
			{
				cout << *i << endl;
			}
		}
	}
	void afisare_max_fidelitate() //  afiseaza clientul sau clientii care au cumparat cele mai multe jocuri
	{
		int max = 0;
		for (auto i = clienti.begin();i != clienti.end();++i)
		{
			if ((*i).get_nr_jocuri() > max)
			{
				max = (*i).get_nr_jocuri();
			}
		}
		cout << "Clientul/Clientii care au cumparat cele mai multe  jocuri (" << max << ") sunt: " << endl;
		for (auto i = clienti.begin();i != clienti.end();++i)
		{
			if ((*i).get_nr_jocuri() == max)
			{
				cout << *i << endl;
			}
		}
	}
	void afisare_jocuri()
	{
		cout << endl;
		cout << "Nr jocuri in inventar: " << nr_jocuri;
		for (auto i = jocuri.begin();i != jocuri.end();++i)
		{
			cout << *i << endl;
		}
	}
	void afisare_clienti()
	{
		cout << endl << "Lista vanzari: " << endl;
		for (auto i = clienti.begin();i != clienti.end();++i)
		{
			cout << (*i).get_nume() << " a cumparat " << (*i).get_nr_jocuri() << " jocuri " << endl;

		}
		cout << "Total vanzari: " << total_vanzari << endl;
	}
};
int main()
{

	Magazin maga;
	int choose;
	do/// ca sa revenim la meniul principal dupa fiecare comanda.
	{
		cout << "1) Adauga joc" << endl;
		cout << "2) Vinde joc" << endl;
		cout << "3) Max client" << endl;
		cout << "4) Max joc" << endl;
		cout << "5) Afisare jocuri din inventar" << endl;
		cout << "6) Afisare vanzari" << endl;
		cout << "7) Iesire" << endl;
		cin >> choose;
		switch (choose)// functie switch, in functie de ce valoare are parametrul choose el va rula un cod
		{
		case 1://daca choose are valoarea 1 ruleaza codul de mai jos pana la intalnirea unui break. Daca nu gaseste un break , intra in cazul 2 ruleaza totul pana gasteste break, nu gaseste intra in cazul 3si tot asa
		{
			board_game bg;
			cin >> bg;
			maga.primire_joc(bg);
			break;
		}
		case 2:
		{
			string nume_client;
			string nume_joc;
			cout << "Nume client: " << endl;
			cin >> nume_client;
			cout << "Nume joc: " << endl;
			cin >> nume_joc;
			maga.vinde_joc(nume_client, nume_joc);
			break;
		}
		case 3:
		{
			maga.afisare_max_fidelitate();
			break;
		}
		case 4:
		{
			maga.afisare_cel_mai_vandut_joc();
			break;
		}
		case 5:
		{
			maga.afisare_jocuri();
			break;
		}
		case 6:
		{
			maga.afisare_clienti();
			break;
		}
		}
	} while (choose != 7);
	return 0;
}
