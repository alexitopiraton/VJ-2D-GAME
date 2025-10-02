# VJ-2D-GAME
* Delete_Tracked_Files.sh  
Script de bash per automatitzar commits i esborrar els arxius innecessaris. Per què hem fet aquest script? Quan fem commits, no sabem per què hi ha arxius binaris (com els directoris .vs/ de Visual Studio o els arxius de compilació i binaris a /Debug) que continuen pujant-se'n al git, tot i que tenim el .gitignore "ben configurat". Com no hem esbrinat la causa, hem dissenyat aquest script que el que fa és fer un commit de tot i després esborrar els arxius que no volem del repositori. Després d'això ja podem fer el push sense problemes. 
