/**
 * @file world.c
 * @brief Exécutable concernant le module : world
 * @authors SCHNEIDER Paul, DOUILLET Esteban
 * @date 28 Novembre 2022
 */
#include "world.h"

void init_world(game_t *game, world_t *world, bool new_game) {
    init_textures(world);
    init_player(world);
    init_letter_e(world);
    init_map(game, world);
    init_buttons(world);
    init_cam(world, SCREEN_W, SCREEN_H);
    init_blocks(world);

    world->cycles = 0;
    world->hearts = new_game ? 3 : world->hearts;
    world->end = !new_game && !world->newLevel;
    world->menu = !new_game && !world->newLevel;
    world->waitingMenu = false;
    world->pause = false;
    world->newLevel = false;
}

void init_player(world_t *world) {
    // Initialisation du joueur
    world->player = calloc(1, sizeof(sprite_t));
    init_sprite(world->player, 4, 0, WIDTH_PLAYER, HEIGHT_PLAYER, SCREEN_W / 2 + SIZE_TEXTURES * 4,
                SCREEN_H - 3 * HEIGHT_PLAYER, WIDTH_PLAYER,HEIGHT_PLAYER, TEXTURE_INDEX_PLAYER,
                false);
}

void init_letter_e(world_t *world) {
    // Initialisation la lettre E (au-dessus du coffre)
    world->letter_e = calloc(1, sizeof(sprite_t));
    init_sprite(world->letter_e, 0, 0, 512, 512, 0, 0, 56, 56, 0, false);
}

void init_textures(world_t *world) {
    // Initialisation des images de tous les blocs
    world->textures = calloc(NUMBER_OF_TEXTURES, sizeof(sprite_t));

    for (int i = 1; i < NUMBER_OF_TEXTURES; ++i) {
        int x = X_FIRST_TEXTURE + (SIZE_TEXTURES + SHIFT_TEXTURE) * ((i - 1) % 11);
        int y = Y_FIRST_TEXTURE + (SIZE_TEXTURES + SHIFT_TEXTURE) * ((i - 1) / 11);
        init_sprite(&world->textures[i], x,y,SIZE_TEXTURES, SIZE_TEXTURES,
                    0, 0, SIZE_TEXTURES, SIZE_TEXTURES, i, false);
    }
}

void init_map(game_t *game, world_t *world) {
    // Défini le nom de la carte
    char mapPath[100];
    sprintf(mapPath, "../assets/map-%s.txt", game->level);

    // Initialisation de la carte
    world->map = malloc(sizeof(map_t));
    world->map->tab = lire_fichier(mapPath);

    // Récupération de la taille de la carte
    taille_fichier(mapPath, &world->map->nb_row, &world->map->nb_col);
}

void init_buttons(world_t *world) {
    // Initialisation des boutons pour le menu
    world->buttons = calloc(4, sizeof(button_t));

    for (int i = 0; i < 4; ++i) {
        world->buttons[i].DestR.x = (1080 - 375 / 2) / 2;
        world->buttons[i].DestR.y = 300 + i * 90;
        world->buttons[i].DestR.w = 375;
        world->buttons[i].DestR.h = 75;
        world->buttons[i].type = i;
        world->buttons[i].enable = i < 3;
    }
}

void init_blocks(world_t *world) {
    // Initialisation de tous les blocs sur la carte
    world->blocks = calloc(sizeof(sprite_t *), world->map->nb_row);
    for (int i = 0; i < world->map->nb_row; i++) world->blocks[i] = calloc(sizeof(sprite_t), world->map->nb_col);

    // Initialisation de tous les blocs sur la map
    for (int i = 0; i < world->map->nb_row; i++) {
        for (int j = 0; j < world->map->nb_col; ++j) {
            sprite_t sprite;
            int textureIndex = world->map->tab[i][j];
            SDL_Rect SrcR = world->textures[textureIndex].SrcR;
            init_sprite(&sprite, SrcR.x, SrcR.y, SrcR.w, SrcR.h, j * SIZE_TEXTURES,
                        i * SIZE_TEXTURES, SIZE_TEXTURES, SIZE_TEXTURES, textureIndex, false);
            world->blocks[i][j] = sprite;
        }
    }
}

void init_cam(world_t *world, int w, int h) {
    // Initialise la caméra
    world->cam = calloc(1, sizeof(cam_t));
    world->cam->x = world->player->DestR.x - w / 2;
    world->cam->y = world->player->DestR.y - h / 2;
    world->cam->h = h;
    world->cam->w = w;

    // Vérifie la bordure gauche
    if (world->cam->x < 0) world->cam->x = 0;

    // vérifie la bordure droite
    if (world->cam->x + world->cam->w >= world->map->nb_col * SIZE_TEXTURES) {
        world->cam->x = world->map->nb_col * SIZE_TEXTURES - world->cam->w;
    }

}

void init_sprite(sprite_t *sprite, int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2, int textureIndex,
                 bool print_e) {
    // Définition du rectangle de source
    sprite->SrcR.x = x1;
    sprite->SrcR.y = y1;
    sprite->SrcR.w = w1;
    sprite->SrcR.h = h1;

    // Définition de mon rectangle de destination
    sprite->DestR.x = x2;
    sprite->DestR.y = y2;
    sprite->DestR.w = w2;
    sprite->DestR.h = h2;

    // Définition des mouvements des images
    sprite->saut = false;
    sprite->timeSinceJumpStart = 0;

    // Définition des autres paramètres de l'image
    sprite->v = SPEED_X_WALK;
    sprite->textureIndex = textureIndex;
    sprite->goRight = true;
    sprite->isAttacked = false;
    sprite->print_e = print_e;
}

void repositioning_camera(world_t *world) {
    int precX = world->player->prec.x;
    int precY = world->player->prec.y;

    // Vérifie les dépassements à gauche et à droite
    if (precX > SIZE_TEXTURES * 10 && precX < world->map->nb_col * SIZE_TEXTURES - SIZE_TEXTURES * 10) {
        world->cam->x = precX - world->cam->w / 2;
    }

    // Vérifie les dépassements en haut et en bas
    if (precY < world->map->nb_row * SIZE_TEXTURES - SIZE_TEXTURES * 6 && precY > SIZE_TEXTURES * 5) {
        world->cam->y = world->player->prec.y - world->cam->h / 2;
    }
}

void blob_movement(world_t *world, sprite_t *sprite) {
    sprite->prec = sprite->DestR;

    // Effectue un saut pour le blob tous les 180 cycles
    if (world->cycles % 180 == 0) {
        sprite->saut = true;
        sprite->ground = sprite->DestR.y;

        // Défini où regarde du blob
        if (world->player->DestR.x >= sprite->DestR.x) sprite->goRight = true;
        else sprite->goRight = false;
    }

    // Vérifie si le blob ne saute pas (gravité)
    if (sprite->saut == false) {
        sprite->DestR.y += sprite->timeSinceJumpStart * GRAVITY;
        sprite->timeSinceJumpStart++;
    }

    // Vérifie la gravité lors d'un saut
    if (sprite->saut == true) {
        // Réalise le saut selon une parabole : y_depart - vitesse * temps + GRAVITE * temps^2
        sprite->DestR.y = (int) round(sprite->ground - JUMP_BLOB_SPEED * sprite->timeSinceJumpStart
                                      +
                                      0.5 * GRAVITY * sprite->timeSinceJumpStart * sprite->timeSinceJumpStart);
        sprite->timeSinceJumpStart++;

        // Vérifie si le joueur est attaqué et effectue le déplacement dans la bonne direction
        if (world->player->isAttacked == false) {
            if (sprite->goRight == true) sprite->DestR.x += sprite->v;
            else sprite->DestR.x -= sprite->v;
        }
    }
}

void clean_data(world_t *world) {
    // Libère toute la mémoire utilisée pour le monde
    desallouer_tab_2D(world->map->tab, world->map->nb_row);
    for (int i = 0; i < world->map->nb_row; i++) free(world->blocks[i]); // Libère toutes les lignes
    free(world->blocks);
    free(world->buttons);
    free(world->map);
    free(world->textures);
    free(world->player);
    free(world->letter_e);
    free(world->cam);
}

void new_level(SDL_Renderer *renderer, game_t *game, ressources_t *ressources) {
    // Fichiers par défaut
    char mapPathBlocks[100] = "../assets/classic.bmp";
    char mapPathBG[100] = "../assets/classic_bg.bmp";

    // Fichier les autres fichiers selon le niveau
    if (strcmp(game->level, "END") != 0) {
        sprintf(mapPathBlocks, "../assets/%s.bmp", game->level);
        sprintf(mapPathBG, "../assets/%s_bg.bmp", game->level);
    }

    // Défini les ressources liées au niveau
    ressources->blocks = load_image(mapPathBlocks, renderer);
    ressources->background = load_image(mapPathBG, renderer);
}

void refresh_level(SDL_Renderer *renderer, game_t *game, ressources_t *ressources, world_t *world) {
    // Vérifie si nous changeons de niveau
    if (!world->newLevel) return;

    // Pause pendant 2sec
    SDL_Delay(2000);

    // Modifie les textures en fonction du niveau
    if (strcmp(game->level, "classic") == 0) sprintf(game->level, "snow");
    else if (strcmp(game->level, "snow") == 0) sprintf(game->level, "lava");
    else {
        struct tm now = getDate();

        // Défini la date actuelle de fin
        sprintf(game->endDate, "%d-%02d-%02d %02d:%02d:%02d", now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
                now.tm_hour, now.tm_min, now.tm_sec);

        sprintf(game->level, "END");
    }

    // Vérifie si s'il y a un prochain niveau
    if (strcmp(game->level, "END") != 0) {
        clean_data(world);
        new_level(renderer, game, ressources);
        init_world(game, world, false);
    } else {
        sprintf(game->level, "classic");
        world->end = true;
        world->waitingMenu = false;
        world->waitingMenu = true;
        game->enteringPseudo = true;
        world->cyclesPause = 0;
    }
}

SDL_Texture *load_image(const char *fileName, SDL_Renderer *renderer) {
    // Charge l'image à partir du chemin
    SDL_Surface *surface = SDL_LoadBMP(fileName);

    // Converti la surface en texture
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void export_blocks_in_map(world_t *world) {
    // Défini les dimensions
    int nb_col = world->map->nb_col;
    int nb_row = world->map->nb_row;

    // Vide la map
    for (int i = 0; i < nb_row; ++i) {
        for (int j = 0; j < nb_col; ++j) world->map->tab[i][j] = 0;
    }

    // Rempli la map
    for (int i = 0; i < nb_row; ++i) {
        for (int j = 0; j < nb_col; ++j) {
            // Redéfini la position dans le tab
            sprite_t *block = &world->blocks[i][j];
            int colIndex = block->DestR.x / block->DestR.w;
            int rowIndex = block->DestR.y / block->DestR.h;
            int textureIndex = world->map->tab[rowIndex][colIndex];

            // Vérifie que le bloc de destination est vide (ou que c'est un blob)
            if (textureIndex != 0 && block->textureIndex != 10 && block->textureIndex != 11) continue;

            // Redéfini la texture du bloc
            world->map->tab[rowIndex][colIndex] = block->textureIndex;
        }
    }
}

void save_world(world_t *world, char folder[100]) {
    // Crée le dossier des données du monde
    char worldFolder[150];
    sprintf(worldFolder, "%s/world", folder);

    // Crée le répertoire dédié
    mkdir(worldFolder, 0700);

    // Sauvegarde toutes les données du monde
    save_blocks(world, worldFolder);
    save_details(world, worldFolder);
    save_player(world, worldFolder);
}

void save_player(world_t *world, char folder[100]) {
    // Défini le chemin vers le fichier
    char playerFileName[100];
    sprintf(playerFileName, "%s/player.txt", folder);

    // Crée et ouvre le fichier en mode écriture
    FILE *playerFile = NULL;
    playerFile = fopen(playerFileName, "w");

    // Inscris les détails
    char player[100];
    sprintf(player, "%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i\n%i", world->player->DestR.x, world->player->DestR.y,
            world->player->prec.x, world->player->prec.y, world->player->timeSinceJumpStart, world->player->goRight,
            world->player->saut, world->player->ground, world->player->print_e);

    // Ajoute le contenu du fichier qui est fermé après
    fputs(player, playerFile);
    fclose(playerFile);
}

void save_details(world_t *world, char folder[100]) {
    // Défini le chemin vers le fichier
    char detailsFileName[100];
    sprintf(detailsFileName, "%s/details.txt", folder);

    // Crée et ouvre le fichier en mode écriture
    FILE *detailsFile = NULL;
    detailsFile = fopen(detailsFileName, "w");

    char details[100];
    sprintf(details, "%i\n%i\n%i\n%i\n%i\n%i", world->cycles, world->hearts, world->end, world->menu, world->pause,
            world->newLevel);

    // Ajoute le contenu du fichier qui est fermé après
    fputs(details, detailsFile);
    fclose(detailsFile);
}

void save_blocks(world_t *world, char folder[100]) {
    // Exporte les positions finales des blocs
    export_blocks_in_map(world);

    // Sauvegarder tous les blocs
    char blocksFiles[150];
    sprintf(blocksFiles, "%s/blocks.txt", folder);

    // Retranscris la matrice dans le fichier
    ecrire_fichier(blocksFiles, world->map->tab, world->map->nb_col, world->map->nb_row);
}

void load_world(world_t *world) {
    load_blocks(world);
    load_player(world);
    load_details(world);
    init_cam(world, SCREEN_W, SCREEN_H);
    if (world->cam->y < SIZE_TEXTURES * 5) world->cam->y = SIZE_TEXTURES * 5 - world->cam->h / 2;
}

void load_player(world_t *world) {
    FILE *fichier = NULL;
    char line[50];
    int size = 50;
    int step = 0;
    fichier = fopen("../backups/world/player.txt", "r"); // Ouvre en mode lecture

    if (fichier != NULL) { // Fichier introuvable
        // Parcours toutes les lignes
        while (fgets(line, size, fichier) != NULL) {
            int value = (int) strtol(line, NULL, 10);

            switch (step) {
                case 0:
                    world->player->DestR.x = value;
                    break;
                case 1:
                    world->player->DestR.y = value;
                    break;
                case 2:
                    world->player->prec.x = value;
                    break;
                case 3:
                    world->player->prec.y = value;
                    break;
                case 4:
                    world->player->timeSinceJumpStart = value;
                    break;
                case 5:
                    world->player->goRight = (bool) value;
                    break;
                case 6:
                    world->player->saut = (bool) value;
                    break;
                case 7:
                    world->player->ground = value;
                    break;
                default:
                    world->player->print_e = (bool) value;
                    break;
            }
            step++;
        }

        fclose(fichier);
    }
}

void load_blocks(world_t *world) {
    // Libère les anciens blocs
    for (int i = 0; i < world->map->nb_row; i++) free(world->blocks[i]);
    free(world->blocks);

    init_blocks(world);
}

void load_details(world_t *world) {
    FILE *fichier = NULL;

    // Vérifie que le fichier s'ouvre
    fichier = fopen("../backups/world/details.txt", "r");
    if (fichier == NULL) return;

    char line[50];
    int size = 50;
    int step = 0;

    // Parcours toutes les lignes
    while (fgets(line, size, fichier) != NULL) {
        int value = (int) strtol(line, NULL, 10);

        // Initialisation de l'image du joueur
        switch (step) {
            case 0:
                world->cycles = value;
                break;
            case 1:
                world->hearts = value;
                break;
            case 2:
                world->end = (bool) value;
                break;
            case 3:
                world->menu = (bool) value;
                break;
            default:
                world->pause = (bool) value;
                break;
        }

        step++;
    }

    fclose(fichier);
}

void update_hearts(game_t *game, world_t *world) {
    // Vérifie si l'utilisateur doit avoir un meilleur score
    if (game->nextLife > 0) return;

    world->hearts++;
    game->nextLife += 100;
}
