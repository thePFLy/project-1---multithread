"""
Tâche 1.5 – Représenter graphiquement les résultats :
- En utilisant matplotlib et un script python, représenter graphiquement le temps d’exécution
en fonction du nombre de threads, en respectant les consignes suivantes :
o Les axes x et y doivent avoir des titres clairs ;
o Chaque mesure doit présenter la moyenne et l’écart type3 ;
o L’axe des y doit systématiquement commencer à 0.
"""
import pandas as pnd
import matplotlib.pyplot as plt
import numpy as np

def pyplot_resultats(csv_file, title, output_image):
    # csv en arg
    data = pnd.read_csv(csv_file)

    # ecart type
    threads = data['Threads']
    times = data['Temps moyen (s)']
    std_devs = data['Temps moyen (s)'].std()

    # donnéess
    plt.errorbar(threads, times, yerr=std_devs, fmt='-o', capsize=5, label='Temps moyen')
    
    plt.xlabel('Nombre de threads')
    plt.ylabel('Temps d\'exécution (s)')
    plt.title(title)
    plt.grid(True)
    plt.ylim(bottom=0)
    plt.legend()
    
    plt.savefig(output_image)
    plt.clf()

pyplot_resultats('resultats/philosophes.csv', 'Temps moyen par rapport au nombre de thread: Problème des philosophes', 'resultats/philosophes.png')
pyplot_resultats('resultats/producteurs_consommateurs.csv', 'Temps moyen par rapport au nombre de thread: Producteurs consommateurs', 'resultats/producteurs_consommateurs.png')
pyplot_resultats('resultats/lecteurs_ecrivains.csv', 'Temps moyen par rapport au nombre de thread: Lecteurs écrivains', 'resultats/lecteurs_ecrivains.png')
