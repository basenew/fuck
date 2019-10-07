#include <iostream>

using namespace std;

class Base {
	public:
		virtual void f() {cout<<"base::f"<<endl;}
		virtual void g() {cout<<"base::g"<<endl;}
		virtual void h() {cout<<"base::h"<<endl;}
};

class Derive : public Base{
	public:
		void g() {cout<<"derive::g"<<endl;}
};


int main () {
	cout<<"size of Base: "<<sizeof(Base)<<endl;

	typedef void(*Func)(void);
	Base b;
	Base *d = new Derive();

	long* pvptr = (long*)d;
	long* vptr = (long*)*pvptr;
	Func f = (Func)vptr[0];
	Func g = (Func)vptr[1];
	Func h = (Func)vptr[2];

	f();
	g();
	h();

	return 0;
}
