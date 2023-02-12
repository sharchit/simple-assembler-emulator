#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

/*
    2. deal with error in removeLabels
    3. error in test2.asm
*/

vector<string> error;
void raiseError(int code, string label)
{
    /*
        0- more than 1 arg passed
        1- 1 argument but unknown mnemonic
        2- 0 argument but unknown mnemonic
        3- bogus label name
        4- Invalid number
        5- Invalid label
        6- Duplicate label
    */
    string temp;
    if(code == 0)
    {
        temp = "ERROR: Unknown mnemonic " + label + " found with more than one argument\n";
        error.push_back(temp);
    }
    else if(code == 1)
    {
        temp = "ERROR: Unknown mnemonic " + label + " found. With one argument\n";
        error.push_back(temp);
    }
    else if(code == 2)
    {
        temp = "ERROR: Unknown mnemonic " + label + "  found. With no argument\n";
        error.push_back(temp);
    }
    else if (code == 3)
    {
        temp = "ERROR: Bogus Label Name " + label + "\n";
        error.push_back(temp);
    }
    else if (code == 4)
    {
        temp = "ERROR: Invalid Number Entered " + label + "\n";
        error.push_back(temp);
    }
    else if(code == 5)
    {
        temp = "ERROR: No such label " + label + " exists\n";
        error.push_back(temp);
    }
    else if(code == 6)
    {
        temp = "ERROR: Duplicate label " + label + " found\n";
        error.push_back(temp);
    }
}

vector<string> getWords(string s)
{
    vector<string> words;
    s += '\n';
    string word = "";
    for(char x: s)
    {
        if(x == ';')
        {
            if(word != "") words.push_back(word);
            break;
        }

        if(x == '\t' || x == ' ' || x == ':' || x == '\n' || x == ',')
        {
            if(word != "") words.push_back(word);
            word = "";
        }

        if(x != ' ' && x != '\t' && x != ',')
            word += x;
    }
    return words;
}

bool validLabel(string label)
{
    if(isalpha(label[0]))
    {
        for(int i = 1; i < label.length(); i++)
        {
            if(!iswalnum(label[i])) return false;
        }
        return true;
    }
    return false;
}

unordered_map <string, int> labelTable;
unordered_set<string> labelSet;

bool checkValidNumber(string n);

int numericalNumber(string n)
{
    int value;
    value = strtol(n.c_str(), NULL, 0);
    return value;
}

vector<string> removeLabels(vector<string> line, int index)
{
    if(*(line.begin() + 1) == ":")
    {
        vector<string> instructions;
        if(validLabel(line[0]))
        {
            if(labelTable.find(line[0]) == labelTable.end())
            {
                if(line[2] == "SET") labelTable[*(line.begin())] = numericalNumber(line[3]);
                else
                    labelTable[*(line.begin())] = index;
                
                labelSet.insert(line[0]);

            }
            else
                raiseError(6, line[0]);
        }
        else
        {
            raiseError(3, line[0]);
        }
        for(auto it = line.begin() + 2; it != line.end(); it++)
        {
            instructions.push_back(*it);
        }
        return instructions;
    }
    else return line;
}

string tohex(int n)
{
    int i = 0;
    string arr = "000000";
    while(n != 0)
    {
        int temp = 0;
        temp = n % 16;
        if(temp < 10)
        {
            arr[5-i] = temp + 48;
            i++;
        }
        else
        {
            arr[5-i] = temp + 55;
            i++;
        }
        n = n/16;
   }
   return arr;
}

bool checkValidNumber(string n)
{
    transform(n.begin(), n.end(), n.begin(), ::tolower);
    if(n[0] == '0')
    {
        if(n[1] == 'x')
        {   
            for(int i = 2; i < n.length(); i++)
            {
                if(!((n[i] >= '0' && n[i] <= '9') || (n[i] >= 'a' && n[i] <= 'f')))
                {
                    raiseError(4, n);
                    return false;
                }
            }
            return true;
        }
        else
        {
            for(int i = 0; i < n.length(); i++)
            {
                if(!(n[i] >= '0' && n[i] <= '7'))
                {
                    raiseError(4, n);
                    return false;
                }
            }
            return true;
        }
    }
    
    for(int i = 0; i < n.length(); i++)
    {
        if(!(n[i] >= '0' && n[i] <= '9'))
        {
            return false;
        }
    }
    return true;
}

string readNumbers(string n)
{
    ll value;
    value = strtol(n.c_str(), NULL, 0);
    if(value >= 0)
    {
        return tohex(value);
    }
    else
    {
        value = 16777216 + value;
        return tohex(value);
    }
    return "000000";
}

string readNumberOrLabel(string value, int pc)
{
    if(checkValidNumber(value))
        return readNumbers(value);
    else
    {
        if(labelTable.find(value) != labelTable.end()) return readNumbers(to_string(labelTable[value] - pc - 1));
        else
        {
            if(validLabel(value)) raiseError(5, value);
        }
    }
    return "000000";
}

string readLabels(string value, int pc)
{
    if(checkValidNumber(value))
        return readNumbers(value);
    else
    {
        if(labelTable.find(value) != labelTable.end()) return readNumbers(to_string(labelTable[value]));
        else
        {
            if(validLabel(value)) raiseError(5, value);
        }
    }
    return "000000";
}

string getInstruction(vector<string> line, int pc)
{
    if(line.size() == 2)
    {
        string mnemonic, value, opcode, operand;
        mnemonic = *line.begin();
        value = *(line.begin() + 1);

        if(mnemonic == "ldc")
        {
            opcode = "00";
            operand = readLabels(value, pc);
            return operand + opcode;
        }
        
        else if (mnemonic == "adc")
        {
            opcode = "01";
            operand = readNumberOrLabel(value, pc);
            return operand + opcode;
        }

        else if(mnemonic == "ldl")
        {
            opcode = "02";
            operand = readNumberOrLabel(value, pc);
            return operand + opcode;
        }

        else if(mnemonic == "stl")
        {
            opcode = "03";
            operand = readNumberOrLabel(value, pc);
            return operand + opcode;
        }

        else if(mnemonic == "ldnl")
        {
            opcode = "04";
            operand = readNumberOrLabel(value, pc);
            return operand + opcode;
        }

        else if(mnemonic == "stnl")
        {
            opcode = "05";
            operand = readNumberOrLabel(value, pc);
            return operand + opcode;
        }

        else if(mnemonic == "adj")
        {
            opcode = "0A";
            operand = readNumberOrLabel(value, pc);
            return operand + opcode;
        }

        else if(mnemonic == "call")
        {
            opcode = "0D";
            operand = readNumberOrLabel(value, pc);
            return operand + opcode;
        }

        else if(mnemonic == "brz")
        {
            opcode = "0F";
            operand = readNumberOrLabel(value, pc);
            return operand + opcode;
        }

        else if(mnemonic == "brlz")
        {
            opcode = "10";
            operand = readNumberOrLabel(value, pc);
            return operand + opcode;
        }

        else if(mnemonic == "br")
        {
            opcode = "11";
            operand = readNumberOrLabel(value, pc);
            return operand + opcode;
        }
        
        else if(mnemonic == "data")
        {
            opcode = "00";
            operand = readNumberOrLabel(value, pc);
            return opcode + operand;
        }
        else if(mnemonic == "SET")
        {
            opcode = "00";
            operand = readNumberOrLabel(to_string(numericalNumber(value)), pc);
            return opcode + operand;
        }
        else raiseError(1, mnemonic);
    }

    else if (line.size() == 1)
    {
        string mnemonic, value, opcode, operand = "000000";
        mnemonic = *line.begin();

        if(mnemonic == "add")
        {
            opcode = "06";
            return operand + opcode;
        }
        
        else if (mnemonic == "sub")
        {
            opcode = "07";
            return operand + opcode;
        }
        
        else if (mnemonic == "shl")
        {
            opcode = "08";
            return operand + opcode;
        }

        else if (mnemonic == "shr")
        {
            opcode = "09";
            return operand + opcode;
        }

        else if (mnemonic == "a2sp")
        {
            opcode = "0B";
            return operand + opcode;
        }

        else if (mnemonic == "sp2a")
        {
            opcode = "0C";
            return operand + opcode;
        }

        else if (mnemonic == "return")
        {
            opcode = "0E";
            return operand + opcode;
        }

        else if(mnemonic == "HALT")
        {
            opcode = "12";
            return operand + opcode;
        }
        else raiseError(2, mnemonic);
    }
    
    else if(line.size()) raiseError(0, *line.begin());
    return "";
}

int main(int argc, char** argv)
{
    string filename;
    if(argc == 2)
    {
        filename =  argv[1];
    }
    else
    {
        cout<<"ASSEMBLY FAILED: Invalid number of arguments passed ("<<argc - 1<<") Only the file name is to be entered."<<endl;
        exit(1);
    }
    ifstream in(filename);
    if(!in.is_open())
    {
        cout<<"ASSEMBLY FAILED: No file named \""<<filename<<"\" exists."<<endl;
        exit(1);
    }
    cout<<"FILE OPENED SUCCESSFULLY"<<endl;
    vector<vector <string>> parsedLines;
    while(in.eof() == 0)
    {
        string code, comment;
        getline(in, code);
        vector<string> words = getWords(code);
        if(words.size()) parsedLines.push_back(words);
    }
    cout<<"FILE PARSED SUCCESSFULLY"<<endl;
    in.close();

    vector<vector<string>> inst;
    int index = 0;
    for(auto line: parsedLines)
    {
        inst.push_back(removeLabels(line, index));
        index++;
    }
    cout<<"LABELS EXAMINED SUCCESSFULLY"<<endl;

    int pc = 0;
    int i = 0;
    string outputCode = "";
    for(auto v: inst)
    {
        outputCode += "00" + readNumbers(to_string(pc)) + " ";
        outputCode += getInstruction(v, pc) + " ";
        for(auto s: parsedLines[i])
            outputCode += s + " ";
        outputCode += "\n";
        if(v.size())pc++;
        i++;
    }
    cout<<"OUTPUT CODE SUCCESSFULLY GENERATED"<<endl;

    string errorString = "; NO ERROR FOUND\n";
    if(error.size() != 0)
    {
        errorString = "";
        for(auto s: error)
            errorString += s;
    }
    cout<<"ERROR CODE GENERATED"<<endl;

    string outputFileName = "", errorFileName;
    for(int i = 0; i < filename.length(); i++)
    {
        
        if(filename[i] == '.')
            break;
        else
        
            outputFileName += filename[i];
    }

    errorFileName = outputFileName + ".log";
    outputFileName += ".lst";
    cout<<"FILE NAMES GENERATED"<<endl;
    
    ofstream out(outputFileName);
    out<<outputCode;
    out.close();

    ofstream outError(errorFileName);
    outError<<errorString;
    outError.close();

    return 0;
}