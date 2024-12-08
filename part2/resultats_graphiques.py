"""
Tâche 2.5 – Adaptez vos 3 programmes de la partie 1 pour utiliser vos primitives d’attente active.
- Mesurez la performance et intégrez là aux 3 plots produits en partie 14 avec les mêmes
contraintes qu’énoncé en Tâche 1.4.
"""
import pandas as pd
import matplotlib.pyplot as plt

def pyplot_resultats(csv_files, titles, output_image, x_label, y_label, plot_title):
    plt.figure(figsize=(10, 6))

    for csv_file, title in zip(csv_files, titles):
        # Lecture des données
        data = pd.read_csv(csv_file)

        # Extraction des colonnes
        threads = data['Threads']
        times = data['Temps moyen (s)']
        std_dev = data['Temps moyen (s)'].std()

        # Ajout de la série au graphique
        plt.errorbar(threads, times, yerr=std_dev, fmt='-o', capsize=5, label=title)

    # Configuration du graphique
    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.title(plot_title)
    plt.grid(True)
    plt.ylim(bottom=0)
    plt.legend()

    # Sauvegarde et affichage
    plt.savefig(output_image)
    plt.show()

# Graphe pour les spinlocks TAS et TTAS
pyplot_resultats(
    csv_files = ['resultats/spinlock_test_tas.csv', 'resultats/spinlock_test_ttas.csv'],
    titles=['test-and-set', 'test-and-test-set'],
    output_image='resultats/comparison_tas_ttas.png',
    x_label='Nombre de threads',
    y_label='Temps d\'exécution (s)',
    plot_title='Comparaison des temps moyens par rapport au nombre de thread pour les philosophes'
)

# Graphe pour les philosophes
pyplot_resultats(
    csv_files=[
        'resultats/philosophes_attente_active.csv',
        '../part1/resultats/philosophes.csv'
    ],
    titles=[
        'Attente active',
        'Mutex classique'
    ],
    output_image='resultats/comparison_philosophes.png',
    x_label='Nombre de threads',
    y_label='Temps d\'exécution (s)',
    plot_title='Comparaison des temps moyens par rapport au nombre de thread pour les philosophes'
)

# Graphe pour les producteurs-consommateurs
pyplot_resultats(
    csv_files=[
        'resultats/producteurs_consommateurs_attente_active.csv',
        '../part1/resultats/producteurs_consommateurs.csv'
    ],
    titles=[
        'Attente active',
        'Mutex classique'
    ],
    output_image='resultats/comparison_producteurs_consommateurs.png',
    x_label='Nombre de threads',
    y_label='Temps d\'exécution (s)',
    plot_title='Comparaison des temps moyens par rapport au nombre de thread pour les producteurs-consommateurs'
)
