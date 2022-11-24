/**
 * @file player.c
 * @brief Exécutable concernant le module : player
 * @authors SCHNEIDER Paul, DOUILLET Esteban
 * @date 14 Novembre 2022
 */
#include "player.h"

void init_player(player_t *joueur, world_t *world){
    joueur->sprite = world->player;
    joueur->saut = false;
    joueur->timeSinceJumpStart = 0;
}

void player_movement(keyboard_status_t *touches, player_t *player) {
    player->prec = player->sprite->DestR;
    // Vérifie le déplacement à gauche
    if (touches->left) {
        player->sprite->DestR.x = player->sprite->DestR.x - player->sprite->v;
    }

    // Vérifie le déplacement à droite
    if (touches->right) {
        player->sprite->DestR.x = player->sprite->DestR.x + player->sprite->v;
    }

    // Vérifie un saut
    if (touches->space && player->timeSinceJumpStart == 0) {
        player->ground = player->sprite->DestR.y;
        player->timeSinceJumpStart = 0;
        player->saut = 1;
    }
    if (player->saut != 1){
        player->sprite->DestR.y = player->sprite->DestR.y + player->timeSinceJumpStart * GRAVITE;
        player->timeSinceJumpStart++;
    }
    // Vérifie la gravité lors d'un saut
    if (player->saut == 1){
        player->sprite->DestR.y = player->ground - VITESSE_Y_SAUT * player->timeSinceJumpStart
                + 0.5 * GRAVITE * player->timeSinceJumpStart * player->timeSinceJumpStart;
        player->timeSinceJumpStart++;

    }
}

void handle_collision(world_t *world, player_t *player) {
    handle_collision_solidBlock(world, player);
}


void handle_collision_solidBlock(world_t *world, player_t *player) {
    for( int i = player->sprite->DestR.y/TAILLE_TEXTURES - 2; i <= player->sprite->DestR.y/TAILLE_TEXTURES + 2; ++i) {
        for (int j = player->sprite->DestR.x/TAILLE_TEXTURES - 1; j <= player->sprite->DestR.x/TAILLE_TEXTURES + 1 ; ++j) {

            if(i < world->map->nb_row && i>=0 && j < world->map->nb_col && j>=0){
                int nb = world->map->tab[i][j];
                SDL_Rect DestR = world->map->DestR[i][j];

                // Collision en bas
                bool condCollision1 = DestR.y < player->sprite->DestR.y + player->sprite->DestR.h  &&
                    DestR.y + DestR.h > player->sprite->DestR.y;
                bool condCollision2 = DestR.x < player->sprite->DestR.x + player->sprite->DestR.w &&
                    DestR.x + DestR.w > player->sprite->DestR.x;
                bool conCollision3 = nb == 1 || nb == 2 || nb == 3 || nb == 12 || nb == 13||nb == 14 || nb == 23 ||
                    nb == 24 || nb == 25 || nb == 18 || nb == 19 || nb == 29 || nb == 30;
        
                // Une collision entre le joueur et le block
                if (condCollision1 && condCollision2 && conCollision3) {

                    // Collision à droite du block

                    if (DestR.x + DestR.w > player->sprite->DestR.x && //Si le coté droit du bloc est inférieur au coté gauche du player
                    DestR.x < player->sprite->DestR.x && //Si le coté gauche du player est inférieur au coté gauche du bloc
                    player->prec.x >= DestR.x + DestR.w //Si le coté gauche précedent est bien supérieur au coté droit du bloc
                    ) { 
                        player->prec.x = DestR.x + DestR.w;
                        player->sprite->DestR.x = player->prec.x;
                    }else{
                        // Collision à gauche du block :
                        if (DestR.x < player->sprite->DestR.x + player->sprite->DestR.w && //Si le coté gauche du bloc est inférieur au coté droit du player
                        DestR.x > player->sprite->DestR.x && //Si le coté gauche du bloc est inférieur au coté droit du player
                        player->prec.x + player->prec.w <= DestR.x) { //Si le coté gauche précédent est bien inférieur ou égal au coté gauche du bloc
                            player->prec.x = DestR.x - player->prec.w;
                            player->sprite->DestR.x = player->prec.x;
                    
                        }else{
                        
                            // Collision en haut du block :

                            if (DestR.y < player->sprite->DestR.y + player->sprite->DestR.h && // Si le haut du bloc est en collision avec le bas du player
                            player->sprite->DestR.y < DestR.y && // Si le haut du player est bien inférieur au haut du bloc
                            player->prec.y + player->prec.h <= DestR.y) { //Si la position precédente du bas du player est bien inférieur au haut du bloc
                                player->saut = 0;
                                player->prec.y = DestR.y-player->prec.h;
                                player->sprite->DestR.y = player->prec.y;
                                player->timeSinceJumpStart = 0;
                            }else{
                            
                                // Collision en bas du block :

                                if (DestR.y + DestR.h > player->sprite->DestR.y && // Si collision bas du bloc et haut du player
                                player->sprite->DestR.y > DestR.y && // Si le haut du player est bien supérieur au haut du bloc
                                player->prec.y >= DestR.y + DestR.h  // Si le prec du player est en dehors du bloc
                                ) {
                                    player->saut = 2;
                                    player->timeSinceJumpStart = 1; //= 1 evite de pouvoir resauter en dessous du bloc
                                    player->prec.y = DestR.y + DestR.h;
                                    player->sprite->DestR.y = player->prec.y;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
