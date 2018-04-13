#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
#include "webpage.h"
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include<vector>
#include<algorithm>

struct Comp{
	bool operator()(const pair<string,double>& a, const pair<string,double>& b) {
		return a.second > b.second;
	}
} comp;




string makeLower(string &w);
set<string> intersection(set<string> &a, set<string> &b, 
	map<string, WebPage*>  &webMap, double &t, double &e, ofstream &output);
void  printer(set <string> &print, ofstream &output);
void collectData(map<string, set<WebPage*> > &data, string &index,string &line, 
	set<string> &webnames,set<WebPage*> &content, string &token, 
	map<string, WebPage*>  &webMap);
void readQuery(map<string, set<WebPage*> > &data, string &query,string &line, 
	set<string> &webnames,set<WebPage*> &content, string &token, ofstream &output, 
	map<string, WebPage*>  &webMap, double &t, double &e);

void config(string &configFile, string &index, string &query, string &out, 
	double &t, double &e);
void addInOut(string link, set<string> &result, map<string, WebPage*>  &webMap);
void pageRank(set<string> &result, double &t, double &e, 
	map<string, WebPage*>  &webMap, ofstream &output);


int main(int argc, char *argv[]) {

	string configFile = "";
	string index;
    string out = "";
	string query = "";
	double t = 0;
	double e = 0;

	if(argc >= 1) {
	configFile = argv[1];

	}else{
		configFile = "config.txt";
	}

    config(configFile, index, query, out, t, e);
   
	ofstream output(out.c_str());	
	set<WebPage*> content;
	set<string> webnames;
	string line;
	string sentence;
	map<string, set<WebPage*> > data;
	string token = "";
	string newLine = "";
	set<WebPage*>:: iterator it;
	map<string, WebPage*>  webMap;
	

	

	collectData(data, index, line, webnames,content, token, webMap);
    readQuery(data, query,line, webnames,content, token, 
    	output, webMap, t, e);

    for(it = content.begin(); it != content.end(); ++it){
    	delete *it;
    }
	return 0;
}

void config(string &configFile, string &index, string &query, 
	string &out, double &t, double &e) {
	string phrase ="";
	string parametr;
	string temp;
	string sentence = "";
	bool flag = true;

	ifstream in_file(configFile.c_str());
	while(getline(in_file, sentence)){
		stringstream ss (sentence);
		flag=true;
		while(ss >> phrase  && flag){
            
			for(size_t i=0; i<phrase.size(); i++){
				if(phrase[i] != '#' && phrase[i] != '='){
					temp += phrase[i];
				}

				else if( phrase[i] == '='){
					parametr = temp;
					temp = "";					
				}
				else if(phrase[i] == '#'){
					if(temp.size() != 0 && parametr.size() != 0){
						if(parametr == "INDEX_FILE"){
                           index = temp;
                           
						}else if(parametr == "QUERY_FILE"){
							query = temp;
						}else if(parametr == "OUTPUT_FILE"){
							out = temp;
						}else if(parametr == "STEP_NUMBER"){
							t = atof(temp.c_str());
							
						    
						}else if(parametr == "RESTART_PROBABILITY"){
						    e = atof(temp.c_str());
							
						}

						temp = "";
                        parametr = "";
					}
					flag = false;
					i=phrase.size();
				}								
			}

			if(temp.size() != 0 && parametr.size() != 0){
				if(parametr == "INDEX_FILE"){
                    index = temp;
                       
				}else if(parametr == "QUERY_FILE"){
					query = temp;
				}else if(parametr == "OUTPUT_FILE"){
					out = temp;						
				}else if(parametr == "STEP_NUMBER"){
					t = atof(temp.c_str());

				}else if(parametr == "RESTART_PROBABILITY"){
				    e = atof(temp.c_str());
					
				}

				temp = "";
                parametr = "";
			}
		}
	}	
}

void collectData(map<string, set<WebPage*> > &data, string &index,
	string &line, set<string> &webnames,set<WebPage*> &content, 
	string &token, map<string, WebPage*>  &webMap){
	data.clear();
	ifstream file(index. c_str());
	set<WebPage*>:: iterator it;
	set<WebPage*>:: iterator it_2;
	string webName = "";
	string newLine = "";
	string word = "";
	bool flag = true;
	

	while(getline(file, line)){
		WebPage* Newline = new WebPage(line);
		webMap[line] = Newline;

		content.insert(Newline);
		webnames.insert(line);
	}
	
	for(it = content.begin(); it != content.end(); ++it){
				
		webName = (*it) -> get_name();		
		ifstream file(webName. c_str());
		while(getline(file, newLine)){
			stringstream ss (newLine);
			while(ss >> token && flag) {
				if(word.size() != 0){
					makeLower(word);					
					(*it) -> set_words(word);
					if(data.find(word) == data.end()){
						set<WebPage*> val;
						val.clear();
						data.insert(make_pair(word, val));
					}					
					data[word].insert(*it);
					word = "";
				}

                for(size_t i = 0; i < token.size(); i++){
					
					if(isalpha(token[i]) || isdigit(token[i])){
                    	word += token[i];
					}

					if(!isalpha(token[i]) && !isdigit(token[i])){
						if(word.size() != 0){
							makeLower(word);					
							(*it) -> set_words(word);
							if(data.find(word) == data.end()){
								set<WebPage*> val;
								val.clear();
								data.insert(make_pair(word, val));
							}
							data[word].insert(*it);
							word = "";
						}	
					}

					if (token[i] == '[') {
						i++;
						while(token[i] != ']') {
							if (i >= token.length()) {
								if (word != "") {
									makeLower(word);					
									(*it) -> set_words(word);
									if(data.find(word) == data.end()){
										set<WebPage*> val;
										val.clear();
										data.insert(make_pair(word, val));
										data[word].insert(*it);
										word = "";
									}
								}
								ss >> token;
								i = 0;
							}
							if (token[i] != ']' && !isalpha(token[i]) && 
								!isdigit(token[i]) && word != "") {
								
								makeLower(word);					
								(*it) -> set_words(word);
								if(data.find(word) == data.end()){
									set<WebPage*> val;
									val.clear();
									data.insert(make_pair(word, val));
									data[word].insert(*it);

									word = "";
								}										
							} else if (token[i] != ']') {
								word += token[i];
							}

							if (token[i] != ']') {
								i++;
							}
						}
					} else if(!isalpha(token[i]) && !isdigit(token[i]) && token[i] != '(' 
						&& token[i] != '(' && token[i] != ')'){
                    	makeLower(word);					
						(*it) -> set_words(word);
						if(data.find(word) == data.end()){
							set<WebPage*> val;
							val.clear();
							data.insert(make_pair(word, val));
						}
						data[word].insert(*it);
						word = "";
					}
                          
					if(token[i] == '(') {
						i++;
						while(token[i] != ')'){

							word += token[i];
							i++;
						}
					    (*it) -> set_out(word);

					    for(it_2 = content.begin(); it_2 != content.end(); ++it_2){
					    	size_t found = (*it_2)->get_name().rfind(word);
					    	if(found != string::npos){
					    		(*it_2) -> set_incom(webName);
					    		break;
					    	}
				        }
				        word = "";
			        }		
                }
			}


			if(word.size() != 0){
            	makeLower(word);					
				(*it) -> set_words(word);
				if(data.find(word) == data.end()){
					set<WebPage*> val;
					val.clear();
					data.insert(make_pair(word, val));
				}
				
				data[word].insert(*it);
				word = "";
			}            
		}
	}
}


void readQuery(map<string, set<WebPage*> > &data, string &query,string &line, 
	set<string> &webnames,set<WebPage*> &content, string &token, 
	ofstream &output, map<string, WebPage*>  &webMap,double &t, double &e){

	string command = "";
    string wName = "";  
    string newLine = ""; 
	ifstream file(query. c_str());
	set<string>:: iterator It;
	size_t found;
	set<WebPage*> name;
	set<string> page;
	set<string> result;
	int count = 0;
	int ch = 0;
	string first = "";
	string second = "";
	set<WebPage*>:: iterator iT;
	bool check = false;
	string tem = "";
	bool in = false;
	set<WebPage*>:: iterator it;
    bool invalid_query;
    string extra;


	while(getline(file, command)){
		result.clear();
		page.clear();
		check = false;
		stringstream ss (command);
		invalid_query = false;
		while(ss >> token && !invalid_query){
            
			if(token == "PRINT"){
			    tem = token;
				if(ss >> token){
					
					const bool is_in = (webnames.find(token) != webnames.end());
					if(is_in){
						output<< token <<endl;

				    	ifstream file(token. c_str());
				    	while(getline(file, newLine)){
							stringstream ss (newLine);
							found = newLine.find('(');
							while(found != string::npos){
								found = newLine.find(')');
								count= newLine.find(')', newLine.find(')'));
								ch= newLine.find('(', newLine.find('('));
								newLine.erase(newLine.begin()+ch, newLine.begin()+count+1);

								
								count =0;
								ch= 0;
								found = newLine.find('('); 
							}
	  						output <<newLine<<endl;												    					    
					    }
					} else{
						output << "Invalid query"<<endl;
						invalid_query = true;
					}  
				} else {
					
	        		makeLower(tem);
	        		name = data[tem];
	        		
	        		for(iT = name.begin(); iT != name.end(); ++iT){
	        			page.insert((*iT)->get_name());
	        		}
                    output <<page.size() <<endl;
		        		
                    for(It = page.begin(); It != page.end(); ++It){
		        		
		        		output<< *It <<endl;	        		
	        		} 
				}  
				page.clear();
	        }

	        else if(token == "OR"){
	        	string temp = token;

	        	while(ss >> token){
	        		check = true;
	        		
	        		makeLower(token);
	        		name = data[token];
	        		for(iT = name.begin(); iT != name.end(); ++iT){
	        			page.insert((*iT)->get_name());	        			
	        		}                    	                  
	        	} 
                if(!check){
		        		            	
	        		makeLower(temp);
	        		name = data[temp];
	        		
	        		for(iT = name.begin(); iT != name.end(); ++iT){
	        			page.insert((*iT)->get_name());	        			
	        		}
	            } 

	            pageRank(page, t,e, webMap, output); 
	        	page.clear();
	        }

	        else if(token == "AND"){
	        	count = 0;
	        	string temporary = token;

	        	while(ss >> token){
	        		check = true;

	        		makeLower(token);
	        		count++;
	                
   					name = data[token];
	        		for(iT = name.begin(); iT != name.end(); ++iT){
	        			page.insert((*iT)->get_name());	        			
	        		}

					if(count == 1){
		        		result = page;	 
		        	} else {
		        		result = intersection(result, page, webMap, t, e, output);		        		    		
		        	}
		        	page.clear();
		        		    	        		
	        	}

	        	if(!check){
	                    	
	        		makeLower(temporary);
	        		name = data[temporary];
	        		
	        		for(iT = name.begin(); iT != name.end(); ++iT){
	        			page.insert((*iT)->get_name());
	        			extra = (*iT)->get_name();
	        			
	        		}
                    pageRank(page, t, e, webMap, output);

	            }


	        	else if(result.size() != 0){


	        		set<string>::iterator x;
		        	pageRank(result, t, e, webMap, output);	
		        }else{
		        	output << result.size() << endl;

		        }
		        page.clear(); 

	        } else if(token == "INCOMING"){
	        	tem = token;
	        	if(ss >> token){
	        		if(ss >> token){
	        			output<< "Invalid query"<<endl;
	        			invalid_query = true;

	        		}else{
			        	for(it = content.begin(); it != content.end(); ++it){
			        		if((*it)->get_name() == token){
			        			
			        			page = (*it)->get_incom();
			        			printer(page, output);
			        			break;
			        		}
			        	}
			        }
		        } else {
		        	makeLower(tem);
	        		name = data[tem];
	        		
	        		for(iT = name.begin(); iT != name.end(); ++iT){
	        			page.insert((*iT)->get_name());
	        		}
                    output<<page.size()<<endl;

                    
                    for(It = page.begin(); It != page.end(); ++It){
                    		        		
		        		output<< *It <<endl;	        		
	        		} 

	        		page.clear();
		        }
	        }	
	        else if (token == "OUTGOING"){
	        	tem = token;
	        	if(ss >> token){
		        	for(it = content.begin(); it != content.end(); ++it){
		        		if((*it)->get_name() == token){

		        			page = (*it)->get_out();
			        		printer(page, output);
		        			break;
		        		}
		        	}
		        } else {
		        	makeLower(tem);
	        		name = data[tem];
	        		
	        		for(iT = name.begin(); iT != name.end(); ++iT){
	        			page.insert((*iT)->get_name());
	        		}
                    output<<page.size()<<endl;
		        		
                    for(It = page.begin(); It != page.end(); ++It){
                    	
		        		
		        		output<< *It <<endl;
	        		
	        		} 
	        		page.clear();

		        }

	        } else{
	        	if( ss >> token){
	        		output<<"Invalid query"<<endl;
	        		invalid_query = true;
	        	} else{
	        		makeLower(token);
	        		in = (data.find(token) != data.end());
					if(in){

		        		name = data[token];
		        		
		        		for(iT = name.begin(); iT != name.end(); ++iT){
		        			page.insert((*iT)->get_name());		        			
		     		    }

		        		pageRank(page, t, e, webMap, output);	                 	         
		        	}else {

		        		output << "0"<<endl;
		        	}

	        	}
	        	page.clear();
	        }       
	    }
    }
}

string makeLower(string &w){
	for(unsigned int i = 0; i < w.length(); ++i) {
	    w[i] = tolower(w[i]);
	}
	return w;
}

set<string> intersection(set<string> &a, set<string> &b, 
	map<string, WebPage*>  &webMap, double &t, double &e, ofstream &output){
	set<string> result;
	result.clear();
	set<string>:: iterator x;
	string link;
	

	for(x = a.begin(); x != a.end(); ++x){
		if (b.find(*x) != b.end()) {
			result.insert((*x));
			link = *x;		
		}
	}

	return result;
}


void  printer(set <string> &print, ofstream &output){
	
	
	set<string>:: iterator k;
	output<<print.size()<<endl;
	
	for(k = print.begin(); k != print.end(); ++k){
		
		output<<*k<<endl;
	}
	print.clear();
}

void addInOut(string link, set<string> &result, 
	map<string, WebPage*>  &webMap){

   set<string> addition;
   set<string>:: iterator k;
    
	addition = webMap[link]->get_incom();
	for(k = addition.begin(); k != addition.end(); ++k){
		result.insert(*k);
	}

	addition.clear();
	addition = webMap[link] ->get_out();
	for(k = addition.begin(); k != addition.end(); ++k){
		result.insert(*k);
	}
}

<<<<<<< HEAD


=======
>>>>>>> bded0d04ffa0ba41b40a5ddd0f79047b28c7fc34
void pageRank(set<string> &result,double &t, double &e, 
	map<string, WebPage*>  &webMap, ofstream &output){
	map<string, double> origMap;
	map<string, double> updateMap;
	set<string>:: iterator k;
	set<string>:: iterator l;
	set<string>:: iterator m;
	map<string, double>:: iterator it;
	double n = result.size();
	double rank = 0;
	set<string> incom;
	set<string> outgo;
	double sum =0;
	double total = 0;
	int count =0;
	set<string> forNode;
	double nodeRank = 0;
	vector<pair<string, double> > mapVector;
	int newCount = 0;
	string extra;
	

	set<string> setTemp;
	setTemp.clear();
	
	
	for(k = result.begin(); k != result.end(); ++k){
		
		extra = (*k);
		addInOut(extra, setTemp, webMap);		
	}

	for(k = setTemp.begin(); k != setTemp.end(); ++k){
		result.insert(*k);
	}
		
	for(k = result.begin(); k != result.end(); ++k){
    	origMap[*k] = 1/n;
    	updateMap[*k] =1/n;
    }

    for(int i = 0; i<t; i++){
	
		for(k = result.begin(); k != result.end(); ++k){
						
			incom = webMap[*k]->get_incom();
			for(l = incom.begin(); l != incom.end(); ++l){
				rank = origMap[*l];
				outgo = webMap[*l] ->get_out();
				for(m = outgo.begin(); m != outgo.end(); ++m){
				
					if (result.find(*m) != result.end()) {
						count++;
			        }
		        }
		        sum += rank* 1.0/(double)((count+1));
		        count =0;		        
		    }
		    newCount =0;
            nodeRank = origMap[*k];
		    forNode = webMap[*k]->get_out();
		    for(m = forNode.begin(); m != forNode.end(); ++m){
				
				if (result.find(*m) != result.end()) {
					newCount++;
		        }
		    }
		    sum += nodeRank*1.0/(double)(newCount+1);			   
		    total = (1.0-e)*sum + (e * (1.0 / n));
		    updateMap[*k] = total;	
		    outgo.clear();
		    incom.clear();	 
		    total =0;
		    rank =0;			
			sum= 0;
			nodeRank=0;   		    
		}
        

        for (it = updateMap.begin(); it != updateMap.end(); ++it) {
        		origMap[it->first] = updateMap[it->first];
   		}            
	}
	
	for (it = updateMap.begin(); it != updateMap.end(); ++it) {
        
        mapVector.push_back(make_pair(it->first, it->second));
    }
    std::sort (mapVector.begin(), mapVector.end(), comp);

    output<<mapVector.size()<<endl;
    
    for(size_t i = 0; i < mapVector.size(); i++){
     	output << mapVector[i].first <<endl;
	}

}

