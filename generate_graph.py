import numpy as np
import configparser
from itertools import combinations
from random import random
import matplotlib.pyplot as plt
import networkx as nx

def main():
    config = configparser.ConfigParser()
    config.read('data-config.ini')
    file_name = config['graph']['file_name']
    number_of_vertices = int(config['graph']['vertices'])
    number_of_edges = int(config['graph']['edges'])
    min_weight = int(config['graph']['min_weight'])
    max_weight = int(config['graph']['max_weight'])
    V = set([v for v in range(number_of_vertices)])
    E = list()
    G = nx.Graph()
    # G.add_nodes_from(V)
    vertices_combinations = [x for x in combinations(V, 2)]
    array_of_weights = np.random.randint(
        min_weight, max_weight, number_of_edges)
    for edge in range(number_of_edges):
        source = str(list(vertices_combinations)[edge][0])
        destination = str(list(vertices_combinations)[edge][1])
        weight = str(array_of_weights[edge])
        E.append(" ".join([source, destination, weight]))
        G.add_edge(source,destination)
    with open(file_name, mode='w') as file:
        for edge in range(number_of_edges):
            file.write(E[edge])
            file.write('\n')
    nx.draw(G)
    plt.show()


main()
