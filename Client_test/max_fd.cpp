#include <iostream>
#include <sys/resource.h>
#include <unistd.h>

int main() {
    struct rlimit rl;
    
    // Obtenir la limite des descripteurs de fichiers
    if (getrlimit(RLIMIT_NOFILE, &rl) == 0) {
        std::cout << "Nombre maximum de descripteurs de fichiers (soft limit) : " << rl.rlim_cur << std::endl;
        std::cout << "Nombre maximum de descripteurs de fichiers (hard limit) : " << rl.rlim_max << std::endl;
    } else {
        std::cerr << "Erreur lors de la récupération des limites des descripteurs de fichiers" << std::endl;
    }

    return 0;
}