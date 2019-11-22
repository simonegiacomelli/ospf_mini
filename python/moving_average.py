import numpy as np


def running_mean(data, window_lenght=5):
    window = []
    moving_average = []
    for element in data:
        if len(window) <= window_lenght:
            window.append(element)
            moving_average.append(sum(window)/len(window))
        else:
            window.pop(0)
            window.append(element)
            moving_average.append(sum(window)/len(window))

    return moving_average


def main():
    lista = [i for i in range(20)]
    print(lista)

    media = running_mean(lista)
    print(media)
    
if __name__ == "__main__":
    main()



    # [0.0, 0.5, 1.0, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5]