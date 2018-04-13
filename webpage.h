#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <string>
#include <set>
using namespace std;

class WebPage {
 public:
  /**
   * Default constructor
   */
  WebPage(string &x);

  /**
   * Destructor
   */
  ~WebPage();
 
 string get_name();

 void set_words(string &w);

 void set_incom(string n);
 void set_out(string m);
 //void printer(set <string> data);
 set <string> get_words();
 set<string> get_out();

set<string> get_incom();



private:
 
 set<string> words;

 set<string> incoming;
 set<string> outgoing;

 string name;
};
  

#endif