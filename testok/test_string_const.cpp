#include <iostream>
#include <locale>

using namespace std;

int main()
{
	unsigned short ustr[] = {0xbde4, 0xe5a0};
    std::string str(char*(ustr));
	//std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
	//std::u16string u16 = {0x4f60, 0x597d, 0x5f20, 0x5c0f, 0x5e86};//conv.from_bytes(str);
	cout << "str:" << str << endl;
	string str2("你你你你你");
	cout << str2 << endl;
	const unsigned char* pstr2 = (const unsigned char*)(str2.data());
	for (int i = 0; i < str2.length(); i++)
	  cout << hex << pstr2[i] <<  " ";

	cout << endl;
	//cout << hex << pstr2[0] << " " << pstr2[1] << " " << pstr2[2] << " len:" << str2.length() << endl;
}
