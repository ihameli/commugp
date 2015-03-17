This software takes a network topology entry (coded as a list of edges) and finds a community partition of the graph using a local method.

The algorithm can be applied to moderately large networks, and has a complexity of O(n\*kmax+m\*log(n)).

It is based on a fitness function which grows monotonically as the process traverses the network graph.