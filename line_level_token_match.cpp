//
// Created by Vikash Gilja on 4/21/20.
//

#include<fstream>
#include<strstream>
#include<iostream>
#include <unordered_map>
#include <string>
#include <regex>

using namespace std;

string ltrim(const string &s) {
	return regex_replace(s, regex("^\\s+"), string(""));
}

string rtrim(const string &s) {
	return regex_replace(s, regex("\\s+$"), string(""));
}

string trim(const string &s) {
	return ltrim(rtrim(s));
}

static unordered_map<int, string> leniencyMessages = {
		{0, ""},
		{1, "Your file has extra tokens"},
		{2, "Your file is missing some tokens"},
		{3, "Missing Paragraph Space(s): Partial Credit\n"},
		{4, "Extra Paragraph Space(s): Partial Credit\n"},
		{5, "Combination of Missing/Extra Paragraph Space(s): Partial Credit\n"},
};

// Open two files and compare, tokens should match on all lines
// return 0 if true, 1 otherwise
int main(int argc, const char * argv[]) {
    if (argc != 3) return 1;  // line_level_token_match file1 file2

    ifstream file1;
    ifstream file2;

    file1.open(argv[1]);
    file2.open(argv[2]);

    if (!file1.is_open() || !file2.is_open()) return 2;  // a file (or two) didn't open

    string line1, line2;
	int missingParagraphSpace = 0;
	while (!file1.eof() && !file2.eof()) {  // if both files have another line, iterate
        getline(file1, line1);
        getline(file2, line2);
		line1 = trim(line1);
		line2 = trim(line2);
        istrstream line_stream1(line1.c_str()), line_stream2(line2.c_str());
        string word1, word2;
		while (line1.empty() && !line2.empty() && !line_stream1.eof()) { // Allowing leniency for extra/missing newlines
			missingParagraphSpace += 4;
			getline(file1, line1);
			line_stream1.clear();
			line_stream1 = istrstream(line1.c_str());
		}
		while (!line1.empty() && line2.empty() && !line_stream2.eof()) {
			missingParagraphSpace += 3;
			getline(file2, line2);
			line_stream2.clear();
			line_stream2 = istrstream(line2.c_str());
		}
        while (!line_stream1.eof() && !line_stream2.eof()) { // if both lines have another word, iterate
            line_stream1 >> word1;
            line_stream2 >> word2;
            if (word1 != word2) 
            {   
                cout << "Token Mismatch" << endl;
                cout << "Line from 1: " << line1 << endl;
                cout << "Line from 2: " << line2 << endl;
                return 3; // word mismatch!
            }
        }
        
        line_stream1 >> ws;  // Consume potential whitespace at the end of lines
        line_stream2 >> ws;
        if (!line_stream1.eof() || !line_stream2.eof()) 
        {
            cout << "Token Count Mismatch" << endl;
            cout << "Line from 1: " << line1 << endl;
            cout << "Line from 2: " << line2 << endl;
            return 4; // lines do not have the same # of tokens
        }
    }
    if(!file1.eof() || !file2.eof()) 
    {
		while (!file1.eof()) {
			std::string extraToken;
			file1 >> extraToken;
			if (!extraToken.empty()) {
				cout << leniencyMessages[0] << endl;
				return 5; // files do not have the same # of lines
			}
		}
		while (!file2.eof()) {
			std::string extraToken;
			file2 >> extraToken;
			if (!extraToken.empty()) {
				cout << leniencyMessages[1] << endl;
				return 5; // files do not have the same # of lines
			}
		}
    }
	cout << leniencyMessages[std::min(missingParagraphSpace, (int) leniencyMessages.size() - 1)];
    return 0;
}
