#ifndef GAME_H
#define GAME_H

/**
 * @file game.h
 * @brief Header concernant le module : game
 * @authors SCHNEIDER Paul, DOUILLET Esteban
 * @date 7 Décembre 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Définition d'une partie d'un joueur
 */
struct game_s {
    char *level; /*!< Champ concernant le niveau. */
    int score; /*!< Champ concernant le score actuel. */
    int nextLife; /*!< Score nécessaire avant la prochaine vie. */
    char *pseudo; /*!< Champ concernant le pseudonyme associé à la partie. */
    char *startDate; /*!< Champ concernant la date à laquelle la partie est lancée. */
    char *endDate; /*!< Champ concernant la date à laquelle la partie est finie. */
    bool enteringPseudo; /*!< Champ concernant le fait que l'utilisateur peut rentrer son pseudonyme. */
    char **leaderboard; /*!< Champ concernant le classement par score décroissant. */
    int leaderboardLength; /*!< Champ concernant la taille du classement. */
};
typedef struct game_s game_t;

/**
 * @brief Initialiser la partie
 * @param game La partie
 */
void init_game(game_t *game);

/**
 * Sauvegarde l'état de la partie
 * @param game La partie
 * @param folder Le dossier de sauvegarde
 */
void save_game(game_t *game, char folder[100]);

/**
 * @brief Libérer la mémoire de la partie
 * @param game La partie
 */
void clean_game(game_t *game);

/**
 * @brief Charge la sauvegarde
 * @param game La partie
 */
void load_game(game_t *game);

/**
 * @brief Charge le classement
 * @param game La partie
 */
void load_leaderboard(game_t *game);

/**
 * @brief Récupère le nombre de joueur dans le classement
 * @param path Le chemin vers le fichier
 * @return Le nombre de joueur
 */
int getNbPlayers(char *path);

#endif
