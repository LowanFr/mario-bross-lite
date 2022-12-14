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
 * @brief La taille maximale d'une ligne du leaderboard
 */
#define LINE_SIZE 100

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

/**
 * @brief Type concernant la partie
 */
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

/**
 * @brief Récupère chaque ligne avec pseudo + score
 * @param path Le chemin vers le fichier
 * @param nbPlayers Le nombre de joueurs
 * @return Les lignes avec les pseudonymes + scores
 */
char **getPlayers(char *path, int nbPlayers);

/**
 * @brief Récupère tous les scores
 * @param players Les joueurs
 * @param nbPlayers Le nombre de joueurs
 * @return Les scores de chaque joueur
 */
int *getScores(char **players, int nbPlayers);

/**
 * @brief Défini le classement à partir des scores
 * @param game La partie
 * @param players Les joueurs
 * @param scores  Les scores
 * @param nbPlayers Le nombre de joueurs
 */
void setLeaderboard(game_t *game, char **players, int *scores, int nbPlayers);

/**
 * @brief Récupère la date actuelle
 * @return La date
 */
struct tm getDate();

/**
 * @brief Défini un attribut de la partie en fonction de la donnée et de l'étape de chargement
 * @param game La partie
 * @param line La donnée sauvegardée
 * @param step L'étape
 */
void setGameData(game_t *game, char *line, int step);

#endif
