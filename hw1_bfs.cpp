#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

string link_filename = "data/links.txt";
string page_filename = "data/pages.txt";

vector<string> read_pages(string page_filename);
vector<vector<long long> > read_links(string link_filename, long long p_num);
vector<long long> route_bfs(const vector<string>& pages, const vector<vector<long long> >& links, long long p_num, string start, string goal);
vector<long long> bfs(const vector<vector<long long> >& links, long long start, long long goal, vector<long long>& visited);
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
	vector<long long> g2s_route_bfs = route_bfs(pages, links, p_num, start , goal);
	show_route(pages, g2s_route_bfs, start , goal);
	
	return 0;
}


void show_route(const vector<string>& pages, const vector<long long>& route, string start, string goal){
	long long route_len = route.size();
	if(route_len==0){ cout << "Route from " <<  start << " to " <<  goal <<  " is not found" << endl; return;}
	else{
		cout << "Start: ";
		for(int i=route_len-1; i>0; i--){
			cout << to_string(route[i]) << " " <<  pages[route[i]] << endl;
			cout << "→: ";
		}
		cout << to_string(route[0]) << " " <<  pages[route[0]] <<  " : Goal" << endl;
	}
}



vector<long long > route_bfs(const vector<string>& pages, const vector<vector<long long> >& links,  long long p_num, string start, string goal){
	long long s_index = title_to_index(pages, start);
	long long g_index = title_to_index(pages, goal);
	
	vector<long long> visited;
	for(int i=0; i<p_num;i++){visited.push_back(-1);}
	visited[s_index] = -2;
	/*
	visited[index] = 0~p_num-1 → come from visited[index] to index
	visited[index] = -2 → come from start to index
	visited[index] = -1 → have not visited yet
	*/
	
	vector<long long> route = bfs(links, s_index, g_index, visited); 
	
	// In case route exists, add the information of start
	return route;
}


 

vector<long long> bfs(const vector<vector<long long> >& links, long long start, long long goal, vector<long long>& visited){
	 vector<long long> found_route;
	 vector<long long> queue; for(int i=0; i<links[start].size();i++){ queue.push_back(links[start][i]); visited[links[start][i]]=start;}
	 long long current = start;
	 int qlen = queue.size();
	 
	 if(qlen==0 && current==goal){found_route.push_back(current);  cout << "Found!" << endl;}  // In case start == goal && qlen == 0

	 while(qlen>0){
	 	if(current==goal){found_route.push_back(current);  cout << "Found!" << endl; break;}
	 	current = queue[0]; queue.erase(queue.begin());
 		for(int i=0; i< links[current].size(); i++){
 			if(visited[links[current][i]]==-1){
 				// 隣接する未探索のノードをqueueに格納+どのノードから移動可能かvisitedに記録
 				queue.push_back(links[current][i]);
 				visited[links[current][i]]=current;
 			}
 		}
 		qlen = queue.size();
	 }
	 
	 // if route is found, memorize it
	if(!found_route.empty()){
		 while( visited[current] != -2){
		 	current = visited[current];
		 	found_route.push_back(current);
		 	// cout << to_string(visited[current]) << "← " << to_string(current) << endl;		 	
		 } 
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

/*
memo 
bfs中のwhile loopについて:
while(1){.... if(qlen>0){}else{break;}}の方が一貫性が出そうだが計算量が2倍に増えそう(?)
do{}while(qlen>0)だとqlen>0のチェック前にqueue[0]を呼んでしまう
*/
