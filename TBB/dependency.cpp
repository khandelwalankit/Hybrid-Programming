#include <cstdio>
#include <iostream>
#include <tbb/tbb.h>
#include <tbb/task.h>
#include "tbb/flow_graph.h"

using namespace std;
using namespace tbb;
using namespace tbb::flow;

typedef continue_node<continue_msg> create_node;
typedef broadcast_node<continue_msg> start_node;

using namespace tbb::flow;
struct body {
 std::string my_name;
 body( const char *name ) : my_name(name) {}
 void operator()( continue_msg ) const {
    printf("%s\n", my_name.c_str());
 }
};

int main() {
 graph g;
 start_node start(g);
 create_node a( g, body("A"));
 create_node c(g, body("C"));
 create_node b( g, body("B"));
 make_edge(start,a);
 make_edge(start,c);
 make_edge(a,b);

 for (int i = 0; i < 3; ++i ) {
    start.try_put( continue_msg() );
    g.wait_for_all();
 }

 return 0;
}
