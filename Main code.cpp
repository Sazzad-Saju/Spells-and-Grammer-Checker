#include<iostream>
#include<stdlib.h>
#include <fstream>
#include<string>
#include <conio.h>
#define DICTIONARY "dictionary.txt"
using namespace std;

/*
class implementing the tries data structure
*/

class tries
{
    bool value;     //marker to indicate leaf node
    tries* pointers[26];    //pointers to the various characters of english alphabet

public:
    tries()
    {
        //initialises all pointers as null
        value = 0;
        for(int i =0 ; i<26; i++)
            pointers[i] = NULL;
    }

    //This function adds a word to the trie
    void add(string s);

    //This function searches a word in the trie
    bool searchtries(string word, int i);

    //This function deletes a word from the trie
    void deletetries();

};
void tries::add(string s)
{
    tries* trav = this;
    for(int i = 0; i<s.length(); i++)
    {
        int pos = tolower(s[i]-'a');
        if(trav->pointers[pos]==NULL)
            trav->pointers[pos] = new tries;
        trav = trav->pointers[pos];
    }

    trav->value = true;
}

bool tries::searchtries(string word, int i)
{
    if(i==word.length())
        return value;
    if(pointers[tolower(word[i])-'a']==NULL)
        return false;
    return pointers[tolower(word[i])-'a']->searchtries(word,i+1);
}

void tries::deletetries()
{
    for(int i = 0; i<26; i++)
    {
        if(pointers[i]!=NULL)
            pointers[i]->deletetries();
    }
    delete this;
}
//A class representing dictionary
class dictionary
{
public:
    char* dict; //contains the file name for text file containing the dictionary words
    tries* root;    //a pointer to the trie
    dictionary(char* file)
    {
        dict = file;
        root = NULL;
    }

    //This is used to load the words from the file to memory. Returns true if the operation is successful, else false
    bool load();

    //This is used to free the memory used by the trie data structure
    void unload();

    //This checks the given word in the dictionary file and returns true if present else false;
    bool check(string word);
};


bool dictionary::load()
{
    fstream f;
    f.open(dict,ios::in);
    if(!f.is_open())
    {
        return false;
    }

    root = new tries;
    string str;

    while(!f.eof())
    {
        f>>str;
        root->add(str);
    }

    f.close();
    return true;
}

bool dictionary::check(string word)
{
	int siz;
	siz = word.size();
	//if(word[siz-1]=='!' || word[siz-1]=='?' || word[siz-1]=='.' || word[siz-1]==',' || word[siz-1]==';' || word[siz-1]=='-' || word[siz-1]==':' || word[siz-1]=='_'){
	if(isalpha(word[siz-1])==0){
		word.erase(siz-1,siz);
	}
    return root->searchtries(word,0);
}


void dictionary::unload(void)
{
    root->deletetries();
}
string rmv_sp(string str)
{
	int i = 0, j = 0;
	while (i<str.size())
	{
		if (str[i] == ' ' && str[i+1] == ' '){
			i++;
		}
		else{
			str[j++] = str[i];
			i++;
		}
	}
	//cout<<j<<endl;
	str.erase(j,str.size());
	return str;
}

string gmr_chk(string str){
	for(int i=0; i<str.size();i++){
		if(islower(str[0])){
			str[0]= toupper(str[0]);
		}
		if(str[i]== '!' || str[i]=='.' || str[i]=='?'){
			if(str[i+1] != ' '){
				str.insert(i+1," ");
				if(islower(str[i+2])){
					str[i+2]=toupper(str[i+2]);
				}
			}
			else if(str[i+1]==' '){
				if(islower(str[i+2])){
					str[i+2]=toupper(str[i+2]);
				}
			}
		}
		if(str[i]==',' || str[i]==';' || str[i]==':' || str[i]=='-'){
			if(str[i+1] != ' '){
				str.insert(i+1," ");
				if(isupper(str[i+2])){
					str[i+2]=tolower(str[i+2]);
				}
			}
			if(str[i+1]==' '){
				if(isupper(str[i+2])){
					str[i+2]=tolower(str[i+2]);
				}
			}
		}
	}
	return str;
}
void checkfile()
{
    system("cls");

    //loading the dictionary from file in the memory
    dictionary d(DICTIONARY);

    if(!d.load())
    {
        cout<<"\n\tcould not load dictionary\n";
        getch();
        return;
    }

    //grammer checkment by saju
    cout<<"\n\tEnter the name of file: ";
    string fname;
    cin>>fname;
	ifstream myfile (fname.c_str());
 	string line,str="";
    if (myfile.is_open())
   	{
    	while ( getline (myfile,line) )
    	{
    	    str=str+line+" ";
    	}
    myfile.close();
   	}
   	else
	   cout << "\tUnable to open file";
	cout<<"\tFile Cn: "<<str<<endl;
	str = rmv_sp(str);
	str = gmr_chk(str);
	cout<<"\tGmr ckp: "<<str<<endl;

	//
    cout<<"\n\tMISSPELLED WORDS:\n\t";

    int i=0;
    string word="";
    while(i<str.size()){
  	if(str[i]!=' '){
  		word = word+str[i];
	  }
	else{
			if(word.size() != 0){
				bool misspelled = !d.check(word);
        		if(misspelled)
            		cout<<word<<"\n\t";
			}
			word="";
		}
  	i++;
  }
    getch();
}

/*
This function saves the file typed by user. User signals the end of file by typing EXIT
*/

void writefile()
{
    system("cls");
    char filename[80];
  	fstream f;
  	char flag;
  	cout<<"\n\tEnter filename: ";
  	cin>>filename;
  	f.open(filename,ios::in);
  	if(f.is_open()){
  		cout<<"\n\talready there, replace? (y/n): ";
  		//cin>>flag;
  		flag = getch();
  		f.close();
  	}
  	if(flag=='n'){
  		return;
	}
	else{
  		f.open(filename,ios::out);
  		string word;
  		cout<<"\n\t(Type word/Type EXIT to stop)\n\t==> ";
  		cin>>word;
  		while(word.compare("EXIT")){
  			f<<word<<" ";
  			cin>>word;
  		}
  		f.close();
  		cout<<"\n\tsaved!"<<endl;
	}
    getch();
}

/*
This funciton adds a word to the dictionary
*/

void addword()
{
    system("cls");
    cout<<"\n\tEnter the word you wish to add\n\t==> ";
    string word;
    cin>>word;

    ofstream f;
    f.open(DICTIONARY,ios::app);
    if(!f.is_open())
        cout<<"\n\tcould not open"<<DICTIONARY;
    f<<word<<"\n";

    cout<<"\n\tAdded!";
    getch();
}

int main()
{
    /*The program gives a menu to user where he/she can choose either to
      check an existing file,
      create a new file or
      to add a word in the dictionary
    */
    while(1)
    {
        system("cls");
        cout<<"\n\tEnter your choice\n";
        cout<<"\t1. Check a file\n\t2. Write a file\n\t3. Add word to dictionary\n\t4. Exit\n";

        char  choice;
		cout<<"\n\t==> ";
        choice = getch();
        switch(choice)
        {
        case '1':
            checkfile();
            break;
        case '2':
            writefile();
            break;
        case '3':
            addword();
            break;
        case '4':
            return 0;
        }
    }

    return 0;
}
