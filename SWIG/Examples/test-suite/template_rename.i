%module template_rename

%warnfilter(801) Foo<int>;    /* Ruby, wrong class name */
%warnfilter(801) Foo<double>; /* Ruby, wrong class name */

%rename(blah_test) Foo::blah(int);
%rename(spam_test) Foo<int>::spam(int);
%rename(grok_test) Foo::grok(int);
%rename(groki_test) Foo<int>::grok(int);

%inline %{

template<class T> class Foo {
public:
     int blah(int x) { return x; }
     int spam(int x) { return x; }
     int grok(int x) { return x; }
};

%}

%template(iFoo) Foo<int>;
%template(dFoo) Foo<double>;

