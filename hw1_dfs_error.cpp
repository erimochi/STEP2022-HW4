#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

string link_filename = "data/links.txt";
string page_filename = "data/pages.txt";

vector<string> read_pages(string page_filename);
vector<vector<long long> > read_links(string link_filename, long long p_num);
vector<long long> route_dfs(const vector<string>& pages, const vector<vector<long long> >& links, long long p_num, string start, string goal);
vector<long long> dfs(const vector<vector<long long> >& links, long long current, long long goal, vector<int>& visited);
long long title_to_index(const vector<string>& pages, string title);
void show_route(const vector<string>& pages, const vector<long long>& route,  string start, string goal);



int main(int argc, char* argv[]){
	// start and goal
	string start = "Google";
	string goal = "渋谷";
	if(argc == 3){start = argv[1]; goal = argv[2];}
	else if(argc!=1){
		cout << "Usage: ./hw1 [start(optional)] [goal(optional)]" << endl;
		cout << "Both start and goal are assigned or omitted (in that case, 'Google' and ' 渋谷' will be assigned.) " << endl;
		exit(1);
	}
	
	// prepare page list, link list as doubly linked list 
	vector<string > pages = read_pages(page_filename);
	long long p_num = pages.size();
	vector<vector<long long> > links = read_links(link_filename, p_num);
	
	// search and show result
	cout << "Search route from " <<  start << " to " <<  goal << "." << endl;
	vector<long long> g2s_route_dfs = route_dfs(pages, links, p_num, start , goal);
	show_route(pages, g2s_route_dfs, start , goal);
	
	return 0;
}


void show_route(const vector<string>& pages, const vector<long long>& route, string start, string goal){
	long long route_len = route.size();
	if(route_len==0){ cout << "Route from " <<  start << " to " <<  goal <<  " is not found" << endl; return;}
	else{
		cout << "Start: ";
		for(int i=route_len-1; i>1; i--){
			cout << to_string(route[i]) << " " <<  pages[route[i]] << endl;
			cout << "→: ";
		}
		cout << to_string(route[0]) << " " <<  pages[route[0]] <<  " : Goal" << endl;
	}
}



vector<long long > route_dfs(const vector<string>& pages, const vector<vector<long long> >& links,  long long p_num, string start, string goal){
	long long s_index = title_to_index(pages, start);
	long long g_index = title_to_index(pages, goal);
	vector<int> visited;
	for(int i=0; i<p_num;i++){visited.push_back(0);}
	visited[s_index] = 1;
	
	vector<long long> route = dfs(links, s_index, g_index, visited); 
	if(!route.empty()){ route.push_back(s_index);} // In case route exists, add the information of start
	
	return route;
}


vector<long long> dfs(const vector<vector<long long> >& links, long long current, long long goal, vector<int>& visited){
	 vector<long long> found_route;
	  cout << "current : " << to_string(current) << endl;
	if(current==goal){
		cout << "Mikke!" << endl;
		found_route.push_back(current);
		return found_route;
	}else{
		long long next_num = links[current].size();
	 	for(int i=0; i<next_num; i++){
	 		if(!found_route.empty()){break;}
	 		switch(visited[links[current][i]]){
	 			case 0:
	 				// have not visited yet
		 			// cout << "next : " <<  to_string(links[current][i]) << " (not visited)" << endl;
		 			visited[links[current][i]] = 1;
		 			found_route = dfs(links, links[current][i], goal, visited);
		 			if(!found_route.empty()){found_route.push_back(links[current][i]);}
		 			else{visited[links[current][i]] = -1;} // ここ-1？
		 			break;
		 		case 1:
		 			// visited (become loop)
	 				// cout << "next : " <<  to_string(links[current][i]) << " (visited)" << endl;
	 				break;
	 			case -1:
	 				// not exist ahead
	 				// cout << "next : " <<  to_string(links[current][i]) << " (not exist ahead)" << endl;
	 				break;
	 			default:
	 				break;
	 		}
	 	}
	 	//if(found_route.empty()){visited[current] = -1; }
	}
	return found_route;
}




long long title_to_index(const vector<string>& pages, string title){
	long long p_num = pages.size();
	long long index = -1;
	for(int i=0; i<p_num; i++){
		if(pages[i] ==title){index = i; break;}
	}
	if(index==-1){cout << "Page title '" << title << "' does not exist in the file." << endl; exit(1);}
	return index;
}


vector<vector<long long> > read_links(string link_filename, long long p_num){
	vector<vector<long long> > links;
	for(int i=0; i<p_num; i++){
		vector<long long > links_child;
		 links.push_back(links_child);
	}
	ifstream ifs(link_filename);
	long long from, to;
	while(ifs >> from >> to){
		links[from].push_back(to);
	}
	return links;
}


vector<string> read_pages(string page_filename){
	/*
	return  vector<string > pages
			that pages[ID] = pagename
	*/
	vector<string> pages;
	ifstream ifs(page_filename);
	long long index;
	long long filled = -1;
	string title;
	while(ifs >> index >> title){
		for (int i=filled+1; i<index; i++){
			pages.push_back("[ Not Exist ]");
		}
		pages.push_back(title);
		filled = index;
	}
	return pages;
}

