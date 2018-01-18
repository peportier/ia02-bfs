/*
Copyright 2018 Pierre-Edouard Portier
peportier.me

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
)
*/

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <functional>
#include <limits>

using namespace std;

struct
node
{
  int val;
  vector<node*> nei;
};

typedef function<void(node*)> Visitor;

void
breadth( node* src, Visitor f )
{
  set<node*> x;                   // in the end x has only white nodes
                                  // during the process, it also stores
                                  // the grey nodes

  queue<node*> y;                 // y is the queue (FIFO) of grey nodes

  y.push(src);
  x.insert(src);

  while( !y.empty() )             // there are still grey nodes
  {
    node* u = y.front();          // u is the 'oldest' element of y

    f(u);                         // visit u

    for( node* n : u->nei )       
    {
      if( x.end() == x.find(n)){ // n is a black successor of u
        y.push(n);               // n becomes grey
        x.insert(n);             // n will be white once removed from y
      }
    }

    y.pop();                      // u is now white
  }
}

Visitor visit = [](node* v){
  cout << v->val << " ; ";
};

void
findDistances( node* src, map<node*,size_t>* distances )
{

  // local function used to return infinity when
  // for a node we do not yet know a distance to the source

  function<size_t(node*)>
  distance = [distances]( node* v )
  {
    map<node*,size_t>::iterator distIt = distances->find(v);
    if( distances->end() == distIt )
    {
      return numeric_limits<size_t>::max();
    }
    return distIt->second;
  };

  Visitor
  visit = [distances, distance]( node* v )
  {
    // distance from the source to the source is zero
    if( distances->empty() ) (*distances)[v] = 0;

    // update the shortest known distance to the
    // successors of the currently visited node

    size_t vDist = distance(v);

    for (node* n : v->nei)
    {
      size_t nDist = distance(n);
      (*distances)[n] = min( nDist, 1 + vDist );
    }
  };

  breadth(src, visit);
}
int
main()
{
  // example of a graph
  struct node n1; n1.val = 1;
  struct node n2; n2.val = 2;
  struct node n3; n3.val = 3;
  struct node n4; n4.val = 4;
  struct node n5; n5.val = 5;
  struct node n6; n6.val = 6;
  n1.nei.insert(n1.nei.begin(), &n2);
  n1.nei.insert(n1.nei.begin(), &n3);
  n2.nei.insert(n2.nei.begin(), &n4);
  n4.nei.insert(n4.nei.begin(), &n3);
  n4.nei.insert(n4.nei.begin(), &n5);
  n5.nei.insert(n5.nei.begin(), &n2);
  n5.nei.insert(n5.nei.begin(), &n6);

  breadth(&n1, visit); cout << endl;

  map<node*,size_t> distances;
  findDistances(&n1, &distances);

  for( map<node*,size_t>::iterator it = distances.begin() ;
       it != distances.end() ; it++ )
  {
    cout << it->first->val << " : " << it->second << " ; ";
  }
  cout << endl;

  return 0;
}