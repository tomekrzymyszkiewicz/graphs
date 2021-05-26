import numpy as np
import configparser

def main():
    config = configparser.ConfigParser()
    config.read('data-config.ini')
    file_name = config['graph']['file_name']
    number_of_vertices = int(config['graph']['vertices'])
    number_of_edges = int(config['graph']['edges'])
    min_weight  = int(config['graph']['min_weight'])
    max_weight = int(config['graph']['max_weight'])
    array_of_verices = np.arange(number_of_vertices)
    print(array_of_verices)
    shuffled_array_of_verices = np.arange(number_of_vertices)
    np.random.shuffle(shuffled_array_of_verices)
    print(shuffled_array_of_verices)
    array_of_weights = np.random.randint(min_weight,max_weight,number_of_edges)
    print(array_of_weights)
    with open(file_name, mode='w') as file:
        for vertex in range(number_of_edges):
            if vertex >= number_of_vertices:
                shuffled_array_of_verices = np.random.shuffle(array_of_verices)
            file.write(" ".join([str(array_of_verices[vertex]),str(shuffled_array_of_verices[vertex]),str(array_of_weights[vertex])]))
            file.write('\n')

main()


