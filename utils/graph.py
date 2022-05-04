from cProfile import label
from turtle import color
import matplotlib.pyplot as plt
import numpy as np

def bytes_to_Mb(bytes_nb):
    return bytes_nb/1048576

def create_memory():
    memory_C1 = bytes_to_Mb(1420107)
    memory_python = 32
    x = ['Python', 'C']
    y = [memory_python, memory_C1]
    c = ['#2ca02c', '#1f77b4']

    fig, ax = plt.subplots() 
    width = 0.85 # the width of the bars 
    ind = np.arange(len(y))  # the x locations for the groups
    bars = ax.barh(ind, y, width, color = c)
    ax.bar_label(bars)
    ax.set_yticks(ind)
    ax.set_yticklabels(x, minor=False)
    plt.title('Mémoire utilisée en C et Python')
    plt.xlabel('Mémoire utilisée en Mb')
    plt.ylabel('Langage')      
    #plt.show()
    plt.savefig("memory.png")

def create_speed():
    x= [5,10,20, 50]
    speed_C1 = [70.47, 135.9, 259.42, 640.3]
    speed_C4 = [35.09, 51.1, 90.21, 222.94]
    speed_python = [52692.27, 103407.31, 208496.07, 518414.97]  

    fig, ax = plt.subplots(1, 2, sharex =False , sharey = False)

    ax[0].plot(x, speed_python, color="#0cad57", label="Python")
    ax[0].grid(True, axis="y")
    ax[0].set_xlabel("Nombre de fichiers")

    ax[1].plot(x, speed_C1, label="C avec 1 thread")
    ax[1].plot(x,speed_C4, label="C avec 4 threads")
    ax[1].set_xlabel("Nombre de fichiers")
    ax[1].grid(True, axis="y")
    ax[1].set_ylabel("Vitesse d'exécution (Ms)")
    ax[1].yaxis.set_label_coords(1.1,0.48)

    lines_labels = [ax.get_legend_handles_labels() for ax in fig.axes]   # https://stackoverflow.com/questions/9834452/how-do-i-make-a-single-legend-for-many-subplots-with-matplotlib
    lines, labels = [sum(lol, []) for lol in zip(*lines_labels)]
    plt.legend(lines, labels, loc='upper center', bbox_to_anchor=(-0.123, 1.10),
            ncol=3, fancybox=True, shadow=True)

    plt.savefig("speed.png")

def create_consumption():

    fig, ax = plt.subplots(1,1 , sharex =False , sharey = False)

    nbr_file = [5, 10, 20, 50, 100, 200]
    c_oneT = [1.8252, 1.8796, 1.581, 1.8252, 1.8796, 1.7238]
    c_fourT = [1.8252, 1.8252, 1.8796, 1.9304, 2.028, 1.8759]

    py_oneT = [1.8324, 1.8324, 1.8759, 1.8759, 1.8288, 1.8288]
    #py_fourT = [1.8324, 1.8288, 1.8288, 1.8252, 1.8759, 1.8759]

    ax.plot(nbr_file, c_oneT, label='C avec 1 Thread')
    ax.plot(nbr_file, c_fourT, label='C avec 4 Thread')
    ax.plot(nbr_file, py_oneT, label='Python')
    #plt.plot(nbr_file, py_fourT, label='Python avec 4 Thread')
    #plt.title('Consommation en énergie en fonction du nombre de fichiers à analyser')
    plt.legend()
    ax.set_xlabel('Nombre de fichiers')
    ax.set_ylabel('Consommation (W)')
    plt.savefig('consumption.png')

x = " "
while (not (x.lower() in "msca")):
    x = input("Select type : (M, S, C, A): ")
if (x.lower() in "ma") :
    create_memory()
if (x.lower() in "sa"):
    create_speed()
if (x.lower() in "ca"):
    create_consumption()