#include<locale.h>
#include<langinfo.h>
#include <iostream>

std::string conv(const wchar_t* str, size_t len)
{
    if(!str){
		std::cout << ("input data null !") << std::endl;
        return "";
    }
    //setlocale(LC_ALL,"zh_CN.GB18030");
    setlocale(LC_ALL,"en_US.UTF-8");
	
    //setlocale(LC_ALL,"");
    size_t new_len = len * 2 ;
    char* mystr = new char[new_len];
    int n = std::wcstombs(mystr,str,new_len);
	std::cout << "after conv data lenth :" << n << std::endl;
    if(-1 == n){
		std::cout << ("convert error!") << std::endl;
        return "";
    }
    
    std::string s(mystr,n);
    return s;
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL,"chs");
	wchar_t wt[] = L"中国你好！";//大写字母L告诉编译器为"中"字分配两个字节的空间
	std::string str = conv(wt, sizeof(wt));
	return 0;
}
