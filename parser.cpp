#include<fstream>
#include<iostream>
#include<string>
#include<string.h>
#include<ctype.h>
#include<stack>
#include<unordered_set>
#include<vector>


using namespace std;

enum states { q0, q1, q2, q3, q4, q5, q6, q7, q8 };

enum Token
{
	PROGRAM_SY, BEGIN_SY, END_SY, PROC_SY, ID_SY, EQUAL_SY, ASSIGNMENT_SY, SIMI_COLON_SY, LEFT_PARNTHES_SY, RIGHT_PARNTHES_SY,
	CALL_SY, INT_SY, BOOL_SY, NUM_SY, TRUE_SY, FALSE_SY, END_OF_FILE_SY, SYNTAX_ERROR_SY
};

enum non_terminals
{
	Program, declaration_seq, declaration_tail, declaration, x, declarer, y, statement_seq, statement_tail, statement, z, type, expr
};


Token current_token;
states currentState = q0;
unordered_set< string > terminal;
unordered_set< string > nonTerminal;
unordered_set< string > tempSet = {};
vector< vector< unordered_set< string > > > ffs(13, vector<unordered_set<string>>(4, tempSet)); // first follow set
vector< vector<string>> rules(23, vector<string>(2, " "));
vector< vector<string> > Table(15, vector<string>(19, "empty"));



class scaner
{
public:
	string s;
	ifstream f;
	scaner()
	{
		f.open("language.txt");
		if (!f)
		{
			cout << "cant open the file" << endl;
		}

		get_Termenal_NonTerminal();
		getProductionRules();
	}
	~scaner()
	{
		f.close();
	};

	non_terminals convert(const string& str)
	{
		if (str == "Program") return Program;
		else if (str == "declaration_seq") return declaration_seq;
		else if (str == "declaration_tail") return declaration_tail;
		else if (str == "declaration") return declaration;
		else if (str == "x") return x;
		else if (str == "declarer") return declarer;
		else if (str == "y") return y;
		else if (str == "statement_seq") return statement_seq;
		else if (str == "statement_tail") return statement_tail;
		else if (str == "statement") return statement;
		else if (str == "z") return z;
		else if (str == "type") return type;
		else if (str == "expr") return expr;
	}

	Token getToken()
	{
		char ch = ' ';
		int currentState = 0;
		s = "";

		while (f.get(ch) && !f.eof())
		{
			switch (currentState)
			{
			case q0:
				if (isalpha(ch))
				{
					currentState = q1;
					s = ch;
					break;
				}
				else if (isdigit(ch))
				{
					currentState = q7;
					s = ch;
					break;
				}
				else if (ch == '(')
				{
					currentState = q2;
					break;
				}
				else if (ch == ')')
				{
					currentState = q3;
					break;
				}
				else if (ch == '=')
				{
					currentState = q4;
					break;
				}
				else if (ch == ';')
				{
					currentState = q5;
					break;
				}
				else if (ch == ':')
				{
					currentState = q6;
					break;
				}
				else if (ch == '$')
				{
					return END_SY;
				}
				else if (isspace(ch))
				{
					break;
				}
				else
				{
					currentState = q7;
					f.putback(ch);
					break;
				}
			case q1:
				while (isalnum(ch) && !f.eof() && !isspace(ch))
				{
					s += ch;
					f.get(ch);
				}
				if (f.eof() || isspace(ch))
				{
					return checkReserved(s);
					currentState = q0;
					break;
				}
				else
				{
					f.putback(ch);
					return checkReserved(s);
					currentState = q0;
					break;
				}
			case q2:
				return LEFT_PARNTHES_SY;
			case q3:
				return RIGHT_PARNTHES_SY;
			case q4:
				return EQUAL_SY;
			case q5:
				return SIMI_COLON_SY;
			case q6:
				if (ch == '=')
				{
					return ASSIGNMENT_SY;
				}
				else
				{
					currentState = q7;
				}
			case q7:
				while (isdigit(ch) && !f.eof() && !isspace(ch))
				{
					s += ch;
					f.get(ch);
				}
				f.putback(ch);
				return NUM_SY;
			case q8:
				//syntaxError() ;
				return SYNTAX_ERROR_SY;
			}
		}
	}

	void syntaxError()
	{
		cout << endl << " syntax error " << endl;
		//return SYNTAX_ERROR_SY ;
	}

	Token checkReserved(string x)
	{
		if (x == "program")
		{
			return PROGRAM_SY;
		}
		else if (x == "begin")
		{
			return BEGIN_SY;
		}
		else if (x == "end")
		{
			return END_SY;
		}
		else if (x == "proc")
		{
			return PROC_SY;
		}
		else if (x == "call")
		{
			return CALL_SY;
		}
		else if (x == "int")
		{
			return INT_SY;
		}
		else if (x == "bool")
		{
			return BOOL_SY;
		}
		else if (x == "true")
		{
			return TRUE_SY;
		}
		else if (x == "false")
		{
			return FALSE_SY;
		}
		else
		{
			return ID_SY;
		}

	}

	string startSymbol()
	{
		unordered_set< string >::iterator itr;

		itr = nonTerminal.begin();
		s = *itr;

		return s;
	}

	void get_Termenal_NonTerminal()
	{
		ifstream ifileG;
		ifileG.open("grammer.txt");

		char c;
		string s = "";
		ifileG.get(c);

		while (!ifileG.eof())
		{
			if (isalnum(c))
			{
				s = "";
				while (isalnum(c) && !ifileG.eof())
				{
					s += c;
					ifileG.get(c);
				}
				terminal.insert(s);
				s = "";
			}

			else if (c == '<')
			{
				s = "";

				while (c != '>')
				{
					s += c;
					ifileG.get(c);
				}
				s += '>';
				nonTerminal.insert(s);
				s = "";
				ifileG.get(c);
			}

			else if (c == '(')
			{
				terminal.insert("(");
				ifileG.get(c);
			}

			else if (c == ')')
			{
				terminal.insert(")");
				ifileG.get(c);
			}

			else if (c == '=')
			{
				terminal.insert("=");
				ifileG.get(c);
			}

			else if (c == '#')
			{
				terminal.insert("#");
				ifileG.get(c);
			}

			else if (c == ';')
			{
				terminal.insert(";");
				ifileG.get(c);
			}

			else if (c == ':')
			{
				ifileG.get(c);
				if (c == '=')
				{
					terminal.insert(":=");
					ifileG.get(c);
				}
				else if (c == ':')
				{
					ifileG.get(c);
					if (c == '=')
					{
						ifileG.get(c);
					}
				}
			}

			else if (isspace(c))
			{
				ifileG.get(c);
			}

		}
	}

	void getProductionRules()
	{
		int i = 0;
		ifstream ifileG;
		ifileG.open("grammer.txt");

		while (!ifileG.eof())
		{
			getline(ifileG, s);
			int p = s.find("::=");

			string nonT = s.substr(0, p);
			string rule = s.substr(p + 3, s.length());

			rules[i][0] = nonT;
			rules[i][1] = rule;

			i++;

		}

	}

	void displaySet(unordered_set< string > x)
	{
		unordered_set< string >::iterator itr;
		for (itr = x.begin(); itr != x.end(); itr++)
		{
			cout << "--------------" << endl;
			cout << *itr << endl;
			cout << "--------------" << endl;
		}
	}

	void display2DVector(vector< vector<string>> x)
	{ 
		for (int i = 0; i < x.size(); i++)
		{
			cout << i << endl << "======================" << endl;
			for (int j = 0; j < x[i].size(); j++)
			{
				cout<< j<<"->>>>>>>>" << x[i][j] << "   " << endl;
			}
			cout << endl;
		}
	}

	void display1DVector(vector< string> x)
	{
		for (int i = 0; i < x.size(); i++)
		{
			cout << x[i] << endl;
		}
	}

	void displayToken()
	{
		string temp[40] = {
			"PROGRAM_SY" , "BEGIN_SY" , "END_SY" , "PROC_SY" , "ID_SY" , "LEFT_PARNTHES_SY" , "RIGHT_PARNTHES_SY" , "EQUAL_SY" , "ASSIGNMENT_SY" ,
			"SIMI_COLON_SY" , "CALL_SY" , "INT_SY" , "BOOL_SY" , "NUM_SY" , "TRUE_SY" , "FALSE_SY" , "SYNTAX_ERROR_SY" , "END_SY"
		};

		Token t = getToken();
		while (!f.eof())
		{
			cout << endl << temp[t] << endl;
			t = getToken();
		}
	}

};

class LL1
{
public:

	scaner* s;

	LL1()
	{
		s = new scaner();
		current_token = s->getToken();
		creatFirstFollowTable();
	}

	void match(Token t)
	{
		if (current_token == t)
		{
			current_token = s->getToken();
		}
		else
		{
			s->syntaxError();
		}
	}

	unordered_set<string> cutStrings(string x)
	{
		unordered_set<string> tempSet2;

		tempSet2.clear();
		char c;
		string s = "";
		int i = 0;

		for (int e = 0, c = x[e]; e < x.size(); )
		{

			if (isalnum(c) && e < x.size())
			{
				s = "";
				while (isalnum(c) && e < x.size())
				{
					s += c;
					c = x[++e];
				}
				tempSet2.insert(s);
				//c = x[++e];
				s = "";
			}
			else if (c == '<' && e < x.size())
			{
				s = "";

				while (c != '>' && e < x.size())
				{
					s += c;
					c = x[++e];
				}
				s += '>';
				tempSet2.insert(s);
				s = "";
				c = x[++e];
			}
			else if (isspace(c) && e < x.size())
			{
				c = x[++e];
			}
			else if (c == '#')
			{
				tempSet2.insert("#");
				c = x[++e];
			}
			else if (c == '(')
			{
				tempSet2.insert("(");
				c = x[++e];
			}
			else if (c == ')')
			{
				tempSet2.insert(")");
				c = x[++e];
			}
			else if (c == '=')
			{
				tempSet2.insert("=");
				c = x[++e];
			}
			else if (c == ';')
			{
				tempSet2.insert(";");
				c = x[++e];
			}
			else if (c == ':')
			{
				c = x[++e];
				if (c == '=')
				{
					tempSet2.insert(":=");
					c = x[++e];
				}
				else if (c == ':')
				{
					c = x[++e];
					if (c == '=')
					{
						c = x[++e];
					}
				}
			}
		}

		return tempSet2;
	}

	unordered_set<string> first(string x)
	{
		bool flag = false;
		unordered_set< string > tempSet5 = { " " };
		unordered_set< string > firstSetHolder = { " " };
		unordered_set< string > tempSet6 = { " " };
		tempSet5.clear();
		tempSet6.clear();
		firstSetHolder.clear();
		if (terminal.find(x) != terminal.end())
		{
			tempSet5.insert(x);
			//cout << x << endl;
		}
		else if (nonTerminal.find(x) != nonTerminal.end())
		{
			//cout << x << endl;
			for (int i = 0; i < rules.size(); i++)
			{
				if (rules[i][0] == x)
				{
					//cout << rules[i][1] << endl;
					tempSet6.clear();
					//tempSet6 = cutStrings(rules[i][1]) ;
					for (auto ele : cutStrings(rules[i][1]))
					{
						tempSet6.insert(ele);
					}
					//s->displaySet(tempSet6);
					/*cout << "////////////////////";
					cout << rules[i][1] << " ///////////////////// " << endl;*/
					//s->displaySet(tempSet6);

					//cout << "/////////////////" << endl;
					//unordered_set< string >::iterator itr2;

					unordered_set< string > ::iterator itr3;
					for (itr3 = tempSet6.begin(); itr3 != tempSet6.end(); itr3++)
					{
						//cout << "->" << *itr3 << endl;
						firstSetHolder = first(*itr3);
						if (firstSetHolder.find("#") != firstSetHolder.end())
						{
							for (auto singleFirst : firstSetHolder)
							{
								tempSet5.insert(singleFirst);
								//cout << singleFirst<<"|" << endl;
								//if (singleFirst != "#")
								//{
								//	flag = true;
								//	//cout << "==========================================" << endl;
								//}
								//cout << "the first value : " << singleFirst << endl;
							}
						}
						else
						{
							for (auto singleFirst : firstSetHolder)
							{
								tempSet5.insert(singleFirst);
								//cout << singleFirst<<"|" << endl;
								//if (singleFirst != "#")
								//{
								//	flag = true;
								//	//cout << "==========================================" << endl;
								//}
								//cout << "the first value : " << singleFirst << endl;
							}
							break;
						}

						//unordered_set< string >::iterator itr3;
						//for (itr3 = first(*itr2).begin(); itr3 != first(*itr2).end(); itr3++) // infinite loop 
						//{
						//	//cout << *itr3 << endl;////////
						//	tempSet5.insert(*itr3);
						//	s->displaySet(tempSet5);
						//}
						//tempSet5.insert( first(*itr2).begin() , first(*itr2).end() ); ////////////

						//if (first(*itr2).find("#") != first(*itr2).end())
						//{
						//	flag = true; //if flag is tru then Epslon is there we don't need to continue
						//}
						/*
						for (singleVal : mainSet )
						*/
					}
					if (flag)
					{
						break;
					}

				}
			}
		}
		else
		{
			cout << "yyyyyyy" << endl;
		}
		return tempSet5;
	}

	unordered_set<string> follow(string x)
	{
		unordered_set< string > tempSet2 = { " " };
		unordered_set< string > tempSet3 = { " " };
		tempSet2.clear();
		tempSet3.clear();
		if (s->startSymbol() == x)
		{
			tempSet2.insert("$");
			return tempSet2;
		}

		else if (nonTerminal.find(x) != nonTerminal.end() && s->startSymbol() != x)
		{
			for (int i = 0; i < rules.size(); i++)
			{
				if (rules[i][1].find(x) != string::npos)
				{
					string y = rules[i][1].substr(rules[i][1].find(x) + x.size(), rules[i][1].size());
					tempSet3 = cutStrings(y);
					if (tempSet3.size() == 0 && x != rules[i][0])
					{
						for (auto t : follow(rules[i][0]))
						{
							tempSet2.insert(t);
						}
					}
					for (auto t : tempSet3)
					{
						if (terminal.find(t) != terminal.end())
						{
							tempSet2.insert(t);
							return tempSet2;
						}
						string g;
						for (auto f : tempSet3)
						{
							g = f;
						}
						for (auto s : first(t))
						{
							if (s != "#")
							{
								tempSet2.insert(s);
							}
							else if (s == "#" && tempSet3.find(t) == tempSet3.find(g) && rules[i][0] != t)
							{
								for (auto t : follow(rules[i][0])) // الفكرة ان بيروح يجبيب الفرست بتاع كل حاجة بعدها 
								{
									tempSet2.insert(t);

								}
							}
						}
						if (first(t).find("#") == first(t).end())
						{
							break;
						}
					}
				}
			}
			return tempSet2;
		}
	}

	void creatFirstFollowTable()
	{
		//ffs.clear();
		unordered_set< string > tempSet2 = { " " };
		tempSet2.clear();

		int i = 0;
		unordered_set< string >::iterator itr;
		for (itr = nonTerminal.begin(); itr != nonTerminal.end(); itr++)
		{
			tempSet2.clear();
			tempSet2.insert(*itr);

			ffs.at(i).at(0) = tempSet2;
			//cout << *ffs.at(i).at(0).begin() << endl ;

			ffs.at(i).at(1) = first(*itr);

			ffs.at(i).at(2) = follow(*itr);

			/*cout  << "@@@@@@@@@@@@@@@@@@@@@@" << endl;
			cout << *itr << endl;
			cout << "#########" << endl;
			s->displaySet(ffs.at(i).at(1));
			cout << "******************" << endl;*/

			//s->displaySet(ffs.at(i).at(1));
			//cout << *ffs.at(i).at(1).begin() << endl;
			//ffs.at(i).at(2) = follow(*itr);
			//ffs.at(i).at(2).insert(follow(*itr).begin(), follow(*itr).end());

			/*
			cout << "end first " << endl;
			unordered_set< string >::iterator itr3;
			for (itr3 = follow(*itr).begin(); itr3 != follow(*itr).end(); itr3++)
			{
			ffs.at(i).at(2).insert(*itr2);
			}



			cout << "end first and start follow" << endl;
			//ffs.at(i).at(1) = first(*itr);
			//ffs.at(i).at(2).insert(follow(*itr).begin(), follow(*itr).end() );
			//ffs.at(i).at(2) = follow(*itr);
			*/
			i++;
		}

	}

	void displayFFS(vector< vector< unordered_set< string > > > x)
	{
		for (int i = 0; i < x.size(); i++)
		{
			for (int j = 0; j < x[i].size(); j++)
			{
				s->displaySet(x[i][j]);
				cout << "================" << endl;

			}
			cout << "##############################" << endl;
		}
	}

	string getFirstPart(string g)
	{
		string y = "";
		int e = 0;
		if (g[e] == '<')
		{
			while (g[e] != '>')
			{
				y += g[e];
				++e;
			}
			return y;
		}
		else
		{
			while (g[e] != '<' || !isspace(g[e]))
			{
				y += g[e];
			}
			return y;
		}
	}

	string* cutString(string x)
	{
		static string y[10];
		for (int i = 0; i < 10; i++)
		{
			y[i] = " ";
		}
		int itr = 0;
		string temp = "";
		for (int i = 0; i < x.length(); i++)//////////////////////////
		{
			if (x[i] == '<')
			{
				while (i < x.length() && x[i] != '>' && i < x.size())
				{
					temp += x[i];
					i++;
				}
				temp += x[i];
				y[itr] = temp;
				temp = "";
				itr++;
			}
			else
			{
				while (i < x.length() && x[i] != '<' || !isspace(x[i]))//////////////
				{
					temp += x[i];
					i++;
				}
				y[itr] = temp;
				temp = "";
				itr++;
				i--;
			}
		}
		return y;
	}

	//void createParsingTable()
	//{
	//	creatFirstFollowTable();
	//	tempSet.clear();
	//	int a;
	//	for (int i = Program; i != expr; i++)  // i is the non-terminal
	//	{
	//		tempSet = ffs[i][0]; ///////////////////////////////////////////////////////////////////////////////////////////////////
	//		unordered_set< string >::iterator itr;
	//		for (itr = tempSet.begin(); itr != tempSet.end(); itr++)
	//		{
	//			a = s->checkReserved(*itr); // convert from string to token ( integer )
	//			for (int q = 0; q < 23; q++)
	//			{
	//				string g = rules[q][1];
	//				g = getFirstPart(g);
	//				if (rules.at(q).at(0) == convertITOS(i) && first(g).find(*itr) != first(g).end())
	//				{
	//					Table.at(i).at(a) = rules.at(q).at(1);
	//					break;
	//				}
	//			}
	//		}
	//		if (tempSet.find("#") != tempSet.end())
	//		{
	//			tempSet = ffs[i][1];
	//			unordered_set< string >::iterator itr;
	//			for (itr = tempSet.begin(); itr != tempSet.end(); itr++)
	//			{
	//				a = s->checkReserved(*itr);
	//				for (int q = 0; q < 23; q++)
	//				{
	//					string g = rules[q][1];
	//					g = getFirstPart(g);
	//					if (rules.at(q).at(0) == convertITOS(i) && first(g).find(*itr) != first(g).end())
	//					{
	//						Table.at(i).at(a) = rules.at(q).at(1);
	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	void createParsingTable()
	{

		unordered_set< string > tempSet1 = { " " };
		tempSet1.clear();
		int x = 0;

		for (auto nonT : nonTerminal)
		{
			for (auto T : first(nonT))
			{
				for (int i = 0; i < rules.size(); i++)
				{
					if ( rules[i][0] == nonT && first( *cutStrings( rules[i][1] ).begin() ).find(T) != first(*cutStrings(rules[i][1]).begin() ).end() && T != "#" )
					{
						Table[x][terminalNo(T)] = rules[i][1] ;
						//break;
					}
				}	
			}
			if (first(nonT).find("#") != first(nonT).end())
			{
				for (auto f : follow(nonT))
				{
					for (int i = 0; i < rules.size(); i++)
					{
						if (rules[i][0] == nonT && rules[i][1] == "#" )
						{
							Table[x][terminalNo(f)] = rules[i][1];
							//break;
						}
					}
				}
			}
			x++;
		}
	}

	int terminalNo(string x)
	{
		if (x == "program")
		{
			return 1;
		}
		else if (x == "begin")
		{
			return 2;
		}
		else if (x == "end")
		{
			return 3;
		}
		else if (x == "proc")
		{
			return 4;
		}
		else if (x == "id")
		{
			return 5;
		}
		else if (x == "=")
		{
			return 6;
		}
		else if (x == ":=")
		{
			return 7;
		}
		else if (x == ";")
		{
			return 8;
		}
		else if (x == "(")
		{
			return 9;
		}
		else if (x == ")")
		{
			return 10;
		}
		else if (x == "call")
		{
			return 11;
		}
		else if (x == "int")
		{
			return 12;
		}
		else if (x == "bool")
		{
			return 13;
		}
		else if (x == "num")
		{
			return 14;
		}
		else if (x == "true")
		{
			return 15;
		}
		else if (x == "false")
		{
			return 16;
		}
		else if (x == "$")
		{
			return 17;
		}
	}

	non_terminals convert(int str)
	{
		if (str == 0) return Program;
		else if (str == 1) return declaration_seq;
		else if (str == 2) return declaration_tail;
		else if (str == 3) return declaration;
		else if (str == 4) return x;
		else if (str == 5) return declarer;
		else if (str == 6) return y;
		else if (str == 7) return statement_seq;
		else if (str == 8) return statement_tail;
		else if (str == 9) return statement;
		else if (str == 10) return z;
		else if (str == 12) return type;
		else if (str == 13) return expr;
	}

	non_terminals convert(string str)
	{
		if (str == "Program") return Program;
		else if (str == "declaration_seq") return declaration_seq;
		else if (str == "declaration_tail") return declaration_tail;
		else if (str == "declaration") return declaration;
		else if (str == "x") return x;
		else if (str == "declarer") return declarer;
		else if (str == "y") return y;
		else if (str == "statement_seq") return statement_seq;
		else if (str == "statement_tail") return statement_tail;
		else if (str == "statement") return statement;
		else if (str == "z") return z;
		else if (str == "type") return type;
		else if (str == "expr") return expr;
	}

	int convertNTOI(string str)
	{
		if (str == "Program") return 1;
		else if (str == "declaration_seq") return 2;
		else if (str == "declaration_tail") return 3;
		else if (str == "declaration") return 4;
		else if (str == "x") return 5;
		else if (str == "declarer") return 6;
		else if (str == "y") return 7;
		else if (str == "statement_seq") return 8;
		else if (str == "statement_tail") return 9;
		else if (str == "statement") return 10;
		else if (str == "z") return 11;
		else if (str == "type") return 12;
		else if (str == "expr") return 13;
	}

	Token convertSTOT(string str)
	{
		if (str == "Program") return PROGRAM_SY;
		else if (str == "begin") return BEGIN_SY;
		else if (str == "end") return END_SY;
		else if (str == "proc") return PROC_SY;
		else if (str == "=") return EQUAL_SY;
		else if (str == ":=") return ASSIGNMENT_SY;
		else if (str == ";") return SIMI_COLON_SY;
		else if (str == "(") return LEFT_PARNTHES_SY;
		else if (str == ")") return RIGHT_PARNTHES_SY;
		else if (str == "call") return CALL_SY;
		else if (str == "int") return INT_SY;
		else if (str == "bool") return BOOL_SY;
		else if (str == "num") return NUM_SY;
		else if (str == "true") return TRUE_SY;
		else if (str == "false") return FALSE_SY;
		else if (str == "$") return END_OF_FILE_SY;
		else return ID_SY;
	}

	string convertITOS(int str)
	{
		if (str == 0) return "Program";
		else if (str == 1) return "declaration_seq";
		else if (str == 2) return "declaration_tail";
		else if (str == 3) return "declaration";
		else if (str == 4) return "x";
		else if (str == 5) return "declarer";
		else if (str == 6) return "y";
		else if (str == 7) return "statement_seq";
		else if (str == 8) return "statement_tail";
		else if (str == 9) return "statement";
		else if (str == 10) return "z";
		else if (str == 12) return "type";
		else if (str == 13) return "expr";
		else { return "error"; }
	}

	void parser()
	{
		createParsingTable();

		string x;
		stack<string> st;
		st.push("$");
		st.push(*nonTerminal.begin());

		current_token = s->getToken();

		while (!st.empty())
		{
			x = st.top();

			if (terminal.find(x) != terminal.end())
			{
				match(convertSTOT(x));
				st.pop();
			}
			else if (nonTerminal.find(x) != nonTerminal.end()) // X is non-terminal
			{
				non_terminals z = convert(x);

				string s = Table[z][current_token];
				string* arr = cutString(s);
				st.pop();
				for (int i = 10; i >= 0; i--)
				{
					if (arr[i] == "@")
					{
						//escape
					}
					else
					{
						st.push(arr[i]);
					}
				}
			}
			else
			{
				s->syntaxError();
				//SyntaxError(current_token);
			}
		}
	}

};

void main()
{


	LL1 p;
	p.createParsingTable();
	scaner s;
	s.display2DVector(Table);

	system("pause");
}







