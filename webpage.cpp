#include "webpage.h"
#include <string>
#include <set>
using namespace std;
#include <iostream>

WebPage::WebPage(string &x){
	name = x;
}

 
  
WebPage::~WebPage(){

}
 


string WebPage::get_name(){
	return name;
}

void WebPage::set_words(string &w){
	words.insert(w);
}

void WebPage::set_incom(string n){
	incoming.insert(n);
}

void WebPage::set_out(string m){
	outgoing.insert(m);
}


set<string> WebPage::get_words(){
	return words;
}

set<string> WebPage::get_out(){
	return outgoing;
}

set<string> WebPage::get_incom(){
	return incoming;
}