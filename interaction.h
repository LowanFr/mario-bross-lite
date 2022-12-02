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
#include "world.h"

/**
 * @brief Définition d'un clavier avec les touches majeures.
 */
struct keyboard_status_s {
    bool left; /*!< Champ concernant la flèche gauche. */
    bool right; /*!< Champ concernant la flèche droite. */
    bool space; /*!< Champ concernant l'espace'. */
    bool lastIsLeft; /*!< Champ concernant la dernière touche appuyé. */
};
typedef struct keyboard_status_s keyboard_status_t;

/**
 * @brief Définition d'une souris avec les boutons et les coordonnées.
 */
struct mouse_status_s {
    bool right; /*!< Champ concernant l'état du click gauche. */
    bool left; /*!< Champ concernant l'état du click droit. */
    int x; /*!< Champ concernant l'abscisse de la souris'. */
    int y; /*!< Champ concernant l'ordonnée de la souris. */
};
typedef struct mouse_status_s mouse_status_t;

/**
 * @brief Actualisation des actions sur le clavier.
 * @param world Le monde
 * @param keyboard Le statut du clavier
 * @param event L'événement
 */
void refresh_keys(world_t *world, keyboard_status_t *keyboard, SDL_Event *event);

/**
 * @brief Initialisation du status du clavier par défaut.
 * @param keyboard Le statut du clavier
 */
void init_keyboard(keyboard_status_t *keyboard);

/**
 * @brief Actualisation des événements.
 * @param mouse La souris
 * @param keyboard Le clavier
 * @param world Le monde
 * @param event L'événement
 */
void handle_event(mouse_status_t *mouse, keyboard_status_t *keyboard, world_t *world, SDL_Event *event);

/**
 * @brief Initialisation de la souris.
 * @param mouse La souris
 * @param event L'événement
 */
void init_mouse(mouse_status_t *mouse);

/**
 * @brief Actualisation des actions de la souris.
 * @param mouse La souris
 */
void refresh_mouse(mouse_status_t *mouse, SDL_Event *event);

/**
 * @brief Actualisations des événements sur les boutons.
 * @param world  Le monde
 * @param mouseStatus Le status de la souris
 */
void handle_button(world_t *world, mouse_status_t *mouseStatus);

#endif
