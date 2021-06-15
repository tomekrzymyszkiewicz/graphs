# Graphs

The project aims to test the implementation of graphs in the implementation of algorithms for determining MST and shortest paths

## Testing algorithms

---

Tasks performed by the program are saved in the config.ini file in the following syntax

```
<name of file which will contain results>
<name of file with graph> <graph structure> <type of graph> <algorithm> <number of repeats> <type of test>
...

```
In the first line specify name of file which will contain result (CSV file)
In the next lines specify:

- `<name of file with graph>` - name of file where you store graph as a list of edges (source_vertex destination_vertex weight)
- `<graph structure>` - adjacency_matrix or adjacency_matrix or incident_matrix (not yet implemented)
- `<type of graph>` - directed or undirected
- `<algorithm>` - MST (Prim) or shortest_paths (Dijkstra)
- `<number of repeats>` - number of repeats algorithm in test window
- `<type of test>` - time or memory or print


## Generating graphs

You can generate graph by setting afv-data-config.ini file and running Python script.

Graph will be generated as a Erdős–Rényi graph. Weights of edges will be randomly generated from specified range.

```
[graph]
graph_name = <name of file where graph will be saved>
vertices = <number of vertices>
density = <density of graph>
min_weight = <minimum weight of edge>
max_weight = <maximum weight of edge>
```

## Documentation

<object data="documentation.pdf" type="application/pdf" width = "100%"></object>