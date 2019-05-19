#include <dirent.h>
#include <iostream>
#include<fstream>
#include<cassert>
#include<cstring>
#include<string>
#include <vector>
#include<list>
#include<stack>

using namespace std;

const char PATH_NAME[40] = "C:\\Users\\rv\\Desktop\\Files\\";

void parseFolder(vector<string>& namesOfFiles, const char* path_name)
{
    DIR *dir;
	dirent *ent;
	dir = opendir(path_name);
	assert(dir);
    ent = readdir(dir);
	while(ent!=nullptr)
    {
        namesOfFiles.push_back(ent->d_name);
        ent = readdir(dir);
    }
	closedir (dir);

	namesOfFiles.erase(namesOfFiles.begin(),namesOfFiles.begin()+2);

}


struct InvertedIndex
{
    string word;
    list<int> indices;

    InvertedIndex(const string& _word=" ", const list<int>& _indices={})
        :word(_word),indices(_indices)
    {}

};
bool operator>(const InvertedIndex& left, const InvertedIndex& right)
{
    if(left.word.compare(right.word)>0)
        return true;
    else
        return false;
}
bool operator<(const InvertedIndex& left, const InvertedIndex& right)
{
    if(left.word.compare(right.word)<0)
        return true;
    else
        return false;
}
bool operator<=(const InvertedIndex& left, const InvertedIndex& right)
{
    if(left.word.compare(right.word)<=0)
        return true;
    else
        return false;
}
bool operator>=(const InvertedIndex& left, const InvertedIndex& right)
{
    if(left.word.compare(right.word)>=0)
        return true;
    else
        return false;
}
ostream& operator<<(ostream& out, const InvertedIndex& idx)
{
    out<<"{"<<idx.word<<", "<<"{";
    for(auto it=idx.indices.begin();it!=idx.indices.end();it++)
    {
        out<<*it<<",";
    }
    cout<<"}}"<<endl;
}


//returns the index of the element before which we'll have to insert the new value
       // 0, if we have to push front
      // size, if we have to push back
int binarySearchInsert(const vector<InvertedIndex>& vec, const InvertedIndex& newValue)
{
    if(newValue<=vec[0])
        return 0;
    if(newValue>=vec[vec.size()-1])
        return vec.size();

    int low=0;
    int high=vec.size()-1;
    int mid;
    while(low<=high)
    {
        mid=(low+high)/2;
        if(newValue>=vec[mid]&&newValue<=vec[mid+1])
            return mid+1;
        else if(newValue>vec[mid])
            low=mid+1;
        else
            high=mid-1;
    }
}
void insertSorted(vector<InvertedIndex>& sorted, const InvertedIndex& newValue)
{
    if(sorted.empty())
    {
        sorted.push_back(newValue);
        return;
    }
    if(sorted.size()==1)
    {
        if(newValue>=sorted[0])
        {
            sorted.push_back(newValue);
            return;
        }
        else
        {
            sorted.insert(sorted.begin(),newValue);
            return;
        }
    }
    if(sorted.size()==2)
    {
        if(newValue>=sorted[sorted.size()-1])
        {
            sorted.push_back(newValue);
            return;
        }
        else if(newValue<=sorted[0])
        {
            sorted.insert(sorted.begin(),newValue);
            return;
        }
        else
        {
            sorted.insert(sorted.begin()+1,newValue);
            return;
        }
    }
    int pos=binarySearchInsert(sorted, newValue);
    if(pos==0)
    {
        sorted.insert(sorted.begin(),newValue);
        return;
    }
    else if(pos==sorted.size())
    {
        sorted.push_back(newValue);
        return;
    }
    else
    {
        sorted.insert(sorted.begin()+pos,newValue);
        return;
    }
}
void toLowerCase(string& word)
{
    for(int i=0;i<word.length();i++)
    {
        if(word[i]>='A'&& word[i]<='Z')
        {
            word[i]=(char)(word[i]+32);
        }
    }
}
int getIdx(const string& word,const vector<InvertedIndex>& words)
{
    for(int i=0;i<words.size();i++)
    {
        if(word==words[i].word)
            return i;
    }
    return -1;
}
bool isInIndices(int idx, const list<int>& indicies)
{
    for(auto it=indicies.begin();it!=indicies.end();it++)
    {
        if(idx==*it)
            return true;
    }
    return false;
}
void parseDocument(vector<InvertedIndex>& words, const vector<string>& namesOfFiles, const char* path_name)
{
    int currentFile=1;
    for(auto it=namesOfFiles.begin();it!=namesOfFiles.end();it++)
    {
        fstream file(path_name + *it);

        assert(file);

        string w;
        while(!file.eof())
        {
            file>>w;
            toLowerCase(w);
            int idx=getIdx(w,words);
            if(idx==-1)
            {
                InvertedIndex toAdd;
                toAdd.word=w;
                toAdd.indices.push_back(currentFile);
                insertSorted(words,toAdd);
            }
            else if(!isInIndices(currentFile,words[idx].indices))
            {
                words[idx].indices.push_back(currentFile);
            }

        }
        currentFile++;
    }
}


int binarySearch(const vector<InvertedIndex>& words, const string& word)
{
    int low=0;
    int high=words.size()-1;
    int mid;
    while(low<=high)
    {
        mid=(low+high)/2;
        if(word==words[mid].word)
            return mid;
        else if(word>words[mid])
        {
            low=mid+1;
        }
        else
        {
            high=mid-1;
        }
    }
    if(low>high)
    {
        return -1;
    }
}
void printFilesNamesFromIndices(const vector<string>& namesOfFiles,const list<int>& indices)
{
    for(auto it=indices.begin();it!=indices.end();it++)
    {
        cout<<namesOfFiles[*it-1]<<",";
    }
    cout<<endl;
}
void simplestQuery(const string& word, const vector<InvertedIndex>& words, const vector<string>& namesOfFiles)
{
    int pos=binarySearch(words,word);
    if(pos==-1)
        cout<<"No results found"<<endl;
    else
    {
        printFilesNamesFromIndices(namesOfFiles,words[pos].indices);
    }
}


void insertIntoSortedIntList(const int&  elem, list<int>& sorted)
{
    auto it = sorted.begin();
    for(it; it!=sorted.end() && *it < elem ;it++)
    {

    }
    if(*it>=elem)
    {
        sorted.insert(it,elem);
    }
    else
    {
        sorted.push_back(elem);
    }
}

list<int> operator+(const list<int>& left, const list<int>& right)
{
    list<int> result;
    for(auto it=left.begin();it!=left.end();it++)
    {
        result.push_back(*it);
    }
    for(auto it=right.begin();it!=right.end();it++)
    {
        if(!isInIndices(*it,result))
        {
            insertIntoSortedIntList(*it,result);

        }
    }

    return result;
}
list<int> operator*(const list<int>& left, const list<int>& right)
{
    list<int> result;
    for(auto it=left.begin();it!=left.end();it++)
    {
        if(isInIndices(*it,right))
        {
            result.push_back(*it);
        }
    }
    return result;
}
list<int> operator-(const list<int>& left, const list<int>& right)
{
    list<int> result;
    list<int> intersection = left * right;
    for(auto it = left.begin();it!=left.end();it++)
    {
        if(!isInIndices(*it,intersection))
        {
            result.push_back(*it);
        }
    }
    return result;
}

//Shunting Yard Algorithm
bool isOperation(char s)
{
    char operations[4]={'+','*','-'};
    for(unsigned i=0;i<strlen(operations);i++)
    {
        if(s==operations[i])
            return true;
    }
    return false;
}
int getPriority(char s)
{
    int result;
    switch(s)
    {
        case '*':result=1;break;
        case '+':
        case '-':result=0;break;
        default: result=-1;

    }
    return result;
}
//adds the white spaces to differentiate the words
void convertToRPN(const char* expression, char* rpn)
{
    stack<char> helper;
    helper.push('(');
    char x;
    int j=-1;
    for(unsigned i=0;i<strlen(expression);i++)
    {
        if(expression[i]>='a' && expression[i]<='z' || expression[i]==' ')
        {
            j++;
            rpn[j]=expression[i];
        }
        else if(expression[i]=='(')
        {
            helper.push(expression[i]);
        }
        else if(expression[i]==')')
        {

            x=helper.top();
            helper.pop();
            while(x!='(')
            {
                j++;
                rpn[j]=x;
                x=helper.top();
                helper.pop();
            }
        }
        else if(isOperation(expression[i]))
        {
            x=helper.top();
            helper.pop();
            while(x!='(' && getPriority(x)>=getPriority(expression[i]))
            {
                j++;
                rpn[j]=x;
                x=helper.top();
                helper.pop();
            }
            helper.push(x);helper.push(expression[i]);
        }
    }
    x=helper.top();
    helper.pop();
    while(x!='(')
    {
        j++;
        rpn[j]=x;
        x=helper.top();
        helper.pop();
    }
    j++;
    rpn[j]='\0';
}
//returns the final list of indexes of files which names should be printed
list<int> evaluate(const char* rpn, const vector<InvertedIndex>& words)
{

    stack<list<int>> lists;

    list<int> l;
    list<int> l1;
    list<int> l2;
    list<int> result;

    int i=0;
    int j;
    while(i<strlen(rpn))
    {
        //reading a word from the rpn-expression;
        if(rpn[i]>='a'&&rpn[i]<='z' || rpn[i]>='A' && rpn[i]<='Z')
        {
            string w;
            while(rpn[i]>='a'&&rpn[i]<='z' || rpn[i]>='A' && rpn[i]<='Z')
            {
                w+=rpn[i];
                i++;
            }
            toLowerCase(w);

            //searching for the word in the vector and adding it's list to the stack;
            //if the word is not found in the vector, it's list is empty;
            int pos=getIdx(w,words);
            if(pos==-1)
            {
                l={};
            }
            else
            {
                l=words[pos].indices;

            }
            lists.push(l);
        }

        else if(isOperation(rpn[i]))
        {
            l2=lists.top();
            lists.pop();
            l1=lists.top();
            lists.pop();
            switch(rpn[i])
            {
                case '+': result = l1 + l2; break;
                case '-': result = l1 - l2; break;
                case '*': result = l1 * l2; break;
            }
            lists.push(result);

            i++;
        }
        else
        {
            i++;
        }
    }
    return lists.top();
}
void advancedQuery(const char* expression, const vector<InvertedIndex>& words, const vector<string>& namesOfFiles)
{
    char rpn[40];
    convertToRPN(expression,rpn);
    list<int> l = evaluate(rpn,words);
    if(l.empty())
    {
        cout<<"No results found"<<endl;
    }
    else
    {
        printFilesNamesFromIndices(namesOfFiles, l);
    }
}

int main()
{
    //Building an inverted index

    //Parse Folder
    vector<string> namesOfFiles;
    parseFolder(namesOfFiles,PATH_NAME);
    cout<<"All files in current folder:"<<endl;
    for(int i=0;i<namesOfFiles.size();i++)
    {
        cout<<namesOfFiles[i]<<endl;
    }
    cout<<"----------------------------"<<endl;

    //Parse Document
    vector<InvertedIndex> words;
    parseDocument(words,namesOfFiles,PATH_NAME);
    cout<<"All words in files and their indices:"<<endl;
    for(int i=0;i<words.size();i++)
    {
        cout<<words[i];
    }
    cout<<"----------------------------"<<endl;

    //Queries

    //Simplest Query
    cout<<"In which files are the following words:"<<endl;
    cout<<"aa- > ";
    simplestQuery("aa",words,namesOfFiles);
    cout<<"cc- > ";
    simplestQuery("cc",words,namesOfFiles);
    cout<<"ee- > ";
    simplestQuery("ee",words,namesOfFiles);
    cout<<"----------------------------"<<endl;

    //Advanced Query
    char query[40];
    cout<<"Query > ";
    cin.getline(query,40);
    while(cin)
    {
        cout<<query<<" - > ";
        advancedQuery(query,words,namesOfFiles);
        cout<<endl;

        cout<<"Query > ";
        cin.getline(query,40);
    }

    return 0;
}
