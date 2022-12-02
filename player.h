#ifndef PLAYER_H
#define PLAYER_H

/**
 * @file player.h
 * @brief Header du module : player
 * @authors SCHNEIDER Paul, DOUILLET Esteban
 * @date 30 Novembre 2022
 */

#include "interaction.h"

/**
 * @brief La gravité, sert au joueur de retomber.
 */
#define GRAVITY 2

/**
 * @brief La vitesse du joueur sur l'axe des ordonnées (saut).
 */
#define JUMP_SPEED 30

/**
 * @brief Définition du joueur avec ses coordonnées et ses caractéristiques.
 */
struct player_s {
    sprite_t *sprite; /*!< Champ concernant l'image du joueur. */
    SDL_Rect prec; /*!< Champ concernant les coordonnées précédentes du joueur. */
    bool saut; /*!< Champ vérifiant si le joueur est en saut. */
    int timeSinceJumpStart; /*!< La durée depuis laquelle le joueur est en saut. */
    int ground; /*!< Le sol sur lequel il doit atterrir. */
};
typedef struct player_s player_t;

/**
 * @brief Définition des données du joueur.
 * @param player Le joueur
 * @param world Le monde
 */
void init_player(player_t *player, world_t *world);

/**
 * @brief Gestion du déplacement du joueur.
 * @param touches Les touches
 * @param player Le joueur
 */
void player_movement(keyboard_status_t *touches, player_t *player);

/**
 * @brief Vérification de toutes les collisions.
 * @param world Le monde
 * @param player Le joueur
 */
void handle_collision(world_t *world, player_t *player);

/**
 * @brief Vérification de collision avec un bloc solide.
 * @param player Le joueur
 * @param sprite Le bloc
 */
void handle_collision_solidBlock(player_t *player, sprite_t *sprite);

/**
 * @brief Gestion des collisions entre le joueur et les pièces.
 * @param player Le joueur
 * @param sprite Le bloc
 */
void handle_collision_pieces(player_t *player, sprite_t *sprite);

#endif
