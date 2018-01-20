```c++
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

```
# Parcours en largeur (breadth-first search)

Répertoire GitHub correspondant à cet article : https://github.com/peportier/ia02-bfs

## Structure FIFO

En utilisant pour $y$ (la structure qui contient les nœuds rencontrés mais dont les successeurs n'ont pas encore été calculés, c'est-à-dire les nœuds gris) une structure de file FIFO (First In First Out) au lieu d'un ensemble, nous obtenons le programme de recherche en largeur dans un graphe.

```c++
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

```

Nous suivons le schéma de l'algorithme générique dérivé précédemment. Pour ne pas avoir à chercher la présence d'un nœud dans la file `y` afin de savoir s'il est gris, l'ensemble `x` contient les nœuds gris et les nœuds noirs. 

```c++
typedef function<void(node*)> Visitor;

void
breadth( node* src, Visitor f )
{
  set<node*> x;                   // x is the set of black and grey nodes

  queue<node*> y;                 // y is the queue (FIFO) of grey nodes

  y.push(src); x.insert(src);     // at the beginning, the source is the only grey node

  while( !y.empty() )             // there are still grey nodes
  {
    node* u = y.front();          // u is the 'oldest' element of y

    f(u);                         // visit u

    for( node* n : u->nei )       
    {
      if( x.end() == x.find(n) )  // n is a white successor of u
      {                                 
        y.push(n); x.insert(n);   // n becomes grey
      }
    }
    
    y.pop();                      // u becomes black
  }
}

```

Nous utilisons un visiteur trivial :

```c++
Visitor visit = [](node* v){
  cout << v->val << " ; ";
};

```

![largeur](media/largeur.png)

## Calcul des plus courts chemins

L'algorithme de recherche en largeur permet de trouver les plus courts chemins d'un nœud source aux autres nœuds du graphe dans le cas d'un graphe orienté pour lequel les arêtes ont toutes le même coût (ou poids) :

```c++
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
```

Le programme principal calcule les plus courtes distances au noeud source 1 sur notre exemple filé.

```c++
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
```

Nous trouvons bien :

![largeur-plus-courts-chemins](media/largeur-plus-courts-chemins.png)