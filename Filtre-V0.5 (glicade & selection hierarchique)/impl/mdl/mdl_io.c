#include "mdl.h"

void ecrire_mdl(Mdl_t * mdl, char * file) {
    FILE * fp = fopen(file, "wb");
    if (fp == 0) ERR("Fichier %s existse pas", file);
    //
    uint params[5] = {C_max, Y_max, N0_max, N1_max, MAX_GLISSADE};
    fwrite(params, sizeof(uint), 5, fp);
    fwrite(mdl, sizeof(Mdl_t), 1, fp);
    fclose(fp);
}

Mdl_t * lire_mdl(char * file) {
    FILE * fp = fopen(file, "rb");
    if (fp == 0) ERR("Fichier %s existse pas", file);

    uint params[5] = {C_max, Y_max, N0_max, N1_max, MAX_GLISSADE};
    uint autres[5];
    fread(autres, sizeof(uint), 5, fp);
    FOR(0, i, 5) assert(params[i] == autres[i]);
    Mdl_t * ret = malloc(sizeof(Mdl_t));
    fread(ret, sizeof(Mdl_t), 1, fp);
    return ret;
};
