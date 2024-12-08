"""
Tâche 1.5 – Représenter graphiquement les résultats :
- En utilisant matplotlib et un script python, représenter graphiquement le temps d’exécution
en fonction du nombre de threads, en respectant les consignes suivantes :
o Les axes x et y doivent avoir des titres clairs ;
o Chaque mesure doit présenter la moyenne et l’écart type ;
o L’axe des y doit systématiquement commencer à 0.
"""
import pandas as pnd
import matplotlib.pyplot as plt

def pyplot_resultats(csv_files, titles, output_image):
    plt.figure(figsize=(10, 6))

    for csv_file, title in zip(csv_files, titles):
        # csv en arg
        data = pnd.read_csv(csv_file)

        # ecart type
        threads = data['Threads']
        times = data['Temps moyen (s)']
        std_dev = data['Temps moyen (s)'].std()

        # données
        plt.errorbar(threads, times, yerr=std_dev, fmt='-o', capsize=5, label=title)

    plt.xlabel('Nombre de threads')
    plt.ylabel('Temps d\'exécution (s)')
    plt.title('Temps moyen par rapport au nombre de threads')
    plt.grid(True)
    plt.ylim(bottom=0)
    plt.legend()

    plt.savefig(output_image)
    plt.show()

csv_files = ['resultats/spinlock_test_tas.csv', 'resultats/spinlock_test_ttas.csv']
titles = ['test-and-set', 'test-and-test-set']
output_image = 'resultats/comparison_tas_ttas.png'

pyplot_resultats(csv_files, titles, output_image)
