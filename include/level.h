#ifndef __LEVEL_H__
#define __LEVEL_H__

typedef struct {
    double min_jump;
    double max_jump;
    double num_breaking;
    double num_monsters;
    double min_monster_distance;
    double max_monster_distance;
    double moving_monster_ratio;
    double moving_plat_ratio;
    double jet_ratio;
    double spring_ratio;
} level_info_t;

#endif