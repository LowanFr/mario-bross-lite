#ifndef INTERACTION_H
#define INTERACTION_H

/**
 * @brief Fichier représentant le module des interactions.
 * @file interaction.h
 * @authors SCHNEIDER Paul, DOUILLET Esteban
 * @date 30 Novembre 2022
 */

#include <stdbool.h>
#include <SDL.h>
#include "graphic.h"

/**
 * @brief Actualisation des actions sur le clavier.
 * @param game La partie
 * @param world Le monde
 * @param keyboard Le statut du clavier
 * @param event L'événement à exécuter
 */
void refresh_keys(game_t *game, world_t *world, keyboard_status_t *keyboard, SDL_Event *event);

/**
 * @brief Initialisation du status du clavier par défaut.
 * @param keyboard Le statut du clavier
 */
void init_keyboard(keyboard_status_t *keyboard);

/**
 * @brief Actualisation des événements.
 * @param renderer Le moteur de rendu
 * @param ressources Les ressources
 * @param mouse La souris
 * @param keyboard Le clavier
 * @param game La partie
 * @param world Le monde
 * @param event Les événements
 */
void handle_event(SDL_Renderer *renderer, ressources_t *ressources, mouse_status_t *mouse, keyboard_status_t *keyboard,
                  game_t *game, world_t *world, SDL_Event *event);

/**
 * @brief Initialisation de la souris.
 * @param mouse La souris
 */
void init_mouse(mouse_status_t *mouse);

/**
 * @brief Actualisation des actions de la souris.
 * @param mouse La souris
 * @param event L'événement à exécuter
 */
void refresh_mouse(mouse_status_t *mouse, SDL_Event *event);

/**
 * @brief Actualisations des événements sur les boutons.
 * @param renderer Le moteur de rendu
 * @param ressources Les ressources
 * @param game La partie
 * @param world  Le monde
 * @param mouseStatus Le status de la souris
 */
void handle_button(SDL_Renderer *renderer, ressources_t *ressources, game_t *game, world_t *world,
                   mouse_status_t *mouseStatus);

/**
 * @brief Écris le pseudonyme de l'utilisateur
 * @param game Le jeu
 * @param key La touche
 */
void write_pseudo(game_t *game, SDL_Keycode key);

/**
 * @brief Actionne le bouton pour reprendre le jeu
 * @param renderer Le moteur de rendu
 * @param game La partie
 * @param world Le monde
 * @param ressources Les ressources
 */
void resume_button(SDL_Renderer *renderer, game_t *game, world_t *world, ressources_t *ressources);

/**
 * @brief Actionne le bouton pour lancer une nouvelle partie
 * @param renderer Le moteur de rendu
 * @param game La partie
 * @param world Le monde
 * @param ressources Les ressources
 */
void new_game_button(SDL_Renderer *renderer, game_t *game, world_t *world, ressources_t *ressources);

/**
 * @brief Actionne le bouton pour quitter le jeu
 * @param world Le monde
 */
void exit_button(world_t *world);

/**
 * @brief Actionne le bouton pour sauvegarder la partie
 * @param game La partie
 * @param world Le monde
 */
void save_button(game_t *game, world_t *world);

#endif
