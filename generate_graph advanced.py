import numpy as np
import configparser
from itertools import combinations
from random import random
import matplotlib.pyplot as plt
import networkx as nx

def main():
    config = configparser.ConfigParser()
    config.read('adv-data-config.ini')
    file_name = config['graph']['file_name']
    number_of_vertices = int(config['graph']['vertices'])
    density = float(config['graph']['density'])
    min_weight = int(config['graph']['min_weight'])
    max_weight = int(config['graph']['max_weight'])
    G = nx.fast_gnp_random_graph(number_of_vertices,density)
    with open(file_name[0:-4] + "-"+ str(G.number_of_edges())+"edges.csv", mode='w') as file:
        for edge in G.edges():
            file.write(" ".join([str(edge[0]),str(edge[1]),str(np.random.randint(min_weight,max_weight))]))
            file.write('\n')
    nx.draw(G)
    plt.show()


main()
