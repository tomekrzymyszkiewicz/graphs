import numpy as np
import configparser
from itertools import combinations
from random import random

def main():
    config = configparser.ConfigParser()
    config.read('data-config.ini')
    file_name = config['graph']['file_name']
    number_of_vertices = int(config['graph']['vertices'])
    number_of_edges = int(config['graph']['edges'])
    min_weight  = int(config['graph']['min_weight'])
    max_weight = int(config['graph']['max_weight'])
    V = set([v for v in range(number_of_vertices)])
    E = list()
    vertices_combinations =[x for x in combinations(V, 2)]
    array_of_weights = np.random.randint(min_weight,max_weight,number_of_edges)
    for edge in range(number_of_edges):
        # print(vertices_combinations)
        # print(list(vertices_combinations))
        # print(len(list(vertices_combinations)))
        # print(list(vertices_combinations)[1])
        source = str(list(vertices_combinations)[edge][0])
        destination = str(list(vertices_combinations)[edge][1])
        weight = str(array_of_weights[edge])
        E.append(" ".join([source,destination,weight]))
    with open(file_name, mode='w') as file:
        for edge in range(number_of_edges):
            file.write(E[edge])
            file.write('\n')
    # array_of_verices = np.arange(number_of_vertices)
    # shuffled_array_of_verices = np.arange(number_of_vertices)
    # np.random.shuffle(shuffled_array_of_verices)
    # array_of_weights = np.random.randint(min_weight,max_weight,number_of_edges)
    # with open(file_name, mode='w') as file:
    #     for vertex in range(number_of_edges):
    #         if vertex >= number_of_vertices:
    #             shuffled_array_of_verices = np.random.shuffle(array_of_verices)
    #         file.write(" ".join([str(array_of_verices[vertex]),str(shuffled_array_of_verices[vertex]),str(array_of_weights[vertex])]))
    #         file.write('\n')

main()


