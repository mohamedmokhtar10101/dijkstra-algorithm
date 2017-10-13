#include <iostream>
#include<vector>
#include<map>
#include<iterator>
using namespace std;
// to get the adjacents of the current nodes (acording to dijkstra algorithm)  
// connectinos multi map will have all connections between any two nodes , and it takes a vertix node to get its adjacents , take  a vector of blocked nodes , and the adjacents 
// vector that will be initially  empty ( we could return  a vector of adjacents element but  we take a reference to adjacents for performance reasons
//hince :: multimap here because connections can have multiple values assigned to  a key ( a nodes can have many adjacents)
void getAdjacents(multimap< int, int > &connections, int vertix, vector< bool > &blocked, vector< int > &adjacents)
{

	typedef multimap< int, int >::iterator Iterator;
	Iterator it;                                                                     // the iterator that iterates through the connections 
	pair< int, int > p;                                       
	pair< Iterator, Iterator > iterpair;                                           // a pair of Iterators afterward
	                                                                              // time complexity for equal_ranges O(log(n))
	 iterpair = connections.equal_range(vertix);                                 // the iterator pair that holds the begin() and the end() iterators return from equal_ranges 
	 it = iterpair.first;                                                       // equal_ranges return two iterator the first points to the beggining of the sub multi map that has only pairs of key equals to (vertix) variable
	                                                                           // connections (1-2, 1-3, 1-5, 3-6, 6-4)  , equal_ranges(1) => sub-connections (1-2, 1-3, 1-5) first iterator on the first 
	for (; it != iterpair.second; ++it)                                       // iterate till you reach end()
	{
		p = *it;
		if (!blocked[p.first] && !blocked[p.second])                       // if the key and the value are not blocked then add them to adjacents
			adjacents.push_back(p.second);
	}
	//iterpair = connections.equal_range(vertix);
	//it = iterpair.first;
	//for (; it != iterpair.second;)
	//	it = connections.erase(it);

}
// the function takes connections which has (  x-y ) key-value and produces y-x  key- value
// ex : (1-3 , 2-4)  => (3-1 , 4-2)
// result connections is not return and passed to the function by reference for performance reasons
// resultConnections is initially empty
void makeItsRversed(multimap< int, int> &connections, multimap< int, int> &resultConnections)
{

	for (multimap< int, int>::iterator it = connections.begin(); it != connections.end(); it++)
	{
		resultConnections.insert(*it);
		resultConnections.insert(make_pair((*it).second, (*it).first));
	}
}
// the function returns the  node that has the minimum value 
// it takes the nodes map
pair< int, int> getMin(map< int, int> &nodes)
{
	map< int, int>::iterator it;
	pair< int, int> p;
	pair< int, int> minPair;
	it = nodes.begin();
	p = *it;

	minPair.first = p.first;
	minPair.second = p.second;

	for (it = ++it; it != nodes.end(); it++)
	{
		p = *it;
		if (p.second < minPair.second)
		{
			minPair.first = p.first;
			minPair.second = p.second;
		}
	}
	return minPair;
}
/*
// extracts the nodes from connections map (1-3 ,3-1) nodes are only (1,3)
void distinctNodes( multimap< int, int>  &theMap , map< int, int> &distincts )
{
pair< int, int > p;

for( multimap< int, int >::iterator it = theMap.begin(); it != theMap.end(); it++ )
{
p = *it;
if(distincts.find( p.first ) == distincts.end())
distincts.insert (make_pair( p.first, 2147483646 ) );

if(distincts.find( p.second) == distincts.end() )
distincts.insert( make_pair( p.second, 2147483646 ) );
}
}
*/
void makeNodes(map< int, int> &nodes, int n)
{
	for (int i = 0; i < n; i++)
		nodes.insert(make_pair(i, 2147483646));
}
// updates a map's key's value from the specified key and value
void update(map< int, int > &m, int key, int value)
{

	map< int, int >::iterator it = m.find(key);
	if (it != m.end())
		(*it).second = value;

}
// get the shortest path from source to every node in the connections map 
// parents is initially empty and will have at the end the shortest path from source to every node
// nodes have all vertices all assigned to Infinity
// blocked nodes that are blocked and can't be reachable by the source node;
// init is the initial value of the source node normally -1 or any other values that nodes can't have;

// the main building blocks for the algorithm  are :
// 1- nodes (updating nodes extract minimum )
// 2- distances ( all nodes shortest distances , the last one is the current working nodes to get its adjacents)
// 3- parents ( inserting parents, updating them , it will have the shortest path )
void findShortest(map< int, int > &nodes, multimap< int, int > &connections, map< int, int > &parents, vector< bool > &blocked, int source, int init)
{

	vector< pair< int, int> > distances;
	map< int, int >::iterator it;
	vector< int >  adjacents;
	pair< int, int > minNode;
	int current = source;
	int currentAdjacent;
	update(nodes, source, 0);                                                 // update the source to have initial value zero ( according to dijkstra algorithm zero means the shortest path from it to itself)
	distances.push_back(make_pair(source, 0));                               // add  the source node to the distances and assign the distance zero to it
	parents.insert(make_pair(source, init));                                // add  the source node to parents and make its parent the initial parent -1 has no parent
	nodes.erase(source);                                                   // extract min and delete it from the nodes and then 
	// while the nodes is not empty
	while (!nodes.empty())            
	{

		getAdjacents(connections, current, blocked, adjacents);                            // get its adjacents and

		for (int i = 0, sum = 0, size = adjacents.size(); i < size; i++)                 // for every adjacent update its distance
		{

			currentAdjacent = adjacents[i];;
			it = nodes.find(currentAdjacent);                                         // time complexity O( log(N) ) red black tree implementation

			if (it == nodes.end())
				continue;

			sum = 1 + distances[distances.size() - 1].second;                     // get the distance of its parent ( it has the shortest distance from source to it) + its distance which is 1

			if (sum < (*it).second)                                             // if the sum of the distance from it to the source is less than its current distance from it to source then its the shortest so update it
			{

				(*it).second = sum;
				it = parents.find(currentAdjacent);                          // if the new sum is less than the current sum then update its parent ( if exists , if not insert it) to be the current parent

				if (it != parents.end())
					(*it).second = distances[distances.size() - 1].first;  // the current parent is always at the end of the array
				else
					parents.insert(make_pair(currentAdjacent, distances[distances.size() - 1].first));
			}

		}         
		adjacents.clear();                                                    // clear the adjacents to get the adjacents of the new min Node
		minNode = getMin(nodes);                                             // get the min node (extract min )
		distances.push_back(make_pair(minNode.first, minNode.second));      // add the min node to be the current parent and add it to have the shortest path from it to the source node
		current = minNode.first;                                           // update current to be the current node to get its adjacents
		nodes.erase(minNode.first);                                       // erase the min node from nodes and redo the cycle until the nodes map is empty
	}

}
// gets how many nodes from source to distination, it takes the parents map that has the shortest path from source to every node
int getNodeToNodeCount(map< int, int > parents, int source, int destination)
{
	map< int, int >::iterator it;
	int count = 0;
	while (destination != source)
	{
		it = parents.find(destination);
		if (it == parents.end())
			break;

		count++;
		destination = (*it).second;
	}
	return count;
}

int main()
{

	multimap< int, int > connections;
	multimap< int, int > connections1;
	map< int, int > nodes;
	map< int, int > shortestPath;
	int t, n, m;
	int user1, user2;
	bool blockedVar;


	cin >> t;                                                     // number of test cases
	for (int i = 0; i < t; i++)
	{
		cin >> n >> m;                                          // n => the number of nodes m=> the number of connections                                 
		vector< bool > blocked(n + 1);                         // blocked n+1 because nodes that are blocked by 0 ( the initial node) are numbered from 1 to n


		for (int j = 1; j < n; j++)                          // get  the blocked nodes from 1 to n
		{
			cin >> blockedVar;
			blocked[j] = blockedVar;
		}

		for (int j = 0; j < m; j++)                                                               // get connections
		{ 
			cin >> user1 >> user2;                                                              // node to node                 users here are synonymes
			connections.insert(make_pair(user1, user2));
		}

		makeNodes(nodes, n);                                                                // make nodes from 0 to n , 0 ,1 ,2, to n
		makeItsRversed(connections, connections1);                                         // (1-4,2-3) => (4-1,3-2)
		findShortest(nodes, connections1, shortestPath, blocked, 0, -1);                  // find the shortest path and assign it to shortest path map

		for (int j = 1, count; j < n; j++)                                              // for every node get the shortest path from the source node (0 in our case ) to it
		{                                                                              // if there is no way to reach the j node from the source ( no connections or blocked node) print -1
			count = getNodeToNodeCount(shortestPath, 0, j);
			if (count == 0)
				cout << -1 << " ";
			else
				cout << count << " ";
		}
		cout << endl;
		connections.clear();                                                    // reinitializing connections ( that has (1-2,2-3) not reversed connections (1-2,2-1,2-3,3-2)) , connections1 that has the reversed connections , the shortest path and the ndoes maps
		connections1.clear();
		shortestPath.clear();
		nodes.clear();

	}
	return 0;
}
